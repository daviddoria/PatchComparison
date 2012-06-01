#include <iostream>
#include <vector>

#include "SSD.h"

// Submodules
#include "Mask/ITKHelpers/ITKHelpers.h"

int main(int argc, char* argv[])
{
  // This program reads a file containing the NNField (offsets) of an image.
  // It then finds the pixels with x or y components of the NNField above
  // a threshold. It then writes the patch and its best match to an image
  // for manual review.

  if(argc < 6)
  {
    std::cerr << "Required arguments: imageFileName offsetImageFileName patchRadius threshold outputFileName" << std::endl;
    return EXIT_FAILURE;
  }

  std::stringstream ss;
  for(int i = 1; i < argc; ++i)
  {
    ss << argv[i] << " ";
  }

  std::string imageFileName;
  std::string nnFieldFileName;
  unsigned int patchRadius;
  unsigned int threshold;
  std::string outputFileName;
  ss >> imageFileName >> nnFieldFileName >> patchRadius >> threshold >> outputFileName;

  std::cout << "Arguments: " << std::endl
            << "imageFileName = " << imageFileName << std::endl
            << "nnFieldFileName = " << nnFieldFileName << std::endl
            << "patchRadius = " << patchRadius << std::endl
            << "threshold = " << threshold << std::endl
            << "outputFileName = " << outputFileName << std::endl;

  // Read the image
  typedef itk::Image<itk::CovariantVector<unsigned char, 3>, 2> ImageType;
  typedef itk::ImageFileReader<ImageType> ImageReaderType;
  ImageReaderType::Pointer imageReader = ImageReaderType::New();
  imageReader->SetFileName(imageFileName);
  imageReader->Update();
  ImageType* image = imageReader->GetOutput();

  // Read the NNField
  typedef itk::Image<itk::CovariantVector<float, 3>, 2> NNFieldImageType;
  typedef itk::ImageFileReader<NNFieldImageType> NNFieldReaderType;
  NNFieldReaderType::Pointer nnFieldReader = NNFieldReaderType::New();
  nnFieldReader->SetFileName(nnFieldFileName);
  nnFieldReader->Update();
  NNFieldImageType* nnField = nnFieldReader->GetOutput();

  itk::Size<2> patchSize = {{patchRadius * 2 + 1, patchRadius * 2 + 1}};

  std::vector<itk::Index<2> > indicesToOutput;

  itk::ImageRegionConstIterator<NNFieldImageType> nnFieldIterator(nnField, nnField->GetLargestPossibleRegion());

  while(!nnFieldIterator.IsAtEnd())
  {
    NNFieldImageType::PixelType nnFieldPixel = nnFieldIterator.Get();
    if(fabs(nnFieldPixel[0]) > threshold || fabs(nnFieldPixel[1]) > threshold)
    {
      indicesToOutput.push_back(nnFieldIterator.GetIndex());
    }
    ++nnFieldIterator;
  }

  std::cout << "There are " << indicesToOutput.size() << " indices that pass the threshold test." << std::endl;

  ImageType::PixelType whitePixel;
  whitePixel.Fill(255);

  itk::Index<2> corner = {{0,0}};
  itk::Size<2> outputSize = {{patchSize[0] * 3, patchSize[1] * indicesToOutput.size() * 2}};
  itk::ImageRegion<2> outputRegion(corner,outputSize);

  ImageType::Pointer outputImage = ImageType::New();
  outputImage->SetRegions(outputRegion);
  outputImage->Allocate();
  outputImage->FillBuffer(whitePixel);

  for(unsigned int i = 0; i < indicesToOutput.size(); ++i)
  {
    itk::Index<2> sourceIndex = indicesToOutput[i];
    itk::ImageRegion<2> sourceRegion(sourceIndex, patchSize);

    itk::Index<2> matchIndex;
    matchIndex[0] = sourceIndex[0] + nnField->GetPixel(sourceIndex)[0];
    matchIndex[1] = sourceIndex[1] + nnField->GetPixel(sourceIndex)[1];

    itk::ImageRegion<2> matchRegion(matchIndex, patchSize);

    // Create the output image
    itk::Index<2> sourceOutputIndex = {{0, patchSize[0] * 2 * i}};
    itk::ImageRegion<2> sourceOutputRegion(sourceOutputIndex, patchSize);

    itk::Index<2> matchOutputIndex = {{patchSize[0] * 2, patchSize[0] * 2 * i}};
    itk::ImageRegion<2> matchOutputRegion(matchOutputIndex, patchSize);

    ITKHelpers::CopyRegion(image, sourceRegion, outputImage.GetPointer(), sourceOutputRegion);
    ITKHelpers::CopyRegion(image, matchRegion, outputImage.GetPointer(), matchOutputRegion);
  }

  ITKHelpers::WriteImage(outputImage.GetPointer(), outputFileName);

  return EXIT_SUCCESS;
}
