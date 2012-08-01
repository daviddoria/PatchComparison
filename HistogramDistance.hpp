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

  itk::ImageRegionConstIterator<TImage> patch1Iterator(image1, region1);
  itk::ImageRegionConstIterator<TImage> patch2Iterator(image2, region2);

  unsigned int numberOfBinsPerDimension = 20;
  typename TypeTraits<typename TImage::PixelType>::ComponentType rangeMin = 0;
  typename TypeTraits<typename TImage::PixelType>::ComponentType rangeMax = 255;

  Histogram::HistogramType histogram1 = Histogram::Compute1DConcatenatedHistogramOfMultiChannelImage(
                                        image1, region1, numberOfBinsPerDimension,
                                        rangeMin, rangeMax);

  Histogram::HistogramType histogram2 = Histogram::Compute1DConcatenatedHistogramOfMultiChannelImage(
                                        image2, region2, numberOfBinsPerDimension,
                                        rangeMin, rangeMax);

  float distance = Histogram::HistogramDifference(histogram1, histogram2);
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
