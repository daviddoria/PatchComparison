#ifndef SSD_H
#define SSD_H

#include "itkImage.h"
#include "itkVectorImage.h"

/** This class computes the average SSD of pixels in two specified regions.
  * Multiple image types (itk::Image<ScalarType, 2>, itk::VectorImage<ScalarType, 2>,
  * and itk::Image<itk::CovariantVector<ScalarType, Dimension> >
  * are handled because the PixelDifferences::SumOfSquaredDifferences can handle those pixel types.
  */
template <typename TImage>
struct SSD
{
  static float Difference(const TImage* const image, const itk::ImageRegion<2>& region1,
                          const itk::ImageRegion<2>& region2);

  float Difference(const itk::ImageRegion<2>& region1,
                   const itk::ImageRegion<2>& region2);

  TImage* Image;
};

#include "SSD.hpp"

#endif
