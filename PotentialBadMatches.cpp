#include <iostream>
#include <vector>

#include "SSD.h"

// Submodules
#include "Mask/ITKHelpers/ITKHelpers.h"

int main(int argc, char* argv[])
{
  std::string inputFileName = argv[1];
  std::string outputFileName = argv[2];

  typedef itk::VectorImage<float, 2> ImageType;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFileName);
  reader->Update();

  const unsigned int patchRadius = 7;

  std::vector<itk::ImageRegion<2> > allPatches = ITKHelpers::GetAllPatches(reader->GetOutput()->GetLargestPossibleRegion(), patchRadius);

  typedef itk::Image<itk::CovariantVector<float, 3>, 2> OutputImageType; // (x, y, score)
  OutputImageType::Pointer output = OutputImageType::New();
  output->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  output->Allocate();
  itk::CovariantVector<float, 3> zeroVector;
  zeroVector.Fill(0);
  output->FillBuffer(zeroVector);

  SSD ssd;
  for(unsigned int i = 0 ; i < allPatches.size(); ++i)
  {
    float minDistance = std::numeric_limits<float>::max();
    unsigned int bestId = 0;
    for(unsigned int j = 0 ; j < allPatches.size(); ++j)
    {
      // Don't compare a patch to itself
      if(i == j)
      {
        continue;
      }

      float distance = ssd(reader->GetOutput(), allPatches[i], allPatches[j]);
      if(distance < minDistance)
      {
        minDistance = distance;
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
