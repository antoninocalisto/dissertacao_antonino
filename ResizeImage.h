// Programa: Redimensiona imagem
// Autor: Otilio Paulo
// Data: 01/11/2014

//#pragma once
#ifndef RESIZEIMAGE_H 
#define RESIZEIMAGE_H

#include "includes.h"
#include "PathSystem.h"

using namespace std;
using namespace cv;

class ResizeImage {

public:

	Mat		_imagemResize;
	int		_altura;
	string	_path;
	string	_imageName;

	ResizeImage(string pathOrigem, string pathDestino, int altura);

	ResizeImage(string path, Mat &img, int altura);

	/*void reSize()
	resize(_imagem,imgResize,t);*/

	
	~ResizeImage(void);

};

#endif