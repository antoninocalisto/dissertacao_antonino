#pragma once
#ifndef MANIPULAARQUIVO_H
#define MANIPULAARQUIVO_H

#include "includes.h"


class ManipulaArquivo{

public:

	string _pathArquivo;
	string _nomeArquivo;

	ManipulaArquivo(void);

	ManipulaArquivo(string path, string arquivo);

	~ManipulaArquivo(void);

	void gravaNoArquivo(string conteudo);

	void limpaArquivo(string pathArquivo);


};


#endif