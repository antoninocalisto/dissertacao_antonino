#pragma once
#include "includes.h"
#include "Funcoes.h"

#ifndef REMOCAOFALSOPOSITIVO_H
#define REMOCAOFALSOPOSITIVO_H

class RemocaoFalsoPositivo{

public:

	RemocaoFalsoPositivo(void);

	~RemocaoFalsoPositivo(void);

	struct forcaG {
		string arquivo;
		double forcaGravitacional;
		double materia;
		float variancia;
		double distancia;
	};

	struct MateriaROI {
		double homogeneo;
		double materia;
		float variancia;
	};

	//Calcula a Força Gravitacional das ROIS
	void removeForcaGravitacional(vector <string> vetorArquivos, int tipoDeMama);

	//Retorna a materia média das ROIS
	double returnMateriaMedia(vector <string> vetorArquivos);

	//Retorna a Materia da ROI
	double returnMateriaROI(Mat img);

	//Retorna a ROI com a menor variancia
	string returnROIMenorVariacia(vector <string> pathROIS);

	//Retorna a Materia Média da ROI
	double returnMateriaMediaROI(Mat img);

	//Retorna o produto vetorial de dois vetores
	double returnProduto2Vetor(MateriaROI M1, MateriaROI M2);



};

#endif