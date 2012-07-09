#ifndef LocalPCADistance_HPP
#define LocalPCADistance_HPP

#include "LocalPCADistance.h"

// Submodules
#include "PatchProjection/PatchProjection.h"

template <typename TImage>
float LocalPCADistance<TImage>::Distance(const TImage* const image,
                                         const itk::ImageRegion<2>& region1,
                                         const itk::ImageRegion<2>& region2)
{
  // Dilate the query region
  itk::Index<2> dilatedRegionCorner = {{region1.GetIndex()[0] - 1, region1.GetIndex()[1] - 1}};
  itk::Size<2> dilatedRegionSize = {{region1.GetSize()[0] + 2, region1.GetSize()[1] + 2}};
  itk::ImageRegion<2> dilatedRegion(dilatedRegionCorner, dilatedRegionSize);
  dilatedRegion.Crop(image->GetLargestPossibleRegion());

  // Compute a local feature matrix (the query patch and all of it's neighbors (if they are inside the image)
  MatrixType featureMatrix = PatchProjection<Eigen::MatrixXf, Eigen::VectorXf>::
                               VectorizeImage(image, region1.GetSize()[0]/2, dilatedRegion);

  MatrixType projectionMatrix = PatchProjection<MatrixType, VectorType>::ProjectionMatrixFromFeatureMatrix(featureMatrix);

  VectorType vectorizedSource = PatchProjection<MatrixType, VectorType>::
                                  VectorizePatch(image, region1);

  VectorType vectorizedTarget = PatchProjection<MatrixType, VectorType>::
                                  VectorizePatch(image, region2);

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
  return operator()(this->Image, region1, region2);
}

#endif
