#ifndef SSD_H
#define SSD_H

#include "itkVectorImage.h"

struct SSD
{
  typedef itk::VectorImage<float, 2> ImageType;

  float operator()(const ImageType* const image, const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);
};

#endif
