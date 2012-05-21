#ifndef CorrelationScore_HPP
#define CorrelationScore_HPP

// ITK
#include "itkAddImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkImageRegion.h"
#include "itkVectorMagnitudeImageFilter.h"

// Custom
#include "Mask/ITKHelpers/Helpers/Helpers.h"
#include "Mask/ITKHelpers/Helpers/Statistics.h"
#include "Mask/ITKHelpers/ITKHelpers.h"

/**

 */
struct CorrelationScore
{
  template <typename TImage>
  float operator()(TImage* const image, Mask* const mask, const itk::ImageRegion<2>& sourceRegion, const itk::ImageRegion<2>& targetRegion)
  {
    typedef itk::Image<float, 2> FloatImageType;

    typedef itk::VectorMagnitudeImageFilter<TImage, FloatImageType> VectorMagnitudeFilterType;
    typename VectorMagnitudeFilterType::Pointer magnitudeFilter = VectorMagnitudeFilterType::New();
    magnitudeFilter->SetInput(image);
    magnitudeFilter->Update();

    std::vector<itk::Offset<2> > validOffsets = mask->GetValidOffsetsInRegion(targetRegion);

    FloatImageType::Pointer sourceImage = FloatImageType::New();
//     sourceImage->SetRegions(ITKHelpers::CornerRegion(sourceRegion.GetSize()));
//     sourceImage->Allocate();
    ITKHelpers::ExtractRegion(magnitudeFilter->GetOutput(), sourceRegion, sourceImage.GetPointer());

    FloatImageType::Pointer targetImage = FloatImageType::New();
//     sourceImage->SetRegions(ITKHelpers::CornerRegion(targetRegion.GetSize()));
//     sourceImage->Allocate();
    ITKHelpers::ExtractRegion(magnitudeFilter->GetOutput(), targetRegion, targetImage.GetPointer());

    std::vector<itk::Index<2> > validIndices = ITKHelpers::OffsetsToIndices(validOffsets);

    if(validIndices.size() == 0)
    {
      std::cerr << "Warning: validIndices.size() = 0!" << std::endl;
      return 0.0f;
    }
    //Helpers::VarianceFunctor varianceFunctor;
    //Helpers::AverageFunctor averageFunctor;
    /////////// Target region //////////
    std::vector<FloatImageType::PixelType> validPixelsTargetRegion = ITKHelpers::GetPixelValues(targetImage.GetPointer(), validIndices);
    //std::cout << "There are " << validPixelsTargetRegion.size() << " validPixelsTargetRegion" << std::endl;
    
    typename TypeTraits<FloatImageType::PixelType>::LargerType targetMean = Statistics::Average(validPixelsTargetRegion);
    typename TypeTraits<FloatImageType::PixelType>::LargerType targetStandardDeviation = sqrt(Statistics::Variance(validPixelsTargetRegion));

    typedef itk::AddImageFilter <FloatImageType, FloatImageType, FloatImageType> AddImageFilterType;
    AddImageFilterType::Pointer targetAddImageFilter = AddImageFilterType::New();
    targetAddImageFilter->SetInput(targetImage);
    targetAddImageFilter->SetConstant2(-1.0f * targetMean);
    targetAddImageFilter->Update();

    typedef itk::MultiplyImageFilter<FloatImageType, FloatImageType, FloatImageType> MultiplyImageFilterType;
    MultiplyImageFilterType::Pointer targetMultiplyImageFilter = MultiplyImageFilterType::New();
    targetMultiplyImageFilter->SetInput(targetImage);
    targetMultiplyImageFilter->SetConstant(1.0f/targetStandardDeviation);
    targetMultiplyImageFilter->Update();

    /////////// Source region //////////
    std::vector<FloatImageType::PixelType> validPixelsSourceRegion = ITKHelpers::GetPixelValues(sourceImage.GetPointer(), validIndices);
    typename TypeTraits<FloatImageType::PixelType>::LargerType sourceMean = Statistics::Average(validPixelsSourceRegion);
    typename TypeTraits<FloatImageType::PixelType>::LargerType sourceStandardDeviation = sqrt(Statistics::Variance(validPixelsSourceRegion));

    AddImageFilterType::Pointer sourceAddImageFilter = AddImageFilterType::New();
    sourceAddImageFilter->SetInput(sourceImage);
    sourceAddImageFilter->SetConstant2(-1.0f * sourceMean);
    sourceAddImageFilter->Update();

    MultiplyImageFilterType::Pointer sourceMultiplyImageFilter = MultiplyImageFilterType::New();
    sourceMultiplyImageFilter->SetInput(sourceImage);
    sourceMultiplyImageFilter->SetConstant(1.0f/sourceStandardDeviation);
    sourceMultiplyImageFilter->Update();

    // Initialize
    float computedEnergy = 0.0f;

    for(std::vector<itk::Index<2> >::const_iterator iter = validIndices.begin(); iter != validIndices.end(); ++iter)
    {
      computedEnergy += (sourceMultiplyImageFilter->GetOutput()->GetPixel(*iter) * targetMultiplyImageFilter->GetOutput()->GetPixel(*iter));
    }

    computedEnergy /= static_cast<float>(validIndices.size());
    return computedEnergy;
  }

};

#endif
