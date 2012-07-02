#include "PixelDifferences.h"

// STL
#include <iostream>

// ITK
#include "itkVariableLengthVector.h"

int main( int argc, char ** argv )
{
  {
  itk::VariableLengthVector<float> a(3);
  a.Fill(10.0f);

  itk::VariableLengthVector<float> b(3);
  b.Fill(21.0f);

  SumOfSquaredDifferencesGeneral diffOperator;
  float diff = diffOperator(a,b);
  std::cout << "Diff = " << diff << std::endl;
  }

  {
  float a = 10.0f;
  float b = 21.0f;
  SumOfSquaredDifferencesGeneral diffOperator;
  float diff = diffOperator(a,b);
  std::cout << "Diff = " << diff << std::endl;
  }

// template <unsigned int TDimension>
// struct SumOfSquaredDifferences
// {
//   typedef itk::CovariantVector<float, TDimension> PixelType;
//   float operator()(const PixelType &a, const PixelType &b);
// };
//
//
// struct SumOfAbsoluteDifferences
// {
//   template <typename TPixel>
//   float operator()(const TPixel &a, const TPixel &b);
// };

  return 0;
}
