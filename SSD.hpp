#ifndef SSD_HPP
#define SSD_HPP

#include "SSD.h"

#include "PixelDifferences.h"

#include "itkImageRegionConstIterator.h"

template <typename TImage>
float SSD<TImage>::Distance
(const TImage* const image, const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  assert(region1.GetSize() == region2.GetSize());
  assert(image->GetLargestPossibleRegion().IsInside(region1));
  assert(image->GetLargestPossibleRegion().IsInside(region2));

  itk::ImageRegionConstIterator<TImage> patch1Iterator(image, region1);
  itk::ImageRegionConstIterator<TImage> patch2Iterator(image, region2);

  float sumSquaredDifferences = 0;

  typename TImage::PixelType pixel1;
  typename TImage::PixelType pixel2;

  while(!patch1Iterator.IsAtEnd())
    {
    pixel1 = patch1Iterator.Get();
    pixel2 = patch2Iterator.Get();

    float squaredDifference = PixelDifferences::SumOfSquaredDifferences(pixel1, pixel2);

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
float SSD<TImage>::Distance(const itk::ImageRegion<2>& region1,
                            const itk::ImageRegion<2>& region2)
{
  return Distance(this->Image, region1, region2);
}

#endif
