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

#ifndef PatchDistance_H
#define PatchDistance_H

// ITK
#include "itkImageRegion.h"

template <typename TImage>
struct PatchDistance
{
  /** Compute the distance between two regions. */
  virtual float Distance(const itk::ImageRegion<2>& region1,
                         const itk::ImageRegion<2>& region2) = 0;

  /** Set the target patch if it is needed. For example, SelfPatchCompareLocalOptimization needs to do this.
    * This function is not needed by all subclasses, so it is not pure virtual. */
  virtual void SetTargetPatch(const itk::ImageRegion<2>& region)
  {
  }

  /** Set the image to use. */
  void SetImage(TImage* const image);

  /** Retrieve the name of the distance object. */
  virtual std::string GetDistanceName() = 0;

  /** Set a modifier to make the distance name more specific. */
  void SetDistanceNameModifier(const std::string& distanceNameModifier);

protected:
  /** The image to use. */
  TImage* Image;

  /** A more detailed description of the distance name. For example, if we are using a HistogramDistance object
    * to compute the histogram of an HSV image, we would set DistanceNameModifier to "HSV". */
  std::string DistanceNameModifier;
};

#include "PatchDistance.hpp"

#endif
