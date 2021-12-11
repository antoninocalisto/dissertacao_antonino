#include "Particula.h"


Particula::Particula( int min, int max, int n, float fitness, vector<int>  vetCluster, int contaParticulas)
{
	
	int numParticulas = n*2;
	//srand( time(0) );
	float int1 = ((rand() % 20)+ 1) / 20.0;
	//float int1 = ceil(((rand() % n)+ 1) / (float) n);
	int int2 = (rand() % (int)n) + 2;
	_posicao[0] = 0;
	_posicao[1] = 0;
	_velocidade = int2;
	_aptidao = fitness;
	_pbest = (int1) *(max-min)+min;
	_centroide = 0;

	//_vetThreshold = new vector <int>;
	
	float rando=0;
	int aux = 0;
	if (contaParticulas==0){
		for (int i =0; i < vetCluster.size(); i++){
			_vetThreshold.push_back(vetCluster.at(i));
		}
		sort(_vetThreshold.begin(),_vetThreshold.end());
	}else{

		_vetThreshold.push_back((int)ceil(((int1)*(max-min)+min)));	
		sort(_vetThreshold.begin(),_vetThreshold.end());
		
		for (int i =1; i < vetCluster.size(); i++){
			//srand( time(0) );
			//int1 = ((rand() % 255)+ 1) / 255.0;
			int1 = ((rand() % numParticulas)+ 1) / (float) numParticulas;
			rando = ceil(((int1) *(max-min)+min));

			while (verificaValorRepetido(_vetThreshold,rando)){
				//srand( time(0) );
				//int1 = ((rand() % 255)+ 1) / 255.0;
				int1 = ((rand() % numParticulas)+ 1) / (float) numParticulas;
				rando = ceil(((int1) *(max-min)+min));
			}
			_vetThreshold.push_back((int)rando);	
			sort(_vetThreshold.begin(),_vetThreshold.end());
		}
			
	}
	int x=0;
		
}
Particula::Particula(void)
{

}


Particula::~Particula(void)
{
}


bool Particula::verificaValorRepetido(vector <int> vetorDeLimiares, int valor){

	int valorVetor = vetorDeLimiares.at(0);
	int valorNovo = valor;

	for (int i =0; i< vetorDeLimiares.size(); i++){
		if (valorNovo == vetorDeLimiares.at(i)){
			return true;
		}
	}
	return false;
}