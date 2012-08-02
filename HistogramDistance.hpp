#ifndef HistogramDistance_HPP
#define HistogramDistance_HPP

#include "HistogramDistance.h"

// ITK
#include "itkImageRegionConstIterator.h"

// Submodules
#include <Histogram/Histogram.h>
#include <Helpers/TypeTraits.h>

template <typename TImage>
float HistogramDistance<TImage>::Distance(const TImage* const image1, const itk::ImageRegion<2>& region1,
                                          const TImage* const image2, const itk::ImageRegion<2>& region2)
{
  assert(region1.GetSize() == region2.GetSize());
  assert(image1->GetLargestPossibleRegion().IsInside(region1));
  assert(image2->GetLargestPossibleRegion().IsInside(region2));


  unsigned int numberOfBinsPerDimension = 20;
  typename TypeTraits<typename TImage::PixelType>::ComponentType rangeMin = 0;
  typename TypeTraits<typename TImage::PixelType>::ComponentType rangeMax = 255;

  std::cout << "rangeMin: " << static_cast<int>(rangeMin) << " rangeMax: " << static_cast<int>(rangeMax) << std::endl;
  
  Histogram<int>::HistogramType histogram1 = Histogram<int>::ComputeImageHistogram1D(
                                        image1, region1, numberOfBinsPerDimension,
                                        rangeMin, rangeMax);

  Histogram<int>::HistogramType histogram2 = Histogram<int>::ComputeImageHistogram1D(
                                        image2, region2, numberOfBinsPerDimension,
                                        rangeMin, rangeMax);

  std::cout << "Histogram 1: " << std::endl;
  Histogram<int>::OutputHistogram(histogram1); std::cout << std::endl;

  std::cout << "Histogram 2: " << std::endl;
  Histogram<int>::OutputHistogram(histogram2); std::cout << std::endl;
  
  float distance = Histogram<int>::HistogramDifference(histogram1, histogram2);
  return distance;
}

template <typename TImage>
float HistogramDistance<TImage>::Distance
(const TImage* const image, const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  return Distance(image, region1, image, region2);
}

template <typename TImage>
float HistogramDistance<TImage>::Distance(const itk::ImageRegion<2>& region1,
                            const itk::ImageRegion<2>& region2)
{
  return Distance(this->Image, region1, region2);
}

#endif
