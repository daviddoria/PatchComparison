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

protected:
  /** The image to use. */
  TImage* Image;
};

#include "PatchDistance.hpp"

#endif
