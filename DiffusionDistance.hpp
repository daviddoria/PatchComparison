#ifndef DiffusionDistance_HPP
#define DiffusionDistance_HPP

#include "DiffusionDistance.h" // Appease syntax parser

// Submodules
#include <Helpers/Helpers.h>
/*
template <typename TPoint>
float DiffusionDistance::operator()(const TPoint& a, const TPoint& b,
                 const std::vector<TPoint> allPoints)
{
  Eigen::MatrixXf L(allPoints.size(), allPoints.size());

  for(unsigned int i = 0; i < allPoints.size(); ++i)
  {
    for(unsigned int j = 0; j < allPoints.size(); ++j)
    {
      float difference = SumOfAbsoluteDifference(allPoints[i], allPoints[j]);
      L(i,j) = difference;
      //L(j,i) = difference; // We could only iterate over half of the matrix and do this.
    }
  }

  Eigen::MatrixXf D(allPoints.size(), allPoints.size());
  D.setIdentity();

  // Sum the rows of L, and set the diagonal entries of D to these sums
  for(unsigned int row = 0; row < allPoints.size(); ++row)
  {
    float sumOfRow = Helpers::SumOfRow(L, row);
    D(row, row) = sumOfRow;
  }

  // Compute M = D^-1 L
  Eigen::MatrixXf M = D.inverse() * L;

  // Compute the eigenvectors of M
  std::cout << "Size of M: " << M.rows() << " " << M.cols() << std::endl;
  
  Eigen::EigenSolver<Eigen::MatrixXf> eigensolver(M);
  if (eigensolver.info() != Eigen::Success)
  {
    throw std::runtime_error("Computing eigenvectors failed!");
  }

  Eigen::VectorXf eigenvalues = eigensolver.eigenvalues();
  Eigen::MatrixXf eigenvectorMatrix = eigensolver.eigenvectors();

  // Sort by increating magnitude
  typedef std::pair<float, Eigen::VectorXf> PairType;
  std::vector<PairType> pairs;
  for(unsigned int i = 0; i < static_cast<unsigned int>(eigenvalues.size()); ++i)
  {
    PairType pair;
    pair.first = eigenvalues[i];
    pair.second = eigenvectorMatrix.col(i);
    pairs.push_back(pair);
  }

  std::sort(pairs.begin(), pairs.end(), Helpers::SortByFirst<PairType>);

  // Keep only the eigenvectors with the largest eigenvalues ("the size of a" of them)
  Eigen::MatrixXf sortedEigenvectorMatrix(eigenvalues.size(), a.size());
  for(unsigned int i = 0; i < static_cast<unsigned int>(a.size()); ++i)
  {
    sortedEigenvectorMatrix.col(i) = pairs[i].second;
  }

  // Conver the inputs to Eigen3 structures
  Eigen::VectorXf a_eigen = Helpers::ConvertVector<Eigen::VectorXf, TPoint>(a);
  Eigen::VectorXf b_eigen = Helpers::ConvertVector<Eigen::VectorXf, TPoint>(b);

  //std::cout << "a_eigen: " << a_eigen << std::endl;
  //std::cout << "sortedEigenvectorMatrix: " << sortedEigenvectorMatrix << std::endl;
//   std::cout << "sortedEigenvectorMatrix size: " << sortedEigenvectorMatrix.rows() << " "
//             << sortedEigenvectorMatrix.cols() << std::endl;

  Eigen::VectorXf a_transformed = sortedEigenvectorMatrix * a_transformed;
  std::cout << "a_transformed: " << a_transformed << std::endl;
  
  Eigen::VectorXf b_transformed = sortedEigenvectorMatrix * b_transformed;
  std::cout << "b_transformed: " << b_transformed << std::endl;
  
  float finalDistance = SumOfAbsoluteDifference(a_transformed, b_transformed);

  return finalDistance;
}*/

#endif
