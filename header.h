#ifndef HEADER_H_
#define HEADER_H_

#include "itkImage.h"
#include <itkGDCMImageIO.h>
#include "itkImageFileReader.h"
#include <itkImageFileWriter.h>
#include "itkImageRegionIterator.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include <itkMinimumMaximumImageCalculator.h>
#include "itksys/SystemTools.hxx"
#include "itkOtsuMultipleThresholdsImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"

//c++ includes
#include <sstream>
#include <iostream>
#include <cstring>
#include <cmath>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <math.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <direct.h>
#include <cstdlib>

struct especie{
	int esp;
	int ind;
};

using namespace std;
using namespace itk;

typedef short PixelType;
const     unsigned short   Dimension = 3;

typedef Image<PixelType, Dimension> ImageType3D;
typedef ImageFileReader<ImageType3D> ReaderType;
typedef ImageFileWriter<ImageType3D> WriterType;
typedef GDCMImageIO  ImageIOType;

typedef Image<float,Dimension> ImageTypeFloat;

#endif

