#ifndef VarianceScore_HPP
#define VarianceScore_HPP

#include "VarianceDifference.h"

// Submodules
#include "Mask/ITKHelpers/ITKHelpers.h"

template <typename TImage>
float VarianceDifference::operator()(const TImage* const image,
                  const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  typename TypeTraits<typename TImage::PixelType>::LargerType variance1 = ITKHelpers::VarianceInRegion(image, region1);
  typename TypeTraits<typename TImage::PixelType>::LargerType variance2 = ITKHelpers::VarianceInRegion(image, region2);

  typename TypeTraits<typename TImage::PixelType>::LargerType varianceDifference = variance1 - variance2;
  float sumVarianceDifference = ITKHelpers::SumOfComponentMagnitudes(varianceDifference);

  return sumVarianceDifference;
}

#endif
