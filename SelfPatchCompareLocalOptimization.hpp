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

#ifndef SelfPatchCompareLocalOptimization_HPP
#define SelfPatchCompareLocalOptimization_HPP

#include "SelfPatchCompareLocalOptimization.h"

#include "ITKHelpers/ITKHelpers.h"

template <typename TImage>
SelfPatchCompareLocalOptimization<TImage>::SelfPatchCompareLocalOptimization()
{

}

template <typename TImage>
void SelfPatchCompareLocalOptimization<TImage>::ComputePatchScores()
{
  // Compute the best patches for the direct query patch
  SelfPatchCompare<TImage>::ComputePatchScores();
  std::cout << "Found top source patches for direct query patch." << std::endl;
  
  unsigned int numberOfPatches = 10;
  std::partial_sort(this->PatchData.begin(), this->PatchData.begin() + numberOfPatches,
                    this->PatchData.end(),
                    Helpers::SortBySecondAccending<typename SelfPatchCompare<TImage>::PatchDataType>);

  this->PatchData.resize(numberOfPatches);

  // Compute the neighbors of the query patch
  itk::Index<2> queryPixel = ITKHelpers::GetRegionCenter(this->TargetRegion);
  std::vector<itk::Index<2> > neighborPixels =
        ITKHelpers::Get8NeighborsInRegion(this->Image->GetLargestPossibleRegion(), queryPixel);
  std::vector<itk::ImageRegion<2> > neighborRegions(neighborPixels.size());
  for(unsigned int i = 0; i < neighborPixels.size(); ++i)
  {
    neighborRegions[i] = ITKHelpers::GetRegionInRadiusAroundPixel(neighborPixels[i],
                                                                  this->TargetRegion.GetSize()[0]/2);
  }

  // For each neighbor, compute a list of candidate filling patches
  std::vector<std::vector<typename SelfPatchCompare<TImage>::PatchDataType> >
        neigbhorBestPatchData(neighborRegions.size());
  for(unsigned int neighborId = 0; neighborId < neighborRegions.size(); ++neighborId)
  {
    SelfPatchCompare<TImage> selfPatchCompare;
    selfPatchCompare.SetImage(this->Image);
    selfPatchCompare.SetPatchDistanceFunctor(this->PatchDistanceFunctor);
    selfPatchCompare.CreateFullyValidMask();
    selfPatchCompare.SetTargetRegion(neighborRegions[neighborId]);
    selfPatchCompare.ComputePatchScores();
    std::vector<typename SelfPatchCompare<TImage>::PatchDataType> patchData =
              selfPatchCompare.GetPatchData();

    std::partial_sort(patchData.begin(), patchData.begin() + numberOfPatches,
                      patchData.end(),
                      Helpers::SortBySecondAccending<typename SelfPatchCompare<TImage>::PatchDataType>);
    neigbhorBestPatchData[neighborId] = patchData;
  }

  std::cout << "Found top source patches for all neighbor patches." << std::endl;

  // For each top patch of the direct query pixel, sum the minimum distance to all
  // matches to the neighbor patch

  // "the minimum differences of a candidate patch for the
  // target region with respect to all the candidate patches in
  // the neighborhood."
  std::vector<float> scores(numberOfPatches, 0.0f);
  assert(this->PatchData.size() == numberOfPatches);

  for(unsigned int directPatchMatchId = 0; directPatchMatchId < this->PatchData.size(); ++directPatchMatchId)
  {
    for(unsigned int neighborId = 0; neighborId < neighborRegions.size(); ++neighborId)
    {
      float minScore = std::numeric_limits<float>::max();

      for(unsigned int neighborTopPatchId = 0; neighborTopPatchId < numberOfPatches; ++neighborTopPatchId)
      {
        float score = this->PatchDistanceFunctor->Distance(
                      neigbhorBestPatchData[neighborId][neighborTopPatchId].first,
                      this->PatchData[directPatchMatchId].first);
        if(score < minScore)
        {
          minScore = score;
        }
      }
      scores[directPatchMatchId] += minScore;
    }
  }

  std::cout << "Computed summed errors." << std::endl;

  // Assign them the new score values to the internal PatchData
  for(size_t i = 0; i < this->PatchData.size(); ++i)
  {
    this->PatchData[i].second += scores[i];
  }

  // Sort the patch data by the new scores
  std::sort(this->PatchData.begin(), this->PatchData.end(),
                      Helpers::SortBySecondAccending<typename SelfPatchCompare<TImage>::PatchDataType>);

  std::cout << "Finished sorting top patches by local optimization criteria." << std::endl;
}

#endif
