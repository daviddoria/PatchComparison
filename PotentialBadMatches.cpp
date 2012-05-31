#include <iostream>
#include <vector>

#include "SSD.h"

// Submodules
#include "Mask/ITKHelpers/ITKHelpers.h"

int main(int argc, char* argv[])
{
  std::string inputFileName = argv[1];
  std::string outputFileName = argv[2];

  //typedef itk::VectorImage<float, 2> ImageType;
  typedef itk::Image<itk::CovariantVector<float, 3>, 2> ImageType;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  reader->Update();

  ImageType* image = reader->GetOutput();

  const unsigned int patchRadius = 7;

  std::vector<itk::ImageRegion<2> > allPatches = ITKHelpers::GetAllPatches(reader->GetOutput()->GetLargestPossibleRegion(), patchRadius);

  typedef itk::Image<itk::CovariantVector<float, 3>, 2> OutputImageType; // (x, y, score)
  OutputImageType::Pointer output = OutputImageType::New();
  output->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  output->Allocate();
  itk::CovariantVector<float, 3> zeroVector;
  zeroVector.Fill(0);
  output->FillBuffer(zeroVector);

  for(unsigned int i = 0; i < allPatches.size(); ++i)
  {
    std::cout << i << " of " << allPatches.size() << std::endl;

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

    itk::CovariantVector<float, 3> outputPixel;
    outputPixel[0] = allPatches[bestId].GetIndex()[0];
    outputPixel[1] = allPatches[bestId].GetIndex()[1];
    outputPixel[2] = minDistance;

    output->SetPixel(allPatches[i].GetIndex(), outputPixel);
  } // end loop i

  ITKHelpers::WriteImage(output.GetPointer(), outputFileName);

  return EXIT_SUCCESS;
}
