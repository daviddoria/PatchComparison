#ifndef ProjectedDistance_H
#define ProjectedDistance_H

// Eigen
#include <Eigen/Dense>

// ITK
#include "itkImage.h"
#include "itkVectorImage.h"

template <unsigned int TDimension>
struct ProjectedDistance
{
  typedef Eigen::MatrixXf MatrixType;
  typedef Eigen::VectorXf VectorType;
  typedef itk::Image<itk::CovariantVector<float, TDimension>, 2> ImageType;

  float operator()(const itk::ImageRegion<2>& region1,
                   const itk::ImageRegion<2>& region2);

  float operator()(const ImageType* const image, const itk::ImageRegion<2>& region1,
                   const itk::ImageRegion<2>& region2);

  ImageType* Image;

  MatrixType ProjectionMatrix;
};

#include "ProjectedDistance.hpp"

#endif
