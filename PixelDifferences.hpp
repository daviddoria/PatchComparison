#ifndef PixelDifferences_HPP
#define PixelDifferences_HPP

#include "PixelDifferences.h"

#include <cmath>

template <typename TPixel>
float SumOfSquaredDifferencesGeneral::operator()(const TPixel &a, const TPixel &b)
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

template <unsigned int TDimension>
float SumOfSquaredDifferences<TDimension>::operator()(const PixelType &a, const PixelType &b)
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

template <typename TPixel>
float SumOfAbsoluteDifferences::operator()(const TPixel &a, const TPixel &b)
{
  float difference = 0;

  for(unsigned int i = 0; i < a.GetSize(); ++i)
    {
    difference += fabs(a[i] - b[i]);
    }
  return difference;
}


#endif
