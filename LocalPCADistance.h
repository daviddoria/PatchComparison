#ifndef LocalPCADistance_H
#define LocalPCADistance_H

// Eigen
#include <Eigen/Dense>

#include "PatchDistance.h"

template <typename TImage>
struct LocalPCADistance : public PatchDistance
{
  typedef Eigen::MatrixXf MatrixType;
  typedef Eigen::VectorXf VectorType;

  LocalPCADistance();

  /** Compute the distance between the regions using internal data. */
  float Distance(const itk::ImageRegion<2>& region1,
                 const itk::ImageRegion<2>& region2);

  /** Compute the distance between the regions. */
  static float Distance(const TImage* const image, const MatrixType& projectionMatrix,
                        const VectorType& meanVector,
                        const itk::ImageRegion<2>& region1,
                        const itk::ImageRegion<2>& region2);

  /** Set the target patch to operate on. */
  void SetTargetPatch(const itk::ImageRegion<2>& region);

  /** Compute the PCA matrix from a patch. */
  void ComputeProjectionMatrix(const itk::ImageRegion<2>& region);

public:

  /** Set the image that the patches refer to. */
  void SetImage(TImage* const image);

private:

  /** The image that the patches refer to. */
  TImage* Image;

  /** The projection matrix. */
  MatrixType ProjectionMatrix;

  /** The mean vector computed when computing the projection matrix. */
  VectorType MeanVector;
};

#include "LocalPCADistance.hpp"

#endif
