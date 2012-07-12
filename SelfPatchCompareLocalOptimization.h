#ifndef SelfPatchCompareLocalOptimization_H
#define SelfPatchCompareLocalOptimization_H

// Eigen
#include "SelfPatchCompare.h"

template <typename TImage>
struct SelfPatchCompareLocalOptimization : public SelfPatchCompare
{

public:
  SelfPatchCompareLocalOptimization();

  /** Perform all of the patch comparisons. */
  void ComputePatchScores();

};

#include "SelfPatchCompareLocalOptimization.hpp"

#endif
