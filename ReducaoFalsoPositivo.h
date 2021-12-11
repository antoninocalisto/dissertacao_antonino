// Programa: GrafhClustering é um grafo agrupador de imagens visinhas 4x4 com uma ligação simples
// Autor: Otilio Paulo
// Data: 17/08/2015

#pragma once

#ifndef REDUCAOFALSOPOSITIVO_H
#define REDUCAOFALSOPOSITIVO_H

#include "includes.h"
#include "Funcoes.h"
//#include "DescritorFormas.h"

class ReducaoFalsoPositivo
{
public:

	Mat image;

	ReducaoFalsoPositivo(void);

	ReducaoFalsoPositivo(Mat &img);

	~ReducaoFalsoPositivo(void);

	vector <float> returnHeightWidth();
};

#endif