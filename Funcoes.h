// Programa: Funcoes.cpp - Funções úteis para o projeto
// Autor: Otilio Pauo
// Data: 21/05/2015
#pragma once
#ifndef FUNCOES_H
#define FUNCOES_H

#include "includes.h"
#include "PathSystem.h"
#include "ManipulaArquivo.h"
//#include "header.h"
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
#include "Util.h"
#include <itkNiftiImageIOFactory.h>
//#include "Cladogram.h"

typedef itk::GDCMImageIO ImageIO;
typedef itk::Image<short, 3> ImageType;
typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;
typedef itk::Image<short, 3> ImageType3D;
typedef itk::Image<short, 2> ImageType2D;
typedef itk::ImageFileReader<ImageType> ReaderType;
typedef itk::ImageFileWriter<ImageType> WriterType;
typedef itk::ImageFileWriter<ImageType2D> WriterType2;
typedef unsigned char PixelTypeChar;   // must be signed for CT since Hounsfield units can be < 0 
typedef Image< PixelTypeChar, 3 > ImageTypeChar;

using namespace std;
using namespace cv;


class Funcoes
{
public:

	Funcoes(void);
	~Funcoes(void);
	int getMinimumFuncoes(ImageType3D::Pointer image);

	void saveIntensidadesImagem(vector< vector<double> > vetorDeCaracteristicas);

	ImageType3D::Pointer createImageFuncoes(ImageType3D::Pointer image);
	int* getHistogram(ImageType3D::Pointer image);
	void saveImage(ImageType3D::Pointer image, string path);
	//void saveIntensidadesImagem(vector< vector<double> > vetorDeCaracteristicas);
	ImageType3D::Pointer createImageRect(ImageType3D::Pointer image, int width, int height, int contaFatias);
	//Mat returnCircleROI(Mat &image); // Retorna um circulo contendo a ROI
	//Point returnPointCenter(Mat &image); // Retorna o ponto central da ROI
	ImageType3D::Pointer cutImage(ImageType3D::Pointer image, bool saveImage); // Retorna a ROI da Imagem
	//Point returnPointXY(Mat &image, int tipo=1); // Retorna os Ponto X e Y, minumos ou maximos -> Tipo: 1 = minimos, 2 = maximos
	//float returnDistancePointXY(Point p1, Point p2); // Retorna a distancia entre dois pontos
	ImageType::Pointer geraImageThreshold(ImageType::Pointer image, float threshold, int tipo); // Gera imagem a partir de um limiar - Tipo: 0 = menor que o limiar, 1 = Maior ou igual ao limiar
	int returnQuantidadePixelMesmoValor(ImageType3D::Pointer image, int valor); // Retorna a quantidade de pixel do mesmo valor
	float Otsu2(ImageType3D::Pointer img); //Otsu 2
	//void kmeansClustering(Mat img); //Kmeans
	vector <int> Funcoes::returnHistogramaImage(ImageType3D::Pointer imageh); // Retorna o histograma da imagem	

	vector <int> getVectorHistograma(ImageType3D::Pointer original, int &negativo);

	int getMinimumF(ImageType::Pointer image);
		
	int getMaximumF(ImageType3D::Pointer image);

	// Retorna um vetor com os arquivos do diretório
	vector <string> returnVetorArquivos(string diretorio);

	//Salva arquivo de caracteristica
	void saveFeatures(vector< vector<double> > vetorDeCaracteristicas, int rotulo);
	
	//Retorna a imagem quantizada
	//Mat quantization(Mat imgOrig, int bit);
	ImageType3D::Pointer quantization(ImageType3D::Pointer imgOrig, int bit);

	// Remove o funda da imagem
	//Mat removeFundo(Mat image, float valorPixel);
	ImageType3D::Pointer Funcoes::removeFundo(ImageType3D::Pointer image, float valorPixel);

	// Gera a borda da Imagem
	Mat geraBordaImagem(Mat image);

	//Retorna se o pixel tem vizinho com valor zero
	bool verificarVizinhoZero(Mat image, int lin, int col);

	//Realce histograma local - CLAHE
	Mat realceHistogramaLocal(Mat img);

	//Unir dois arquivos
	void unity2File(string arquivo1, string arquivo2);

	// Retorna a quantidade de pixel da imagem
	int returnQuantidadePixelDaImagem(Mat image);

	//Retorna a distancia entre duas ROIs
	//float Funcoes::returnDistance2ROIs(Mat imgROI1, Mat imgROI2);

	//--- Calcula a variancia ou Desvio padrão da imagem -> 1 = Variancia; 2 = Desvio Padrao
	float calculaVarianciaDesvioPadraoImagem(Mat &imgOrigem, int tipo);

	// GeraROI com os pixels originais da imagem
	Mat geraROIPixelOriginal(Mat imgOriginalDDSM, Mat imgOvelay);

	

};

#endif