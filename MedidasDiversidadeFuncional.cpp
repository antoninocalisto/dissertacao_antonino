#include "MedidasDiversidadeFuncional.h"
#include "moran\statistic.h"

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include <iostream>
#include "itkGDCMImageIO.h"
#include "itkMinimumMaximumImageCalculator.h"
#include <itkMatrix.h>
#include <Math.h>
#include "stdio.h"
#include <stdlib.h>
#include "itkBinaryContourImageFilter.h"
#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include "itkTranslationTransform.h"
#include "itkResampleImageFilter.h"
#include "itkBinaryThinningImageFilter.h"
#include "QuickView.h"
//#include "itkBinaryThinningImageFilter3D.h"
#include <itkShapeLabelMapFilter.h>
//#include "ImageIO.h"
//#include "Util.h"
#include <itkNiftiImageIOFactory.h>
//#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include "quantizacao.h"
#include "itkImageDuplicator.h"
#include "itkIdentityTransform.h"
#include "itkResampleImageFilter.h"

//#include "Cladogram.h"
//#include "IndexFeature.h"


///
//#include <iostream>
//#include "itkGDCMImageIO.h"
//#include "itkMinimumMaximumImageCalculator.h"
//#include <itkMatrix.h>
//#include <Math.h>
//#include "stdio.h"
//#include "itkBinaryContourImageFilter.h"
//#include <vtkSmartPointer.h>
//#include <vtkImageViewer2.h>
//#include <vtkDICOMImageReader.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkRenderer.h>
//#include "itkTranslationTransform.h"
//#include "itkResampleImageFilter.h"
//#include "itkBinaryThinningImageFilter.h"
////#include "itkBinaryThinningImageFilter3D.h"
//#include <itkShapeLabelMapFilter.h>
//#include "Util.h"
//
////#include <itkDICOMSeriesFileNames.h>
////#include <itkGDCMSeriesFileNames.h>
//#include <itkImageFileWriter.h>
//#include <itkImageSeriesWriter.h>
//#include <itkImageToVTKImageFilter.h>
//#include <itkRegionOfInterestImageFilter.h>
#include <itkGDCMImageIOFactory.h>
//#include "vtkImageActor.h"
//#include "vtkInteractorStyleImage.h"
//#include "vtkImageFlip.h"
//
////#include "itkImageToImageFilter.h"
//#include "itkMinimumMaximumImageFilter.h"
//
//#include "itkStatisticsImageFilter.h"
//
////#include "VOI.h"
//
//#include "itkNeighborhoodIterator.h"
//#include "itkSmartPointer.h"
//
//#include <itkNiftiImageIOFactory.h>
////#include "Cladogram.h"

typedef itk::GDCMImageIO ImageIO;
typedef itk::Image<short, 3> ImageType;
typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;
typedef itk::Image<short, 3> ImageType3D;

typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;

typedef unsigned char PixelTypeChar;   // must be signed for CT since Hounsfield units can be < 0 
typedef Image< PixelTypeChar, 3 > ImageTypeChar;

typedef itk::Image<short, 2> ImageType2D;
typedef  itk::ImageFileWriter<ImageType2D> WriterType2D;
typedef itk::ImageFileReader<ImageType2D> ReaderType2D;
typedef itk::ImageFileWriter<ImageType2D> WriterType2;

MedidasDiversidadeFuncional::MedidasDiversidadeFuncional(void){ }

MedidasDiversidadeFuncional::MedidasDiversidadeFuncional(string diretorioMassa, string diretorioNaoMassa, int tipoImage, int qtdMascarasInterna, int qtdMascarasExterna, bool usaMascara, int pasta){
																//(pathMassa, pathNaoMassa, tipoImage, 1, 0, 1, 0)
	cout << "\n" << "Entrou no Contrutor de MedidasDiversidadeFuncional " << endl;
	this->_diretorioMassa = diretorioMassa;
	this->_diretorioNaoMassa = diretorioNaoMassa;
	this->_tipoImage = tipoImage;
	this->_qtdMascarasInterna = qtdMascarasInterna;
	this->_qtdMascarasExterna = qtdMascarasExterna;
	//this->_usaMascaras = usaMascara;
	this->_usaMascaras = 1;
	this->_rotulo = pasta;

	executa();

}


MedidasDiversidadeFuncional::~MedidasDiversidadeFuncional(void)
{
}

//
int MedidasDiversidadeFuncional::getMinimum1(ImageType::Pointer image) {
	typedef MinimumMaximumImageCalculator <ImageType> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMinimum();
}
int MedidasDiversidadeFuncional::getMaximum1(ImageType3D::Pointer image) {
	typedef MinimumMaximumImageCalculator <ImageType3D> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMaximum();
}



ImageType3D::Pointer MedidasDiversidadeFuncional::createImageMD(ImageType3D::Pointer image) {

	ImageType3D::Pointer newImage = ImageType3D::New();
	/*ImageType3D::SpacingType spacing;
	spacing = image->GetSpacing();
	ImageType3D::SpacingType spacingOut;
	spacingOut[0] = spacing[0];
	spacingOut[1] = spacing[1];
	spacingOut[2] = spacing[2];*/
	ImageType3D::IndexType start;
	start.Fill(0);
	ImageType3D::RegionType region;
	region.SetSize(image->GetLargestPossibleRegion().GetSize());
	region.SetIndex(start);
	newImage->SetRegions(region);
	newImage->SetSpacing(image->GetSpacing());
	newImage->Allocate();
	newImage->FillBuffer(getMinimum1(image));
	return newImage;
}
//

// OTSU

double MedidasDiversidadeFuncional::wClustering(ImageType3D::Pointer image, int numCluster) {
	typedef itk::OtsuMultipleThresholdsImageFilter <ImageType3D, ImageType3D> FilterType;
	FilterType::Pointer otsuFilter = FilterType::New();
	otsuFilter->SetInput(image);
	otsuFilter->SetNumberOfThresholds(numCluster);
	float maximo = getMaximum1(image);
	float minimo = getMinimum1(image);
	otsuFilter->SetNumberOfHistogramBins(getMaximum1(image) - getMinimum1(image) + 1);
	otsuFilter->Update();
	//otsuFilter->nu
	FilterType::ThresholdVectorType thresholds = otsuFilter->GetThresholds();
	double limiar = thresholds.at(thresholds.size() - 1);
	return limiar;
}


//Classe de execucao


void MedidasDiversidadeFuncional::geraSVMfile(int rotulo, vector<float> features, string savePath)
{
	ofstream featureFile(savePath.c_str(), ofstream::app);
	featureFile.precision(10);
	featureFile << rotulo << " ";
	for (int l = 0; l < features.size(); l++)
		featureFile << l + 1 << ":" << features.at(l) << " ";
	featureFile << "\n";
	featureFile.close();
}
//
ImageType3D::Pointer MedidasDiversidadeFuncional::readVolume1(string fileName)
{
	itk::GDCMImageIOFactory::RegisterOneFactory();

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(fileName);
	reader->Update();

	return reader->GetOutput();
}

void MedidasDiversidadeFuncional::flush() {
	//funçao excelente para limpar o buffer de entrada.
	int ch;
	while ((ch = fgetc(stdin)) != EOF && ch != '\n') {}
}

ImageType3D::Pointer MedidasDiversidadeFuncional::equalizacaoHistograma(ImageType3D::Pointer image) {
	ImageType3D::SizeType size;
	size = image->GetLargestPossibleRegion().GetSize();
	int min, max;
	min = getMinimum1(image);
	max = getMaximum1(image);

	int sizeHistograma = max - min + 1;

	int *hist = new int[sizeHistograma];
	int *histAcum = new int[sizeHistograma];
	float *histProbabilidade = new float[sizeHistograma];

	// initialize all intensity values to 0
	for (int i = 0; i < sizeHistograma; i++) {
		hist[i] = 0;
		histAcum[i] = 0;
	}

	int background = min;

	if (min < 0)
		min *= -1;

	// calculate the no of pixels for each intensity values

	ImageType3D::Pointer imgEqualizada = ImageType3D::New();
	imgEqualizada = createImageMD(image);
	saveImage(imgEqualizada, "IMAGEMNEGRA.nrrd");
	for (int z = 0; z < size[2]; z++) {
		for (int y = 0; y < size[1]; y++) {
			for (int x = 0; x < size[0]; x++) {
				ImageType3D::IndexType position;
				position[0] = x;
				position[1] = y;
				position[2] = z;
				int value = (int)image->GetPixel(position);
				//todos os voxels começam de 0
				if (value != background) {
					if ((value + min) < 0 || value > sizeHistograma)
						cout << "DEU MERDA: " << value << endl;
						
					hist[value + min]++;
				}
			}
		}
	}

	// Caluculate the size of image
	float alpha = (float)(max + min) / (size[1] * size[2] * size[0]);

	// Calculate the probability of each intensity
	for (int i = 0; i < sizeHistograma; i++)
		histProbabilidade[i] = (double)hist[i] / sizeHistograma;

	//gera o acumulado
	histAcum[0] = hist[0];
	for (int i = 1; i < sizeHistograma; i++)
		histAcum[i] = hist[i] + histAcum[i - 1];

	//scala o histrograma
	int *histEscaldado = new int[sizeHistograma];
	for (int i = 0; i<sizeHistograma; i++)
		histEscaldado[i] = (int)((double)histAcum[i] * alpha);

	////gera histograma equalizado
	//float *histEqualizado = new float[sizeHistograma];
	//
	////inicializar
	//for (int i=0; i<sizeHistograma; i++)
	//	histEqualizado[i] = 0;
	//
	//////calcular
	//for (int i=0; i<sizeHistograma; i++)
	//	histEqualizado[histEscaldado[i]] += histProbabilidade[i];

	//int *final = new int[sizeHistograma];
	//for (int i=0; i<sizeHistograma; i++)
	//	final[i] = (int)(histEqualizado[i]*max);


	for (int z = 0; z < size[2]; z++) {
		for (int y = 0; y < size[1]; y++) {
			for (int x = 0; x < size[0]; x++) {
				ImageType3D::IndexType position;
				position[0] = x;
				position[1] = y;
				position[2] = z;

				int value = (int)image->GetPixel(position);

				if (value == background)
					imgEqualizada->SetPixel(position, background);
				else {
					//cout<<value+(min*-1)-min<<endl;
					imgEqualizada->SetPixel(position, (int)histEscaldado[value + min] - min);
				}
			}
		}
	}
	return imgEqualizada;
}

