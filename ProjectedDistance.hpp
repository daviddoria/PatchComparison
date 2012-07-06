#ifndef ProjectedDistance_H
#define ProjectedDistance_H

#include "ProjectedDistance.h"

// Submodules
#include "PatchProjection/PatchProjection.h"

template <unsigned int TDimension>
float ProjectedDistance<TDimension>::operator()(const ImageType* const image,
                                                const itk::ImageRegion<2>& region1,
                                                const itk::ImageRegion<2>& region2)
{
  VectorType vectorizedSource = PatchProjection<MatrixType, VectorType>::VectorizePatch(this->Image.GetPointer(),
                                                              region1);

  VectorType vectorizedTarget = PatchProjection<MatrixType, VectorType>::VectorizePatch(this->Image.GetPointer(),
                                                              region2);

  VectorType projectedSource = this->ProjectionMatrix.transpose() * vectorizedSource;

  VectorType projectedTarget = this->ProjectionMatrix.transpose() * vectorizedTarget;

  // Compute distance between patches in projected space
  float projectedDistance = (projectedSource - projectedTarget).squaredNorm();
  
  return projectedDistance;
}

template <unsigned int TDimension>
float ProjectedDistance<TDimension>::operator()(const itk::ImageRegion<2>& region1,
                                                const itk::ImageRegion<2>& region2)
{
  return operator()(this->Image, region1, region2);
}

#endif
