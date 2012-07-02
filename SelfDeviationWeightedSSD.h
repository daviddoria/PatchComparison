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

#ifndef SelfDeviationWeightedSSD_H
#define SelfDeviationWeightedSSD_H

#include "WeightedSSD.h"

// ITK
#include "itkVectorImage.h"

// STL
#include <vector>

/** This method is described in "Fuzzy c-means clustering with weighted
 *  image patch for image segmentation"
 */
class SelfDeviationWeightedSSD : public WeightedSSD
{
public:

  template <typename TImage>
  float operator()(const TImage* const image,
                   const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);

  template <typename TImage>
  static std::vector<float> ComputeWeights(const TImage* const image,
                         const itk::ImageRegion<2>& region1);

  template <typename TImage>
  static std::vector<float> ComputeDeviations(const TImage* const image,
                         const itk::ImageRegion<2>& region1);
};

#include "SelfDeviationWeightedSSD.hpp"

#endif
