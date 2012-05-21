#include "DiffusionDistance.h"

#include <stdexcept>

#include "Mask/ITKHelpers/Helpers/Helpers.h"

#include "EigenHelpers/EigenHelpers.h"

float DiffusionDistance::SumOfAbsoluteDifference(const std::vector<float>& a, const std::vector<float>& b)
{
  float sum = 0.0f;

  for(unsigned int i = 0; i < a.size(); ++i)
  {
    sum += fabs(a[i] - b[i]);
  }
  return sum;
}

float DiffusionDistance::SumOfAbsoluteDifference(const Eigen::VectorXf& a, const Eigen::VectorXf& b)
{
  float sum = 0.0f;

  for(unsigned int i = 0; i < static_cast<unsigned int>(a.size()); ++i)
  {
    sum += fabs(a[i] - b[i]);
  }
  return sum;
}

float DiffusionDistance::operator()(const Eigen::VectorXf& a, const Eigen::VectorXf& b,
                                    const std::vector<Eigen::VectorXf> allPoints)
{
  std::cout << "DiffusionDistance()..." << std::endl;
  Eigen::MatrixXf L(allPoints.size(), allPoints.size());

//   for(unsigned int i = 0; i < allPoints.size(); ++i)
//   {
//     for(unsigned int j = 0; j < allPoints.size(); ++j)
//     {
//       float difference = SumOfAbsoluteDifference(allPoints[i], allPoints[j]);
//       L(i,j) = difference;
//       //L(j,i) = difference; // We could only iterate over half of the matrix and do this.
//     }
//   }

  for(unsigned int i = 0; i < allPoints.size(); ++i)
  {
    if(i % 100 == 0)
    {
      std::cout << i << " out of " << allPoints.size() << std::endl;
    }
    for(unsigned int j = 0; j <= i; ++j)
    {
      float difference = SumOfAbsoluteDifference(allPoints[i], allPoints[j]);
      L(i,j) = difference;
      L(j,i) = difference;
    }
  }

  std::cout << "Created L." << std::endl;
  
  Eigen::MatrixXf D(allPoints.size(), allPoints.size());
  D.setIdentity();

  // Sum the rows of L, and set the diagonal entries of D to these sums
  for(unsigned int row = 0; row < allPoints.size(); ++row)
  {
    float sumOfRow = EigenHelpers::SumOfRow(L, row);
    D(row, row) = sumOfRow;
  }

  std::cout << "Computing inverse..." << std::endl;
  // Compute M = D^-1 L
  Eigen::MatrixXf M = D.inverse() * L;
  std::cout << "Finished computing inverse." << std::endl;

  // Compute the eigenvectors of M
  std::cout << "Size of M: " << M.rows() << " " << M.cols() << std::endl;
  std::cout << "Computing eigenvectors..." << std::endl;
  Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> eigensolver(M);
  //Eigen::EigenSolver<Eigen::MatrixXf> eigensolver(M);
  
  if (eigensolver.info() != Eigen::Success)
  {
    throw std::runtime_error("Computing eigenvectors failed!");
  }

  Eigen::VectorXf eigenvalues = eigensolver.eigenvalues();
  Eigen::MatrixXf eigenvectorMatrix = eigensolver.eigenvectors();

  std::cout << "Finished computing eigenvectors." << std::endl;

  // Sort by increating magnitude
  typedef std::pair<float, Eigen::VectorXf> PairType;
  std::vector<PairType> pairs;
  for(unsigned int i = 0; i < static_cast<unsigned int>(eigenvalues.size()); ++i)
  {
    PairType pair;
    pair.first = fabs(eigenvalues[i]); // fabs() to make sure negative eigenvalues are sorted properly
    pair.second = eigenvectorMatrix.col(i);
    pairs.push_back(pair);
  }

  std::cout << "There are " << pairs.size() << " pairs of eigenvalues/vectors." << std::endl;
  
  std::cout << std::endl << "Unsorted eigenvalue magnitudes:" << std::endl;
  Helpers::OutputFirst(pairs);
  
  std::sort(pairs.rbegin(), pairs.rend(), Helpers::SortByFirstAccending<PairType>);
  std::cout << std::endl << "Sorted eigenvalue magnitudes:" << std::endl;
  Helpers::OutputFirst(pairs);

  Eigen::MatrixXf sortedEigenvectorMatrix(eigenvalues.size(), a.size());
  // Keep only the eigenvectors with the largest eigenvalues ("the size of a" of them)
  std::cout << "sortedEigenvectorMatrix size: " << sortedEigenvectorMatrix.rows()
            << " " << sortedEigenvectorMatrix.cols() << std::endl;
  
  for(unsigned int i = 0; i < static_cast<unsigned int>(a.size()); ++i)
  {
    sortedEigenvectorMatrix.col(i) = pairs[i].second;
  }

  //std::cout << "a_eigen: " << a_eigen << std::endl;
  //std::cout << "sortedEigenvectorMatrix: " << sortedEigenvectorMatrix << std::endl;
//   std::cout << "sortedEigenvectorMatrix size: " << sortedEigenvectorMatrix.rows() << " "
//             << sortedEigenvectorMatrix.cols() << std::endl;

  Eigen::VectorXf a_transformed = sortedEigenvectorMatrix * a;
  std::cout << "a_transformed: " << a_transformed << std::endl;

  Eigen::VectorXf b_transformed = sortedEigenvectorMatrix * b;
  std::cout << "b_transformed: " << b_transformed << std::endl;

  float finalDistance = SumOfAbsoluteDifference(a_transformed, b_transformed);

  return finalDistance;
}