void MedidasDiversidadeFuncional::saveImage(ImageType3D::Pointer image, string path) {
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(path);
	writer->SetInput(image);
	writer->Update();
}
//
// Carrega uma imagem Dicom em série a partir de um diretório  
ImageType3D::Pointer MedidasDiversidadeFuncional::readDICOMSeriesMD(string directoryName) {
	//configura o a leitura
	itk::ImageSeriesReader<ImageType3D>::Pointer reader = itk::ImageSeriesReader<ImageType3D>::New();
	itk::GDCMImageIO::Pointer dicomIO = itk::GDCMImageIO::New();
	reader->SetImageIO(dicomIO);

	//ler os nomes dos arquivos
	GDCMSeriesFileNames::Pointer namesGenerator = itk::GDCMSeriesFileNames::New();
	try
	{
		namesGenerator->SetDirectory(directoryName);
	}
	catch (itk::ExceptionObject &e)
	{
		ImageType3D::Pointer imgError = ImageType3D::Pointer();
		return imgError;
	}

	const auto &seriesUID = namesGenerator->GetSeriesUIDs();
	auto seriesIdentifier = seriesUID.begin()->c_str();
	auto fileNames = namesGenerator->GetFileNames(seriesIdentifier);

	//reader->SetReverseOrder(1);
	reader->SetFileNames(fileNames);

	try {
		reader->Update();
	}
	catch (itk::ExceptionObject &e) {
		std::cerr << "Erro in DicomUtil<TImage>::readSeries" << std::endl;
		std::cerr << e << std::endl;
		return nullptr;
	}

	//Return the pointer
	return reader->GetOutput();

}

ImageType3D::Pointer MedidasDiversidadeFuncional::readVolumeMD(string fileName)
{
	itk::GDCMImageIOFactory::RegisterOneFactory();

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(fileName);
	reader->Update();

	return reader->GetOutput();
}

ImageType3D::Pointer MedidasDiversidadeFuncional::createImageMD2(int x, int y, int z, ImageType3D::SpacingType spacing, int value)
{
	ImageType3D::Pointer volume = ImageType3D::New();

	ImageType3D::RegionType region;
	ImageType3D::IndexType start;
	start[0] = 0;
	start[1] = 0;
	start[2] = 0;

	ImageType3D::SizeType size;
	size[0] = x;
	size[1] = y;
	size[2] = z;

	region.SetSize(size);
	region.SetIndex(start);

	volume->SetRegions(region);
	volume->SetSpacing(spacing);
	volume->Allocate();
	volume->FillBuffer(value/*itk::NumericTraits<ITKImage3D::PixelType>::min()*/);

	return volume;
}

ImageType3D::Pointer MedidasDiversidadeFuncional::getExtraiLesao(ImageType3D::Pointer volume, ImageType3D::Pointer volumeMks)
{
	unsigned long x = volume->GetLargestPossibleRegion().GetSize()[0];
	unsigned long y = volume->GetLargestPossibleRegion().GetSize()[1];
	unsigned long z = volume->GetLargestPossibleRegion().GetSize()[2];

	ImageType3D::IndexType pixelIndex;

	ImageType3D::SpacingType spacing = volume->GetSpacing();

	//RGBImageType::Pointer imgTemp  = createImageRGB(x, y, z, spacing, itk::NumericTraits<ITKImage3D::PixelType>::min());
	ImageType3D::Pointer imagemZero = createImageMD2(x, y, z, spacing, itk::NumericTraits<ImageType3D::PixelType>::min());

	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			for (int k = 0; k < z; k++)
			{
				pixelIndex[0] = i;
				pixelIndex[1] = j;
				pixelIndex[2] = k;

				if (volumeMks->GetPixel(pixelIndex) == 1)
				{
					imagemZero->SetPixel(pixelIndex, volume->GetPixel(pixelIndex));
				}
				else {
					imagemZero->SetPixel(pixelIndex, -1000);
				}

			}
		}
	}

	return imagemZero;
}

ImageType3D::Pointer MedidasDiversidadeFuncional::createImageSintetica(ImageType3D::SpacingType spacing)
{
	ImageType3D::Pointer volume = ImageType3D::New();

	ImageType3D::RegionType region;
	ImageType3D::IndexType start;
	start[0] = 0;
	start[1] = 0;
	start[2] = 0;

	ImageType3D::SizeType size;
	size[0] = 30;
	size[1] = 30;
	size[2] = 20;

	region.SetSize(size);
	region.SetIndex(start);

	volume->SetRegions(region);
	volume->SetSpacing(spacing);
	volume->Allocate();
	volume->FillBuffer(-1024/*itk::NumericTraits<ITKImage3D::PixelType>::min()*/);

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			for (int k = 0; k < 30; k++)
			{
				ImageType3D::IndexType pixelIndex;
				pixelIndex[0] = k;
				pixelIndex[1] = j;
				pixelIndex[2] = i;

				if (((k>9) && (k <= 15)) && ((j>9) && (j <= 15)) && ((i>5) && (i <= 10)))
				{
					volume->SetPixel(pixelIndex, 1500);
				}
				if (((k>15) && (k <= 20)) && ((j>15) && (j <= 20)) && ((i>5) && (i <= 10))) {
					volume->SetPixel(pixelIndex, -250);
				}
				if (((k>20) && (k <= 25)) && ((j>20) && (j <= 25)) && ((i>5) && (i <= 10))) {
					volume->SetPixel(pixelIndex, 900);
				}


			}
		}
	}

	return volume;
}

ImageType2D::Pointer redimencionarMascara(int valor, int altura, int largura) {
	ImageType2D::Pointer input = ImageType2D::New();
	ReaderType2D::Pointer mascara = ReaderType2D::New();
	mascara->SetFileName("C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/Mascaras/Aneis/2.png");
	if (valor == 1) {
		mascara->SetFileName("C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/Mascaras/Aneis/2.png");
	}
	else if (valor == 2) {
		mascara->SetFileName("C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/Mascaras/Aneis/4.png");
	}
	else if (valor == 3) {
		mascara->SetFileName("C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/Mascaras/Aneis/5.png");
	}
	else if (valor == 4) {
		mascara->SetFileName("C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/Mascaras/Aneis/6.png");
	}
	else if (valor == 5) {
		mascara->SetFileName("C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/Mascaras/Circulos/1.png");
	}
	else if (valor == 6) {
		mascara->SetFileName("C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/Mascaras/Circulos/2.png");
	}
	else if (valor == 7) {
		mascara->SetFileName("C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/Mascaras/Circulos/4.png");
	}
	else if (valor == 8) {
		mascara->SetFileName("C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/Mascaras/Circulos/5.png");
	}
	else if (valor == 9) {
		mascara->SetFileName("C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/Mascaras/Circulos/6.png");
	}

	mascara->Update();
	input = mascara->GetOutput();
	//"C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/Mascaras/Aneis/2.png"
	//CreateImage(input);
	ImageType2D::SizeType inputSize = input->GetLargestPossibleRegion().GetSize();

	/*WriterType2D::Pointer writer = WriterType2D::New();
	writer->SetFileName("input.png");
	writer->SetInput(input);
	writer->Update();*/

	// Resize
	ImageType2D::SizeType outputSize;
	outputSize[0] = largura;
	outputSize[1] = altura;
	ImageType2D::SpacingType outputSpacing;
	outputSpacing[0] = input->GetSpacing()[0] * (static_cast<double>(inputSize[0]) / static_cast<double>(outputSize[0]));
	outputSpacing[1] = input->GetSpacing()[1] * (static_cast<double>(inputSize[1]) / static_cast<double>(outputSize[1]));

	typedef itk::IdentityTransform<double, 2> TransformType;
	typedef itk::ResampleImageFilter<ImageType2D, ImageType2D> ResampleImageFilterType;
	ResampleImageFilterType::Pointer resample = ResampleImageFilterType::New();
	resample->SetInput(input);
	resample->SetSize(outputSize);
	resample->SetOutputSpacing(outputSpacing);
	resample->SetTransform(TransformType::New());
	resample->UpdateLargestPossibleRegion();
	ImageType2D::Pointer output = resample->GetOutput();

	//std::cout << "ImagemSaida size: " << outputSize << std::endl;

	/*WriterType2D::Pointer writer = WriterType2D::New();
	writer->SetFileName("output.png");
	writer->SetInput(output);
	writer->Update();*/

	return output;

}

