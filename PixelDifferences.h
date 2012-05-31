#ifndef PixelDifferences_H
#define PixelDifferences_H

#include <cmath>

struct SumOfSquaredDifferencesGeneral
{
  template <typename TPixel>
  float operator()(const TPixel &a, const TPixel &b);
};

template <unsigned int TDimension>
struct SumOfSquaredDifferences
{
  typedef itk::CovariantVector<float, TDimension> PixelType;
  float operator()(const PixelType &a, const PixelType &b);
};


struct SumOfAbsoluteDifferences
{
  template <typename TPixel>
  float operator()(const TPixel &a, const TPixel &b);
};

#include "PixelDifferences.hpp"

#endif
