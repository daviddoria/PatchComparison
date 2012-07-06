#ifndef SSD_H
#define SSD_H

#include "itkImage.h"
#include "itkVectorImage.h"

// Generic - we will not allow this (simply by not implementing any functions)
template <typename TImage>
struct SSD
{
};

// itk::Image<scalar, 2> images
template <>
template <typename TPixel>
struct SSD<itk::Image<TPixel, 2> >
{
  typedef itk::Image<TPixel, 2> ImageType;
  
  static float Difference(const ImageType* const image, const itk::ImageRegion<2>& region1,
                          const itk::ImageRegion<2>& region2);

  float Difference(const itk::ImageRegion<2>& region1,
                   const itk::ImageRegion<2>& region2);

  ImageType* Image;
};

// itk::VectorImage<scalar, 2> images
template <>
template <typename TPixel>
struct SSD<itk::VectorImage<TPixel, 2> >
{
  typedef itk::VectorImage<TPixel, 2> ImageType;

  static float Difference(const ImageType* const image, const itk::ImageRegion<2>& region1,
                   const itk::ImageRegion<2>& region2);
    
  float Difference(const itk::ImageRegion<2>& region1,
                   const itk::ImageRegion<2>& region2);

  ImageType* Image;
};

// itk::Image<itk::CovariantVector<scalar, N>, 2> images
template <>
template <unsigned int TDimension, typename TPixel>
struct SSD<itk::Image<itk::CovariantVector<TPixel, TDimension>, 2> >
{
  typedef itk::Image<itk::CovariantVector<TPixel, TDimension>, 2> ImageType;

  float Difference(const itk::ImageRegion<2>& region1,
                   const itk::ImageRegion<2>& region2);
  
  static float Difference(const ImageType* const image, const itk::ImageRegion<2>& region1,
                   const itk::ImageRegion<2>& region2);

  ImageType* Image;
};

#include "SSD.hpp"

#endif
