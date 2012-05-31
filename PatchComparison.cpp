#include "PatchComparison.h"

// Submodules
#include "Mask/ITKHelpers/ITKHelpers.h"

#include "CorrelationScore.h"
#include "SSD.h"
#include "AveragePixelDifference.h"
#include "VarianceDifference.h"
#include "VarianceWeightedSSD.h"

PatchComparison::PatchComparison()
{
  this->Image = ImageType::New();
  this->VarianceImage = VarianceImageType::New();
}

float PatchComparison::ComputeAverageSSD(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  SSD ssd;
  return ssd(this->Image.GetPointer(), region1, region2);
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

float PatchComparison::ComputeCorrelationScore(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  CorrelationScore correlationScore;
  return correlationScore(this->Image.GetPointer(), region1, region2);
}

float PatchComparison::ComputeVarianceWeightedSSD(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  VarianceWeightedSSD varianceWeightedSSD;
  return varianceWeightedSSD(this->Image.GetPointer(), region1, region2);
}

void PatchComparison::SetImage(ImageType* const image)
{
  ITKHelpers::DeepCopy(image, this->Image.GetPointer());
}
