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

#include "SumOfSquaredDifferencesGeneral.h"

// ITK
#include "itkImageRegion.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkNormalizeToConstantImageFilter.h"

// STL
#include <vector>

template <typename TPixel>
std::vector<float> SelfDeviationWeightedSSD::ComputeWeights(const itk::VectorImage<TPixel, 2>* const image,
                         const itk::ImageRegion<2>& region)
{
  typedef itk::VectorImage<TPixel, 2> ImageType;

  std::vector<float> meanSquareDeviationVector(region.GetNumberOfPixels());
  std::vector<float> weightVector(region.GetNumberOfPixels());

  itk::ImageRegionConstIteratorWithIndex<ImageType> region1Iterator(image, region);

  unsigned int pixelCounter = 0;
  SumOfSquaredDifferencesGeneral diffOperator;

  while(!region1Iterator.IsAtEnd())
    {
    float sumDeviations = 0.0f;
    itk::ImageRegionConstIteratorWithIndex<ImageType> deviationIterator(image, region);
    while(!deviationIterator.IsAtEnd())
      {
      if(deviationIterator.GetIndex() == region1Iterator.GetIndex())
        {
        ++deviationIterator;
        continue;
        }

      //sumDeviations += pow(region1Iterator.Get() - deviationIterator.Get(), 2);
      sumDeviations += diffOperator(region1Iterator.Get(), deviationIterator.Get());

      ++deviationIterator;
      }
    float meanSquareDeviation = sqrt(sumDeviations/region.GetNumberOfPixels() - 1);

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
    weightVector[pixelCounter] = exp(-1.0f * (meanSquareDeviationVector[pixelCounter] - averageDeviation));
    sumWeights += weightVector[pixelCounter];
  }

  // Normalize
  for(unsigned int i = 0; i < weightVector.size(); ++i)
  {
    weightVector[i] = weightVector[i] / sumWeights;
  }

  return weightVector;
}

template <typename TPixel>
float SelfDeviationWeightedSSD::operator()(const itk::VectorImage<TPixel, 2>* const image,
                  const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2)
{
  std::vector<float> weights = ComputeWeights(image, region1);

  return WeightedSSD(image, region1, region2, weights);
}

#endif

struct SumOfSquaredDifferencesGeneral;
