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
  typedef itk::VectorImage<TPixel, 2> ImageType;

  std::vector<float> meanSquareDeviationVector(region1.GetNumberOfPixels());
  std::vector<float> weightVector(region1.GetNumberOfPixels());

  itk::ImageRegionConstIteratorWithIndex<ImageType> region1Iterator(image, region1);

  unsigned int pixelCounter = 0;
  while(!region1Iterator.IsAtEnd())
    {
    float sumDeviations = 0.0f;
    itk::ImageRegionConstIteratorWithIndex<ImageType> deviationIterator(image, region1);
    while(!deviationIterator.IsAtEnd())
      {
      if(deviationIterator.GetIndex() == region1Iterator.GetIndex())
        {
        ++deviationIterator;
        continue;
        }
      sumDeviations += pow(region1Iterator.Get() - deviationIterator.Get(), 2);
      ++deviationIterator
      }
    float meanSquareDeviation = sqrt(sumDeviations/region1.GetNumberOfPixels() - 1);

    meanSquareDeviationVector[pixelCounter] = meanSquareDeviation;

    pixelCounter++;
    ++region1Iterator;
    }

  float sumAllDeviations = 0.0f;
  for(unsigned int i = 0; i < meanSquareDeviationVector.size(); ++i)
  {
    sumAllDeviations += meanSquareDeviationVector[i];
  }

  float averageDeviation = sumAllDeviations / static_cast<float>(meanSquareDeviationVector.size());

  float sumWeights = 0.0f;
  for(unsigned int i = 0; i < meanSquareDeviationVector.size(); ++i)
  {
    weightVector[pixelCounter] = exp(-1.0f * (meanSquareDeviation[pixelCounter] - averageDeviation));
    sumWeights += weightVector[pixelCounter];
  }

  // Normalize
  for(unsigned int i = 0; i < weightVector.size(); ++i)
  {
    weightVector[i] = weightVector[i] / sumWeights;
  }
  
  return WeightedSSD(image, region1, region2, weightVector);
}

#endif
