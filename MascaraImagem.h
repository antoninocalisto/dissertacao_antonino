// Programa: Gera as mascaras da imagem
// Autor...: Otilio Paulo 
// data....: 25/11/2015
//

#pragma once
#ifndef MASCARAIMAGEM_H
#define MASCARAIMAGEM_H

#include "includes.h"
#include "Funcoes.h"


class MascaraImagem {

public:
	MascaraImagem(void);
	~MascaraImagem(void);

	//Retorna a mascara da borda de uma imagem
	Mat retornaMascaraBorda(Mat img);

	ImageType3D::Pointer retornaMascaraImageLuana(ImageType3D::Pointer img, float scale);

	int getMinimumMa(ImageType::Pointer image);
	
	int getMaximumMa(ImageType3D::Pointer image);

	ImageType3D::Pointer createImageMa(ImageType3D::Pointer image);

	//Retorna a mascara de uma imagem de acordo com a escala
	ImageType3D::Pointer retornaMascaraImage(ImageType3D::Pointer img, float scale);

	//Retorna a mascara Externa de duas imagens 
	Mat retornaMascaraImageExterna(Mat img1, Mat img2);
};

#endif