int getMinimumFuncoes(ImageType3D::Pointer image) {
	typedef MinimumMaximumImageCalculator <ImageType> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMinimum();
}

ImageType3D::Pointer createImageRect(ImageType3D::Pointer image, int width, int height, int contaFatias) {
	ImageType3D::Pointer newImage = ImageType3D::New();
	ImageType3D::IndexType start;
	start.Fill(0);
	ImageType3D::RegionType region;
	ImageType3D::SizeType size;

	//ImageType3D::SizeType sizeOriginal;
	size = image->GetLargestPossibleRegion().GetSize();
	size[1] = height;
	size[0] = width;
	size[2] = contaFatias;
	region.SetSize(size);
	region.SetIndex(start);
	//cout << "region.GetSize():" << region.GetSize() << endl;
	newImage->SetRegions(region);
	newImage->SetSpacing(image->GetSpacing());
	newImage->Allocate();
	newImage->FillBuffer(getMinimumFuncoes(image));
	return newImage;
}

ImageType3D::Pointer cutImage2(ImageType3D::Pointer image, bool saveImage) {

	ImageType3D::Pointer imageContorno = image;
	ImageType3D::SizeType size = image->GetLargestPossibleRegion().GetSize();
	int intensidadeMinima = (getMinimumFuncoes(image));

	int lin = size[1];
	int col = size[0];
	int menorX = 0;
	int maiorX = 0;
	int menorY = 0;
	int maiorY = 0;
	int contaFatias = 0;
	int AtivaFatia = 0;
	int AtivaLinha = 0;
	bool primeiro = true;
	Mat imagem = Mat::zeros(lin, col, 1);

	for (int z = 0; z < size[2]; z++) {
		AtivaFatia = 0;
		for (int i = 0; i < lin; i++) {
			for (int j = 0; j < col; j++) {
				ImageType3D::IndexType position;
				position[0] = j;
				position[1] = i;
				position[2] = z;
				if (((int)image->GetPixel(position) > intensidadeMinima)) {

					if (AtivaFatia == 0) {
						contaFatias++;
						AtivaFatia = 1;
					}

					//if (((int)image->GetPixel(position) > intensidadeMinima) && (i>190) && (j>120)) {
					if (primeiro) {
						primeiro = false;
						menorX = j;
						maiorX = j;
						menorY = i;
						maiorY = i;
					}

					if (j < menorX) {
						menorX = j;
					}
					else if (j > maiorX) {
						maiorX = j;
					}

					if (i < menorY) {
						menorY = i;
					}
					else if (i > maiorY) {
						maiorY = i;
					}
				}
			}
		}
	} // Fim Z

	int width = maiorX - menorX + 1;
	int height = maiorY - menorY + 1;

	//	cout << "Maior X: "<< maiorX << "Maior Y"<< maiorY << "menor X:" << menorX << "Menor Y: " << menorY << endl;

	//Mat imgsaida(image,Rect(menorX,menorY,width,height));
	Mat imgsaida2(imagem, Rect(menorX, menorY, width, height));
	//cout << "Linhas da imagem: " << imgsaida2.rows << " Colunas da imagem: " << imgsaida2.cols << endl;
	ImageType3D::Pointer imgsaida = createImageRect(image, imgsaida2.cols, imgsaida2.rows, contaFatias);
	Util util;
	//util.saveImage(imgsaida, "imagemCorteDentro.nrrd");
	/*ImageType3D::SizeType size2 = imgsaida->GetLargestPossibleRegion().GetSize();
	cout << "Linhas da imagem Original: " << size2[1] << " Colunas da imagem: " << size2[0] << endl*/;

	int jc = 0, ic = 0, zc = 0;
	ImageType3D::IndexType positionD;
	ImageType3D::IndexType positionCopia;
	ImageType3D::SizeType sizeCopia = imgsaida->GetLargestPossibleRegion().GetSize();

	for (int z = 0; z < size[2]; z++) {
		AtivaFatia = 0;
		for (int i = 0; i < lin; i++) {
			AtivaLinha = 0;
			for (int j = 0; j < col; j++) {
				positionD[0] = j;
				positionD[1] = i;
				positionD[2] = z;
				if ((((j >= menorX) && (i >= menorY)) && ((j <= maiorX) && (i <= maiorY)))) {
					if (AtivaLinha == 0) {
						AtivaLinha = 1;
					}

					if ((image->GetPixel(positionD)) > intensidadeMinima) {
						if (AtivaFatia == 0) {
							AtivaFatia = 1;
						}
						positionCopia[0] = jc;
						positionCopia[1] = ic;
						positionCopia[2] = zc;
						imgsaida->SetPixel(positionCopia, image->GetPixel(positionD));
					}

					jc++;
				}
			}
			//cout << "VALOR DO JC (COMPRIMENTO DA IMAGEM): " << jc << endl;
			jc = 0;
			if (AtivaLinha == 1) {
				ic++;
			}

		}
		if (AtivaFatia == 1) {
			zc++;
		}
		ic = 0;
		jc = 0;
	}

	//if (saveImage) imwrite(_pathFeats+"imgRecortada.png",imgsaida);;

	//util.saveImage(imgsaida, "imagemCorteDentro2.nrrd");
	return imgsaida;

}

ImageType3D::Pointer extraiAnelCirculo(ImageType3D::Pointer image, ImageType2D::Pointer imagem2D) {
	ImageType3D::SizeType size;
	ImageType2D::SizeType size2D;
	int min = getMinimumFuncoes(image);
	size = image->GetLargestPossibleRegion().GetSize();

	typedef itk::ImageDuplicator< ImageType3D > DuplicatorType;
	DuplicatorType::Pointer duplicator = DuplicatorType::New();
	duplicator->SetInputImage(image);
	duplicator->Update();
	ImageType3D::Pointer imagemNova = duplicator->GetOutput();

	for (int z = 0; z < size[2]; z++) {
		for (int y = 0; y < size[1]; y++) {
			for (int x = 0; x < size[0]; x++) {
				ImageType::IndexType position;

				position[0] = x;
				position[1] = y;
				position[2] = z;
				imagemNova->SetPixel(position, min);
			}
		} // For do Y
	}

	//size2D = imagem2D->GetLargestPossibleRegion().GetSize();
	for (int z = 0; z < size[2]; z++) {
		for (int y = 0; y < size[1]; y++) {
			for (int x = 0; x < size[0]; x++) {
				ImageType::IndexType position;
				ImageType2D::IndexType position2D;
				position2D[0] = x;
				position2D[1] = y;

				position[0] = x;
				position[1] = y;
				position[2] = z;
				int value = (int)image->GetPixel(position);

				if (((int)imagem2D->GetPixel(position2D)) > 0) {
					if (value != min) {
						imagemNova->SetPixel(position, value);
					}
				}
			}
		} // For do Y
	}
	return imagemNova;
}


