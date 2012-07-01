/*=========================================================================
 *
 *  Copyright David Doria 2012 daviddoria@gmail.com
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

#ifndef SelfDeviationWeightedSSD_HPP
#define SelfDeviationWeightedSSD_HPP

#include "SelfDeviationWeightedSSD.h"

// ITK
#include "itkImageRegion.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkNormalizeToConstantImageFilter.h"

// STL
#include <vector>

template <typename TPixel>
float SelfDeviationWeightedSSD::operator()(const itk::VectorImage<TPixel, 2>* const image,
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
  return WeightedSSD(image, region1, region2, weightVector);
}

#endif
