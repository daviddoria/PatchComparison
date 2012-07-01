/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef VarianceWeightedSSD_HPP
#define VarianceWeightedSSD_HPP

#include "VarianceWeightedSSD.h"

// Submodules
#include "Mask/ITKHelpers/Helpers/Helpers.h"

// ITK
#include "itkImageRegion.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkNormalizeToConstantImageFilter.h"

// STL
#include <vector>

template <typename TPixel>
float VarianceWeightedSSD::operator()(const itk::VectorImage<TPixel, 2>* const image,
                  const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  // Compute the variance image in the required regions
  typedef itk::VectorImage<TPixel, 2> ImageType;

  std::vector<float> weightVector(region1.GetNumberOfPixels());

  itk::ImageRegionConstIterator<ImageType> region1Iterator(image, region1);

  unsigned int pixelCounter = 0;
  while(!region1Iterator.IsAtEnd())
    {
    itk::ImageRegion<2> region =
           ITKHelpers::GetRegionInRadiusAroundPixel(region1Iterator.GetIndex(), region1.GetSize()[0]/2);

    float variance = ITKHelpers::VarianceInRegion(image, region);
    weightVector[pixelCounter] = variance;
    pixelCounter++;
    ++region1Iterator;
    }

  // TODO: Normalize weight vector
  Helpers::NormalizeVector(weightVector);

  return WeightedSSD(image, region1, region2, weightVector);
}

template <typename TPixel>
float VarianceWeightedSSD::operator()(const itk::VectorImage<TPixel, 2>* const image,
                                      const itk::Image<float, 2>* const varianceImage,
                                      const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  typedef itk::VectorImage<TPixel, 2> ImageType;
  typedef itk::Image<float, 2> VarianceImageType;

  // Multiple use filter types
  typedef itk::RegionOfInterestImageFilter<ImageType, ImageType> VarianceExtractFilterType;
  typedef itk::NormalizeToConstantImageFilter<ImageType, ImageType>
          NormalizeVarianceToConstantImageFilterType;

  // Normalize weights for patch
  typename VarianceExtractFilterType::Pointer extractVarianceFilter = VarianceExtractFilterType::New();
  extractVarianceFilter->SetRegionOfInterest(region1);
  extractVarianceFilter->SetInput(varianceImage);
  extractVarianceFilter->Update();

  typename NormalizeVarianceToConstantImageFilterType::Pointer normalizeVarianceFilter =
           NormalizeVarianceToConstantImageFilterType::New();
  normalizeVarianceFilter->SetInput(extractVarianceFilter->GetOutput());
  normalizeVarianceFilter->SetConstant(1.0f);
  normalizeVarianceFilter->Update();

  // Sum the weighted differences
  itk::ImageRegionConstIterator<ImageType> region1Iterator(image, region1);
  itk::ImageRegionConstIterator<ImageType> region2Iterator(image, region2);
  itk::ImageRegionConstIterator<VarianceImageType>
         weightsIterator(normalizeVarianceFilter->GetOutput(),
                         normalizeVarianceFilter->GetOutput()->GetLargestPossibleRegion());

  float sumDifferences = 0.0f;

  SSDGeneral ssdFunctor;

  unsigned int numberOfPixels = region1.GetNumberOfPixels();
  while(!region1Iterator.IsAtEnd())
    {
    float difference = weightsIterator.Get() *
                        ssdFunctor(region1Iterator.Get() - region2Iterator.Get());

    sumDifferences +=  difference;

    ++region1Iterator;
    ++region2Iterator;
    ++weightsIterator;
    }

  float averageDifference = sumDifferences / static_cast<float>(numberOfPixels);
  return averageDifference;
}

#endif