void MedidasDiversidadeFuncional::executa(void) {
	cout << "\n" << "Entrou na classe executa  " << endl;
	Funcoes funcoes1;
	string savePathLBP;
	int qtdMascaras = this->_qtdMascarasInterna + _qtdMascarasExterna;
	bool usaMascaras = this->_usaMascaras;
	int rotulo = 3;

	//APAGAR AQUI
	//Mat imagem, imgROI, imgCut, imgLBP, imgQuantizada;

	ImageType3D::Pointer imgQuantizada;
	ImageType3D::Pointer imgROI;
	ImageType3D::Pointer imgCut;
	ImageType3D::Pointer imgBase;

	// E LIGAR AQUI
	/*
	//Mat imgROI
	ImageIO::Pointer imageIOOimgROI = ImageIO::New();
	ReaderType::Pointer readerimgROI = ReaderType::New();
	readerimgROI->SetFileName(path.str());
	readerimgROI->SetImageIO(imageIOOimgROI);
	readerimgROI->Update();
	ImageType::Pointer imgROI = ImageType::New();
	imgROI = readerimgROI->GetOutput();

	//Mat imgLBP
	ImageIO::Pointer imageIOOimgLBP = ImageIO::New();
	ReaderType::Pointer readerimgLBP = ReaderType::New();
	readerimgLBP->SetFileName(path.str());
	readerimgLBP->SetImageIO(imageIOOimgLBP);
	readerimgLBP->Update();
	ImageType::Pointer imgLBP = ImageType::New();
	imgLBP = readerimgLBP->GetOutput();
	*/
	////Mat imgQuantizada
	//ImageIO::Pointer imageIOOimgQuantizada = ImageIO::New();
	//ReaderType::Pointer readerimgQuantizada = ReaderType::New();
	//readerimgQuantizada->SetFileName(path.str());
	//readerimgQuantizada->SetImageIO(imageIOOimgQuantizada);
	//readerimgQuantizada->Update();
	//ImageType::Pointer imgQuantizada = ImageType::New();
	//imgQuantizada = readerimgQuantizada->GetOutput();
	//

	double df, dfAP, dfAPE, dfA, dfAE, fad, fadE, Q, QE;

	PathSystem path;
	//string pathFeats = path._pathFeatures + "caracteristica.txt";
	string pathFeats = path._pathFeaters; // + "caracteristica.txt";

	ManipulaArquivo file(pathFeats, "caracteristica.txt");

	file.limpaArquivo(pathFeats + "caracteristica.txt");
	cout << "\n" << "Entrou na linha:   " << pathFeats << endl;
	vector <double> diverdidadeFuncional, vetorPrimarioIntensidades;
	vector <vector<double>> vetorDeCaracteristicas, vetorDeCaracteristicasTemporario, vetorDeIntensidades;

	/*cout << "\n" << "Diretorio Massa:   " << _diretorioMassa.c_str() << endl;
	vector <string> vetMassa	= funcoes1.returnVetorArquivos(_diretorioMassa.c_str());
	vector <string> vetNaoMassa = funcoes1.returnVetorArquivos(_diretorioNaoMassa.c_str());

	cout << "\n" << "Tamanho do vetor de massa: " << vetMassa.size() << endl;

	vector <vector <string>> vetorPath;
	vetorPath.push_back(vetNaoMassa);
	vetorPath.push_back(vetMassa);*/
	//cout << "\n" << "Passou Vetor path: " << endl;
	// DEFINIR MASCARAS
	vector <float> vetorMascaras;
	vetorMascaras.push_back(0.8);
	//vetorMascaras.push_back(0.6);
	//vetorMascaras.push_back(0.4);
	//vetorMascaras.push_back(0.2);

	//Vetor de Quantização
	vector <int> ncolors;
	ncolors.push_back(12);
	ncolors.push_back(11);
	ncolors.push_back(10);
	ncolors.push_back(9);
	//ncolors.push_back(8);
	//int rotulo = 1;
	int pasta = 1;
	vector <string> vetMassa;
	vector <string> vetNaoMassa;
	try
	{
	
	while (pasta < 6) {
		int finalPasta = 0;
		if (pasta == 1) {
			//finalPasta = 2;
			finalPasta = 65;
		}
		else if (pasta == 2) {
			finalPasta = 52;
		}
		else if (pasta == 3) {
			finalPasta = 21;
		}
		else if (pasta == 4) {
			finalPasta = 57;
		}
		else if (pasta == 5) {
			finalPasta = 39;
		}
		MedidasDiversidadeFuncional md;
		int inicio = 1;
		cout << "\n######################################  Pasta:  " << pasta << "\n\n" << endl;
		stringstream path;
		path << "C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/BaseParaFuncionais/_  " << pasta;
		//path << "C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/_  " << "1Extraidos" << "/";

		//  " _(" << m << ") / 1 / 2 /"
		this->_diretorioMassa = path.str();
		this->_diretorioNaoMassa = path.str();
		//cout << "\n" << "Diretorio Massa:   " << _diretorioMassa.c_str() << endl;
		vetMassa = funcoes1.returnVetorArquivos(_diretorioMassa.c_str());
		vetNaoMassa = funcoes1.returnVetorArquivos(_diretorioNaoMassa.c_str());

		//cout << "\n" << "Tamanho do vetor de massa: " << vetMassa.size() << endl;

		vector <vector <string>> vetorPath;
		vetorPath.push_back(vetNaoMassa);
		vetorPath.push_back(vetMassa);
		//vetorPath.push_back(vetNaoMassa);
		//vetorPath.push_back(vetMassa);
		//while ((vetorPath.size()) > 0) {
			vector <string> vetorDeArquivos = vetorPath.back();
			this->_rotulo = pasta;
			vetorPath.pop_back();
			//cout << "\n" << "Tamanho do  Vetor path: "<< vetorPath.size() << endl;
			if (vetorPath.size() == 1) {
				savePathLBP = "Massa/";
				rotulo = pasta;
				this->_rotulo = pasta;
				inicio = 1;
				//inicio = 1227; //inicio = 1369;
				//inicio = 2049; //inicio = 2738;
				//inicio = 2904; //inicio = 4113;
				//inicio = 4076; //inicio = 5489;
				//inicio = 4501; //inicio = 5953;
			}
			else {
				savePathLBP = "Nao_Massa/";
				rotulo = pasta;
				this->_rotulo = pasta;
				inicio = 1;
				//inicio = 2417; //inicio = 2897;
				//inicio = 4656; //inicio = 6085;
				//inicio = 7095; //inicio = 8936;
				//inicio = 9959; //inicio = 11827;
				//inicio = 12453; //inicio = 14748;
				//inicio = 14712; //inicio = 17510;
				//inicio = 17071; //inicio = 20205;
				//inicio = 19658; //inicio = 23153;
				//inicio = 21784; //inicio = 26232;
				//inicio = 24039; //inicio = 29558;
				//inicio = 26516; //inicio = 32473;
				//inicio = 28838; //inicio = 35393;
				//inicio = 31296; //inicio = 38121;
				//inicio = 34162; //inicio = 41308;
				//inicio = 36722; //inicio = 44273;
				//inicio = 38930; //inicio = 47243;
				//inicio = 41467; //inicio = 49704;
				//inicio = 43662; //inicio = 52319;
				//inicio = 55082;
				//inicio = 57670;
				//inicio = 60228;
				//inicio = 63060;

			}

			////Rotulo da classe
			//int inicio = 0;
			//Normal sem realce com Mascara - data: 27/11/2015		
			//int inicio = 1369;
			//int inicio = 2738;
			//int inicio = 4113;
			//int inicio = 5489;
			//int inicio = 5953;

			//Normal sem realce com Mascara Interna e Externa - data: 27/11/2015
			//int inicio = 1261;
			//int inicio = 5770;
			//cout << "\n" << "Tamanho do vetorDeArquivos : " << vetorDeArquivos.size() << endl;
			//cout << "\n" << "Tamanho do inicio:  " << inicio << endl;


			for (int i = inicio; i < finalPasta; i++) { // FOR VETOR DE ARQUIVOS
				//flush();
				stringstream path2;
				path2 << path.str() << "/_  (" << i << ")";
				//setbuf(stdout, 0);
			//for (int i=0; i < 3; i++){

				if (i == 50) {
					int parte = 1;
					string bla = "pare";
					cout << bla << " " << i << endl;
					//system("pause");
				}

				stringstream pathFatias;

				pathFatias << path2.str() << "/1/2/";
				stringstream pathMascaras;
				pathMascaras << path2.str() << "/1/3/MarkingsVolume.nii";

				string nameImage = pathFatias.str();
				//cout << "\nLendo diretorio da imagem original" << endl;
				cout << "\Caminhoo:" << nameImage << endl;
				ImageType3D::Pointer imagemOriginal = readDICOMSeriesMD(pathFatias.str());
				//saveImage(imagemOriginal, "FILHODAPUTA.nrrd");
				//cout << "\nLendo diretorio da mascara" << endl;
				//cout << "\Caminhoo:" << pathMascaras.str() << endl;
				ImageType3D::Pointer imagemMks = readVolumeMD(pathMascaras.str());
				//	cout << "\nExtraindo Lesao" << endl;
				ImageType3D::Pointer lesao;
				lesao = getExtraiLesao(imagemOriginal, imagemMks);

				//ImageType3D::Pointer lesao = getExtraiLesao(imagemOriginal, imagemMks);
				typedef itk::ImageDuplicator< ImageType3D > DuplicatorType;
				DuplicatorType::Pointer duplicator = DuplicatorType::New();
				duplicator->SetInputImage(lesao);
				duplicator->Update();
				ImageType3D::Pointer imagem = duplicator->GetOutput();

				imagem = cutImage2(imagem, 0);

				//saveImage(imagem, "imagemUsar.nrrd");
				md._valorMinimoDaimagem = getMinimum1(imagem);

				//	cout << "Criando imagem Sintética" << endl;
				//ImageType3D::Pointer imagemSintetica = createImageSintetica(imagem->GetSpacing());
				////saveImage(imagemSintetica, "imagemSintetica.nrrd");
				////imagem = imagemSintetica;
				////saveImage(imagem, "imagemSinteticaImagem.nrrd");
				////	cout << "\n Colocando instensidades no txt ... \n" << endl;

				//ImageType3D::SizeType sizeLinhasImagem;
				//sizeLinhasImagem = imagemSintetica->GetLargestPossibleRegion().GetSize();

				//int lin = sizeLinhasImagem[1];
				//int col = sizeLinhasImagem[0];
				//for (int z = 0; z < sizeLinhasImagem[2]; z++) {
				//	for (int i = 0; i < lin; i++) {
				//		for (int j = 0; j < col; j++) {
				//			ImageType3D::IndexType position;
				//			position[0] = j;
				//			position[1] = i;
				//			position[2] = z;
				//			vetorPrimarioIntensidades.push_back(imagemSintetica->GetPixel(position));
				//		}
				//		vetorDeIntensidades.push_back(vetorPrimarioIntensidades);
				//		vetorPrimarioIntensidades.clear();
				//	}
				//}

				//funcoes1.saveIntensidadesImagem(vetorDeIntensidades);
				//vetorDeIntensidades.clear();
				//	cout << "\n ###################### Colocou instensidades no txt #####################################... \n" << endl;

				//saveImage(imagem, "lesao.nrrd");

				imagem = funcoes1.cutImage(imagem, 0);
				ImageType3D::SizeType TamanhoGrandeDemais;
				TamanhoGrandeDemais = imagem->GetLargestPossibleRegion().GetSize();
				cout << imagem->GetLargestPossibleRegion().GetSize() << endl;
				//saveImage(imagem, "ImagemCortada.nrrd");

				if ((TamanhoGrandeDemais[0] != 512)) {  // IF TAMANHO DAS IMAGENS GRANDES


					//	cout << "\n" << "Nome do  Vetor vetorDeArquivos: " << nameImage << endl;

					// Pega o tamanho da string até a última barra "/ ou \\"			
					size_t nameArquivo = nameImage.find_last_of("/\\");


					//Mat imagem, imgROI, imgCut, imgRealce, imgLBP;
					//LBPImage lbpImage;
					// Transforma a imagem em LBP
					// Tipo de Imagem: 1=Normal, 2=LBP

					//ImageType3D::Pointer imagem;
					// Imagem no formato LBP       {
					if (_tipoImage == 2) {
						//imagem = imread(nameImage, CV_LOAD_IMAGE_GRAYSCALE);						
						////medianBlur(imagem,imgLBP,3);	*************************					
						//string pathSave = path._pathLBP + savePathLBP + nameImage.substr(nameArquivo+1);
						////imgLBP = lbpImage.LBPImageTranfer(imgLBP, pathSave, 0);
						//imgLBP = lbpImage.LBPImageTranfer(imagem, pathSave, 0);
						////imgCut = funcoes1.cutImage(imgLBP,0);
						////imgRealce = funcoes1.realceHistogramaLocal(imgCut);	

						//equalizeHist(imgCut,imgROI);
						//imgROI = imgCut;										
						//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/LBP_" + nameImage.substr(nameArquivo+1), imgROI);					
					}
					//  } // FecHA LBP
					else {

						// iMAGEM OPENCV
						//imagem = imread(nameImage, CV_LOAD_IMAGE_GRAYSCALE);


						//IMAGEM ITK
						//Mat imagem
						//ImageIO::Pointer imageIOOimagem = ImageIO::New();
						//ReaderType::Pointer readerimagem = ReaderType::New();
						//readerimagem->SetFileName(nameImage);
						//readerimagem->SetImageIO(imageIOOimagem);
						//readerimagem->Update();
						//ImageType::Pointer imagem = ImageType::New();
						//imagem = readerimagem->GetOutput();
						///**/

						/*imagem = readVolume1(nameImage);
						saveImage(imagem,"readVolume1.nrrd");*/
						//saveImage(imagem, "teste.nrrd");
						/*
						// IMAGEM ITK
						//Mat imgCut
						ImageIO::Pointer imageIOOimgCut = ImageIO::New();
						ReaderType::Pointer readerimgCut = ReaderType::New();
						readerimgCut->SetFileName(path.str());
						readerimgCut->SetImageIO(imageIOOimgCut);
						readerimgCut->Update();
						ImageType::Pointer imgCut = ImageType::New();
						imgCut = readerimgCut->GetOutput();

						*/

						// funcao OPENCV
						//imgCut = funcoes1.cutImage(imagem,0);


						//saveImage(imgCut, "imgCut.nrrd");
						//imgROI = funcoes1.realceHistogramaLocal(imgCut);


						//imgROI = equalizacaoHistograma(imgCut);
						//equalizeHist(imgCut,imgROI);

						//imgROI = imgCut;
						//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/Realce_" + nameImage.substr(nameArquivo+1), imgROI);

						//Grava as ROI - BoundingBox
						if (rotulo == 1) {
							//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/6-BoundingBoxe-ROIs/Massa/" + nameImage.substr(nameArquivo+1), imgROI);				
						}
						else {
							//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/6-BoundingBoxe-ROIs/Nao_Massa/" + nameImage.substr(nameArquivo+1), imgROI);				
						}
					}
					int qtdColor = 0;
					int usaMascaras = 1;
					int numMascara = 1;
					//int qtdMascaras = 10;
					int qtdMascaras = 10;
					int qtdNColors = ncolors.size();
					int entraMascara = 0;

					//QUANTIZA A IMAGEM AQUI

					// for (qtdColor = 0; qtdColor < qtdNColors; qtdColor++ ){
					while (qtdColor < qtdNColors) { // WHILE DO QUANTIDADE COLORS
						numMascara = 1;

						if (usaMascaras == 0) { //Caso nao queira usar as mascaras
							qtdMascaras = 1; // Só para saber se vai usar as mascaras
						}
						// MODIFICAR PARA 0 PARA DAR CERTO
						entraMascara = 0;
						while (numMascara<qtdMascaras) {                  // WHILE DO NUMERO DE MASCARAS
							cout << "\n numMascara = " << numMascara << " e QtsMascaras =   " << qtdMascaras << endl;

							if (qtdColor == 0) {
								imgQuantizada = imagem;
								//saveImage(imgQuantizada,"ImagemQuantizadaWhile.nrrd");
								//saveImage(imagem, "teste.nrrd");
							}
							else {
								// ncolors é o vetor que tem o valor da quantização e o qtdColor indica a posicao
								Quantizacao *quant = new Quantizacao(imagem, ncolors[qtdColor]);
								imgQuantizada = quant->getVOI();
								quant->~Quantizacao();

								//imgQuantizada = funcoes1.quantization(imagem,ncolors[qtdColor]);
								//saveImage(imgQuantizada, "ImagemQuantizadaWhile.nrrd");
								//saveImage(imagem, "imgQuantizadaELSE.nrrd");

							}

							ImageType::SizeType size;
							size = imgQuantizada->GetLargestPossibleRegion().GetSize();
							ImageType2D::Pointer ImagemMascara2D;

							if (entraMascara == 1) {
								ImagemMascara2D = redimencionarMascara(numMascara, size[1], size[0]);
								//tools.saveImage2D(ImagemMascara2D, "imagemMascara9.dcm");
								imgQuantizada = extraiAnelCirculo(imgQuantizada, ImagemMascara2D); // Passando a imagem e a sua mascara para extrair o anel
																								   //tools.saveImage(imgQuantizada, "imagemAnelCirculo9.nrrd");
							}

							//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/Qtz_" + nameImage.substr(nameArquivo+1), imgQuantizada);
							imgBase = imgQuantizada;
							//saveImage(imagem, "imgBase.nrrd");

							// Gera as mascaras das imagens
							int contaMascaras = 0;
							int contaMascarasExterna = 0;
							int qualMascara = 1; // 1-> Mascara Interna; 2-> Mascara Externa
							//qtdMascaras = 0;
								//contaMascaras++;

							   //imgQuantizada = funcoes1.quantization(imgROI,ncolors[qtdColor]);
							   //imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/Qtz_" + nameImage.substr(nameArquivo+1), imgQuantizada);
							   //Mat imgtemp;

							   //funcoes1.kmeansClustering(imgtemp);			
								//wClustering
								//float limiar1 = funcoes1.Otsu2(imgQuantizada); //Voltar para imgtemp ********
								//	 cout << "Entrou no primeiro otsu de todos!" << endl;
								float limiar1 = wClustering(imgQuantizada, 2); //Voltar para imgtemp ********
					   //		 cout << "\nValor do primeiro otsu de todos!" << limiar1 <<endl;
							   /* QuickView viewer;
								viewer.AddImage(
								   imgQuantizada.GetPointer(), true,"Generated image");
							   cout << "Valor dao limiar do otsu: "<< limiar1 << endl;*/
								Arvores arvore;
								//vector <No> vetorArvore = arvore.criaArvore(imagem);
								vector <No> vectorNos;
								vector <int> vetRaiz;
								int posicao = 0;
								//vetRaiz.push_back(limiar1);
								No no;
								no.pos = 0;
								no.pai = 0;
								no.esp = limiar1;
								no.esq = 0;
								no.dir = 0;
								no.ind = arvore.returnQuantidadePixelsImage(imgQuantizada, limiar1);
								//		cout << " Quantidade de pixels da imagem igual do limiar No.ind  : " << no.ind << endl;
								vectorNos.push_back(no);
								vetRaiz.push_back(limiar1);
								//arvore.criaArvore(imagem, imgtemp, limiar1, posicao, vectorNos, vetRaiz);
								//arvore.criaArvore(imgROI, imgtemp, limiar1, posicao, vectorNos, vetRaiz);
								arvore.criaArvore(imgQuantizada, imgQuantizada, limiar1, posicao, vectorNos, vetRaiz);

								//Calcula Medida de Diversidade Funcional ---------------------------------------------
								//======================================================================================
								//Diversidade  funcional
								df = retunrDiverdidadeFuncional(vectorNos, md._valorMinimoDaimagem);
								/*	for (int i = 0; i < vectorNos.size();i++) {
										cout << "\ndir " << vectorNos.at(i).dir << endl;
										cout << "esp " << vectorNos.at(i).esp << endl;
										cout << "esq " << vectorNos.at(i).esq << endl;
										cout << "ind " << vectorNos.at(i).ind << endl;
										cout << "pai " << vectorNos.at(i).pai << endl;
										cout << "pos " << vectorNos.at(i).pos << endl;
										cout << "\n";

									}*/


									//Diversidade  funcional - Abundante em relação ao valor pixel e adistancia das posições		
								dfAP = retunrDiverdidadeFuncionalAbundantePixel(vectorNos, md._valorMinimoDaimagem);

								//Diversidade  funcional - Abundante em relação ao valor pixel e adistancia das especies			
								dfAPE = retunrDiverdidadeFuncionalAbundantePixelEspecie(vectorNos, md._valorMinimoDaimagem);

								//Diversidade  funcional - Abundante			
								dfA = retunrDiverdidadeFuncionalAbundante(vectorNos, md._valorMinimoDaimagem);

								//Diversidade  funcional - Abundante Especie
								dfAE = retunrDiverdidadeFuncionalAbundanteEspece(vectorNos, md._valorMinimoDaimagem);

								// FAD - dos tracos da arvore
								fad = retunrDiverdidadeAtributoFuncional(vectorNos, md._valorMinimoDaimagem);

								// FAD - das especies 
								fadE = retunrDiverdidadeAtributoFuncionalEspecie(vectorNos, md._valorMinimoDaimagem);

								// Entropia Quadrática (Q) - FAD - traços
								Q = retunrEntropiaQuadratica(fad, vectorNos, md._valorMinimoDaimagem);

								// Entropia Quadrática (Q) - FADe - Pixel
								QE = retunrEntropiaQuadraticaEspecie(fadE, vectorNos, md._valorMinimoDaimagem);

								diverdidadeFuncional.push_back(df);
								diverdidadeFuncional.push_back(dfAP);
								diverdidadeFuncional.push_back(dfAPE);
								diverdidadeFuncional.push_back(dfA);
								diverdidadeFuncional.push_back(dfAE);
								diverdidadeFuncional.push_back(fad);
								diverdidadeFuncional.push_back(fadE);
								diverdidadeFuncional.push_back(Q);
								diverdidadeFuncional.push_back(QE);

								

								// Medidas de Forma
								//DescritorFormas descritorFormas;
								////Fator de Forma Circular = FFC
								//float FFC = descritorFormas.circularidade(imgQuantizada);
								////Fator de Forma Circular Comprimento = FFCc
								//float FFCc = descritorFormas.calculaAlogamentoImage(imgQuantizada);
								////Razão de aspecto de Alongamentos
								//float RA = descritorFormas.calculaRazaoDeAspectoImage(imgQuantizada);

								//diverdidadeFuncional.push_back(FFC);
								//diverdidadeFuncional.push_back(FFCc);
								//diverdidadeFuncional.push_back(RA);
								//----------------------------------------------------------------

								//--- Moran features
								//double moran;
								//double stdDevMoran;
								//cvaux::MoranIndicator(imgQuantizada,&moran,&stdDevMoran,3,200,imgQuantizada);
								////cout << moran << endl;
								//
								//diverdidadeFuncional.push_back(moran);
								//---------------------------------------------------------------


								//Haralick Features--------------------------------------------
								//int nlevels = 256;
								//int nDistancia = 4 * 2; // valor para calcular as distancias da matriz (4_angulos * valor_distancia) EX: para distancia de 1 a 4, o valor será 4 * 4 = 16 
								///*for(int k=0,dist=1,theta=0; k < nDistancia; k++,theta+=45) {
								//	
								//	if(theta > 135){theta = 0; dist++;}
								//	if(dist > 2) dist = 1;*/
								//	
								//	int k=0,dist=1,theta=0;
								//	HaralickFeature haralickFeature;
								//	vector<double> tempFeats = haralickFeature.extractTextureFeatures(imgQuantizada, dist,theta,nlevels);
								//	
								//	//Recebe as caracteristicas Haralick
								//	vetorDeCaracteristicasTemporario.push_back(tempFeats);

								//	tempFeats.clear();
								//}
								//

								//int lin = vetorDeCaracteristicasTemporario.size();
								//int col = vetorDeCaracteristicasTemporario.at(0).size();
								//
								//for (int i=0; i < lin; i++){
								//	for (int j=0; j < col; j++) {
								//	
								//		diverdidadeFuncional.push_back(vetorDeCaracteristicasTemporario.at(i)[j]);
								//	}
								//}
								//-------------------------------------------------------------------------

								vetorDeCaracteristicas.push_back(diverdidadeFuncional);

								vetorDeCaracteristicasTemporario.clear();
								diverdidadeFuncional.clear();
								//fflush(stdout);
								//Limpa os vetores
								vectorNos.clear();
								vetRaiz.clear();

								arvore.~Arvores();
								/*imagem.release();

								imgQuantizada.release();
								imgCut.release();
								imgLBP.release();
								lbpImage.~LBPImage();*/

								//cout << "Tamanho do vetor de arquivo das mascaras: " <<vetorMascaras.size() << endl;
								//cout << "Tamanho do contador de mascaras: " << contaMascaras<< endl;
								////*********** Gera mascara Interna
								//if (usaMascaras) {
								//	cout << "ENTROU NO USA MASCARA!" << endl;
								//	if (contaMascaras < vetorMascaras.size()) {
								//		cout << "ENTROU NO 'IF' DO USA MASCARA!" << endl;
								//		MascaraImagem mascaraImagem;
								//		//Mat imgMascara = mascaraImagem.retornaMascaraImage(imgBase, vetorMascaras.at(contaMascaras));
								//		//saveImage(imgBase, "imgEntradaMascara.nrrd");
								//		//ImageType3D::Pointer imgMascara = mascaraImagem.retornaMascaraImage(imgBase, vetorMascaras.at(contaMascaras));
								//		ImageType3D::Pointer imgMascara = mascaraImagem.retornaMascaraImage(imgBase, vetorMascaras.at(0));
								//		saveImage(imgMascara, "imgMascaraInterna.nrrd");
								//		//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/1-MascaraOriginal_" + nameImage.substr(nameArquivo+1), imgBase);
								//		//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/2-MascaraInterna_" + nameImage.substr(nameArquivo+1), imgMascara);

								//		//imgQuantizada = funcoes1.quantization(imgMascara,ncolors[qtdColor]);
								//		ImageType3D::Pointer imgQuantizada2 = funcoes1.cutImage(imgQuantizada, 0);
								//		saveImage(imgQuantizada2, "imgMascaraCUTimage.nrrd");
								//		//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/3-MascaraInternaROI_" + nameImage.substr(nameArquivo+1), imgQuantizada);					

								//	}// else {
								//	//	qualMascara = 2;
								//	//	//*********** Gera mascara Externa
								//	//	if (contaMascarasExterna < vetorMascaras.size()) {
								//	//		
								//	//		Mat imgMascaraExterna;

								//	//		if (contaMascarasExterna == 0){
								//	//			MascaraImagem mascaraImagem;
								//	//			Mat imgMascara1 = mascaraImagem.retornaMascaraImage(imgBase, vetorMascaras.at(contaMascarasExterna));
								//	//			//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/4-MascaraBase_" + nameImage.substr(nameArquivo+1), imgBase);
								//	//			//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/5-MascaraExterna1_" + nameImage.substr(nameArquivo+1), imgMascara1);
								//	//		
								//	//			imgMascaraExterna = imgBase - imgMascara1;
								//	//			//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/6-MascaraExternaROI_" + nameImage.substr(nameArquivo+1), imgMascaraExterna);

								//	//			imgQuantizada = funcoes1.cutImage(imgMascaraExterna,0);
								//	//		}else{

								//	//			MascaraImagem mascaraImagem;
								//	//			Mat imgMascara1 = mascaraImagem.retornaMascaraImage(imgBase, vetorMascaras.at(contaMascarasExterna - 1));
								//	//			//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/7-Mascara1_" + nameImage.substr(nameArquivo+1), imgMascara1);
								//	//			
								//	//			Mat imgMascara2 = mascaraImagem.retornaMascaraImage(imgBase, vetorMascaras.at(contaMascarasExterna));
								//	//			//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/8-Mascara2_" + nameImage.substr(nameArquivo+1), imgMascara2);

								//	//			imgMascaraExterna = imgMascara1 - imgMascara2;
								//	//			//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/9-MascaraExterna2_" + nameImage.substr(nameArquivo+1), imgMascaraExterna);

								//	//			//imgQuantizada = funcoes1.quantization(imgMascara,ncolors[qtdColor]);
								//	//			imgQuantizada = funcoes1.cutImage(imgMascaraExterna,0);
								//	//			//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/10-MascaraExterna2ROI_" + nameImage.substr(nameArquivo+1), imgQuantizada);
								//	//		}
								//	//	}
								//	//	contaMascarasExterna++;
								//	//}

								//}
								//else {

								//	break;
								//}
								//cout << "CONTA MASCARAS ++" << endl;
							
							if (entraMascara == 1) {
								numMascara++;
							}
							entraMascara = 1;
						} // FIM WHILE DO NUMERO DE MASCARAS
						qtdColor++;
					} // FIM WHILE DO QUANTIDADE COLORS

					/*imgBase.release();
					imgROI.release();*/

					funcoes1.saveFeatures(vetorDeCaracteristicas, rotulo);
					vetorDeCaracteristicas.clear();

					//funcoes1.saveFeatures(vetorDeCaracteristicas,rotulo);


				} // FIM  IF TAMANHO DAS IMAGENS GRANDES
				else {
					stringstream teste;
					teste << vetorDeArquivos.at(i) << ".nrrd";
					saveImage(imagem, teste.str());
				} // FIM DO FOR 1

			//Rotulo da classe
			//rotulo--;

			} // FIM FOR VETOR DE ARQUIVOS

			vetorPath.clear();
			funcoes1.~Funcoes();
			pasta++;
			//funcoes1.saveFeatures(vetorDeCaracteristicas,1);
	}
  }  // try catch
  catch (int e)
  {
	  cout << "An exception occurred. Exception Nr. " << e << '\n';
  }
	vetMassa.clear();
	vetNaoMassa.clear();
	
}


