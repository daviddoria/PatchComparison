#include <iostream>
#include <vector>

#include "DiffusionDistance.h"
#include "Helpers.h"

int main()
{
  std::vector<Eigen::VectorXf> allPoints;

  unsigned int numberOfFeatures = 6;
  // Add many points that are close to each other
  for(unsigned int patchId = 0; patchId < 30; ++patchId)
  {
    Eigen::VectorXf point(numberOfFeatures);
    for(unsigned int featureId = 0; featureId < numberOfFeatures; ++featureId)
    {
      point[featureId] = 5.0f + drand48() - .5;
    }
    allPoints.push_back(point);
  }

  // Add an outlier
  Eigen::VectorXf outlier(numberOfFeatures);
  for(unsigned int featureId = 0; featureId < numberOfFeatures; ++featureId)
  {
    outlier[featureId] = 7.0f;
  }

  allPoints.push_back(outlier);
  
  DiffusionDistance diffusionDistanceFunctor;

  float badMatchScore = diffusionDistanceFunctor(allPoints[0], outlier, allPoints);
  std::cout << "Bad match score: " << badMatchScore << std::endl;

  float goodMatchScore = diffusionDistanceFunctor(allPoints[0], allPoints[1], allPoints);
  std::cout << "Good match score: " << goodMatchScore << std::endl;
  
  return 0;
}
