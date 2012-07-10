#ifndef TruncatedQuadraticDifference_H
#define TruncatedQuadraticDifference_H

#include "itkImage.h"
#include "itkVectorImage.h"

template <typename TImage>
struct TruncatedQuadraticDifference
{
  static float Difference(const TImage* const image, const float truncationPoint,
                          const itk::ImageRegion<2>& region1,
                          const itk::ImageRegion<2>& region2);

  float Difference(const itk::ImageRegion<2>& region1,
                   const itk::ImageRegion<2>& region2);

  TImage* Image;

  float TruncationPoint;
};

#include "TruncatedQuadraticDifference.hpp"

#endif
