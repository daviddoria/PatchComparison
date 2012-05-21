#ifndef DiffusionDistance_H
#define DiffusionDistance_H

// Eigen
#include <Eigen/Dense>


struct DiffusionDistance
{
//   template <typename TPoint>
//   float operator()(const TPoint& a, const TPoint& b,
//                    const std::vector<TPoint> allPoints);

  float operator()(const Eigen::VectorXf& a, const Eigen::VectorXf& b,
                   const std::vector<Eigen::VectorXf> allPoints);

  float SumOfAbsoluteDifference(const std::vector<float>& a, const std::vector<float>& b);

  float SumOfAbsoluteDifference(const Eigen::VectorXf& a, const Eigen::VectorXf& b);
};

#include "DiffusionDistance.hpp"

#endif
