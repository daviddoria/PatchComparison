#ifndef SelfPatchCompareLocalOptimization_HPP
#define SelfPatchCompareLocalOptimization_HPP

#include "SelfPatchCompareLocalOptimization.h"

#include "PatchComparison/Mask/ITKHelpers/ITKHelpers.h"

template <typename TImage>
SelfPatchCompareLocalOptimization<TImage>::SelfPatchCompareLocalOptimization()
{

}

template <typename TImage>
void SelfPatchCompareLocalOptimization<TImage>::ComputePatchScores()
{
  // Compute the best patches for the direct query patch
  SelfPatchCompare::ComputePatchScores();

  unsigned int numberOfPatches = 10;
  std::partial_sort(this->PatchData.begin(), this->PatchData.begin() + numberOfPatches,
                    this->PatchData.end(), Helpers::SortBySecondAccending<SelfPatchCompare::PatchDataType>);

  this->PatchData.resize(numberOfPatches);

  // Compute the neighbors of the query patch
  itk::Index<2> queryPixel = ITKHelpers::GetRegionCenter(this->TargetRegion);
  std::vector<itk::Index<2> > neighborPixels = ITKHelpers::Get8NeighborsInRegion(this->TargetRegion, this->Image->GetLargestPossibleRegion());
  std::vector<itk::ImageRegion<2> > neighborRegions(neighborPixels.size());
  for(unsigned int i = 0; i < neighborPixels.size(); ++i)
  {
    neighborRegions[i] = ITKHelpers::GetRegionInRadiusAroundPixel(neighborPixels[i], this->TargetRegion.GetSize()[0]/2);
  }

  // For each top patch of the direct query pixel, sum the minimum distance to all matches to the neighbor patch
  for(unsigned int directPatchMatchId = 0; directPatchMatchId < this->PatchData.size(); ++directPatchMatchId)
  {
    for(unsigned int neighborId = 0; neighborId < neighborRegions.size(); ++neighborId)
    {
      SelfPatchCompare selfPatchCompare;
      selfPatchCompare.SetImage(this->Image);
      selfPatchCompare.CreateFullyValidMask();
      selfPatchCompare.SetTargetRegion(neighborRegions[neighborId]);
      selfPatchCompare.ComputePatchScores();
      PatchDataType patchData = selfPatchCompare.GetPatchData();
    }
  }
}

#endif
