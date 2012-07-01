#include "PatchComparison.h"

#include "SelfDeviationWeightedSSD.h"

// STL
#include <iostream>

void TestSelfDeviationWeightedSSD();

int main( int argc, char ** argv )
{
  TestSelfDeviationWeightedSSD();

  return 0;
}

void TestSelfDeviationWeightedSSD()
{
  typedef itk::Image<unsigned char, 2> ImageType;
  ImageType::Pointer image = ImageType::New();
  itk::Index<2> corner = {{0,0}};
  itk::Size<2> size = {{3,3}};
  itk::ImageRegion<2> region(corner, size);
  image->SetRegions(region);
  image->Allocate();

  /* Image:
    255 51 102
    227 102 102
    255 77 99
    */
  itk::Index<2> currentIndex = {{0,0}};
  image->SetPixel(currentIndex, 255);
  
  currentIndex[0] = 0; currentIndex[1] = 1;
  image->SetPixel(currentIndex, 51);

  currentIndex[0] = 0; currentIndex[1] = 2;
  image->SetPixel(currentIndex, 102);

  currentIndex[0] = 1; currentIndex[1] = 0;
  image->SetPixel(currentIndex, 227);

  currentIndex[0] = 1; currentIndex[1] = 1;
  image->SetPixel(currentIndex, 102);

  currentIndex[0] = 1; currentIndex[1] = 2;
  image->SetPixel(currentIndex, 102);

  currentIndex[0] = 2; currentIndex[1] = 0;
  image->SetPixel(currentIndex, 255;

  currentIndex[0] = 2; currentIndex[1] = 1;
  image->SetPixel(currentIndex, 77);

  currentIndex[0] = 2; currentIndex[1] = 2;
  image->SetPixel(currentIndex, 99);

  /* Expected deviations:
    145.19 124.99 90.60
    121.60 90.60 90.60
    145.19 105.41 92.10
    */

  /* Expected weights:
     0 0 .31
     0 .31 .31
     0 0 .069
  */
}
