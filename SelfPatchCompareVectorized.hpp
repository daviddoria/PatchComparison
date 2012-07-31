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

#ifndef SelfPatchCompareVectorized_HPP
#define SelfPatchCompareVectorized_HPP

#include "SelfPatchCompareVectorized.h"

// Submodules
#include <Helpers/Helpers.h>
#include <ITKHelpers/ITKHelpers.h>
#include <Mask/MaskOperations.h>
#include <PatchProjection/PatchProjection.h>

// STL
#include <algorithm> // for sort()

template <typename TImage>
SelfPatchCompareVectorized<TImage>::SelfPatchCompareVectorized() :
Image(NULL)
{

}

template <typename TImage>
void SelfPatchCompareVectorized<TImage>::SetImage(TImage* const image)
{
  this->Image = image;
}

template <typename TImage>
void SelfPatchCompareVectorized<TImage>::Initialize()
{
  unsigned int patchRadius = this->TargetRegion.GetSize()[0]/2;
  this->VectorizedPatches = PatchProjection<MatrixType, VectorType>::
                            VectorizeImage(this->Image, patchRadius);

}

template <typename TImage>
void SelfPatchCompareVectorized<TImage>::SetTargetRegion(const itk::ImageRegion<2>& region)
{
  this->TargetRegion = region;
}

template <typename TImage>
void SelfPatchCompareVectorized<TImage>::ComputePatchScores()
{
  this->PatchData.clear();

  // Get the patch regions corresponding to the columns of VectorizedPatches
  unsigned int patchRadius = this->TargetRegion.GetSize()[0]/2;
  std::vector<itk::ImageRegion<2> > fullSourcePatches =
       ITKHelpers::GetAllPatches(this->Image->GetLargestPossibleRegion(), patchRadius);

  if(static_cast<int>(fullSourcePatches.size()) != this->VectorizedPatches.cols())
  {
    std::stringstream ss;
    ss << "The number of fullSourcePatches is " << fullSourcePatches.size()
       << " but the number of columns in VectorizedPatches is " << this->VectorizedPatches.cols();
    throw std::runtime_error(ss.str());
  }

  // Determine the ID of the target patch
  unsigned int targetRegionId = 0;
  bool validTarget = false;
  for(unsigned int i = 0; i < fullSourcePatches.size(); ++i)
  {
    if(fullSourcePatches[i] == this->TargetRegion)
    {
      targetRegionId = i;
      validTarget = true;
      break;
    }
  }

  if(validTarget == false)
  {
    throw std::runtime_error("The target patch was not found!");
  }

//   for(unsigned int i = 0; i < fullSourcePatches.size(); ++i)
//     {
//     //std::cout << "Comparing " << this->TargetRegion << " to " << fullSourcePatches[i] << std::endl;
//     float score = (this->VectorizedPatches.col(targetRegionId) -
//                                   this->VectorizedPatches.col(i)).squaredNorm();
// 
//     //std::cout << "score: " << averageAbsoluteScore << std::endl;
//     PatchDataType patchData;
//     patchData.first = fullSourcePatches[i];
//     patchData.second = score;
//     this->PatchData.push_back(patchData);
//     }

  VectorType queryVector = this->VectorizedPatches.col(targetRegionId);
  VectorType scores = (this->VectorizedPatches.colwise() - queryVector).colwise().squaredNorm();

  for(unsigned int i = 0; i < fullSourcePatches.size(); ++i)
    {
    //std::cout << "score: " << averageAbsoluteScore << std::endl;
    PatchDataType patchData;
    patchData.first = fullSourcePatches[i];
    patchData.second = scores[i];
    this->PatchData.push_back(patchData);
    }

}

template <typename TImage>
std::vector<typename SelfPatchCompareVectorized<TImage>::PatchDataType>
SelfPatchCompareVectorized<TImage>::GetPatchData()
{
  return this->PatchData;
}

#endif
