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
}

template <typename TPixel>
float VarianceWeightedSSD::operator()(const itk::VectorImage<TPixel, 2>* const image, const itk::Image<float, 2>* const varianceImage,
                  const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  typedef itk::VectorImage<TPixel, 2> ImageType;
  typedef itk::Image<float, 2> VarianceImageType;

  // Multiple use filter types
  typedef itk::RegionOfInterestImageFilter<ImageType, ImageType> VarianceExtractFilterType;
  typedef itk::NormalizeToConstantImageFilter<ImageType, ImageType> NormalizeVarianceToConstantImageFilterType;

  // Normalize weights for patch 1
  typename VarianceExtractFilterType::Pointer extractVarianceFilter1 = VarianceExtractFilterType::New();
  extractVarianceFilter1->SetRegionOfInterest(region1);
  extractVarianceFilter1->SetInput(varianceImage);
  extractVarianceFilter1->Update();

  typename NormalizeVarianceToConstantImageFilterType::Pointer normalizeVarianceFilter1 = NormalizeVarianceToConstantImageFilterType::New();
  normalizeVarianceFilter1->SetInput(extractVarianceFilter1->GetOutput());
  normalizeVarianceFilter1->SetConstant(1.0f);
  normalizeVarianceFilter1->Update();

  // Normalize weights for patch 2
  typename VarianceExtractFilterType::Pointer extractVarianceFilter2 = VarianceExtractFilterType::New();
  extractVarianceFilter2->SetRegionOfInterest(region2);
  extractVarianceFilter2->SetInput(varianceImage);
  extractVarianceFilter2->Update();

  typename NormalizeVarianceToConstantImageFilterType::Pointer normalizeVarianceFilter2 = NormalizeVarianceToConstantImageFilterType::New();
  normalizeVarianceFilter2->SetInput(extractVarianceFilter2->GetOutput());
  normalizeVarianceFilter2->SetConstant(1.0f);
  normalizeVarianceFilter2->Update();

  // Sum the weighted differences

  itk::ImageRegionConstIterator<ImageType> region1Iterator(image, region1);
  itk::ImageRegionConstIterator<ImageType> region2Iterator(image, region2);
  itk::ImageRegionConstIterator<VarianceImageType> weights1Iterator(normalizeVarianceFilter1->GetOutput(),
                                                                    normalizeVarianceFilter1->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionConstIterator<VarianceImageType> weights2Iterator(normalizeVarianceFilter2->GetOutput(),
                                                                    normalizeVarianceFilter2->GetOutput()->GetLargestPossibleRegion());

  float sumDifferences = 0.0f;

  SSD ssdFunctor;

  unsigned int numberOfPixels = region1.GetNumberOfPixels();
  while(!region1Iterator.IsAtEnd())
    {
    float difference = weights1Iterator.Get() * weights2Iterator.Get() *
                        ssdFunctor(region1Iterator.Get() - region2Iterator.Get());

    sumDifferences +=  difference;

    ++region1Iterator;
    ++region2Iterator;
    ++weights1Iterator;
    ++weights2Iterator;
    }

  float averageDifference = sumDifferences / static_cast<float>(numberOfPixels);
  return averageDifference;
}

#endif
