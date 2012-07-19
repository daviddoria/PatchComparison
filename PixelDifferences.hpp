#ifndef PixelDifferences_HPP
#define PixelDifferences_HPP

#include "PixelDifferences.h"

#include <cmath>

namespace PixelDifferences
{

template <typename TPixel>
float SumOfSquaredDifferences(const TPixel &a, const TPixel &b)
{
  // This assumes that TPixel is a scalar
  float diff = a - b;
  return diff * diff;
}

template <typename TComponent>
float SumOfSquaredDifferences(const itk::VariableLengthVector<TComponent> &a,
                              const itk::VariableLengthVector<TComponent> &b)
{
  float difference = 0;

  float diff = 0;
  for(unsigned int i = 0; i < a.GetSize(); ++i)
    {
    diff = a[i] - b[i]; // even if a[i] and b[i] are unsigned which would lead to a potential invalid calculation, since the result is stored as float it works properly
    difference += diff * diff;
    }
  return difference;
}


template <typename TComponent, unsigned int TDimension>
float SumOfSquaredDifferences(const itk::CovariantVector<TComponent, TDimension>& a,
                              const itk::CovariantVector<TComponent, TDimension>& b)
{
  float difference = 0;

  float diff = 0;

  for(unsigned int i = 0; i < TDimension; ++i)
  {
    diff = a[i] - b[i]; // even if a[i] and b[i] are unsigned which would lead to a potential invalid calculation, since the result is stored as float it works properly
    difference += diff * diff;
  }

  return difference;
}

// template <typename TPixel>
// float SumOfAbsoluteDifferences::operator()(const TPixel &a, const TPixel &b)
// {
//   float difference = 0;
// 
//   for(unsigned int i = 0; i < a.GetSize(); ++i)
//     {
//     difference += fabs(a[i] - b[i]);
//     }
//   return difference;
// }

}

#endif
