#ifndef PatchDistance_H
#define PatchDistance_H

// ITK
#include "itkImageRegion.h"

struct PatchDistance
{
  virtual float Distance(const itk::ImageRegion<2>& region1,
                         const itk::ImageRegion<2>& region2) = 0;

  virtual void SetTargetPatch(const itk::ImageRegion<2>& region) {}
};

#endif
