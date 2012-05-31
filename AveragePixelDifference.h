#ifndef AveragePixelDifference_H
#define AveragePixelDifference_H

#include "itkVectorImage.h"
/**
  * Compute the difference between the averages of each patch.
 */

struct AveragePixelDifference
{
  template <typename TImage>
  float operator()(const TImage* const image,
                   const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);
};

#include "AveragePixelDifference.hpp"

#endif
