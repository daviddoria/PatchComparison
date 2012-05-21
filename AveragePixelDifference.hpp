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

// Submodules
#include "Mask/ITKHelpers/Helpers/Helpers.h"
#include "Mask/Mask.h"

// Custom
#include "Types.h"
#include "PixelDifferences.hpp"

// ITK
#include "itkImageRegion.h"

// STL
#include <vector>

template <typename TPixelDifferenceFunctor>
class AveragePixelDifference
{
public:
  template <typename TImage>
  float operator()(const TImage* const image, const Mask* const mask, const itk::ImageRegion<2>& sourceRegion, const itk::ImageRegion<2>& targetRegion)
  {
    itk::ImageRegionConstIterator<TImage> sourcePatchIterator(image, sourceRegion);
    itk::ImageRegionConstIterator<TImage> targetPatchIterator(image, targetRegion);
    itk::ImageRegionConstIterator<Mask> maskIterator(mask, targetRegion);

    float sumDifferences = 0;

    TPixelDifferenceFunctor differenceFunctor;

    unsigned int validPixelCounter = 0;

    while(!sourcePatchIterator.IsAtEnd())
      {
      itk::Index<2> currentPixel = maskIterator.GetIndex();
      if(mask->IsValid(currentPixel))
	{
	//std::cout << "Offset from iterator: " << this->Image->ComputeOffset(maskIterator.GetIndex()) * componentsPerPixel;
	VectorImageType::PixelType sourcePixel = sourcePatchIterator.Get();
	VectorImageType::PixelType targetPixel = targetPatchIterator.Get();

	float difference = differenceFunctor(sourcePixel, targetPixel);

	sumDifferences +=  difference;

	validPixelCounter++;
	}

      ++sourcePatchIterator;
      ++targetPatchIterator;
      ++maskIterator;
      } // end while iterate over sourcePatch

    if(validPixelCounter == 0)
      {
      return 0;
      }

    float averageDifference = sumDifferences / static_cast<float>(validPixelCounter);
    return averageDifference;
  }

};

#endif
