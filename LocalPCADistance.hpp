#ifndef LocalPCADistance_HPP
#define LocalPCADistance_HPP

#include "LocalPCADistance.h"

// Submodules
#include "PatchProjection/PatchProjection.h"
#include "EigenHelpers/EigenHelpers.h"

template <typename TImage>
LocalPCADistance<TImage>::LocalPCADistance() : Image(NULL)
{

}

template <typename TImage>
float LocalPCADistance<TImage>::Distance(const TImage* const image, const MatrixType& projectionMatrix,
                                         const VectorType& meanVector,
                                         const itk::ImageRegion<2>& region1,
                                         const itk::ImageRegion<2>& region2)
{
  VectorType vectorizedSource = PatchProjection<MatrixType, VectorType>::
                                  VectorizePatch(image, region1);
  vectorizedSource -= meanVector;

  VectorType vectorizedTarget = PatchProjection<MatrixType, VectorType>::
                                  VectorizePatch(image, region2);
  vectorizedTarget -= meanVector;

  assert(vectorizedSource.size() == projectionMatrix.rows());

  VectorType projectedSource = projectionMatrix.transpose() * vectorizedSource;

  VectorType projectedTarget = projectionMatrix.transpose() * vectorizedTarget;

  // Compute distance between patches in projected space
  float projectedDistance = (projectedSource - projectedTarget).squaredNorm();

  return projectedDistance;
}

template <typename TImage>
float LocalPCADistance<TImage>::Distance(const itk::ImageRegion<2>& region1,
                                         const itk::ImageRegion<2>& region2)
{
  return Distance(this->Image, this->ProjectionMatrix, this->MeanVector, region1, region2);
}

template <typename TImage>
void LocalPCADistance<TImage>::ComputeProjectionMatrix(const itk::ImageRegion<2>& region)
{
  assert(this->Image);

  // Dilate the query region
  itk::Index<2> dilatedRegionCorner = {{region.GetIndex()[0] - 1, region.GetIndex()[1] - 1}};
  itk::Size<2> dilatedRegionSize = {{region.GetSize()[0] + 2, region.GetSize()[1] + 2}};
  itk::ImageRegion<2> dilatedRegion(dilatedRegionCorner, dilatedRegionSize);
  dilatedRegion.Crop(this->Image->GetLargestPossibleRegion());

  // Compute a local feature matrix (the query patch and all of it's neighbors (if they are inside the image)
  MatrixType featureMatrix = PatchProjection<Eigen::MatrixXf, Eigen::VectorXf>::
                               VectorizeImage(this->Image, region.GetSize()[0]/2, dilatedRegion);

  unsigned int inputPoints = featureMatrix.cols();

  this->ProjectionMatrix = PatchProjection<MatrixType, VectorType>::ProjectionMatrixFromFeatureMatrix(featureMatrix, this->MeanVector);

  // Only keep the number of columns corresponding to the number of input points used
  this->ProjectionMatrix = EigenHelpers::TruncateColumns<MatrixType>(this->ProjectionMatrix, inputPoints);
}

template <typename TImage>
void LocalPCADistance<TImage>::SetImage(TImage* const image)
{
  this->Image = image;
}

template <typename TImage>
void LocalPCADistance<TImage>::SetTargetPatch(const itk::ImageRegion<2>& region)
{
  ComputeProjectionMatrix(region);
}

#endif
