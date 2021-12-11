#include "RemocaoFalsoPositivo.h"


RemocaoFalsoPositivo::RemocaoFalsoPositivo(void)
{
}


RemocaoFalsoPositivo::~RemocaoFalsoPositivo(void)
{
}

//Remove as ROIs em relacao a Forca gravitacional 
void RemocaoFalsoPositivo::removeForcaGravitacional(vector <string> vetorArquivos, int tipoDeMama) {
	
	//RemocaoFalsoPositivo remocaoFP;
	//double materiaMediaROIs = returnMateriaMedia(vetorArquivos);
	double forcaGravitacional = 0;
	double somaForcaGravitacionalTotal = 0, somaVariancia = 0, somaDistancia = 0, somaMediaSomaDistancia = 0;
	double maiorForcaGravitacional = 0;
	float distancia = 0;	
	
	// Constante fixa da Gravidade
	int Gravidade = 10;

	Funcoes funcoes;	

	vector <string> vetorCalsse1;
	vector <string> vetorCalsse2;
	vector <forcaG> vetorForcaGavitacional;
	vector <MateriaROI> vetorM1, vetorM2;
	
	for (int i=0; i < vetorArquivos.size(); i++){
		string arquivo = vetorArquivos.at(i);			
		Mat img = imread(arquivo,CV_LOAD_IMAGE_GRAYSCALE);
		double homogeneoROI1 = returnMateriaROI(img);
		double materiaROI1 = returnMateriaMediaROI(img);
		float varianciaROI1 = funcoes.calculaVarianciaDesvioPadraoImagem(img,1);

		MateriaROI M1;
		M1.homogeneo = homogeneoROI1;
		M1.materia = materiaROI1;
		M1.variancia = varianciaROI1;
		vetorM1.push_back(M1);
				
		int contROI = 0;
		int qtfROIs = vetorArquivos.size();
		int posicaoROI = i;
		int qtdForcaGravitacional = 0;
		double somaForcaGravitacionalROI = 0;
		
		while (contROI < qtfROIs){
			
			if (posicaoROI != contROI){
				string arquivoROI = vetorArquivos.at(contROI);
				Mat imgROI = imread(arquivoROI,CV_LOAD_IMAGE_GRAYSCALE);
				double homogeneoROI2 = returnMateriaROI(imgROI);
				double materiaROI2 = returnMateriaMediaROI(imgROI);
				float varianciaROI2 = funcoes.calculaVarianciaDesvioPadraoImagem(imgROI,1);
				MateriaROI M2;
				M2.homogeneo = homogeneoROI1;
				M2.materia = materiaROI1;
				M2.variancia = varianciaROI1;
				vetorM1.push_back(M2);

				double M1_M2 = returnProduto2Vetor(M1, M2);

				//distancia =  funcoes.returnDistance2ROIs(img,imgROI);
				//float distancia =  abs(variancia1 - variancia2);
				//double distancia = sqrt(pow(materiaROI,2) - pow(materiaMediaROIs,2));
				//double distancia = pow(abs(materiaROI - materiaMediaROIs),2);
				//cout << "distancia " << distancia << endl;	
				//forcaGravitacional = (Gravidade * materiaROI1 * materiaROI2) / pow(distancia,2);
				forcaGravitacional = (Gravidade * M1_M2) / pow(distancia,2);
				//cout << "forcaGravitacional " << forcaGravitacional << endl;
		
				somaForcaGravitacionalROI += forcaGravitacional;
				qtdForcaGravitacional++;
				somaDistancia += distancia;

			}
			
			contROI++;
		}

		double mediaForcaGravitacionalROI = somaForcaGravitacionalROI / qtdForcaGravitacional;
		//Media de todas as distancias
		double mediaSomaDistancia = somaDistancia / qtdForcaGravitacional;

		somaVariancia += varianciaROI1;
		
		somaMediaSomaDistancia += mediaSomaDistancia;

		somaForcaGravitacionalTotal += mediaForcaGravitacionalROI;
		forcaG FG;
		FG.arquivo = arquivo;
		FG.forcaGravitacional = mediaForcaGravitacionalROI;
		FG.materia = materiaROI1;
		FG.variancia = varianciaROI1;
		FG.distancia = somaMediaSomaDistancia;

		vetorForcaGavitacional.push_back(FG);

		if (mediaForcaGravitacionalROI > maiorForcaGravitacional ){
			maiorForcaGravitacional = mediaForcaGravitacionalROI;
			string imgROIReferencia= arquivo;
		}
		
	}

	int qtdObjetos = vetorArquivos.size();
	double mediaFocaGravitacional = somaForcaGravitacionalTotal / qtdObjetos;
	double mediaVariancia = somaVariancia / qtdObjetos;
	double mediaTotalDistacia = somaMediaSomaDistancia / qtdObjetos;
	double somaMateria1 = 0, somaMateria2 = 0, somaFG1 = 0, somaFG2 = 0;

	Mat imgBase = imread(vetorArquivos.at(0),CV_LOAD_IMAGE_GRAYSCALE);
	Mat imgGrupo1 = Mat::zeros(imgBase.rows,imgBase.cols,imgBase.type());
	Mat imgGrupo2 = Mat::zeros(imgBase.rows,imgBase.cols,imgBase.type());

	for (int i=0; i < vetorForcaGavitacional.size(); i++){
		//cout << vetorForcaGavitacional.at(i).forcaGravitacional << endl;
		//cout << vetorForcaGavitacional.at(i).variancia << endl;
		//cout << vetorForcaGavitacional.at(i).arquivo << endl;
		//if (vetorForcaGavitacional.at(i).forcaGravitacional > mediaFocaGravitacional) { //  &&  vetorForcaGavitacional.at(i).variancia < mediaVariancia){
		if (vetorForcaGavitacional.at(i).forcaGravitacional > mediaFocaGravitacional) { //&&  vetorForcaGavitacional.at(i).distancia > mediaTotalDistacia) { // &&  vetorForcaGavitacional.at(i).variancia > mediaVariancia){
			vetorCalsse1.push_back(vetorForcaGavitacional.at(i).arquivo);
			somaMateria1 += vetorForcaGavitacional.at(i).materia;
			somaFG1 += vetorForcaGavitacional.at(i).forcaGravitacional;
			Mat img1 = imread(vetorForcaGavitacional.at(i).arquivo,CV_LOAD_IMAGE_GRAYSCALE);
			imgGrupo1 += img1;
		}else{
			vetorCalsse2.push_back(vetorForcaGavitacional.at(i).arquivo);
			somaMateria2 += vetorForcaGavitacional.at(i).materia;
			somaFG2 += vetorForcaGavitacional.at(i).forcaGravitacional;
			Mat img2 = imread(vetorForcaGavitacional.at(i).arquivo,CV_LOAD_IMAGE_GRAYSCALE);
			imgGrupo2 += img2;
		}
	}

	int qtdClasse1 = vetorCalsse1.size();
	int qtdClasse2 = vetorCalsse2.size();
	int areagrupo1 = funcoes.returnQuantidadePixelDaImagem(imgGrupo1);
	int areagrupo2 = funcoes.returnQuantidadePixelDaImagem(imgGrupo2);
	double homgeneaROI1 = returnMateriaROI(imgGrupo1);
	double homgeneaROI2 = returnMateriaROI(imgGrupo2);
	float varianciROI1 = funcoes.calculaVarianciaDesvioPadraoImagem(imgGrupo1,1);
	float varianciROI2 = funcoes.calculaVarianciaDesvioPadraoImagem(imgGrupo2,1);

	cout << "Area 1: " << areagrupo1 << " Homo 1: " << homgeneaROI1 << " Variancia1: " << varianciROI1 << " %Var1: " << (varianciROI2 / varianciROI1 * 100) << endl;
	cout << "Area 2: " << areagrupo2 << " Home 2: " << homgeneaROI2 << " Variancia2: " << varianciROI2 << " %Var2: " << (varianciROI1 / varianciROI2 * 100) << endl;
	
	int classeCandidata = 0;
	int tamClasse = 0;
	string arquivoOut;

	int percentualDeVariancia = 60;
		
	if (areagrupo1 > areagrupo2 ){
		if (homgeneaROI1 > homgeneaROI2) {
			if (varianciROI1 > varianciROI2){											
				classeCandidata = 1;
				tamClasse = vetorCalsse1.size();						
			}else{
				float percentualMaior = (varianciROI1 / varianciROI2 * 100);
				if (percentualMaior > percentualDeVariancia ){
					classeCandidata = 1;
					tamClasse = vetorCalsse1.size();
				}else {
					classeCandidata = 2;
					tamClasse = vetorCalsse2.size();
				}					
			}			
		}else {
			if (varianciROI2 < varianciROI1){
			
				classeCandidata = 2;
				tamClasse = vetorCalsse2.size();
			}else{
				classeCandidata = 1;
				tamClasse = vetorCalsse1.size();
			}			
		}

	}else {
		if (homgeneaROI2 > homgeneaROI1) {
			if (varianciROI2 > varianciROI1){								
				classeCandidata = 2;
				tamClasse = vetorCalsse2.size();												
			}else {
				classeCandidata = 1;
				tamClasse = vetorCalsse1.size();
			}			
		}else {

			if (varianciROI1 < varianciROI2){			
				classeCandidata = 1;
				tamClasse = vetorCalsse1.size();
			}else{
				float percentualMaior = (varianciROI2 / varianciROI1 * 100);
				if (percentualMaior > percentualDeVariancia ){
					classeCandidata = 2;
					tamClasse = vetorCalsse2.size();
				}else {
					classeCandidata = 1;
					tamClasse = vetorCalsse1.size();
				}					
			}			
		}
	}


	for (int i=0; i < tamClasse; i++){
		
		if (classeCandidata == 1){
			arquivoOut = vetorCalsse1.at(i);
		}else{
			arquivoOut = vetorCalsse2.at(i);
		}
							
		Mat imgOut = imread(arquivoOut, CV_LOAD_IMAGE_GRAYSCALE);
		size_t tamanhoNomeArquivo = arquivoOut.find_last_of("/\\");

		PathSystem pathSystem;
		string pathOut;
		
		if(tipoDeMama == 1){
			pathOut = pathSystem._pathReducaoFP + "Nao_Densas/" + arquivoOut.substr(tamanhoNomeArquivo+1);			
		}else{
			pathOut = pathSystem._pathReducaoFP + "Densas/" + arquivoOut.substr(tamanhoNomeArquivo+1);
		}

		imwrite(pathOut,imgOut);
		
		//string pathOut = "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/4-PSO-Segmentacao-RFP/" + arquivoOut.substr(tamanhoNomeArquivo+1);
		//string pathOut = "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/9-Teste-Remocao-FP/" + arquivoOut.substr(tamanhoNomeArquivo+1);
		//imwrite(pathOut,imgOut);
	}
	
	PathSystem pathGrupos;
	string pathGrupo = pathGrupos._pathReducaoFP;
	imwrite(pathGrupo + "imgGrupo1.png", imgGrupo1);
	imwrite(pathGrupo + "imgGrupo2.png", imgGrupo2);
	int x = 0;
}

