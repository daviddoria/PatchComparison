#ifndef AverageValueDifference_H
#define AverageValueDifference_H

#include "itkVectorImage.h"

/**
  * Compute the difference between the average value of each patch.
 */
struct AverageValueDifference
{
  template <typename TImage>
  float Difference(const TImage* const image,
                   const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);
};

#include "AverageValueDifference.hpp"

#endif
