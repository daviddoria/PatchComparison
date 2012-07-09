#ifndef LocalPCADistance_H
#define LocalPCADistance_H

// Eigen
#include <Eigen/Dense>

template <typename TImage>
struct LocalPCADistance
{
  typedef Eigen::MatrixXf MatrixType;
  typedef Eigen::VectorXf VectorType;
  //typedef itk::Image<itk::CovariantVector<float, TDimension>, 2> ImageType;

  float Distance(const itk::ImageRegion<2>& region1,
                 const itk::ImageRegion<2>& region2);

  static float Distance(const TImage* const image, const itk::ImageRegion<2>& region1,
                        const itk::ImageRegion<2>& region2);

  TImage* Image;
};

#include "LocalPCADistance.hpp"

#endif
