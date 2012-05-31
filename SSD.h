#ifndef SSD_H
#define SSD_H

#include "itkImage.h"
#include "itkVectorImage.h"

struct SSDGeneral
{
  typedef itk::VectorImage<float, 2> ImageType;

  float operator()(const ImageType* const image, const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);
};

template <unsigned int TDimension>
struct SSD
{
  typedef itk::Image<itk::CovariantVector<float, TDimension>, 2> ImageType;

  float operator()(const ImageType* const image, const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);
};

#include "SSD.hpp"

#endif
