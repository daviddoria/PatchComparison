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

#ifndef SelfPatchCompareVectorized_H
#define SelfPatchCompareVectorized_H

// Custom
#include "PatchDistance.h"

// Eigen
#include <Eigen/Dense>

// ITK
#include "itkImageRegion.h"

// STL
#include <vector>

/**
 * This class is for situations when you have a very large set of source patches
 * that are entirely valid, and you want to compare them all to a target patch
 * that is entirely valid. It first vectorizes all of the patches so that the cache
 * coherency is much much higher than iterating over image patches. Additionally, 
 * Eigen's sse2 computations can be used for an even bigger speedup.
 */
template <typename TImage>
class SelfPatchCompareVectorized
{
public:

  /** Constructor. */
  SelfPatchCompareVectorized();

  /** Set the image to use to compare patch regions. */
  void SetImage(TImage* const image);

  /** Set the patch to compare all other patches to. */
  void SetTargetRegion(const itk::ImageRegion<2>& targetRegion);

  /** Perform all of the patch comparisons. */
  void ComputePatchScores();

  /** Initialize everything (compute the feature matrix, etc) once the image and target have been set. */
  void Initialize();

  /** A structure to store the source patch patch regions and their corresponding distances. */
  typedef std::pair<itk::ImageRegion<2>, float> PatchDataType;

  /** Get the patch data (the regions and their corresponding distances). */
  std::vector<PatchDataType> GetPatchData();

protected:

  /** This is the target region we wish to compare. It may be partially invalid. */
  itk::ImageRegion<2> TargetRegion;

  /** This is the image from which to take the patches. */
  TImage* Image;

  /** Stores a list of source regions and their associated distances. */
  std::vector<PatchDataType> PatchData;

//   typedef Eigen::MatrixXi MatrixType;
//   typedef Eigen::VectorXi VectorType;

  typedef Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> MatrixType;
  typedef Eigen::Matrix<unsigned char, Eigen::Dynamic, 1> VectorType;

//   typedef Eigen::Matrix<short, Eigen::Dynamic, Eigen::Dynamic> MatrixType;
//   typedef Eigen::Matrix<short, Eigen::Dynamic, 1> VectorType;

  /** All of the patches in the image, vectorized.. */
  MatrixType VectorizedPatches;
};

#include "SelfPatchCompareVectorized.hpp"

#endif
