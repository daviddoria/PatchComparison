#ifndef HistogramDistance_HPP
#define HistogramDistance_HPP

#include "HistogramDistance.h"

#include "PixelDifferences.h"

#include "itkImageRegionConstIterator.h"

template <typename TImage>
float HistogramDistance<TImage>::Distance(const TImage* const image1, const itk::ImageRegion<2>& region1,
                                          const TImage* const image2, const itk::ImageRegion<2>& region2)
{
  assert(region1.GetSize() == region2.GetSize());
  assert(image1->GetLargestPossibleRegion().IsInside(region1));
  assert(image2->GetLargestPossibleRegion().IsInside(region2));

  itk::ImageRegionConstIterator<TImage> patch1Iterator(image1, region1);
  itk::ImageRegionConstIterator<TImage> patch2Iterator(image2, region2);

  float sumSquaredDifferences = 0.0f;

  while(!patch1Iterator.IsAtEnd())
    {
    float squaredDifference = PixelDifferences::SumOfSquaredDifferences(patch1Iterator.Get(), patch2Iterator.Get());

//       std::cout << "Source pixel: " << static_cast<unsigned int>(sourcePixel)
//                 << " target pixel: " << static_cast<unsigned int>(targetPixel)
//                 << "Difference: " << difference << " squaredDifference: " << squaredDifference << std::endl;

    sumSquaredDifferences +=  squaredDifference;

    ++patch1Iterator;
    ++patch2Iterator;
    } // end while iterate over sourcePatch

  unsigned int numberOfPixels = region1.GetNumberOfPixels();

  float averageSSD = sumSquaredDifferences / static_cast<float>(numberOfPixels);

  return averageSSD;
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
