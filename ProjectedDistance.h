#ifndef ProjectedDistance_H
#define ProjectedDistance_H

// Eigen
#include <Eigen/Dense>

// ITK
#include "itkImage.h"
#include "itkVectorImage.h"

#include "PatchDistance.h"

template <typename TImage>
struct ProjectedDistance : public PatchDistance
{
  typedef Eigen::MatrixXf MatrixType;
  typedef Eigen::VectorXf VectorType;

  float Distance(const itk::ImageRegion<2>& region1,
                 const itk::ImageRegion<2>& region2);

  static float Distance(const TImage* const image, const MatrixType& projectionMatrix, const itk::ImageRegion<2>& region1,
                        const itk::ImageRegion<2>& region2);

  void SetImage(TImage* const image);

  void SetProjectionMatrix(const MatrixType& projectionMatrix);
  
private:
  TImage* Image;

  MatrixType ProjectionMatrix;
};

#include "ProjectedDistance.hpp"

#endif