//Retorna a materia média de todas as ROIs
double RemocaoFalsoPositivo::returnMateriaMedia(vector <string> vetorArquivos) {


	double materiaROI = 0;
	int contaROI = 0;
	for (int i=0; i < vetorArquivos.size(); i++){		
		string arquivo = vetorArquivos.at(i);	
		Mat img = imread(arquivo,CV_LOAD_IMAGE_GRAYSCALE);
		materiaROI += returnMateriaROI(img);
		contaROI++;
	}

	//cout << "materiaROI / contaROI " << materiaROI / contaROI << endl;
	//return materiaROI / contaROI;
	return materiaROI;

}

//Retorna a Materia da ROI
double RemocaoFalsoPositivo::returnMateriaROI(Mat img){

	int lin = img.rows;
	int col = img.cols;
	double materiaROI = 0;
	Funcoes funcoes;

	for (int i=0; i < lin; i++){
		for(int j=0; j < col; j++){
			
			if (img.at<uchar>(i,j) > 0){
				
				int pixelCentral = img.at<uchar>(i,j);
				//Verifica se o pixel tem vizinho igual
				for (int y=i; y < i+3; y++){
					for(int x=j; x < j+3; x++){
						if (img.at<uchar>(y-1, x-1) == pixelCentral){

							materiaROI++;
						}
					}
				}				
			}
		}
	}
	
	//cout << "materiaROI " << materiaROI << endl;
	return materiaROI;

}

