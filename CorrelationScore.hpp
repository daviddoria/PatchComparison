#ifndef CorrelationScore_HPP
#define CorrelationScore_HPP

#include "CorrelationScore.h"

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
template <typename TImage>
float CorrelationScore::operator()(TImage* const image, const itk::ImageRegion<2>& region1,
                                   const itk::ImageRegion<2>& region2)
{
  throw std::runtime_error("CorrelationScore not yet implemented!");
//   typedef itk::Image<float, 2> FloatImageType;
// 
//   typedef itk::VectorMagnitudeImageFilter<TImage, FloatImageType> VectorMagnitudeFilterType;
//   typename VectorMagnitudeFilterType::Pointer magnitudeFilter = VectorMagnitudeFilterType::New();
//   magnitudeFilter->SetInput(image);
//   magnitudeFilter->Update();
// 
//   FloatImageType::Pointer region1Image = FloatImageType::New();
//   ITKHelpers::ExtractAndNormalizeRegion(magnitudeFilter->GetOutput(), region1, region1Image.GetPointer());
// 
//   FloatImageType::Pointer region2Image = FloatImageType::New();
//   ITKHelpers::ExtractAndNormalizeRegion(magnitudeFilter->GetOutput(), region2, region2Image.GetPointer());
// 
//   // Perform correlation
//   
}

#endif
