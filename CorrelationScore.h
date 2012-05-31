#ifndef CorrelationScore_H
#define CorrelationScore_H

struct CorrelationScore
{
  template <typename TImage>
  float operator()(TImage* const image, const itk::ImageRegion<2>& region1,
                   const itk::ImageRegion<2>& region2);
};

#include "CorrelationScore.hpp"

#endif