//Retiorna a Diversidade Funcional - FD - Dendrograma
float MedidasDiversidadeFuncional::retunrDiverdidadeFuncional(vector <No> vetorDeNos, int fundo){

	float df = 1.0;

	for (int i=1; i< vetorDeNos.size(); i++){
		df++;
		if (vetorDeNos.at(i).esq > fundo){
			df++;
		}
		if (vetorDeNos.at(i).dir > fundo){
			df++;
		}
	}

	return df;
}

//Retorna a Diversidade Funcional - FDaP - Abundante em relação ao valor do pixel 
//sum (sum( d_ij.Pi.Pj))
float MedidasDiversidadeFuncional::retunrDiverdidadeFuncionalAbundantePixel(vector <No> vetorDeNos, int fundo){

	float FDaP = 0;
	int tamVetor = vetorDeNos.size();		
	int qtdEpecies = 0;
	int abundancia = 0; 

	for(int i=0; i < tamVetor; i++ ){
		if (vetorDeNos.at(i).ind > 0){
			int vetEsp = vetorDeNos.at(i).esp;
			if (vetEsp < 0){
				vetEsp = vetEsp*(-1);
			}
			abundancia += vetEsp;
			//abundancia += vetorDeNos.at(i).esp;
		}
	}

	if (abundancia == 0){
		return FDaP;
	}

	for(int i = 0; i < tamVetor; i++){
			
		if (vetorDeNos.at(i).ind > 0){
			
			//float distanciaI = vetorDeNos.at(i).pos;			
			float distanciaI = vetorDeNos.at(i).pos;
			
			//float pixel_i = vetorDeNos.at(i).esp;
			float pixel_i;
			if (vetorDeNos.at(i).esp<0) {
				pixel_i = (vetorDeNos.at(i).esp*(-1));
			}
			else {
				pixel_i = vetorDeNos.at(i).esp;
			}

			qtdEpecies++;
				
			for(int j = 0; j < tamVetor; j++){
				
				if (vetorDeNos.at(j).ind > 0){							
					//float pixel_j = vetorDeNos.at(j).esp;
					float pixel_j = vetorDeNos.at(j).esp;

					if (vetorDeNos.at(j).esp<0) {
						pixel_j = (vetorDeNos.at(j).esp*(-1));
					}
					else {
						pixel_j = vetorDeNos.at(j).esp;
					}

					//fdA += sqrt(pow(distanciaI - vetorDeNos.at(j).pos, 2)) * (pixel_i / abundancia) * (pixel_j / abundancia);
					FDaP += fabs(distanciaI - vetorDeNos.at(j).pos) * (pixel_i / abundancia) * (pixel_j / abundancia);
					//fdA += dij * (pixel_i / abundancia) * (pixel_j / abundancia);			
				}
			}
		}							
	}
	return FDaP;
	
}

