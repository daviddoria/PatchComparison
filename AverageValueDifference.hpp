#ifndef AverageValueDifference_HPP
#define AverageValueDifference_HPP

// Appease syntax parser
#include "AverageValueDifference.h"

// Submodules
#include "Mask/ITKHelpers/ITKHelpers.h"

/**
  * Compute the difference between the averages of each patch.
 */

template <typename TImage>
float AverageValueDifference::operator()(const TImage* const image,
                                         const itk::ImageRegion<2>& region1,
                                         const itk::ImageRegion<2>& region2)
{
  typename TypeTraits<typename TImage::PixelType>::LargerType region1Average = ITKHelpers::AverageInRegion(image, region1);

  typename TypeTraits<typename TImage::PixelType>::LargerType region2Average = ITKHelpers::AverageInRegion(image, region2);

  typename TypeTraits<typename TImage::PixelType>::LargerType averageDifference = region1Average - region2Average;

  float sumAverageDifference = ITKHelpers::SumOfComponentMagnitudes(averageDifference);

  return sumAverageDifference;
}

#endif
