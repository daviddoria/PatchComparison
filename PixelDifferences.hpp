#ifndef PixelDifferences_H
#define PixelDifferences_H

#include <cmath>

struct SumOfSquaredDifferences
{
  template <typename TPixel>
  float operator()(const TPixel &a, const TPixel &b)
  {
    float difference = 0;

    float diff = 0;
    for(unsigned int i = 0; i < a.GetSize(); ++i)
      {
      diff = a[i] - b[i];
      difference += diff * diff;
      }
    return difference;
  }
};


struct SumOfAbsoluteDifferences
{
  template <typename TPixel>
  float operator()(const TPixel &a, const TPixel &b)
  {
    float difference = 0;

    for(unsigned int i = 0; i < a.GetSize(); ++i)
      {
      difference += fabs(a[i] - b[i]);
      }
    return difference;
  }
};

#endif