//Retorna a Diversidade Funcional - FDaPE - Abundante em relação ao valor do pixel com a distancia das especias
//sum (sum( d_ij.Pi.Pj))
float MedidasDiversidadeFuncional::retunrDiverdidadeFuncionalAbundantePixelEspecie(vector <No> vetorDeNos, int fundo){

	float FDaPE = 0;
	int tamVetor = vetorDeNos.size();		
	int qtdEpecies = 0;
	int abundancia = 0; 

	for(int i=0; i < tamVetor; i++ ){
		/*if (vetorDeNos.at(i).ind > 0){
			abundancia += vetorDeNos.at(i).esp;
		}*/

		if (vetorDeNos.at(i).ind > 0) {
			int vetEsp = vetorDeNos.at(i).esp;
			if (vetEsp < 0){
				vetEsp = vetEsp*(-1);
			}
			abundancia += vetEsp;
			//abundancia += vetorDeNos.at(i).esp;
		}

	}

	for(int i = 0; i < tamVetor; i++){
			
		if (vetorDeNos.at(i).ind > 0){
			
			//float distanciaI = vetorDeNos.at(i).esp;			
			//float pixel_i = vetorDeNos.at(i).esp;

			float pixel_i, distanciaI;
			if (vetorDeNos.at(i).esp<0) {
				pixel_i = (vetorDeNos.at(i).esp*(-1));
				distanciaI = (vetorDeNos.at(i).esp*(-1));
			}
			else {
				distanciaI = vetorDeNos.at(i).esp;
				pixel_i = vetorDeNos.at(i).esp;
			}

			qtdEpecies++;
				
			for(int j = 0; j < tamVetor; j++){
				
				if (vetorDeNos.at(j).ind > 0){							
					float pixel_j = vetorDeNos.at(j).esp;

					if (vetorDeNos.at(j).esp<0) {
						pixel_j = (vetorDeNos.at(j).esp*(-1));
					}
					else {
						pixel_j = vetorDeNos.at(j).esp;
					}

					//fdA += sqrt(pow(distanciaI - vetorDeNos.at(j).pos, 2)) * (pixel_i / abundancia) * (pixel_j / abundancia);
					//FDaPE += fabs(distanciaI - vetorDeNos.at(j).esp) * (pixel_i / abundancia) * (pixel_j / abundancia);
					FDaPE += fabs(distanciaI - pixel_j) * (pixel_i / abundancia) * (pixel_j / abundancia);
					//fdA += dij * (pixel_i / abundancia) * (pixel_j / abundancia);			
				}
			}
		}							
	}
	return FDaPE;
	
}

