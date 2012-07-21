// STL
#include <iostream>
#include <vector>

// Submodules
#include "ITKHelpers/ITKHelpers.h"

// Custom
#include "SSD.h"

int main(int argc, char* argv[])
{
  if(argc != 4)
  {
    std::cerr << "Required arguments: patch1filename patch2filename outputFileName" << std::endl;
    return EXIT_FAILURE;
  }

  std::string patch1FileName = argv[1];
  std::string patch2FileName = argv[2];
  std::string outputFileName = argv[3];

  std::cout << "patch1FileName: " << patch1FileName << std::endl
            << "patch2FileName: " << patch2FileName << std::endl
            << "outputFileName: " << outputFileName << std::endl;

  typedef itk::VectorImage<unsigned char, 2> ImageType;

  typedef  itk::ImageFileReader<ImageType> ImageReaderType;
  ImageReaderType::Pointer patch1Reader = ImageReaderType::New();
  patch1Reader->SetFileName(patch1FileName);
  patch1Reader->Update();
  ImageType* patch1 = patch1Reader->GetOutput();

  typedef  itk::ImageFileReader<ImageType> ImageReaderType;
  ImageReaderType::Pointer patch2Reader = ImageReaderType::New();
  patch2Reader->SetFileName(patch2FileName);
  patch2Reader->Update();
  ImageType* patch2 = patch2Reader->GetOutput();

  float sigmaStep = .1;

  std::vector<float> distances;
  for(unsigned int i = 1; i < 20; ++i) // Start at 1 because blur of 0 produces a black image
  {
    float sigma = sigmaStep * static_cast<float>(i);

    ImageType::Pointer patch1blurred = ImageType::New();
    ITKHelpers::BlurAllChannels(patch1, patch1blurred.GetPointer(), sigma);

    ImageType::Pointer patch2blurred = ImageType::New();
    ITKHelpers::BlurAllChannels(patch2, patch2blurred.GetPointer(), sigma);

    float distance = SSD<ImageType>::Distance(patch1blurred.GetPointer(), patch1blurred->GetLargestPossibleRegion(),
                                              patch2blurred.GetPointer(), patch2blurred->GetLargestPossibleRegion());

    std::cout << "Sigma " << sigma << " : " << distance << std::endl;
    distances.push_back(distance);
  }

  // Open the output file for writing
  std::ofstream fout(outputFileName.c_str());

  for(unsigned int i = 0; i < distances.size(); ++i)
  {
    fout << distances[i] << std::endl;
  }

  return EXIT_SUCCESS;
}
