#include "PathSystem.h"


PathSystem::PathSystem(void){

	// 0_Base_Wener					-> Base de dados do Wener e overlay
	// 0_Imagem_Original			-> Base de imagens Selecionadas com overlay
	// 1_Imagem_Redimensionada		-> Imgens Reduzidas
	// 2_Imagem_Base				-> Imgens Base para processamento, sem Bordas e musculo peitoral
	// 3_PSO-Clusters				-> Clusters gerado pelo PSO
	// 4_Crescimento_Regiao			-> Cresmineto de Região
	// 5_Segmentacao				-> Segmentação: Graph Clustering, e redução de falsos positivos
	// 6_Reducao_FP					-> Redução de falsos positovos ****************
	// 7_Separar_MassaNao_Massa		-> Separação das massas e nao massa
	// 8_Extracao_Caracteristicas	-> path do Arquivo de características

	// Variavel da unidade de disco
	const string unidade = "c:";

	// Path das imagens originais e overlays (Wener)
	_pathBaseWener	= unidade + "/op/DDSM/0_Base_Wener/0_Original/";	
	// Path das imagens selecionadas
	_pathBaseSelecionada = unidade + "/op/DDSM/0_Imagem_Original/";
	// Path das imagens Redimensionadas
	_pathRedimensionada = unidade + "/op/DDSM/1_Imagem_Redimensionada/";
	//path origem das imagens nao denssas
	_pathOrigem		= unidade + "/op/DDSM/2_Imagem_Base/";	
	//path destino das imagens dos clusters	
	//_pathDestino	= unidade + "/op/DDSM/3_PSO-Clusters/";	
	_pathClusters	= unidade + "/op/DDSM/3_PSO-Clusters/";	
	
	//path origem das imagens Overlay
	_pathOverlay	= unidade + "/op/DDSM/0_Base_Wener/0_Gabarito/";	//Base Wener
	//_pathOverlay	= unidade + "/op/DDSM/0_Overlay_Imagem/";			//Base OP

	//path dos arquivos Overlay das imagens
	_pathOverlayArquivo	= unidade + "/op/DDSM/0_Overlay_Arquivo/";		
	
	//path origem das imagens nao denssas - Crescimento de Regiao
	_pathCrescimentoRegiao		= unidade + "/op/DDSM/4_Crescimento_Regiao/";	
	//_pathCrescimentoRegiao = unidade + "/op/DDSM/4_Crescimento_Regiao/";	

	//*******************************************	
	//path origem das imagens nao denssas - Segmentação
	_pathSegmentacao= unidade + "/op/DDSM/5_Segmentacao/";  // Pasta correta para segmentação ***********		
	
	//_pathSegmentacao= unidade + "/op/DDSM/5_Segmentacao/X-Imagens_B/";  // Pasta para segmentação das Imagens B
	//_pathSegmentacao= unidade + "/op/DDSM/5_Segmentacao/X_Img-Erro/"; // Pasta das imagens segmentados com eero no overlay do Wener

	//*******************************************
	
	//path origem das imagens nao denssas - Redução falso Positivo
	_pathReducaoFP	= unidade + "/op/DDSM/6_Reducao_FP/";	
	
	//path do arquivo de características
	_pathFeaters	= unidade + "/Extracao/";	

	//path origem das imagens nao denssas Massa
	_pathMassaNaoMassa = unidade + "/op/DDSM/7_Separar_MassaNao_Massa/";	// ********* Falta ver as pastas densas
	
	//path origem das imagens nao denssas Não_Massa
	//_pathNaoMassa	= unidade + "/op/DDSM/7_Separar_MassaNao_Massa/";	
	
	//path origem das imagens nao denssas
	_pathResize		= unidade + "/op/DDSM/1_Imagem_Redimensionada/Nao_densas/";	
	//path origem das imagens nao denssas
	//_pathResize		= unidade + "/op/mestrado_ufma/imagens/mama/ddsm/0_Original_Reduzida/";	
	
	// path das imagens LBP
	_pathLBP		= unidade + "/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/"; 
	// path da ROIs circulares
	_pathROICirclular = unidade + "/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/8-imagens-Circular/"; 
	// path das caracteristicas
	//_pathFeature	= unidade + "/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/5-PSO-Features/"; 
	//pegar path + nomeArquivo para referenciar a imagem a ser processada
	_imagem;	
	//pegar nome do arquivo no diretorio
	_nomeArquivoOverlay;	
	_nomeArquivoImagem;
	_pathChoice;

	//_pathOrigem		= "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/";	//path origem das imagens nao denssas
	//_pathDestino	= "f:/op/mestrado_ufma/imagens/imagemPSO/1-PSO-teste/";	//path destino das imagens	
	//_pathOverlay	= "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/0-PSO-Overlay_Imagem/";	//path origem das imagens nao denssas
	//_pathImagem		= "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/2-PSO-Crescimento_Regiao/";	//path origem das imagens nao denssas
	//_pathMassa		= "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/3-PSO-Separacao-Massa-Nao_massa/Massa/";	//path origem das imagens nao denssas
	//_pathNaoMassa	= "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/3-PSO-Separacao-Massa-Nao_massa/Nao_Massa/";	//path origem das imagens nao denssas
	//_pathResize		= "f:/op/DDSM/1_Imagem_Redimensionada/Nao_densas/";	//path origem das imagens nao denssas
	////_pathResize		= "f:/op/mestrado_ufma/imagens/mama/ddsm/0_Original_Reduzida/";	//path origem das imagens nao denssas
	//_pathLBP		= "F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/"; // path das imagens LBP
	//_pathROICirclular = "F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/8-imagens-Circular/"; // path da ROIs circulares
	//_pathFeature	= "F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/5-PSO-Features/"; // path das caracteristicas
	//_imagem;	//pegar path + nomeArquivo para referenciar a imagem a ser processada
	//_nomeArquivoOverlay;	//pegar nome do arquivo no diretorio
	//_nomeArquivoImagem;
	//_pathChoice;
}


string PathSystem::PathChoice(String path){

	_pathChoice	= path;
	return _pathChoice;
}

PathSystem::~PathSystem(void)
{
}
