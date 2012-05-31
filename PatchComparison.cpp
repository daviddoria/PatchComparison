#include "PatchComparison.h"

#include "SSD.h"
#include "AveragePixelDifference.h"
#include "VarianceDifference.h"

float PatchComparison::ComputeAverageSSD(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  return AverageSSD(this->Image.GetPointer(), region1, region2);
}

float PatchComparison::ComputeAveragePixelDifference(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  AveragePixelDifference averagePixelDifference;
  return averagePixelDifference(this->Image.GetPointer(), region1, region2);
}

float PatchComparison::ComputeVarianceDifference(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  VarianceDifference varianceDifference;
  return varianceDifference(this->Image.GetPointer(), region1, region2);
}
