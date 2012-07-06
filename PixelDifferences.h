#ifndef PixelDifferences_H
#define PixelDifferences_H

// STL
#include <cmath>

// ITK
#include "itkCovariantVector.h"
#include "itkVariableLengthVector.h"

namespace PixelDifferences
{
  /** For scalar pixel types */
  template <typename TPixel>
  float SumOfSquaredDifferences(const TPixel &a, const TPixel &b);

  /** For VariableLengthVector<T> pixel types */
  template <typename TPixel>
  float SumOfSquaredDifferences(const itk::VariableLengthVector<TPixel> &a, const itk::VariableLengthVector<TPixel> &b);

  /** For CovariantVector<T, N> pixel types */
  template <typename TPixel, unsigned int TDimension>
  float SumOfSquaredDifferences(const itk::CovariantVector<TPixel, TDimension>& a, const itk::CovariantVector<TPixel, TDimension>& b);
};

// struct SumOfAbsoluteDifferences
// {
//   template <typename TPixel>
//   float operator()(const TPixel &a, const TPixel &b);
// };

#include "PixelDifferences.hpp"

#endif
