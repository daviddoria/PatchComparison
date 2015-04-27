#ifndef ProjectedDistance_H
#define ProjectedDistance_H

// Eigen
#include <Eigen/Dense>

// ITK
#include "itkImage.h"
#include "itkVectorImage.h"

#include "PatchDistance.h"

template <typename TImage>
struct ProjectedDistance : public PatchDistance<TImage>
{
  typedef Eigen::MatrixXf MatrixType;
  typedef Eigen::VectorXf VectorType;

  float Distance(const itk::ImageRegion<2>& region1,
                 const itk::ImageRegion<2>& region2);

  static float Distance(const TImage* const image, const MatrixType& projectionMatrix,
                        const VectorType& meanVector,
                        const itk::ImageRegion<2>& region1,
                        const itk::ImageRegion<2>& region2);

  /** Set the image that the patches refer to. */
  void SetImage(TImage* const image);

  /** Set the mean vector. */
  void SetMeanVector(const VectorType& meanVector);

  /** Set the projection matrix. */
  void SetProjectionMatrix(const MatrixType& projectionMatrix);

  /** Retrieve the name of the distance object. */
  std::string GetDistanceName()
  {
    return "ProjectedDistance";
  }
private:

  /** The image that the patches refer to. */
  TImage* Image;

  /** The matrix to use to project the patches. */
  MatrixType ProjectionMatrix;

  /** The mean vector computed from the original feature matrix. */
  VectorType MeanVector;
};

#include "ProjectedDistance.hpp"

#endif
