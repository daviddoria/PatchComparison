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

#ifndef AveragePixelDifference_HPP
#define AveragePixelDifference_HPP

// ITK
#include "itkImageRegion.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkNormalizeToConstantImageFilter.h"

// STL
#include <vector>

class VarianceWeigtedSSD
{
public:
  /**
   * This function operates on a single channel image.
   */
  template <typename TPixel>
  float operator()(const itk::Image<TPixel, 2>* const image, const itk::Image<float, 2>* const varianceImage,
                   const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
  {
    typedef itk::Image<TPixel, 2> ImageType;
    typedef itk::Image<float, 2> VarianceImageType;

    // Multiple use filter types
    typedef itk::RegionOfInterestImageFilter<ImageType, ImageType> ExtractFilterType;
    typedef itk::NormalizeToConstantImageFilter<ImageType, ImageType> NormalizeToConstantImageFilterType;

    // Normalize weights for patch 1
    ExtractFilterType::Pointer extractFilter1 = ExtractFilterType::New();
    extractFilter1->SetRegionOfInterest(region1);
    extractFilter1->SetInput(varianceImage);
    extractFilter1->Update();

    NormalizeToConstantImageFilterType::Pointer normalizeImageFilter1 = NormalizeToConstantImageFilterType::New();
    normalizeImageFilter1->SetInput(extractFilter1->GetOutput());
    normalizeImageFilter1->SetConstant(1);
    normalizeImageFilter1->Update();

    // Normalize weights for patch 2
    ExtractFilterType::Pointer extractFilter2 = ExtractFilterType::New();
    extractFilter2->SetRegionOfInterest(region2);
    extractFilter2->SetInput(image);
    extractFilter2->Update();

    NormalizeToConstantImageFilterType::Pointer normalizeImageFilter2 = NormalizeToConstantImageFilterType::New();
    normalizeImageFilter2->SetInput(extractFilter2->GetOutput());
    normalizeImageFilter2->SetConstant(1);
    normalizeImageFilter2->Update();

    // Sum the weighted differences

    itk::ImageRegionConstIterator<ImageType> region1Iterator(image, region1);
    itk::ImageRegionConstIterator<ImageType> region2Iterator(image, region2);
    itk::ImageRegionConstIterator<VarianceImageType> weights1Iterator(normalizeImageFilter1->GetOutput(),
                                                                       normalizeImageFilter1->GetOutput()->GetLargestPossibleRegion());
    itk::ImageRegionConstIterator<VarianceImageType> weights2Iterator(normalizeImageFilter2->GetOutput(),
                                                                       normalizeImageFilter2->GetOutput()->GetLargestPossibleRegion());

    float sumDifferences = 0.0f;

    TPixelDifferenceFunctor differenceFunctor;

    while(!region1Iterator.IsAtEnd())
      {
      float difference = weights1Iterator.Get() * weights2Iterator.Get() *
                          fabs(region1Iterator.Get() - region2Iterator.Get());

      sumDifferences +=  difference;

      ++region1Iterator;
      ++region2Iterator;
      ++weights1Iterator;
      ++weights2Iterator;
      }

    float averageDifference = sumDifferences / static_cast<float>(region1.GetNumberOfPixels);
    return averageDifference;
  }

};

#endif
