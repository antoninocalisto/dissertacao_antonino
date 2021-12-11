#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include <iostream>
#include "itkGDCMImageIO.h"
#include "itkMinimumMaximumImageCalculator.h"
#include <itkMatrix.h>
#include <Math.h>
#include "stdio.h"
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
//#include "itkBinaryThinningImageFilter3D.h"
#include <itkShapeLabelMapFilter.h>
#include "ImageIO.h"
#include "Util.h"
#include "quantizacao.h"
#include <itkNiftiImageIOFactory.h>
#include "Cladogram.h"
#include "IndexFeature.h"
#include "itkImageDuplicator.h"
#include "itkIdentityTransform.h"
#include "itkResampleImageFilter.h"
#include <iostream>
#include<opencv\cv.h>
#include<opencv\highgui.h>
#include<math.h>



using namespace  itk;  
using namespace  std;
using namespace cv;

typedef itk::GDCMImageIO ImageIO;
typedef itk::Image<short,3> ImageType;
typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;
typedef itk::Image<short, 3> ImageType3D;
typedef itk::Image<short, 2> ImageType2D;
typedef  itk::ImageFileWriter<ImageType2D> WriterType2D;
typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileReader<ImageType2D> ReaderType2D;
typedef itk::ImageFileWriter<ImageType> WriterType;
typedef itk::ImageFileWriter<ImageType2D> WriterType2;
typedef unsigned char PixelTypeChar;   // must be signed for CT since Hounsfield units can be < 0 
typedef Image< PixelTypeChar, 3 > ImageTypeChar;

#define PI 3.14159265359
#define R3 1.732050808 //Raiz quadrada de 3; 


int getMinimum(ImageType::Pointer image){
	typedef MinimumMaximumImageCalculator <ImageType> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New ();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMinimum();
}
int getMaximum(ImageType::Pointer image){
	typedef MinimumMaximumImageCalculator <ImageType> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New ();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMaximum();
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
	else if (valor ==3) {
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
	int min = getMinimum(image);
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
				imagemNova->SetPixel(position,min);
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
					
					if ( ((int)imagem2D->GetPixel(position2D)) > 0 ) {
						if (value != min) {
							imagemNova->SetPixel(position,value);
						}
					}
			}
		} // For do Y
	}
	return imagemNova;
}

void geraSVMfile(int rotulo, vector<float> features, string savePath)
{
    ofstream featureFile(savePath.c_str(),ofstream::app);
    featureFile.precision(10);
    featureFile<<rotulo<<" ";
    for (int l = 0; l < features.size(); l++)
        featureFile<<l+1<<":"<<features.at(l)<<" ";
    featureFile<<"\n";
    featureFile.close();
}