//Retiorna a Diversidade Funcional - FDa - Abundante - Posição na arvore = *******************************
//sum (sum( d_ij.Pi.Pj))
float MedidasDiversidadeFuncional::retunrDiverdidadeFuncionalAbundante(vector <No> vetorDeNos, int fundo){

	float fdA = 0;
	int tamVetor = vetorDeNos.size();		
	int qtdEpecies = 0;
	int abundancia = 0; 

	for(int i=0; i < tamVetor; i++ ){
		if (vetorDeNos.at(i).ind > 0){
			abundancia += vetorDeNos.at(i).ind;
		}
	}

	for(int i = 0; i < tamVetor; i++){
			
		if (vetorDeNos.at(i).ind > 0){
			
			float distanciaI = vetorDeNos.at(i).pos;			
			float pixel_i = vetorDeNos.at(i).ind;
			qtdEpecies++;
				
			for(int j = 0; j < tamVetor; j++){
				
				//if (vetorDeNos.at(j).ind > 0){							
					float pixel_j = vetorDeNos.at(j).ind;
					//fdA += sqrt(pow(distanciaI - vetorDeNos.at(j).pos, 2)) * (pixel_i / abundancia) * (pixel_j / abundancia);
					fdA += fabs(distanciaI - vetorDeNos.at(j).pos) * (pixel_i / abundancia) * (pixel_j / abundancia);
					//fdA += dij * (pixel_i / abundancia) * (pixel_j / abundancia);			
				//}
			}
		}							
	}
	return fdA;

	//for(int i = 0; i < tamVetor; i++){
	//		
	//	if (vetorDeNos.at(i).ind > 0){
	//		
	//		float distanciaI = vetorDeNos.at(i).pos;			
	//		float pixel_i = vetorDeNos.at(i).ind;
	//		qtdEpecies++;
	//			
	//		for(int j = i+1; j < tamVetor; j++){
	//			float dij = 0;
	//			int n =  j-i;
	//			if (vetorDeNos.at(j).ind > 0){
	//				for(int k = 0; k <n; k++){
	//					//dij+= fabs((xik) - (xjk));						
	//					if(vetorDeNos.at(k).ind > 0){
	//						dij+= fabs(abs(distanciaI - vetorDeNos.at(k).pos) - abs(vetorDeNos.at(j).pos - vetorDeNos.at(k).pos));
	//					}
	//				}
	//				dij/=(float)n;					
	//				float pixel_j = vetorDeNos.at(j).ind;
	//				//fdA += sqrt(pow(distanciaI - vetorDeNos.at(j).pos, 2)) * (pixel_i / abundancia) * (pixel_j / abundancia);
	//				fdA += dij * (pixel_i / abundancia) * (pixel_j / abundancia);
	//			}
	//		}
	//	}							
	//}
	//return fdA;
}

