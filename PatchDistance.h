#ifndef PatchDistance_H
#define PatchDistance_H

// ITK
#include "itkImageRegion.h"

template <typename TImage>
struct PatchDistance
{
  /** Compute the distance between two regions. */
  virtual float Distance(const itk::ImageRegion<2>& region1,
                         const itk::ImageRegion<2>& region2) = 0;

  /** Set the target patch if it is needed. For example, SelfPatchCompareLocalOptimization needs to do this.
    * This function is not needed by all subclasses, so it is not pure virtual. */
  virtual void SetTargetPatch(const itk::ImageRegion<2>& region)
  {
  }

  /** Set the image to use. */
  void SetImage(TImage* const image);

  /** Retrieve the name of the distance object. */
  virtual std::string GetDistanceName() = 0;

  /** Set a modifier to make the distance name more specific. */
  void SetDistanceNameModifier(const std::string& distanceNameModifier);

protected:
  /** The image to use. */
  TImage* Image;

  /** A more detailed description of the distance name. For example, if we are using a HistogramDistance object
    * to compute the histogram of an HSV image, we would set DistanceNameModifier to "HSV". */
  std::string DistanceNameModifier;
};

#include "PatchDistance.hpp"

#endif
