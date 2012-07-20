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

#ifndef PairReader_H
#define PairReader_H

#include "itkImageRegion.h"

#include <string>
#include <vector>

struct PairReader
{
  /** The type of patch pairs. */
  typedef std::pair<itk::ImageRegion<2>, itk::ImageRegion<2> > PairType;

  static std::vector<PairType> Read(const std::string& fileName, const itk::Size<2>& patchSize);
};

#endif
