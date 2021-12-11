// Programa: Medidas de Diversidade funcional
// Autor: Otilio Paulo
// Data: 08/06/2015

#pragma once
#ifndef MEDIDASDIVERSIDADEFUNCIONAL_H
#define MEDIDASDIVERSIDADEFUNCIONAL_H

#include "includes.h"
#include "Funcoes.h"
#include "Arvores.h"
//#include "LBPImage.h"
#include "MascaraImagem.h"
//#include "DescritorFormas.h"
//#include "HaralickFeature.h"
#include <itkGDCMSeriesFileNames.h>




class MedidasDiversidadeFuncional
{

private:
	
	string _diretorioMassa;
	string _diretorioNaoMassa;
	int _tipoImage; //Tipo de Imagem: 1=Normal, 2=LBP
	int _qtdMascarasInterna;
	int _qtdMascarasExterna;
	bool _usaMascaras; // Variavel para usar ou não as mascaras
	int _rotulo;
	

public:

	int _conta = 1;
	int _valorMinimoDaimagem = 4000;
	int _PassouDireita = 0;
	MedidasDiversidadeFuncional(void);

	

	ImageType3D::Pointer createImageSintetica(ImageType3D::SpacingType spacing);

	int getMinimum1(ImageType3D::Pointer image);

	int getMaximum1(ImageType3D::Pointer image);

	double wClustering(ImageType3D::Pointer image, int numCluster);

	ImageType3D::Pointer createImageMD2(int x, int y, int z, ImageType3D::SpacingType spacing, int value);
	
	void MedidasDiversidadeFuncional::flush();

	ImageType3D::Pointer readDICOMSeriesMD(string directoryName);
	
	ImageType3D::Pointer readVolumeMD(string fileName);

	ImageType3D::Pointer createImageMD(ImageType3D::Pointer image);

	ImageType3D::Pointer getExtraiLesao(ImageType3D::Pointer volume, ImageType3D::Pointer volumeMks);

	ImageType3D::Pointer equalizacaoHistograma(ImageType3D::Pointer image);

	ImageType3D::Pointer readVolume1(string fileName);

	void saveImage(ImageType3D::Pointer image, string path);

	void geraSVMfile(int rotulo, vector<float> features, string savePath);
	
	// Calcula os indeces de Diversidade Funcional
	MedidasDiversidadeFuncional(string diretorioMassa, string diretorioNaoMassa, int tipoImage, int qtdMascaras, int qtdMascarasExterna, bool usaMascara, int pasta);
	
	//Clase de execucao
	void executa(void);

	//Retiorna a Diversidade Funcional
	float retunrDiverdidadeFuncional(vector <No> vetorDeNos, int fundo);

	//Retiorna a Diversidade Funcional - FD - Abundante - posição na arvre FD= sum (sum( d_ij.Pi.Pj))
	float retunrDiverdidadeFuncionalAbundante(vector <No> vetorDeNos, int fundo);

	//Retiorna a Diversidade Funcional - FDaE - Abundante - Valor Pixel FDaE = sum (sum( d_ij.Pi.Pj))
	float MedidasDiversidadeFuncional::retunrDiverdidadeFuncionalAbundanteEspece(vector <No> vetorDeNos, int fundo);

	//Retiorna a Diversidade Atributo Funcional - FAD - dos tracos da arvore (dendrograma)
	float retunrDiverdidadeAtributoFuncional(vector <No> vetorDeNos, int fundo);

	//Retiorna a Diversidade Atributo Funcional - FAD - das especie (valor do pixel)
	float retunrDiverdidadeAtributoFuncionalEspecie(vector <No> vetorDeNos, int fundo);

	//Retiorna a Entropia Quadrática Funcional - Q
	float retunrEntropiaQuadratica(float fad, vector <No> vetorDeNos, int fundo);

	//Retiorna a Entropia Quadrática Funcional - (Q) - FADe - Especie do Pixel
	float retunrEntropiaQuadraticaEspecie(float fadE, vector <No> vetorDeNos, int fundo);

	//Retorna a Diversidade Funcional - FDaP - Abundante em relação ao valor do pixel 	
	float MedidasDiversidadeFuncional::retunrDiverdidadeFuncionalAbundantePixel(vector <No> vetorDeNos, int fundo);

	//Retorna a Diversidade Funcional - FDaPE - Abundante em relação ao valor do pixel com a distancia das especias
	float MedidasDiversidadeFuncional::retunrDiverdidadeFuncionalAbundantePixelEspecie(vector <No> vetorDeNos, int fundo);

	//ImageType3D::Pointer createImageSintetica(ImageType3D::SpacingType spacing);


	~MedidasDiversidadeFuncional(void);
};

#endif