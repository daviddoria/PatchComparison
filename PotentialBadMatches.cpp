#include <iostream>
#include <vector>

#include "SSD.h"

// Submodules
#include "Mask/ITKHelpers/ITKHelpers.h"

int main(int argc, char* argv[])
{
  if(argc < 4)
  {
    std::cerr << "Required arguments: inputFileName patchRadius stride" << std::endl;
    return EXIT_FAILURE;
  }
  std::stringstream ss;
  ss << argv[1] << " " << argv[2] << " " << argv[3];
  std::string inputFileName;
  unsigned int patchRadius;
  unsigned int stride;
  ss >> inputFileName >> patchRadius >> stride;

  std::cout << "Running on " << inputFileName << " with patchRadius = " << patchRadius << " and stride = " << stride << std::endl;

  //typedef itk::VectorImage<float, 2> ImageType;
  typedef itk::Image<itk::CovariantVector<float, 3>, 2> ImageType;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  reader->Update();

  ImageType* image = reader->GetOutput();

  std::vector<itk::ImageRegion<2> > allPatches = ITKHelpers::GetAllPatches(reader->GetOutput()->GetLargestPossibleRegion(), patchRadius);

  itk::CovariantVector<float, 3> zeroVector;
  zeroVector.Fill(0);

  typedef itk::Image<itk::CovariantVector<float, 3>, 2> OutputImageType; // (x, y, score)

  OutputImageType::Pointer locationField = OutputImageType::New();
  locationField->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  locationField->Allocate();
  locationField->FillBuffer(zeroVector);


  OutputImageType::Pointer offsetField = OutputImageType::New();
  offsetField->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  offsetField->Allocate();
  zeroVector.Fill(0);
  offsetField->FillBuffer(zeroVector);

  for(unsigned int i = 0; i < allPatches.size(); ++i)
  {
    std::cout << i << " of " << allPatches.size() << std::endl;

    if(allPatches[i].GetIndex()[0] % stride != 0 ||
       allPatches[i].GetIndex()[1] % stride != 0)
    {
      continue;
    }
    float minDistance = std::numeric_limits<float>::max();
    unsigned int bestId = 0;

    itk::ImageRegionConstIterator<ImageType> patch1Iterator(image, allPatches[i]);

    typename ImageType::PixelType pixel1;
    typename ImageType::PixelType pixel2;

    for(unsigned int j = 0; j < allPatches.size(); ++j)
    {
      //std::cout << j << " of " << allPatches.size() << std::endl;
      // Don't compare a patch to itself
      if(i == j)
      {
        continue;
      }

      patch1Iterator.GoToBegin();
      itk::ImageRegionConstIterator<ImageType> patch2Iterator(image, allPatches[j]);

      float sumSquaredDifferences = 0.0f;
      float distance = 0.0f;

      while(!patch1Iterator.IsAtEnd())
        {
        pixel1 = patch1Iterator.Get();
        pixel2 = patch2Iterator.Get();

        distance = (pixel1[0] - pixel2[0]) * (pixel1[0] - pixel2[0]) +
                        (pixel1[1] - pixel2[1]) * (pixel1[1] - pixel2[1]) +
                        (pixel1[2] - pixel2[2]) * (pixel1[2] - pixel2[2]);

        //       std::cout << "Source pixel: " << static_cast<unsigned int>(sourcePixel)
        //                 << " target pixel: " << static_cast<unsigned int>(targetPixel)
        //                 << "Difference: " << difference << " squaredDifference: " << squaredDifference << std::endl;

        sumSquaredDifferences +=  distance;

        ++patch1Iterator;
        ++patch2Iterator;
        } // end while iterate over patch

      if(sumSquaredDifferences < minDistance)
      {
        minDistance = sumSquaredDifferences;
        bestId = j;
      }
    } // end loop j

    // Location
    itk::CovariantVector<float, 3> locationPixel;
    locationPixel[0] = allPatches[bestId].GetIndex()[0];
    locationPixel[1] = allPatches[bestId].GetIndex()[1];
    locationPixel[2] = minDistance;

    locationField->SetPixel(allPatches[i].GetIndex(), locationPixel);

    // Offset
    itk::Offset<2> offset = allPatches[bestId].GetIndex() - allPatches[i].GetIndex();

    itk::CovariantVector<float, 3> offsetPixel;
    offsetPixel[0] = offset[0];
    offsetPixel[1] = offset[1];
    offsetPixel[2] = minDistance;

    offsetField->SetPixel(allPatches[i].GetIndex(), offsetPixel);
  } // end loop i

  std::stringstream ssLocation;
  ssLocation << "Location_" << patchRadius << "_" << stride << ".mha";
  ITKHelpers::WriteImage(locationField.GetPointer(), ssLocation.str());

  std::stringstream ssOffset;
  ssOffset << "Offset_" << patchRadius << "_" << stride << ".mha";
  ITKHelpers::WriteImage(offsetField.GetPointer(), ssOffset.str());

  return EXIT_SUCCESS;
}
