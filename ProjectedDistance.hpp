#ifndef ProjectedDistance_HPP
#define ProjectedDistance_HPP

#include "ProjectedDistance.h"

// Submodules
#include "PatchProjection/PatchProjection.h"

template <typename TImage>
float ProjectedDistance<TImage>::Distance(const TImage* const image, const MatrixType& projectionMatrix,
                                          const itk::ImageRegion<2>& region1,
                                          const itk::ImageRegion<2>& region2)
{
  VectorType vectorizedSource = PatchProjection<MatrixType, VectorType>::VectorizePatch(image,
                                                              region1);

  VectorType vectorizedTarget = PatchProjection<MatrixType, VectorType>::VectorizePatch(image,
                                                              region2);

  VectorType projectedSource = projectionMatrix.transpose() * vectorizedSource;

  VectorType projectedTarget = projectionMatrix.transpose() * vectorizedTarget;

  // Compute distance between patches in projected space
  float projectedDistance = (projectedSource - projectedTarget).squaredNorm();

  return projectedDistance;
}

template <typename TImage>
float ProjectedDistance<TImage>::Distance(const itk::ImageRegion<2>& region1,
                                          const itk::ImageRegion<2>& region2)
{
  return Distance(this->Image, this->ProjectionMatrix, region1, region2);
}

template <typename TImage>
void ProjectedDistance<TImage>::SetImage(TImage* const image)
{
  this->Image = image;
}

template <typename TImage>
void ProjectedDistance<TImage>::SetProjectionMatrix(const MatrixType& projectionMatrix)
{
  this->ProjectionMatrix = ProjectionMatrix;
}

#endif
