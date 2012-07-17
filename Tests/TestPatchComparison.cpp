#include "PatchComparison.h"

#include "SelfDeviationWeightedSSD.h"
#include "LocalPCADistance.h"
#include "ProjectedDistance.h"
#include <PatchProjection/PatchProjection.h>

// STL
#include <iostream>

void TestSelfDeviationWeightedSSD();
void TestLocalPCADistance();
void TestProjectedDistance();

int main( int argc, char ** argv )
{
  //TestSelfDeviationWeightedSSD();
  TestLocalPCADistance();
  TestProjectedDistance();
  return 0;
}

void TestProjectedDistance()
{
  typedef itk::Image<unsigned char, 2> ImageType;
  ImageType::Pointer image = ImageType::New();
  itk::Index<2> corner = {{0,0}};
  const unsigned int imageSize = 10;
  itk::Size<2> size = {{imageSize,imageSize}};
  itk::ImageRegion<2> region(corner, size);
  image->SetRegions(region);
  image->Allocate();

  const unsigned int patchRadius = 1;
  typedef Eigen::VectorXf VectorType;
  typedef Eigen::MatrixXf MatrixType;
  VectorType meanVector;
  std::vector<typename VectorType::Scalar> sortedEigenvalues;
  MatrixType projectionMatrix = PatchProjection<MatrixType, VectorType>::
      ComputeProjectionMatrixFromImageOuterProduct(image.GetPointer(), patchRadius,
                                                   meanVector, sortedEigenvalues);
  
  itk::Size<2> patchSize = {{patchRadius*2 + 1, patchRadius*2 + 1}};

  itk::Index<2> queryCorner = {{2,2}};
  itk::ImageRegion<2> queryRegion(queryCorner, patchSize);

  itk::Index<2> testCorner = {{2,2}};
  itk::ImageRegion<2> testRegion(testCorner, patchSize);

  ProjectedDistance<ImageType> projectedDistanceFunctor;
  projectedDistanceFunctor.SetProjectionMatrix(projectionMatrix);
  projectedDistanceFunctor.SetImage(image);
  float distance = projectedDistanceFunctor.Distance(queryRegion, testRegion);
  std::cout << "distance = " << distance << std::endl;
  
  float distanceStatic = ProjectedDistance<ImageType>::Distance(image.GetPointer(), projectionMatrix, meanVector,
                                                                queryRegion, testRegion);
  std::cout << "distanceStatic = " << distanceStatic << std::endl;
}

void TestLocalPCADistance()
{
  typedef itk::Image<unsigned char, 2> ImageType;
  ImageType::Pointer image = ImageType::New();
  itk::Index<2> corner = {{0,0}};
  const unsigned int imageSize = 10;
  itk::Size<2> size = {{imageSize,imageSize}};
  itk::ImageRegion<2> region(corner, size);
  image->SetRegions(region);
  image->Allocate();

  const unsigned int patchRadius = 1;
  itk::Size<2> patchSize = {{patchRadius*2 + 1, patchRadius*2 + 1}};
  
  itk::Index<2> queryCorner = {{2,2}};
  itk::ImageRegion<2> queryRegion(queryCorner, patchSize);

  itk::Index<2> testCorner = {{2,2}};
  itk::ImageRegion<2> testRegion(testCorner, patchSize);

//   float distance = LocalPCADistance<ImageType>::Distance(image.GetPointer(), queryRegion, testRegion);
// 
//   std::cout << "distance = " << distance << std::endl;
}

void TestSelfDeviationWeightedSSD()
{
  typedef itk::Image<unsigned char, 2> ImageType;
  ImageType::Pointer image = ImageType::New();
  itk::Index<2> corner = {{0,0}};
  const unsigned int imageSize = 3;
  itk::Size<2> size = {{imageSize,imageSize}};
  itk::ImageRegion<2> region(corner, size);
  image->SetRegions(region);
  image->Allocate();

  /* Image:
    255 51 102
    227 102 102
    255 77 99
    */
  const unsigned int row = 1;
  const unsigned int col = 0;

  itk::Index<2> currentIndex = {{0,0}};
  currentIndex[row] = 0; currentIndex[col] = 0;
  image->SetPixel(currentIndex, 255);

  currentIndex[row] = 0; currentIndex[col] = 1;
  image->SetPixel(currentIndex, 51);

  currentIndex[row] = 0; currentIndex[col] = 2;
  image->SetPixel(currentIndex, 102);

  currentIndex[row] = 1; currentIndex[col] = 0;
  image->SetPixel(currentIndex, 227);

  currentIndex[row] = 1; currentIndex[col] = 1;
  image->SetPixel(currentIndex, 102);

  currentIndex[row] = 1; currentIndex[col] = 2;
  image->SetPixel(currentIndex, 102);

  currentIndex[row] = 2; currentIndex[col] = 0;
  image->SetPixel(currentIndex, 255);

  currentIndex[row] = 2; currentIndex[col] = 1;
  image->SetPixel(currentIndex, 77);

  currentIndex[row] = 2; currentIndex[col] = 2;
  image->SetPixel(currentIndex, 99);

  std::cout << "image:" << std::endl;
  for(unsigned int r = 0; r < imageSize; ++r)
    {
    for(unsigned int c = 0; c < imageSize; ++c)
      {
      itk::Index<2> index = {{c,r}};
      std::cout << static_cast<int>(image->GetPixel(index)) << " ";
      }
    std::cout << std::endl;
    }

  /* Expected deviations:
    145.19 124.99 90.60
    121.60 90.60 90.60
    145.19 105.41 92.10
    */
    
  std::vector<float> deviations = SelfDeviationWeightedSSD::ComputeDeviations(image.GetPointer(), image->GetLargestPossibleRegion());
  std::cout << "deviations:" << std::endl;
  for(unsigned int i = 0; i < deviations.size(); ++i)
  {
    unsigned int col = i % imageSize;
    
    std::cout << deviations[i] << " ";
    if(col == 2)
    {
      std::cout << std::endl;
    }
  }

  /* Expected weights:
     0 0 .31
     0 .31 .31
     0 0 .069
  */
  std::vector<float> weights = SelfDeviationWeightedSSD::ComputeWeights(image.GetPointer(), image->GetLargestPossibleRegion());

  std::cout << "weights:" << std::endl;
  for(unsigned int i = 0; i < weights.size(); ++i)
  {
    unsigned int col = i % imageSize;
    
    std::cout << weights[i] << " ";
    if(col == 2)
    {
      std::cout << std::endl;
    }
  }
}
