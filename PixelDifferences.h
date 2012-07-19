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
  template <typename TComponent>
  float SumOfSquaredDifferences(const itk::VariableLengthVector<TComponent> &a,
                                const itk::VariableLengthVector<TComponent> &b);

  /** For CovariantVector<T, N> pixel types */
  template <typename TComponent, unsigned int TDimension>
  float SumOfSquaredDifferences(const itk::CovariantVector<TComponent, TDimension>& a,
                                const itk::CovariantVector<TComponent, TDimension>& b);
};

// struct SumOfAbsoluteDifferences
// {
//   template <typename TPixel>
//   float operator()(const TPixel &a, const TPixel &b);
// };

#include "PixelDifferences.hpp"

#endif