//Retorna a Materia Média da ROI
double RemocaoFalsoPositivo::returnMateriaMediaROI(Mat img){

	int lin = img.rows;
	int col = img.cols;
	double materiaROI = 0;
	int contaPixel=0;
	Funcoes funcoes;

	for (int i=0; i < lin; i++){
		for(int j=0; j < col; j++){
			
			if (img.at<uchar>(i,j) > 0){

				materiaROI += img.at<uchar>(i,j);
				contaPixel++;
			}
		}
	}
	
	double materiaMediaROI = materiaROI / contaPixel;
	//cout << "materiaROI " << materiaROI << endl;
	return materiaMediaROI;

}

//Retorna a ROI com a menor variancia
string RemocaoFalsoPositivo::returnROIMenorVariacia(vector <string> pathROIS){

	Mat imgVariancia;
	Funcoes funcoes;

	string referenciaROI;

	int tamVetor = pathROIS.size();
		
	float varianciaAnterior = 999999999999999999999.0;
	for (int i=0; i < tamVetor; i++){
		
		Mat imgTemp = imread(pathROIS.at(i),CV_LOAD_IMAGE_GRAYSCALE);
		float varianciaAtual = funcoes.calculaVarianciaDesvioPadraoImagem(imgTemp, 1);
		if (varianciaAtual < varianciaAnterior ) {
			imgVariancia = imgTemp;
			varianciaAnterior = varianciaAtual;
			referenciaROI = pathROIS.at(i);
		}
	
	}

	return referenciaROI;
}