//Retiorna a Diversidade Funcional - FDaE - Abundante - Valor do Pixel FD = sum (sum( d_ij.Pi.Pj))
float MedidasDiversidadeFuncional::retunrDiverdidadeFuncionalAbundanteEspece(vector <No> vetorDeNos, int fundo){

	float fdAE = 0;
	int tamVetor = vetorDeNos.size();		
	int qtdEpecies = 0;
	int abundancia = 0; 

	for(int i=0; i < tamVetor; i++ ){
		if (vetorDeNos.at(i).ind > 0){
			abundancia += vetorDeNos.at(i).ind;
		}
	}


	for(int i = 0; i < tamVetor; i++){			
		if (vetorDeNos.at(i).ind > 0){			
			float distanciaI = vetorDeNos.at(i).esp;

			if (vetorDeNos.at(i).esp<0) {
				distanciaI = (vetorDeNos.at(i).esp*(-1));
			}
			else {
				distanciaI = vetorDeNos.at(i).esp;
			}

			float pixel_i = vetorDeNos.at(i).ind;
			qtdEpecies++;

			for(int j = 0; j < tamVetor; j++){
				
				if (vetorDeNos.at(j).ind > 0){				
					float pixel_j = vetorDeNos.at(j).ind;
					float espNo;
					if (vetorDeNos.at(j).esp<0) {
						espNo = ( vetorDeNos.at(j).esp*(-1) );
					}
					else {
						espNo = vetorDeNos.at(j).esp;
					}
					//dij+= fabs(abs(distanciaI - vetorDeNos.at(k).esp) - abs(vetorDeNos.at(j).esp - vetorDeNos.at(k).esp));
					//fdAE += fabs(distanciaI - vetorDeNos.at(j).esp) * (pixel_i / abundancia) * (pixel_j / abundancia);				
					fdAE += fabs(distanciaI - espNo) * (pixel_i / abundancia) * (pixel_j / abundancia);
				}								
			}			
		}						
	}

	return fdAE;

	//for(int i = 0; i < tamVetor; i++){			
	//	if (vetorDeNos.at(i).ind > 0){			
	//		float distanciaI = vetorDeNos.at(i).esp;			
	//		float pixel_i = vetorDeNos.at(i).ind;
	//		qtdEpecies++;

	//		for(int j = i+1; j < tamVetor; j++){
	//			float dij = 0;
	//			int n =  j-i;
	//			if (vetorDeNos.at(j).ind > 0){
	//				for(int k = 0; k <n; k++){
	//					//dij+= fabs((xik) - (xjk));
	//					if (vetorDeNos.at(j).ind > 0){
	//						dij+= fabs(abs(distanciaI - vetorDeNos.at(k).esp) - abs(vetorDeNos.at(j).esp - vetorDeNos.at(k).esp));
	//					}
	//				}
	//				dij/=(float)n;
	//				float pixel_j = vetorDeNos.at(j).ind;
	//				fdAE +=  dij * (pixel_i / abundancia) * (pixel_j / abundancia);
	//			}				
	//		}			
	//	}						
	//}

	//return fdAE;
}

//Retorna a Diversidade Atributo Funcional - FAD - dos tracos da arvore (dendrograma)
float MedidasDiversidadeFuncional::retunrDiverdidadeAtributoFuncional(vector <No> vetorDeNos, int fundo){

	float fad = 0;
	int tamVetor = vetorDeNos.size();
	int qtdEspece = 0;
	


	//for(int i = 0; i < tamVetor; i++){
	//	if (vetorDeNos.at(i).ind > 0){
	//		
	//		float distanciaI = vetorDeNos.at(i).pos;
	//		qtdEspece++;

	//		for(int j = i+1; j < tamVetor; j++){
	//			float dij = 0;
	//			int n =  j-i;
	//			if (vetorDeNos.at(j).ind > 0){
	//				for(int k = 0; k <n; k++){
	//					//dij+= fabs((xik) - (xjk));
	//					if (vetorDeNos.at(j).ind > 0){
	//						dij+= fabs(abs(distanciaI - vetorDeNos.at(k).pos) - abs(vetorDeNos.at(j).pos - vetorDeNos.at(k).pos));
	//					}
	//				}
	//				dij/=(float)n;
	//				//float pixel_j = vetorDeNos.at(j).ind;
	//				fad +=  dij ; //* (pixel_i / abundancia) * (pixel_j / abundancia);
	//			}				
	//		}
	//	}
	//}

								
	for(int i = 0; i < tamVetor; i++){
		if (vetorDeNos.at(i).ind > 0){
			
			float distanciaI = vetorDeNos.at(i).pos;
			qtdEspece++;

			for(int j = 0; j < tamVetor; j++){
				if (vetorDeNos.at(j).ind > 0){
					fad += fabs(distanciaI - vetorDeNos.at(j).pos);						
					
				}
			}
		}
	}						
	
	return fad  ;// / qtdEspece; // ***************
}

//Retorna a Diversidade Atributo Funcional - FADe - da especie (valor do pixel)
float MedidasDiversidadeFuncional::retunrDiverdidadeAtributoFuncionalEspecie(vector <No> vetorDeNos, int fundo){

	float fadE = 0;
	int tamVetor = vetorDeNos.size();
	int qtdEspece = 0;
	
	
	//for(int i = 0; i < tamVetor; i++){
	//	if (vetorDeNos.at(i).ind > 0){
	//					
	//		float pixel = vetorDeNos.at(i).esp;
	//		qtdEspece++;

	//		for(int j = i+1; j < tamVetor; j++){
	//			float dij = 0;
	//			int n =  j-i;
	//			if (vetorDeNos.at(j).ind > 0){
	//				for(int k = 0; k <n; k++){
	//					//dij+= fabs((xik) - (xjk));
	//					if (vetorDeNos.at(j).ind > 0){
	//						dij+= fabs(abs(pixel - vetorDeNos.at(k).esp) - abs(vetorDeNos.at(j).esp - vetorDeNos.at(k).esp));
	//					}
	//				}
	//				dij/=(float)n;
	//				//float pixel_j = vetorDeNos.at(j).ind;
	//				fadE +=  dij ; //* (pixel_i / abundancia) * (pixel_j / abundancia);
	//			}				
	//		}

	//	}
	//}


	for(int i = 0; i < tamVetor; i++){
		if (vetorDeNos.at(i).ind > 0){
						
			float pixel = vetorDeNos.at(i).esp;
			if (vetorDeNos.at(i).esp<0) {
				pixel = (vetorDeNos.at(i).esp*(-1));
			}
			else {
				pixel = (vetorDeNos.at(i).esp);
			}

			qtdEspece++;
			for(int j = 0; j < tamVetor; j++){
				if (vetorDeNos.at(j).ind > 0){					
					//fadE += sqrt(pow(pixel - vetorDeNos.at(j).esp, 2));	
					float espNo;
					if (vetorDeNos.at(j).esp<0) {
						espNo = (vetorDeNos.at(j).esp*(-1));
					}
					else {
						espNo = (vetorDeNos.at(j).esp);
					}
					//fadE += fabs(pixel - vetorDeNos.at(j).esp);
					fadE += fabs(pixel - espNo);
				}
			}

		}
	}						
	
	return fadE; // / qtdEspece;
	
}

//Retiorna a Entropia Quadrática Funcional - Q
float MedidasDiversidadeFuncional::retunrEntropiaQuadratica(float fad, vector <No> vetorDeNos, int fundo){

	int tamVetor = vetorDeNos.size();
	float entropiaQuadratica = 0;
	int especies = 0;

	for (int x=0; x < tamVetor; x++){				
		if (vetorDeNos.at(x).ind > 0){
			especies++;						
		}
	}

	entropiaQuadratica = fad / pow(especies,2.0);

	return entropiaQuadratica;
}

//Retiorna a Entropia Quadrática Funcional - (Q) - FADe - Especie do Pixel
float MedidasDiversidadeFuncional::retunrEntropiaQuadraticaEspecie(float fadE, vector <No> vetorDeNos,  int fundo){

	int tamVetor = vetorDeNos.size();
	float entropiaQuadraticaEspecie = 0;
	int especies = 0;

	for (int x=0; x < tamVetor; x++){				
		if (vetorDeNos.at(x).ind > 0){
			especies++;						
		}
	}

	entropiaQuadraticaEspecie = fadE / pow(especies,2.0);

	return entropiaQuadraticaEspecie;
}
