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

#include "SelfPatchCompare.h"

#include "Helpers.h"
#include "Types.h"

SelfPatchCompare::SelfPatchCompare(const unsigned int components)
{
  this->NumberOfComponentsPerPixel = components;
}

bool SelfPatchCompare::IsReady()
{
  if(this->Image && this->MaskImage)
    {
    return true;
    }
  else
    {
    return false;
    }
}

void SelfPatchCompare::SetImage(VectorImageType::Pointer image)
{
  this->Image = image;
}

void SelfPatchCompare::SetMask(Mask::Pointer mask)
{
  this->MaskImage = mask;
}

void SelfPatchCompare::SetTargetRegion(const itk::ImageRegion<2>& region)
{
  this->TargetRegion = region;
}

void SelfPatchCompare::SetSourceRegion(const itk::ImageRegion<2>& region)
{
  this->SourceRegion = region;
}

float SelfPatchCompare::PixelSquaredDifference(const VectorType &a, const VectorType &b)
{
  float difference = 0;
  
  float diff = 0;
  for(unsigned int i = 0; i < this->NumberOfComponentsPerPixel; ++i)
    {
    diff = a[i] - b[i];
    difference += diff * diff;
    }
  return difference;
}


float SelfPatchCompare::PixelDifference(const VectorType &a, const VectorType &b)
{
  float difference = 0;
  
  for(unsigned int i = 0; i < this->NumberOfComponentsPerPixel; ++i)
    {
    difference += fabs(a[i] - b[i]);
    }
  return difference;
}


float SelfPatchCompare::SlowTotalAbsoluteDifference()
{
  // This function assumes that all pixels in the source region are unmasked.
  
  // This method uses 3 iterators - one for the mask, and one for each image patch.
  // The entire mask is traversed looking for valid pixels, and then comparing the image pixels.
  // This is very inefficient because, since the target region stays constant for many thousands of patch
  // comparisons, the mask need only be traversed once. This method is performed by ComputeOffsets()
  // and PatchDifference*(). This function is only here for comparison purposes (to ensure the result of the other functions
  // is correct).

  itk::ImageRegionConstIterator<VectorImageType> sourcePatchIterator(this->Image, this->SourceRegion);
  itk::ImageRegionConstIterator<VectorImageType> targetPatchIterator(this->Image, this->TargetRegion);
  itk::ImageRegionConstIterator<Mask> maskIterator(this->MaskImage, this->TargetRegion);

  float sumDifferences = 0;

  unsigned int validPixelCounter = 0;
  
  while(!sourcePatchIterator.IsAtEnd())
    {
    itk::Index<2> currentPixel = maskIterator.GetIndex();
    if(this->MaskImage->IsValid(currentPixel))
      {
      //std::cout << "Offset from iterator: " << this->Image->ComputeOffset(maskIterator.GetIndex()) * componentsPerPixel;
      VectorImageType::PixelType sourcePixel = sourcePatchIterator.Get();
      VectorImageType::PixelType targetPixel = targetPatchIterator.Get();
            
      float difference = PixelDifference(sourcePixel, targetPixel);

//       std::cout << "Source pixel: " << static_cast<unsigned int>(sourcePixel)
//                 << " target pixel: " << static_cast<unsigned int>(targetPixel)
//                 << "Difference: " << difference << " squaredDifference: " << squaredDifference << std::endl;

      sumDifferences +=  difference;

      validPixelCounter++;
      }

    ++sourcePatchIterator;
    ++targetPatchIterator;
    ++maskIterator;
    } // end while iterate over sourcePatch

  //std::cout << "totalDifference: " << sum << std::endl;
  //std::cout << "Valid pixels: " << validPixelCounter << std::endl;

  if(validPixelCounter == 0)
    {
    return 0;
    }

  return sumDifferences;

}


float SelfPatchCompare::SlowTotalSquaredDifference()
{
  // This function assumes that all pixels in the source region are unmasked.

  // This method uses 3 iterators - one for the mask, and one for each image patch.
  // The entire mask is traversed looking for valid pixels, and then comparing the image pixels.
  // This is very inefficient because, since the target region stays constant for many thousands of patch
  // comparisons, the mask need only be traversed once. This method is performed by ComputeOffsets()
  // and PatchDifference*(). This function is only here for comparison purposes (to ensure the result of the other functions
  // is correct).

  itk::ImageRegionConstIterator<VectorImageType> sourcePatchIterator(this->Image, this->SourceRegion);
  itk::ImageRegionConstIterator<VectorImageType> targetPatchIterator(this->Image, this->TargetRegion);
  itk::ImageRegionConstIterator<Mask> maskIterator(this->MaskImage, this->TargetRegion);

  float sumSquaredDifferences = 0;
  unsigned int validPixelCounter = 0;

  while(!sourcePatchIterator.IsAtEnd())
    {
    itk::Index<2> currentPixel = maskIterator.GetIndex();
    if(this->MaskImage->IsValid(currentPixel))
      {
      //std::cout << "Offset from iterator: " << this->Image->ComputeOffset(maskIterator.GetIndex()) * componentsPerPixel;
      VectorImageType::PixelType sourcePixel = sourcePatchIterator.Get();
      VectorImageType::PixelType targetPixel = targetPatchIterator.Get();

      float squaredDifference = PixelSquaredDifference(sourcePixel, targetPixel);

//       std::cout << "Source pixel: " << static_cast<unsigned int>(sourcePixel)
//                 << " target pixel: " << static_cast<unsigned int>(targetPixel)
//                 << "Difference: " << difference << " squaredDifference: " << squaredDifference << std::endl;

      sumSquaredDifferences +=  squaredDifference;

      validPixelCounter++;
      }

    ++sourcePatchIterator;
    ++targetPatchIterator;
    ++maskIterator;
    } // end while iterate over sourcePatch

  //std::cout << "totalDifference: " << sum << std::endl;
  //std::cout << "Valid pixels: " << validPixelCounter << std::endl;

  if(validPixelCounter == 0)
    {
    return 0;
    }

  return sumSquaredDifferences;

}
