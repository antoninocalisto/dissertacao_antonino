// Programa	: Arvores.cpp - 
// Objetivo	: Criar esturturas de avores 
// Author	: Otilio Paulo
// Data		: 21/05/2015


#pragma once
#ifndef ARVORES_H
#define ARVORES_H

#include "includes.h"
#include "Funcoes.h"
//#include "Otsu.h"

struct No {
	int pos;
	int pai;
	int esp;
	int esq;
	int dir;
	int ind;
};


class Arvores
{
public:
	
	int _pos;
	No  _nos;
	int _qtdElementos;

	Arvores(void);

	void criaArvore(ImageType::Pointer imgOriginal, ImageType::Pointer image, float limite, int &position, vector <No> &vectorNos, vector <int> &vetRaiz);
	
	ImageType3D::Pointer readVolumeMDArvores(string fileName);

	double wClusteringA(ImageType3D::Pointer image, int numCluster);

	ImageType3D::Pointer createImageA(ImageType3D::Pointer image);

	int getMaximumA(ImageType3D::Pointer image);

	int getMinimumA(ImageType::Pointer image);

	void saveImageA(ImageType3D::Pointer image, string path);

	int getNumberSpecies(ImageType3D::Pointer image, int limite);

	int returnQuantidadeEspecies(vector <int> histograma, int pixelFundo); // Retorna a quantidade de especies do histograma
	
	vector <int> returnPixelsHistograma(vector <int> histograma, int fundo); // Encontra um vetor de pixel > 0 apartir do hitograma	
	
	// Retorna a quatidade de individos (pixels) apartir do hitograma
	int returnQuantidadePixelsHistograma(vector <int> histograma, int individo);
	
	// Verifica se o no Raiz foi usado
	bool Arvores::verificaRaizUsado(vector <int> vetorDeNos, int no);
	
	//Retorna a quantida de eletemtos de um pixels da imagem
	int Arvores::returnQuantidadePixelsImage(ImageType3D::Pointer img, int pixel);

	// Verifica se o no foi usado
	bool Arvores::verificaNoUsado(vector <No> vetorDeNos, int no);


	~Arvores(void);
};

#endif