#ifndef PatchComparison_H
#define PatchComparison_H

#include "itkVectorImage.h"

class PatchComparison
{
public:

  typedef itk::VectorImage<float, 2> ImageType;

  /** The SSD of the pixel components divided by the number of pixels. **/
  float ComputeAverageSSD(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);

  float ComputeAveragePixelDifference(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);

  float ComputeVarianceDifference(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);

private:
  ImageType::Pointer Image;

};

#endif