//Retorna o produto vetorial de dois vetores
double RemocaoFalsoPositivo::returnProduto2Vetor(MateriaROI M1, MateriaROI M2) {

	
	double produtoHome = M1.homogeneo * M2.homogeneo;
	double produtoMateria = M1.materia * M2.materia;
	float produtoVariancia = M1.variancia * M2.variancia;

	double somaTudo = produtoHome + produtoMateria; // +produtoVariancia;

	return somaTudo;

}

//
//if (areagrupo1 > areagrupo2 && homgeneaROI1 > homgeneaROI2 && varianciROI1 > varianciROI2){
//			
//			classeCandidata = 1;
//			tamClasse = vetorCalsse1.size();
//			
//		}else {
//			classeCandidata = 2;
//			tamClasse = vetorCalsse2.size();
//		}	
//
//	for (int i=0; i < tamClasse; i++){
//		
//		if (classeCandidata == 1){
//			arquivoOut = vetorCalsse1.at(i);
//		}else{
//			arquivoOut = vetorCalsse2.at(i);
//		}
//							
//		Mat imgOut = imread(arquivoOut, CV_LOAD_IMAGE_GRAYSCALE);
//		size_t tamanhoNomeArquivo = arquivoOut.find_last_of("/\\");
//
//		string pathOut = "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/4-PSO-Segmentacao-RFP/" + arquivoOut.substr(tamanhoNomeArquivo+1);
//		//string pathOut = "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/9-Teste-Remocao-FP/" + arquivoOut.substr(tamanhoNomeArquivo+1);
//		//imwrite(pathOut,imgOut);
//	}