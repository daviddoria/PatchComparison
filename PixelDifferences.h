#ifndef PixelDifferences_H
#define PixelDifferences_H

// STL
#include <cmath>

// ITK
#include "itkCovariantVector.h"
#include "itkVariableLengthVector.h"

struct SumOfSquaredDifferencesGeneral
{
  /** For scalar pixel types */
  template <typename TPixel>
  float operator()(const TPixel &a, const TPixel &b);

  /** For VariableLengthVector<T> pixel types */
  template <typename TPixel>
  float operator()(const itk::VariableLengthVector<TPixel> &a, const itk::VariableLengthVector<TPixel> &b);
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
