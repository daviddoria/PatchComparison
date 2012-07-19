#ifndef SSD_H
#define SSD_H

// ITK
#include "itkImage.h"
#include "itkVectorImage.h"

#include "PatchDistance.h"

/** This class computes the average SSD of pixels in two specified regions.
  * Multiple image types (itk::Image<ScalarType, 2>, itk::VectorImage<ScalarType, 2>,
  * and itk::Image<itk::CovariantVector<ScalarType, Dimension> >
  * are handled because the PixelDifferences::SumOfSquaredDifferences can handle those pixel types.
  */
template <typename TImage>
struct SSD : public PatchDistance
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

  /** Set the image to use. */
  void SetImage(TImage* const image);

private:
  /** The image to use. */
  TImage* Image;
};

#include "SSD.hpp"

#endif
