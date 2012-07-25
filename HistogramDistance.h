#ifndef HistogramDistance_H
#define HistogramDistance_H

// ITK
#include "itkImage.h"
#include "itkVectorImage.h"

#include "PatchDistance.h"

/** This class computes the histogram distance of pixels in two specified regions.
  */
template <typename TImage>
struct HistogramDistance : public PatchDistance<TImage>
{
  /** The single image distance function that does not use the internal data. */
  static float Distance(const TImage* const image, const itk::ImageRegion<2>& region1,
                        const itk::ImageRegion<2>& region2);

  /** The two image distance function that does not use the internal data. */
  static float Distance(const TImage* const image1, const itk::ImageRegion<2>& region1,
                        const TImage* const image2, const itk::ImageRegion<2>& region2);

  /** The distance function that uses the internal data. */
  float Distance(const itk::ImageRegion<2>& region1,
                 const itk::ImageRegion<2>& region2);

};

#include "HistogramDistance.hpp"

#endif
