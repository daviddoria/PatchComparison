#ifndef PixelDifferences_H
#define PixelDifferences_H

#include <cmath>

struct SumOfSquaredDifferences
{
  template <typename TPixel>
  float operator()(const TPixel &a, const TPixel &b);
};


struct SumOfAbsoluteDifferences
{
  template <typename TPixel>
  float operator()(const TPixel &a, const TPixel &b);
};

#endif
