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

#ifndef SelfPatchCompare_HPP
#define SelfPatchCompare_HPP

#include "SelfPatchCompare.h"

// Submodules
#include "Mask/ITKHelpers/Helpers/Helpers.h"
#include "Mask/ITKHelpers/ITKHelpers.h"
#include "Mask/MaskOperations.h"

// STL
#include <algorithm> // for sort()

template <typename TImage>
SelfPatchCompare<TImage>::SelfPatchCompare() : Image(NULL), MaskImage(NULL), PatchDistanceFunctor(NULL)
{
  this->FullyValidMask = Mask::New();
}

template <typename TImage>
void SelfPatchCompare<TImage>::SetImage(TImage* const image)
{
  this->Image = image;
}

template <typename TImage>
void SelfPatchCompare<TImage>::CreateFullyValidMask()
{
  this->FullyValidMask->SetRegions(this->Image->GetLargestPossibleRegion());
  this->FullyValidMask->Allocate();
  ITKHelpers::SetImageToConstant(this->FullyValidMask.GetPointer(), this->FullyValidMask->GetValidValue());

  this->MaskImage = this->FullyValidMask;
}

template <typename TImage>
void SelfPatchCompare<TImage>::SetMask(Mask* const mask)
{
  // Ensure the image is set first, so the mask size can be checked against the image.
  if(!this->Image)
  {
    throw std::runtime_error("You must set the image before you set the mask!");
  }

  if(mask->GetLargestPossibleRegion() != this->Image->GetLargestPossibleRegion())
  {
    std::stringstream ss;
    ss << "Mask size (" << mask->GetLargestPossibleRegion() << " does not match image size ("
                        << this->Image->GetLargestPossibleRegion() << "!";
    throw std::runtime_error(ss.str());
  }

  this->MaskImage = mask;
}

template <typename TImage>
void SelfPatchCompare<TImage>::SetTargetRegion(const itk::ImageRegion<2>& region)
{
  assert(this->PatchDistanceFunctor);
  this->TargetRegion = region;
  this->PatchDistanceFunctor->SetTargetPatch(region);
}

template <typename TImage>
void SelfPatchCompare<TImage>::ComputePatchScores()
{
  assert(this->PatchDistanceFunctor);

  this->PatchData.clear();

  unsigned int patchRadius = this->TargetRegion.GetSize()[0]/2;
  std::vector<itk::ImageRegion<2> > fullSourcePatches =
       MaskOperations::GetAllFullyValidRegions(this->MaskImage, patchRadius);

  for(unsigned int i = 0; i < fullSourcePatches.size(); ++i)
    {
    //std::cout << "Comparing " << this->TargetRegion << " to " << fullSourcePatches[i] << std::endl;
    float averageAbsoluteScore = this->PatchDistanceFunctor->Distance(this->TargetRegion,
                                                                      fullSourcePatches[i]);

    //std::cout << "score: " << averageAbsoluteScore << std::endl;
    PatchDataType patchData;
    patchData.first = fullSourcePatches[i];
    patchData.second = averageAbsoluteScore;
    this->PatchData.push_back(patchData);
    }
}

template <typename TImage>
std::vector<typename SelfPatchCompare<TImage>::PatchDataType>
SelfPatchCompare<TImage>::GetPatchData()
{
  return this->PatchData;
}

template <typename TImage>
void SelfPatchCompare<TImage>::SetPatchDistanceFunctor(PatchDistance* const patchDistanceFunctor)
{
  this->PatchDistanceFunctor = patchDistanceFunctor;
}

#endif
