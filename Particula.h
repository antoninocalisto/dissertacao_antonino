#pragma once
#ifndef PARTICULA_H
#define PARTICULA_H
//#include <iostream>
//#include <vector>
//#include <cstdlib>
//#include <time.h>
//#include <algorithm>
//#include <math.h>
#include "includes.h"

using namespace std;

class Particula
{
public:
	//Particula( int min, int max, int n, float fitness );
	Particula( int min, int max, int n, float fitness, vector<int> vetCluster, int contaParticulas);
	int _posicao[2];
	float _aptidao;
	float _velocidade;
	float _pbest;
	float _centroide;
	//float _gbest[2];
	vector <int> _vetThreshold;

	Particula(void);
	~Particula(void);

	//Metodo para verificar se tem valor repetido no vetor da particula
	bool verificaValorRepetido(vector <int> vetorDeLimiares, int valor);

};

#endif