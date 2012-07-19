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

#ifndef SelfPatchCompare_H
#define SelfPatchCompare_H

// Custom
#include "PatchDistance.h"

// Eigen
#include <Eigen/Dense>

// Submodules
#include "Mask/Mask.h"

// ITK
#include "itkImageRegion.h"

// STL
#include <vector>

/**
 * This class is for situations when you have a very large set of source patches
 * that are entirely valid, and you want to compare them all to a target patch
 * that is partially masked. It computes the linear offsets of the masked pixels
 * once, and then uses them to do all of the patch comparisons.
 */
template <typename TImage>
class SelfPatchCompare
{
public:

  /** Constructor. */
  SelfPatchCompare();

  /** Set the image to use to compare patch regions. */
  void SetImage(TImage* const image);

  /** Set the mask to use in the patch comparisons. */
  void SetMask(Mask* const mask);

  /** Set the patch to compare all other patches to. */
  void SetTargetRegion(const itk::ImageRegion<2>& targetRegion);

  /** Perform all of the patch comparisons. */
  void ComputePatchScores();

  /** A structure to store the source patch patch regions and their corresponding distances. */
  typedef std::pair<itk::ImageRegion<2>, float> PatchDataType;

  /** Get the patch data (the regions and their corresponding distances). */
  std::vector<PatchDataType> GetPatchData();

  /** Set the function to use to compare patches. */
  void SetPatchDistanceFunctor(PatchDistance* const patchDistanceFunctor);

  /** If we do not want to use a mask, call this function. */
  void CreateFullyValidMask();

protected:

  /** This is the target region we wish to compare. It may be partially invalid. */
  itk::ImageRegion<2> TargetRegion;

  /** This is the image from which to take the patches. */
  TImage* Image;

  /** This is the mask to check the validity of target pixels. */
  Mask* MaskImage;

  /** Maintain a fully valid mask in case no real mask is specified. */
  Mask::Pointer FullyValidMask;

  /** Stores a list of source regions and their associated distances. */
  std::vector<PatchDataType> PatchData;

  /** The functor to use to compare two patches. */
  PatchDistance* PatchDistanceFunctor;
};

#include "SelfPatchCompare.hpp"

#endif
