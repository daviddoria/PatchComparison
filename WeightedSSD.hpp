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

#ifndef WeightedSSD_HPP
#define WeightedSSD_HPP

#include "WeightedSSD.h"

// ITK
#include "itkImageRegion.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkNormalizeToConstantImageFilter.h"

// STL
#include <vector>

template <typename TPixel>
float WeightedSSD::operator()(const itk::VectorImage<TPixel, 2>* const image,
                              const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2,
                              const std::vector<float>& weights)
{
  typedef itk::VectorImage<TPixel, 2> ImageType;

  // Sum the weighted differences
  itk::ImageRegionConstIterator<ImageType> region1Iterator(image, region1);
  itk::ImageRegionConstIterator<ImageType> region2Iterator(image, region2);

  float sumDifferences = 0.0f;

  unsigned int pixelCounter = 0;
  while(!region1Iterator.IsAtEnd())
    {
    float difference = weights[pixelCounter] *
                       (region1Iterator.Get() - region2Iterator.Get()).GetSquaredNorm();

    sumDifferences +=  difference;

    ++region1Iterator;
    ++region2Iterator;
    }
  unsigned int numberOfPixels = region1.GetNumberOfPixels();
  float averageDifference = sumDifferences / static_cast<float>(numberOfPixels);
  return averageDifference;
}

#endif
