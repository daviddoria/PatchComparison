#ifndef PatchComparison_H
#define PatchComparison_H

#include "itkImage.h"
#include "itkVectorImage.h"

class PatchComparison
{
public:

  PatchComparison();

  typedef itk::VectorImage<float, 2> ImageType;

  typedef itk::Image<float, 2> VarianceImageType;

  /** The SSD of the pixel components divided by the number of pixels. **/
  float ComputeAverageSSD(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);

  float ComputeAveragePixelDifference(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);

  float ComputeVarianceDifference(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);

  float ComputeVarianceWeightedSSD(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);

  float ComputeCorrelationScore(const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);

  void GenerateVarianceImage(const unsigned int kernelRadius);

  void SetImage(ImageType* const image);

private:
  ImageType::Pointer Image;

  VarianceImageType::Pointer VarianceImage;
};

#endif
