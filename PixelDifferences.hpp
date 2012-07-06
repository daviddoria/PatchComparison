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

template <typename TPixel>
float SumOfSquaredDifferences(const itk::VariableLengthVector<TPixel> &a,
                              const itk::VariableLengthVector<TPixel> &b)
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


template <typename TPixel, unsigned int TDimension>
float SumOfSquaredDifferences(const itk::CovariantVector<TPixel, TDimension>& a,
                              const itk::CovariantVector<TPixel, TDimension>& b)
{
  float difference = 0;

  float diff = 0;

  for(unsigned int i = 0; i < TDimension; ++i)
  {
    diff = a[i] - b[i];
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
