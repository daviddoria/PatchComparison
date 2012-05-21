#ifndef PatchComparison_H
#define PatchComparison_H

class PatchComparison
{
  float Compare(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);
};

#endif
