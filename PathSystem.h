// Programa: Path define os caminhos dos arquivos do systema
// Autor: Otilio Paulo
// Data: 09/12/2014

#pragma once
#ifndef PATHSYSTEM_H
#define PATHSYSTEM_H
#include "includes.h"

class PathSystem
{
public:

	string _pathBaseWener;
	string _pathBaseSelecionada;
	string _pathRedimensionada;
	string _pathOrigem;  
	//string _pathDestino; 
	string _pathClusters;
	string _pathOverlay;
	string _pathOverlayArquivo;
	//string _pathImagem;	
	string _pathCrescimentoRegiao;
	string _pathSegmentacao;
	string _pathReducaoFP;
	string _pathFeaters;
	string _pathMassaNaoMassa;
	//string _pathMassa;	
	//string _pathNaoMassa;
	string _imagem;	
	string _nomeArquivoOverlay;	
	string _nomeArquivoImagem;
	string _pathChoice;
	string _pathResize;
	string _pathLBP;
	string _pathROICirclular;	

	PathSystem(void);

	string PathChoice(String path);

	~PathSystem(void);
};

#endif