void lerTxtFile(){
	    int prodind = 0;
        int dis = 0;
        int valorMPD = 0;
        float SPD=0.0;
        float S1 = 0.0;
        float S2 = 0.0;
        float PD=0.0;
        int soma1 = 0;
        int somatorio = 0;
        float MNND = 0.0;
		long double q=0.0;
		float psv = 0.0;
        float psr = 0.0;
        float Ai = 0.0;
        float somatorioDistanciasEsp = 0.0;
        float somatorioDistanciasEsp2 = 0.0;
        float EQE = 0.0;
        float AVTD = 0.0;
        float TTD = 0.0;
		long double IDT = 0.0;
        float DMP = 0.0;
		float EQI = 0.0;													
		float AVDT = 0.0;
		// Indice Normalizado
		float somatorioDistanciasEsp3 = 0.0;
		Util tools;
		itk::GDCMImageIOFactory::RegisterOneFactory();
		itk::NiftiImageIOFactory::RegisterOneFactory();
		int pasta = 1;
		while(pasta<2){
			int finalPasta = 0;
			if (pasta==1) {
				//finalPasta = 2;
				finalPasta = 102;
			}else if (pasta == 2) {
				finalPasta = 92;
			}
			else if (pasta == 3) {
				finalPasta = 31;
			}
			else if (pasta == 4) {
				finalPasta = 57;
			}
			else if (pasta == 5) {
				finalPasta = 39;
			}
		cout << "\n######################################  Pasta:  " <<pasta << "\n\n" <<endl;
			for (int m = 1; m < finalPasta; m++){
				cout << "\nPasta: " << pasta << endl;
				cout << "\nPACIENTE: " << m << endl;
				// Lendo volume do paciente
				stringstream path;
				path << "C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/BaseParaFilogeneticos/_  " <<pasta<< "/_  (" << m << ")/1/2/";
				// pegando marcações
				stringstream pathMascaras;
				//pathMascaras << "C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/_  LC/_  (" << m << ")/1/3/MascaraVolume.nii";
				pathMascaras << "C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/BaseParaFilogeneticos/_  " <<pasta<< "/_  (" << m << ")/1/3/MarkingsVolume.nii";
				string pathE;
				string pathMascara;
				vector<double> Inds;
				//for (int i = 0; i < 3;i++) {
				cout << "\nObtendo volume" << endl;
				pathE = path.str();
				cout << "\nObtendo mascara" << endl;
				pathMascara = pathMascaras.str();
				cout << "\nLendo diretorio da imagem original" << endl;
				ITKImage3D::Pointer imagemOriginal = readDICOMSeries(pathE);
				//writeVolume(imagemOriginal, "C:/Users/labpai/ImagemOriginalTesteRTStruct.nrrd");
				cout << "\nLendo diretorio da mascara" << endl;
				ITKImage3D::Pointer imagemMks = readVolume(pathMascara);
				//writeVolume(imagemMks, "C:/Users/labpai/ImagemMascaraTesteTesteRTStruct.nrrd");
				//printaValorIntensidadeVolxels(imagemMks);
				cout << "\nExtraindo Lesao" << endl;
				ITKImage3D::Pointer lesao = getExtraiLesao(imagemOriginal, imagemMks);
				typedef itk::ImageDuplicator< ImageType3D > DuplicatorType;
				DuplicatorType::Pointer duplicator = DuplicatorType::New();
				duplicator->SetInputImage(lesao);
				duplicator->Update();
				ImageType3D::Pointer imagem = duplicator->GetOutput();
				
				//writeDICOMSeries(lesao, "C:/Users/labpai/DicomSeriesExtratores/");
				//stringstream path;
				/*cout << "\nObtendo a lesao extraida da pasta labpai" << endl;
				path << "C:/Users/labpai/DicomSeriesExtratores/lesao.dcm";*/
				//C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics/_ (" << i << ")/1/2/
				/*ImageIO::Pointer imageIOO = ImageIO::New();
				ReaderType::Pointer reader = ReaderType::New();
				reader->SetFileName(path.str());
				reader->SetImageIO(imageIOO);
				reader->Update();
				*/		
				imagem = cutImage(imagem, 0);
				//tools.saveImage(imagem, "imagemCortada.nrrd");
				
				ITKImage3D::Pointer imgQuantizada;
				//Vetor de Quantização
				vector <int> ncolors;
				ncolors.push_back(12);
				ncolors.push_back(11);
				ncolors.push_back(10);
				ncolors.push_back(9);
				//ncolors.push_back(8);
				//ImageType::Pointer image = ImageType::New();
				int qtdColor = 0;
				int usaMascaras = 1;
				int numMascara = 1;
				int qtdMascaras = 10;
				int qtdNColors = ncolors.size();
				int entraMascara = 0;
				vector<float> feat;
				//QUANTIZA A IMAGEM AQUI
				while (qtdColor < qtdNColors) {
					numMascara = 1;
					if (usaMascaras==0) { //Caso nao queira usar as mascaras
						qtdMascaras = 1; // Só para saber se vai usar as mascaras
					}
					entraMascara = 0;
					while (numMascara<qtdMascaras) {
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
							//size[0] = coluna  |   size[1] = linha
							//image = reader->GetOutput();
							ImageType::SizeType size;
							size = imgQuantizada->GetLargestPossibleRegion().GetSize();
							ImageType2D::Pointer ImagemMascara2D;

							if (entraMascara==1) {
								ImagemMascara2D = redimencionarMascara(numMascara, size[1],size[0]);
								//tools.saveImage2D(ImagemMascara2D, "imagemMascara9.dcm");
								imgQuantizada = extraiAnelCirculo(imgQuantizada, ImagemMascara2D); // Passando a imagem e a sua mascara para extrair o anel
								//tools.saveImage(imgQuantizada, "imagemAnelCirculo9.nrrd");
							}
							//cout<<size[0]<<" - "<<size[1]<<" - "<<size[2]<<" - ";

							int min, max;
							min = getMinimum(imgQuantizada);
							max = getMaximum(imgQuantizada);

							//cout<<"Maximo "<<max;
							//cout<<"\nMinimo "<<min;

							int sizeHistograma = max - min + 1;
							int *hist = new int[sizeHistograma];
							int *distance = new int[sizeHistograma];


							for (int i = 0; i < sizeHistograma; i++) {
								hist[i] = 0;
							}

							int background = min;

							if (min < 0)
								min *= -1;
							int diagnal = 0, foraDiagonal = 0;

							for (int z = 0; z < size[2]; z++) {
								for (int y = 0; y < size[1]; y++) {
									for (int x = 0; x < size[0]; x++) {
										ImageType::IndexType position;
										position[0] = x;
										position[1] = y;
										position[2] = z;
										int value = (int)imgQuantizada->GetPixel(position);

										//todos os voxels começam de 0
										if (value != background) {
											if ((value + min) < 0 || value > sizeHistograma)
												cout << "DEU MERDA: " << value << endl;
											hist[value + min]++;

										}
									}
								} // FOr do Y
							}
							for (int z = 0; z < size[2]; z++) {
								for (int y = 0; y < size[1]; y++) {
									for (int x = 0; x < size[0]; x++) {
										ImageType::IndexType position;
										position[0] = x;
										position[1] = y;
										position[2] = z;
										int value = (int)imgQuantizada->GetPixel(position);
										if (value != background) {
											if (x == y)
												diagnal += (value + min);
											else
												foraDiagonal += (value + min);
										}
									}
								} // For do Y
							}
							//cod laercio
							int especie = 0;

							for (int i = 0; i < sizeHistograma; i++) {
								if (hist[i] > 0)
									especie++; // "especie" é a quantidade de especies existentes
							}

							psv = (double)((especie * diagnal) - foraDiagonal) / (especie * (especie - 1));
							psr = (double)especie * psv;

							int i, j;
							for (i = 0, j = i + 1; i < sizeHistograma, j < sizeHistograma; i++, j++) {

								if (i == 0) {
									// j é o root no de oseas
									// e distance[i] é o sumDistancias
									distance[i] = (j - i + 1);
								}
								else {
									distance[i] = (j - i + 2);
								}
								prodind = dis *hist[i] * hist[j];
								somatorio += (hist[i] * hist[j]);

								for (int q = i; q < j; q++) {
									// "distance[i]" é a distancia minima (DMP)
									MNND = MNND + (distance[i] * hist[q]);
									// Indice de diversidade pura DMP
									DMP = DMP + distance[i];
									// Indice de distincao taxonomico (Que) no de oseas é Q

								}

							}

							//Pesos normalizados
							float w = 999999;

							//sizeHistograma é o numero de especies do nódulo
							float t = ((sizeHistograma - 1) / 2.0);
							int total = sizeHistograma*t;

							for (int i = 0; i < sizeHistograma; i++) {
								for (int j = i + 1; j < sizeHistograma; j++)
								{

									//cacula a distancia
									if (i == 0)
									{
										dis = (j - i) + 1;
									}
									else
									{
										dis = (j - i) + 2;
									}
									// dis é a distancia entre as especies i e j
									prodind = dis *hist[i] * hist[j];
									somatorio += (hist[i] * hist[j]);

									for (int x = i; x <= j; x++)
									{
										soma1 += hist[x];

									}

									// "Ai" é o numero de individuos de uma 
									//determinada especie existente numa determinada área
									Ai = soma1 / ((j - i) + 1);
									S1 += (float)((float)dis*Ai);
									S2 += Ai;
									somatorioDistanciasEsp = somatorioDistanciasEsp + dis; // Fazendo a soma das distancias das especies deste VOI
									if (somatorioDistanciasEsp < w) {
										w = somatorioDistanciasEsp;
									}

								}
								valorMPD = valorMPD + prodind;

							}



							for (int i = 0; i < sizeHistograma; i++) {
								for (int j = i + 1; j < sizeHistograma; j++)
								{

									//cacula a distancia
									if (i == 0)
									{
										dis = (j - i) + 1;
									}
									else
									{
										dis = (j - i) + 2;
									}

									somatorioDistanciasEsp2 = somatorioDistanciasEsp2 + somatorioDistanciasEsp / (dis*((j - i) + 1)); // Fazendo a soma das distancias das especies deste VOI

								}
							}



							for (int i = 0; i < sizeHistograma; i++) {
								for (int j = i + 1; j < sizeHistograma; j++)
								{

									//cacula a distancia
									if (i == 0)
									{
										dis = (j - i) + 1;
									}
									else
									{
										dis = (j - i) + 2;
									}

									somatorioDistanciasEsp3 = somatorioDistanciasEsp3 + somatorioDistanciasEsp / ((dis*((j - i) + 1)) / w); // Fazendo a soma das distancias das especies deste VOI

								}
							}

							
							PD = (float)((float)S1 / (float)S2); // S2 é o numero total de individuos

							//Entropia quadratica intensiva (IQI)
							EQI = (float)somatorioDistanciasEsp / (float)(especie*especie);    // Oseas chama de "J" entropia quadratica intensiva

							q = (long double)((long double)valorMPD / (long double)somatorio);

							//Soma basica dos Pesos
							float Que = somatorioDistanciasEsp2;

							//Distincao taxonomica media (AvTD)
							AVDT = (float)somatorioDistanciasEsp / (float)((especie*(especie - 1)) / 2);

							//Distincao taxonomica Total (TTD)
							TTD = (float)somatorioDistanciasEsp / (float)(especie - 1);

							// Indice de diversidade taxonomico (IDT) 
							IDT = (long double)valorMPD / total;

							//Entropia quadratica Extensiva (EQE)
							EQE = (float)somatorioDistanciasEsp;

							SPD = (float)((float)total *(float)q); // total é [(Numero de especies (numero de especies-1))/2]

							//cout<<"psv"<<psv<<endl;
							//cout << "Indice de distincao taxonomico (q): " << q << endl;
							//cout << "Indice de diversidade taxonomico (IDT): " << IDT << endl;
							////cout<<"psr"<<psr<<endl;
							//cout << "\nMNND :" << MNND << endl; //Diversidade filogenetica media
							//cout << "\nPD :" << PD << endl; // PDnode (OseasTese)
							//cout << "\nSPD :" << SPD << endl; // PDRoot (OseasTese)
							//cout << "\nEntropia quadratica intensiva :" << EQI << endl; // Entropia quadratica intensiva, oseas chama de (j
							//cout << "\nEntropia quadratica Extensiva :" << EQE << endl; // Entropia quadratica Extensiva,oseas chama de (F)
							//cout << "\nDistincao taxonomica media :" << AVDT << endl; // Entropia quadratica Extensiva,oseas chama de (F)
							//cout << "\nDistincao taxonomica Total :" << TTD << endl; //
							//cout << "\nIndice de diversidade pura :" << DMP << endl; //
							//cout << "\nSoma Basica dos Pesos :" << Que << endl; //
							//cout << "\nSoma Basica dos Pesos Normalizados :" << somatorioDistanciasEsp3 << endl; //

							//INDICES LAERCIO
							//feat.push_back(psr);
							//feat.push_back(psv);

							// RIQUEZA DE ESPECIES
							//////feat.push_back(q); // Indice de distincao taxonomico (q)
							//////feat.push_back(IDT); // Indice de diversidade taxonomico (IDT)
							//
							// PARES DE ESPECIES
							/*feat.push_back(EQI);
							feat.push_back(EQE);
							feat.push_back(AVDT);
							feat.push_back(TTD);
							feat.push_back(DMP);*/

							// TOPOLOGICOS
							//feat.push_back(Que); // Soma Basica dos pesos
							//feat.push_back(somatorioDistanciasEsp3); // Soma Basica dos pesos Normalizados

							// CAMINHO MINIMO
							feat.push_back(MNND);
							feat.push_back(PD);
							feat.push_back(SPD);
							if (entraMascara==1) {
								numMascara++;
							}
							entraMascara = 1;
						}
						qtdColor++;
					}
				//	cout<<"imagem " <<voi<< " foi processada"<<endl;
					/*
						  ################ TIPOS DE CARCINOMAS NA BASE ##############################################
							  1 - large cell - célula grande
							  2 - squamous cell carcinoma - Carcinoma de células escamosas
							  3 - adenocarcinoma - adenocarcinoma
							  4 - nos -
							  5 - NA -
						   ########################################################################################*/
					geraSVMfile(pasta,feat,"__caminhoMinimoOseasQuantizados_AneisCirculos.libsvm");
					feat.clear();
					/*path.str("");
					path.str("");*/

		}
			pasta++;
		}
}
int main(){
	lerTxtFile();
	system("pause");
}
