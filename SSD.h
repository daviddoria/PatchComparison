#ifndef SSD_H
#define SSD_H

typedef itk::VectorImage<float, 2> ImageType;

float AverageSSD(const ImageType* const image, const itk::ImageRegion<2>& region1, const itk::ImageRegion<2>& region2);

#endif
