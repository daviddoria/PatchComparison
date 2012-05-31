#ifndef VarianceScore_H
#define VarianceScore_H

/**

 */
struct VarianceDifference
{
  template <typename TImage>
  float operator()(const TImage* const image,
                   const itk::ImageRegion<2>& sourceRegion, const itk::ImageRegion<2>& targetRegion);
};

#include "VarianceDifference.hpp"

#endif
