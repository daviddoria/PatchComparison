// STL
#include <iostream>
#include <vector>

// Submodules
#include <ITKHelpers/ITKHelpers.h>

// Custom
#include "SSD.h"

int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    std::cerr << "Required arguments: patch1filename patch2filename" << std::endl;
    return EXIT_FAILURE;
  }

  std::string patch1FileName = argv[1];
  std::string patch2FileName = argv[2];

  std::cout << "patch1FileName " << patch1FileName << std::endl
            << "patch2FileName " << patch2FileName << std::endl;

  typedef itk::VectorImage<unsigned char, 2> ImageType;

  // Read patch 1
  typedef  itk::ImageFileReader<ImageType> ImageReaderType;
  ImageReaderType::Pointer patch1Reader = ImageReaderType::New();
  patch1Reader->SetFileName(patch1FileName);
  patch1Reader->Update();
  ImageType* patch1 = patch1Reader->GetOutput();

  // Read patch 2
  typedef  itk::ImageFileReader<ImageType> ImageReaderType;
  ImageReaderType::Pointer patch2Reader = ImageReaderType::New();
  patch2Reader->SetFileName(patch2FileName);
  patch2Reader->Update();
  ImageType* patch2 = patch2Reader->GetOutput();

  float distance = SSD<ImageType>::Distance(patch1, patch1->GetLargestPossibleRegion(),
                                            patch2, patch2->GetLargestPossibleRegion());

  std::cout << "Distance: " << distance << std::endl;
  
  return EXIT_SUCCESS;
}
