#include "SSD.h"

#include "itkImageRegionConstIterator.h"

#include "PixelDifferences.h"

float SSD::operator()(const ImageType* const image, const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  assert(region1.GetSize() == region2.GetSize());
  assert(image->GetLargestPossibleRegion().IsInside(region1));
  assert(image->GetLargestPossibleRegion().IsInside(region2));

  itk::ImageRegionConstIterator<ImageType> patch1Iterator(image, region1);
  itk::ImageRegionConstIterator<ImageType> patch2Iterator(image, region2);

  float sumSquaredDifferences = 0;

  while(!patch1Iterator.IsAtEnd())
    {
    itk::Index<2> currentPixel = patch1Iterator.GetIndex();

    ImageType::PixelType pixel1 = patch1Iterator.Get();
    ImageType::PixelType pixel2 = patch2Iterator.Get();

    SumOfSquaredDifferences sumOfSquaredDifferencesFunctor;
    float squaredDifference = sumOfSquaredDifferencesFunctor(pixel1, pixel2);

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
