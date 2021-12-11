
// Programa: PSO - Otimiza��o por Emxame de Particulas
// Autor: Otilio Paulo
// Data: 30/05/2014
//
//--------------------- Paths do sistema -------------------------------------------------
// Todas as pastas possuem duas sub pastas (densas e Nao_densas) 
// 
// 0_Base_Wener					-> Base de dados do Wener e overlay
// 1_Imagem_Redimensionada		-> Imgens Reduzidas
// 2_Imagem_Base				-> Imgens Base para processamento, sem Bordas e musculo peitoral
// 3_PSO-Clusters				-> Clusters gerado pelo PSO
// 4_Crescimento_Regiao			-> Cresmineto de Regi�o
// 5_Segmentacao				-> Segmenta��o: Graph Clustering, e redu��o de falsos positivos
// 6_Reducao_FP					-> Redu��o de falsos positovos ****************
// 7_Separar_MassaNao_Massa		-> Separa��o das massas e nao massa
// 8_Extracao_Caracteristicas	-> path do Arquivo de caracter�sticas
//------------------------------------------------------------------------------------------

//#include <iostream>
//#include "dirent.h"
//#include <fstream>
//#include <string>
//#include <cmath>
//#include <opencv2\core\core.hpp>
//#include <opencv2\highgui\highgui.hpp>
//#include <time.h>


#include "Arvores.h"
#include "MedidasDiversidadeFuncional.h"


using namespace std;
using namespace cv;

struct Pixel {
	int x;
	int y;
	int valor;
	int lado;
};

struct Cluster {
	float threshold;
	float centroid1;
	float centroid2;
	int qtdElementos1;
	int qtdElementos2;
	//vector <int> pixels;
	vector <Pixel> pixels;
	float somaPixels;
};

void menu();

//int segmentacaoPSO();
float atualizaFitness(Mat &img, float Xi, int pos_Cluster);
void calculaCentroide(Mat &img, float Threshold);
void criaImagemOverlay(Mat &img, string file);
void calculaThreshold(Mat &img, float Threshold);
void calculaCentroide2(vector<Pixel> pixels, Mat &img, bool repetidor, float desvioPadraoMedio);
Mat geraImagemMelhorParticula(vector <int> vetorPbest, Mat &img);
float atualizaFitnessParticula(Mat &img, vector <int> centroide);
//void pintaImagem(Mat &img, vector <int> centroide);
void pegaArquivoDiretorio(); //Pega arquivo do diretorio
string geraNomeDoArquivo(string pathArquivo);	//Gera o nome do arquivo de saida da imagem
float calculaVarianciaMediaDesvioPadraoImagem(Mat &imgOrigem, int kernelX, int kernelY);	//--- Calcula a variancia media do desvio padrao das regioes da imagem kernel x por y
bool verificaVizinhoZero(Mat &img, float x, float y, int kernelX, int kernelY);	// Verifica a visinhan�a do pixel
float circularidade(Mat &img);	//Calcula a circularidade da imagem
void encontrarCirculosImagem(Mat &imagem); //Encontra circulos na imagem
										   //void regionGrowing(Mat &img, Mat &img2, float x, float y);	// Crecismento de Regi�o (region growing)
Mat realceHistogramaLocal(Mat img);
// Retorna tipo de mama (1-> Nao Densa, 2-> Densa)
int returnTipoDeMama();




//Vetor de clusters
vector<Cluster*> clusteres;
Cluster c;

//Vetor de clusrer gerado pelo Otsu para a particula inicial
vector<Cluster* > clusterOtsu;
Cluster clusterParticula;

// Variaveis globais 
int thresholdeRepetido = 0;				//Contador de threshold repetido
int limiteDeRepeticaoThreshold = 10;	//Limite para se repetir a busca do threshold Otsu
float fitnessOtsu = 0;					//Valor do fitness gerado pelo otsu inicial para as particulas
int contadorImgTeste = 1;				//Contador para gerar imagens de teste do clusters
const int QTD_IMAGENS = 99999999999999; //Quantidade de imagens a serem processadas
float desvioPadraoMedioImagem = 0;		// Variavel para armazenar o desvio padrao medio da imagem 
Mat removeFundoImagem(Mat &img, float limiar); // Remove funda da imagem


string overlay;
string nomeArquivo;												//pegar nome do arquivo no diretorio
																//-- Pasta notbook
																//*string pathOrigem  = "C:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo/Nao_densas/";	//path origem das imagens
																//string pathDestino = "C:/op/mestrado_ufma/imagens/imagemPSO/1-PSO-teste/";	//path destino das imagens de teste

																//-- PAsta computador
PathSystem pathSystem;
//path origem das imagens nao denssas
string pathOrigem; // = pathSystem._pathOrigem;
				   //path destino das imagens
string pathDestino; // = pathSystem._pathClusters;

					//string pathOrigem  = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/";	
					//string pathOrigem  = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay_100_img/Nao_densas/";		//path das 100 imgs para teste
					//string pathOrigem  = "f:/op/mestrado_ufma/imagens/mama/ddsm/0_Original_mamografias_dasa_Reduzida/";		//path das 22 imgs para teste - Ari
					//string pathOrigem  = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo/Densas/";	//path origem das imagens denssas
					//string pathDestino = "f:/op/mestrado_ufma/imagens/imagemPSO/1-PSO-teste/";	//path destino das imagens

string imagem;


int main(int argv, char argc[]) {

	menu();

	system("pause");

	return 0;
	/*Mat img = imread("f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/imgs2/7.png",CV_LOAD_IMAGE_GRAYSCALE);
	imshow("ImageTest",img);

	Mat res = quantization(img.clone(),3);

	imshow("Result",res);

	waitKey(0);

	return 0;*/
}


void menu() {

	int choise = 0;
	int n = 0;


	do {
		cout << "PSO K-Clusters v.1.0.008-D:GC.10 \n";
		cout << "================================ \n";
		cout << "Escolha a opcao: \n";
		//cout<<"0----> Redimensionar Imagem DDSM \n";		
		//cout<<"1----> Clusteriza��o PSO \n";
		cout << "2----> Crescimento de Regi�o \n";
		cout << "3----> Gera imagem do arquivo overlay \n";
		cout << "4----> Separar Massa/Nao Massa - Verifica se a ROI toca o overlay \n";
		cout << "5----> Grafh Clustering unir as ROIs Vizinhas \n";
		cout << "6----> Extrair caracteristicas Haralick \n";
		cout << "7----> Gera Imagem LBP \n";
		cout << "8----> Extrair caracteristicas - Arvares FD \n";
		cout << "9----> Reducao das ROIs 12x12 e 300x300 \n";
		cout << "88---> Redimensionar Imagem - Dasa - Ari \n";
		cout << "89---> Uniao de dois arquivos TXT \n";
		cout << "90---> Remocao Falso Positivo - Forca Gravitacional \n";
		cout << "91---> Contar Massa Perdida \n";
		cout << "98---> Escolher Imagens a partir de um Diretorio \n\n";
		cout << "99---> Sair \n\n";

		cout << "Opcao:\n";
		cin >> choise;

		switch (choise) {
		case(88): //Redimensionar as imagens Dasa - Ari
		{

			system("pause");
			int x = 0;

			//ResizeImage redimensionaImagem("f:/op/mestrado_ufma/imagens/mama/ddsm/0_Original_mamografias_dasa/",1024); // Imagens Ari

		}
		break;

		case(0): //Redimensionar as imagens
		{
			//Pega os arquivos no diretorio padr�o-------------------
			int tipoMama = 0;

			cout << "Escolha o tipo de Mama: " << endl;
			cout << "1 - Mama Nao Densa " << endl;
			cout << "2 - Mama Densa " << endl;

			cin >> tipoMama;

			PathSystem pathSystem;

			if (tipoMama == 1) {
				pathOrigem = pathSystem._pathBaseSelecionada + "Nao_Densas/";
				pathDestino = pathSystem._pathRedimensionada + "Nao_Densas/";
			}
			else {
				pathOrigem = pathSystem._pathBaseSelecionada + "Densas/";
				pathDestino = pathSystem._pathRedimensionada + "Densas/";
			}
			//system("pause");
			//int x=0;
			//ResizeImage redimensionaImagem("f:/op/mestrado_ufma/imagens/mama/ddsm/0_Original_benign/",1024);
			//ResizeImage redimensionaImagem("f:/op/mestrado_ufma/imagens/mama/ddsm/50-Maligno/",1024);
			//ResizeImage redimensionaImagem("f:/op/mestrado_ufma/imagens/mama/ddsm/0_Original_mamografias_dasa/",1024); // Imagens Ari
			//ResizeImage redimensionaImagem("f:/op/mestrado_ufma/imagens/mama/ddsm/50-Benigno/",1024);
			//ResizeImage redimensionaImagem("f:/op/DDSM/0_Imagem_Original/Nao_densas/",1024); // Base do Wener
			//ResizeImage redimensionaImagem(pathOrigem, pathDestino, 1024); // Base do Wener

		}
		break;

		case(1): {//Segmaenta��o das imagens com PSO

			//segmentacaoPSO();
		}
				 break;
		case(2): {// Crescimento de Regiao

				  // Retorna tipo de mama (1-> Nao Densa, 2-> Densa)
			int tipoMama = 0;

			tipoMama = returnTipoDeMama();

			//RegionGrowing(tipoMama); 
			//RegionGrowing regionGrowing(tipoMama);
		}

				 break;

		case(3): // Gera o overlay da imagem
			//OverlayImage();
			//menu();
			//n=1;
			break;

		case(4): {// verifica se a imagem toca no overlay - Separa Mass de N�o Massa

			int tipoPath; // 1=path Cluster 2=path CrescimentoRegiao

			cout << "Escolha a op��o do Path \n";
			cout << "1----> Crescimento Regiao \n";
			cout << "2----> Segmenta��o \n";
			cout << "3----> Segmenta��o - RFP \n";
			cout << "Op��o: \n";

			cin >> tipoPath;
			cout << tipoPath;

			int tipoMama = returnTipoDeMama();

			//SelectMassaImageOverlay((int)tipoPath, (int)tipoMama);
		}

				 break;

		case(5): //Utiliza GrafhClusterin para unir as ROIs vizinhas
		{
			//GrathClustering gf;
			//new GrathClustering();
			//gf.GrathClusteringImage();
			//ClusteringImage();
		}
		break;

		case(6):
		{//Extrai caracteristicas Haralick

			int tipoImage; // 1=path Cluster 2=path CrescimentoRegiao

			cout << "Escolha a op��o da Imagem \n";
			cout << "1----> Normal \n";
			cout << "2----> LBP \n";
			cout << "Op��o: \n";

			cin >> tipoImage;
			cout << tipoImage;

			ofstream myfile;
			myfile.open("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/5-PSO-Features/HaralickFeature-libSVM.txt");//, fstream::app|fstream::out);				
			myfile.close();
			//ExtractFeatures extractFeatures;
			//extractFeatures.imageSelect("f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/3-PSO-Separacao-Massa-Nao_massa/Massa/", "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/3-PSO-Separacao-Massa-Nao_massa/Nao_Massa/", tipoImage);
			//extractFeatures.imageSelect("f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/imgs2/", "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/3-PSO-Separacao-Massa-Nao_massa/Nao_Massa/");
		}

		break;

		case(7): // Gera imagem LBP
		{
			//string nameImage = "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/3-PSO-Separacao-Massa-Nao_massa/Massa/A_1003_1.LEFT_CC.LJPEG.1.image.8.png.Part5.png.Part5.png";
			//string nameImage = "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/3-PSO-Separacao-Massa-Nao_massa/Massa/A_1004_1.RIGHT_MLO.LJPEG.1.image.8.png.Part10.png.Part13.png";
			//string nameImage = "F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/face.png";
			string nameImage = "F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/A_1004_1.RIGHT_MLO.LJPEG.1.image.8.png";
			Mat imagem = imread(nameImage, CV_LOAD_IMAGE_GRAYSCALE);
			Mat imagemMedia;
			medianBlur(imagem, imagemMedia, 3);
			PathSystem path;
			string pathSaveImage = path._pathLBP + "imgLBP.png";
			//LBPImage lbpImage;
			//Mat imgOut = lbpImage.LBPImageTranfer(imagemMedia, pathSaveImage, 1);
			//imwrite("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/5-PSO-Features/imgLBP.png",imgOut);


			int x = 0;

		}
		break;

		case(8): {// Cria arvores

			string extensaoTipoImagem;
			int tipoImage; // 1=Imagem Normal 2=Imagem LBP

			cout << "Escolha a op��o da Imagem \n";
			cout << "1----> Normal \n";
			cout << "2----> LBP \n";
			cout << "Op��o: \n";

			cin >> tipoImage;
			cout << tipoImage << endl;

			int tipoMassa = returnTipoDeMama();
			PathSystem pathSystem;
			string pathMassa;
			string pathNaoMassa;

			if (tipoMassa == 1) {
				cout << "\n" << "Entrou no tipo de massa " << endl;
				pathMassa = "C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/_  ";
				pathNaoMassa = "C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/_  ";
			}
			else {
				pathMassa = "C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/_  ";
				pathNaoMassa = "C:/Users/labpai/Downloads/NovaBaseRadiomics/NSCLC-Radiomics - SeparadasSemMarcacaoRenomeada/_  ";
			}

			//********** Principal
			cout << "\n" << "Entrou no MedidasDiversidadeFuncional " << endl;


			MedidasDiversidadeFuncional df(pathMassa, pathNaoMassa, tipoImage, 1, 0, 1, 0); // ultimo parametro é a pasta que será modificada no medidasDiversidadeFuncional

			//MedidasDiversidadeFuncional df("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/3-PSO-Separacao-Massa-Nao_massa/Massa/", "F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/3-PSO-Separacao-Massa-Nao_massa/Nao_Massa/", tipoImage,4,4);

			//MedidasDiversidadeFuncional df("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/Massa/", "F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/Nao_Massa/",1);
			//MedidasDiversidadeFuncional df("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/Massa/", "F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/Teste/");
			//MedidasDiversidadeFuncional df("F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/Massa/", "F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/7-imagens-LBP/imagem_erro/");

			break;
		}

		case(9): {//Redu��o de Falsos Positivos "em breve..."			


			break;
		}

		case(89): { //Unir dois arquivos TXT			

			Funcoes funcoes;

			//string arquivo1 = "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/5-PSO-Features/FD-1-Normal.txt";
			//string arquivo2 = "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/5-PSO-Features/FD-2-Realce.txt";
			string arquivo1 = "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/5-PSO-Features/FD-N-R.txt";
			string arquivo2 = "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/5-PSO-Features/FD-3-LBP.txt";

			funcoes.unity2File(arquivo1, arquivo2);

			break;
		}

		case(90): { //Remo��o de Falsos Positivos		

			int tipoMassa = returnTipoDeMama();

			PathSystem pathSystem;
			string arquivo1;

			if (tipoMassa == 1) {
				//arquivo1 = pathSystem._pathSegmentacao + "Nao_Densas/";
				arquivo1 = pathSystem._pathReducaoFP + "X-Nao_Densas/";
			}
			else {
				//arquivo1 = pathSystem._pathSegmentacao + "X-Densas/";
				arquivo1 = pathSystem._pathReducaoFP + "X-Densas/";
			}


			//string arquivo1 = "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/9-Teste-Remocao-FP/";
			//string arquivo1 = "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/3-PSO-Separacao-Massa-Nao_massa/0-Massa_Nao_Massa/";
			Funcoes funcoes;
			vector <string> pathArquivos = funcoes.returnVetorArquivos(arquivo1);

			for (int i = 0; i< pathArquivos.size(); i++) {
				vector <string> vetorPathTemp = pathArquivos;
				string nomePath = pathArquivos.at(i);
				size_t nomeArquivo = nomePath.find_last_of("/\\");
				string nomeCluster1 = nomePath.substr(nomeArquivo + 1, 17);

				vector <string> pathROI;
				//for (int i=0; i < pathArquivos.size(); i++){
				while (!vetorPathTemp.empty()) {

					string nomeCluster2 = vetorPathTemp.at(0).substr(nomeArquivo + 1, 17);
					if (nomeCluster1 == nomeCluster2) {
						pathROI.push_back(vetorPathTemp.at(0));
						pathArquivos.erase(pathArquivos.begin());
					}
					vetorPathTemp.erase(vetorPathTemp.begin());
				}
				/*RemocaoFalsoPositivo remocaoFP;
				remocaoFP.removeForcaGravitacional(pathROI, tipoMassa);*/
			}
			break;
		}

		case(91): { //Conta as massa perdidas

			int tipoMassa = returnTipoDeMama();

			PathSystem pathSystem;
			string arquivo1, arquivo2;
			string pathDoArquivo, nomeDoArquivo;


			if (tipoMassa == 1) {
				//arquivo1 = pathSystem._pathOverlay + "Nao_Densas/";
				arquivo1 = pathSystem._pathOrigem + "Nao_Densas/";
				arquivo2 = pathSystem._pathMassaNaoMassa + "Nao_Densas/Massa/";
				pathDoArquivo = pathSystem._pathMassaNaoMassa + "Nao_Densas/";
				nomeDoArquivo = "massaPerdida-NaoDensa.txt";
			}
			else {
				//arquivo1 = pathSystem._pathOverlay + "Densas/";					
				arquivo1 = pathSystem._pathOrigem + "Densas/";
				arquivo2 = pathSystem._pathMassaNaoMassa + "Densas/Massa/";
				pathDoArquivo = pathSystem._pathMassaNaoMassa + "Densas/";
				nomeDoArquivo = "massaPerdida-Densa.txt";
			}

			Funcoes funcoes;
			vector <string> pathArquivos1 = funcoes.returnVetorArquivos(arquivo1);
			vector <string> pathArquivos2 = funcoes.returnVetorArquivos(arquivo2);

			ManipulaArquivo manipulaARquivos(pathDoArquivo, nomeDoArquivo);
			manipulaARquivos.limpaArquivo(pathDoArquivo + nomeDoArquivo);

			for (int i = 0; i< pathArquivos1.size(); i++) {
				string nomePath = pathArquivos1.at(i);
				size_t nomeArquivo = nomePath.find_last_of("/\\");
				string nomeCluster1 = nomePath.substr(nomeArquivo + 1, 17);
				cout << nomeCluster1 << endl;

				int contaArquivos = 0;
				bool massPerdina = true;
				while (contaArquivos < pathArquivos2.size()) {
					string nomePath2 = pathArquivos2.at(contaArquivos);
					size_t nomeArquivo2 = nomePath2.find_last_of("/\\");
					string nomeCluster2 = nomePath2.substr(nomeArquivo2 + 1, 17);

					if (nomeCluster1 == nomeCluster2) {

						massPerdina = false;
						break;
					}
					contaArquivos++;
				}

				if (massPerdina) {
					cout << "Perdeu : " << nomeCluster1 << endl;
					manipulaARquivos.gravaNoArquivo(nomeCluster1);
				}

			}
			break;
		}

		case(98): //Selecionar as Imagens do Diretorio
		{

			string extensaoTipoImagem;
			cout << "Escolha a extensao da Imagem \n";
			cin >> extensaoTipoImagem;

			int tipoImage; // 1=path Cluster 2=path CrescimentoRegiao

			cout << "Escolha a op��o da Imagem \n";
			cout << "1----> Nao Densa \n";
			cout << "2----> Densa \n";
			cout << "Op��o: \n";

			cin >> tipoImage;
			cout << tipoImage;


			//string pathDestino = "f:/op/DDSM/0_Base_Wener/0_Original/Nao_densas/"; //Imagens Nao densas
			string pathDestino = "f:/op/DDSM/0_Base_Wener/0_Original/Densas/"; //Imagens Densas


																			   // PAths para copiar arquivos no Windows--------------------
																			   // N�o Densas
																			   //string pathCopiaOrigem  = "f:\\op\\DDSM\\0_Base_Wener\\0_Original\\Nao_densas\\";
																			   //string pathCopiaDestino = "f:\\op\\DDSM\\0_Imagem_Original\\Nao_densas\\"; // Path das imagens selecionadas
																			   // Densas
			string pathCopiaOrigem = "f:\\op\\DDSM\\0_Base_Wener\\0_Original\\Densas\\";
			string pathCopiaDestino = "f:\\op\\DDSM\\0_Imagem_Original\\Densas\\"; // Path das imagens selecionadas
																				   //----------------------------------------------

			Funcoes funcoes;
			vector <string> vetorDeArquivos = funcoes.returnVetorArquivos(pathDestino);

			for (int i = 0; i < vetorDeArquivos.size(); i++) {
				string nomeArquivo = vetorDeArquivos.at(i);

				string ext = nomeArquivo.substr(nomeArquivo.size() - 3, 3);
				if (extensaoTipoImagem == ext) {

					string encontrouNome = nomeArquivo.substr(0, nomeArquivo.size() - 19) + "OVERLAY";
					for (int j = 0; j < vetorDeArquivos.size(); j++) {

						if (encontrouNome == vetorDeArquivos.at(j)) {

							//cout << "Encontrou " ;
							stringstream executa;
							string arquivoOrigem;
							if (tipoImage == 1) {
								arquivoOrigem = pathCopiaOrigem + nomeArquivo.substr(46, 50);
							}
							else {
								arquivoOrigem = pathCopiaOrigem + nomeArquivo.substr(42, 50);
							}
							string arquivoSaida = pathCopiaDestino; // + nomeArquivo.substr(46,50);
							executa << "copy " << arquivoOrigem << " " << arquivoSaida; //+ arquivoSaida;									
							cout << executa.str() << endl;
							system(executa.str().c_str());
							executa.str("");
							break;
						}
					}
				}
			}
			//ResizeImage redimensionaImagem("f:/op/mestrado_ufma/imagens/mama/ddsm/0_Original_mamografias_dasa/",1024); // Imagens Ari
			system("pause");

		}
		break;

		case(99):
		{
			n = 99;
			break;
		}

		default:
		{
			cout << "Por favor selecione a opcao correta: \n";
		}
		break;
		}

	} while (n != 99);

}


//// Segmenta��o usando PSO
//int segmentacaoPSO() {
//
//	//Pega os arquivos no diretorio padr�o-------------------
//	int tipoMama = 0;
//	string pasta;
//
//	cout << "Escolha o tipo de Mama: " << endl;
//	cout << "1 - Mama Nao Densa " << endl;
//	cout << "2 - Mama Densa " << endl;
//
//	cin >> tipoMama;
//
//	if (tipoMama == 1) {
//		pasta = "Nao_Densas/";
//	}
//	else {
//		pasta = "Densas/";
//	}
//
//
//	int processar = 0;
//
//	DIR *dir;
//	struct dirent *lsdir;
//
//	//"c:\\Windows\\"
//	//dir = opendir("C:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo/Nao_densas/");	//path do notebook	
//	//dir = opendir("f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay_100_img/Nao_densas/");		//path das 100 imgs para teste
//	//dir = opendir("f:/op/mestrado_ufma/imagens/mama/ddsm/0_Original_mamografias_dasa_Reduzida/");		//path das 22 imagens do Ari
//	//dir = opendir("f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo/Densas/");		//path do computador
//	//dir = opendir("f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/");		//path do computador
//	//dir = opendir("f:/op/1-PSO/3_Sem_fundo/Nao_densas/");
//
//	PathSystem pathSystem;
//	// Path das imagens de Origem
//	pathOrigem = pathSystem._pathOrigem + pasta;
//	// Path das imagens destino
//	pathDestino = pathSystem._pathClusters + pasta;
//
//	dir = opendir(pathOrigem.c_str());		//path do computador
//
//											/* print all the files and directories within directory */
//	while ((lsdir = readdir(dir)) != NULL && processar < QTD_IMAGENS) {
//
//		clusteres.clear();		// Zera clusteres
//		clusterOtsu.clear();	// Zera clsuteres de Otsu
//		desvioPadraoMedioImagem = 0;	// Desvio padrao medio da imagem 
//
//		if (strcmp(lsdir->d_name, ".") != 0 && strcmp(lsdir->d_name, "..") != 0) {
//			processar++;
//			printf("%s\n", lsdir->d_name);
//			nomeArquivo = lsdir->d_name;
//			imagem = pathOrigem + nomeArquivo; // strcat(path,nomeArquivo);
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo/Nao_densas/A_1006_1.LEFT_CC.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo/Nao_densas/A_1011_1.RIGHT_CC.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo/Nao_densas/A_1004_1.RIGHT_MLO.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/A_1063_1.LEFT_CC.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/A_1009_1.RIGHT_MLO.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/A_1009_1.RIGHT_CC.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/A_1003_1.LEFT_MLO.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/A_1006_1.LEFT_CC.LJPEG.1.image.8.png";			
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/A_1016_1.LEFT_CC.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/A_1016_1.LEFT_MLO.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/A_1022_1.LEFT_CC.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/A_1075_1.RIGHT_CC.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/A_1084_1.RIGHT_CC.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/A_1005_1.RIGHT_CC.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/A_1005_1.RIGHT_MLO.LJPEG.1.image.8.png";
//											   //imagem = "f:/op/mestrado_ufma/imagens/mama/ddsm/3_Sem_fundo_com_Overlay/Nao_densas/A_1007_1.RIGHT_MLO.LJPEG.1.image.8.png";
//			cout << nomeArquivo << endl;
//			Mat imgOrigem = imread(imagem, CV_LOAD_IMAGE_GRAYSCALE);
//
//			//system("pause");
//
//			//imshow(nomeArquivo,imgOrigem);
//			//waitKey(0);
//
//
//			//arquivo overlay
//			//string arquivo = "C:/op/mestrado_ufma/imagens/mama/massas/DDSM/DDSM_cancer_15/case3138/B_3138_1.LEFT_CC.OVERLAY";
//
//
//			Mat imgOri = imgOrigem.clone();
//			Mat imgMod;
//
//			//Redimensionar a imagem para o Altura 512;
//			int lin = imgOri.rows;
//			int col = imgOri.cols;
//
//			//Redimensionar a imagem caso seja maior que 1024
//			//if (col > 1024){
//			//	Size t;
//			//	t.height=512;
//			//	t.width=col/((float)lin/t.height);
//			//	resize(imgOri,imgMod,t); // imgMod � a imagem redimensionada para altura de 512 x proporcional 
//			//}else{
//			imgMod = imgOri.clone();
//			//}
//
//			Mat imgBin = imgMod.clone();
//			Mat imgRed(imgMod.rows, imgMod.cols, imgMod.type());
//			Mat imgtemp(imgMod.rows, imgMod.cols, imgMod.type());
//			Mat imgtemp2(imgMod.rows, imgMod.cols, imgMod.type());
//			Mat imgtemp3(imgMod.rows, imgMod.cols, imgMod.type());
//			Mat imgtemp4(imgMod.rows, imgMod.cols, imgMod.type());
//			Mat imgThreshold(imgMod.rows, imgMod.cols, imgMod.type());
//			Mat imgOverlay(imgMod.rows, imgMod.cols, imgMod.type());
//			Mat imgParticula(imgRed.rows, imgRed.cols, imgRed.type());
//
//
//			////Filtor da mediana para o projeto otilio ----------------------------------------------------------
//			//Point ponto(13,13);  // 13x13
//			//GaussianBlur(imgMod,imgRed,ponto,2); // Com valor 2 fica bom mas diminue as imagens, com o valor 1 aumenta as imagens
//
//			//Teste 23/04/2015---------------------------
//			//Realce
//			Mat imgClahe = realceHistogramaLocal(imgMod);
//			//imwrite(pathDestino+"imgCLAHE0.png",imgClahe);
//
//			//Remove fundo
//			Otsu o(imgClahe);
//			float limiarObjeto = o.OtsuBinariza();
//			cout << "limiarObjeto: " << limiarObjeto << endl;
//			Mat imgSemFundo = removeFundoImagem(imgClahe, limiarObjeto);
//			//imwrite(pathDestino+"imgCLAHE00.png",imgSemFundo);
//
//			//Filtro da m�dia			
//			//Mat imgClaheMedia;
//			medianBlur(imgSemFundo, imgRed, 13);
//			//imwrite(pathDestino+"imgCLAHE01.PNG",imgRed);
//
//			//medianBlur(imgSemFundo,imgRed,15);	//15x15
//			//imwrite(pathDestino+"imgCLAHE02.PNG",imgRed);
//
//			/*for (int i=0; i < imgMod.rows; i++){
//			for(int j=0; j < imgMod.cols; j++){
//			if (imgClahe.at<uchar>(i,j) != 0) {
//			int pixel0 = imgClahe.at<uchar>(i,j);
//			int pixel1 = imgClaheMedia.at<uchar>(i,j);
//			int pixel2 = imgRed.at<uchar>(i,j);
//			cout << "Pixel0: " << pixel0 << "Pixel1: " << pixel1 << "Pixel2: " << pixel2 << endl;
//			}
//			}
//			}*/
//
//			//-----------------------------------------------------
//
//			//medianBlur(imgMod,imgtemp,5);	//15x15
//			//imgtemp2 = realceHistogramaLocal(imgtemp);
//			//imwrite(pathDestino+"imgCLAHE1.png",imgtemp2);
//
//			//Otsu o(imgtemp2);
//			//float limiarObjeto = o.OtsuBinariza();
//			//cout << "limiarObjeto: " << limiarObjeto << endl;
//			//imgtemp3 = removeFundoImagem(imgtemp2,limiarObjeto);
//			//imwrite(pathDestino+"imgCLAHE2.png",imgtemp3);
//
//
//			//medianBlur(imgtemp3,imgtemp4,15);	//15x15
//			//medianBlur(imgtemp3,imgRed,15);	//15x15
//			//imwrite(pathDestino+"imgCLAHE3.png",imgtemp4);
//
//			//medianBlur(imgtemp4,imgRed,15);	//15x15
//			//imwrite(pathDestino+"imgCLAHE4.png",imgRed);
//
//
//			//Mostra imagens para visualizar a imagem a ser processada- uasar so para teste-======================================================================
//			//imshow("Imagem Original",imgMod);
//			//imshow("Imagem Filtro da Media 1",imgtemp);
//			//imshow("Imagem Histograma Relace-CLAHE",imgtemp2);
//			//imshow("Imagem Histograma Relace-CLAHE-sem fundo",imgtemp3);
//			//imshow("Imagem Histograma Relace-CLAHE-Media 2",imgtemp4);
//			//imshow("Imagem Filtro da Media 3",imgRed);
//
//			//waitKey(0);
//
//
//
//			//--- Calcula a variancia da regiao da imagem kernel 5x5
//			desvioPadraoMedioImagem = calculaVarianciaMediaDesvioPadraoImagem(imgRed, 12, 12);
//			//desvioPadraoMedioImagem = calculaVarianciaMediaDesvioPadraoImagem(imgRed,17,17);
//			//encontrarCirculosImagem(imgRed);
//			//system("pause");
//
//			//--------------------------------------------------------------	
//
//			float k = 10;					// Fator da quantidade minima do enxame (Nr Clusteres)
//			float d = 2;					// Dimens�o dos objetos
//			float Dim = k*d;				// Dimens�o da fun��o de aptid�o (caracteristicas)
//			float L = 255;					// Nr. do nivel de cinza
//			int SwarmSize = 3 * k*d;			// Tamanho do enxame de particulas
//			float lb = 0;					// Limite inferior de aptidao de cada variavel
//			float ub = L;					// Limite superior de aptidao de cada variavel
//			float Iterations = 10 * k*d;		// Qunatidade de Itera��es
//			float Vmax = L;					// Velocidade maxima de cada particula
//			float Xi = 0;					// Solucao inicial
//			float Vi = 0;					// Velociada inicial da particula
//			float Pbest = 0;				// Melhor posi��o local (local)
//			float Gbest = 0;	 				// Melhor posi��o da visinhan�a (global)
//			float w = 0.729f;				// Peso da Inercia 
//			float c1 = 2.05f, c2 = 2.05f;	// Constantes de apendizado cognitivo (c1) e social (c2)
//			float n = SwarmSize - Dim + 1;	// Intervalo para SwarmSize ate Dim
//											//int i=0;
//			float Threshold = 0.;				// Threshold inicial da solu��o Otsu
//			float fitness = 0;				// Fitiness ou Aptid�o (Melhor)
//			float fitnessAtual = 0;			// Fitiness ou Aptid�o (Atual)
//			float centroid = 0;				// Centroid best
//			int contFitness = 0;				// Contador de fitness do vetor de cluster
//			float randon = 0;					// Variavel para nr. aleatorio				
//			int contaMudanca = 0;				// Contador de mudancas do Fitness se apos 10 iteracoes n�o mudar para o algoritmo  
//
//												//int paticulas = imgOrigem.rows*imgOrigem.cols; // Nr. de particulas
//												/*Particula p;
//
//												srand( time(0) );*/
//
//			cout << " Incicio " << endl;
//
//			// Usa o Otsu para cluster da soloc��o inicial
//			Otsu otsu(imgRed);
//			Threshold = otsu.OtsuBinariza();
//			//Threshold = threshold(imgRed,imgtemp,1,255,CV_THRESH_OTSU);
//			//fitness=calculaThreshold(imgRed,threshold);
//
//
//			//********************************** imgRed para o projeto OP
//			calculaThreshold(imgRed, Threshold);
//
//			// Inicializa as particulas
//			//vector <Particula*> particulas;
//			vector <float>* vetorTemp;
//			vector <int> vetorThreshold;
//			//float* vetInt;					
//
//			vetorTemp = new vector <float>;
//
//			for (int i = 0; i < clusterOtsu.size(); i++) {
//				vetorTemp->push_back(clusterOtsu.at(i)->threshold);
//			}
//
//			//ordena vetor de threshold e Remove valore repetidos
//			sort(vetorTemp->begin(), vetorTemp->end());
//			if (vetorTemp->size() != 1) {
//				for (int i = 0; i < vetorTemp->size() - 1; i++) {
//					//vetorThreshold.at(i) = vetorTemp->at(i);
//					if (i == vetorTemp->size() - 2) {
//						if (vetorTemp->at(i) != vetorTemp->at(i + 1)) {
//							vetorThreshold.push_back(vetorTemp->at(i));
//							vetorThreshold.push_back(vetorTemp->at(i + 1));
//						}
//						else {
//							vetorThreshold.push_back(vetorTemp->at(i));
//						}
//					}
//					else {
//						if (vetorTemp->at(i) != vetorTemp->at(i + 1)) {
//							vetorThreshold.push_back(vetorTemp->at(i));
//						}
//					}
//				}
//			}
//			else {
//				vetorThreshold.push_back(vetorTemp->at(0));
//			}
//
//
//			//Calcula fitness do primeiro treshold do vetor vetoThreshold, vetor de limiares
//			fitness = atualizaFitnessParticula(imgRed, vetorThreshold);
//
//
//
//			// inicializa as particulas
//			for (int i = 0; i < SwarmSize; i++) {
//		//		particulas.push_back(new Particula(lb, ub, n, fitness, vetorThreshold, i));
//			}
//
//			Gbest = Pbest = fitness;
//			vector <int> GbestVet, PbestVet;
//			for (int i = 0; i<vetorThreshold.size(); i++) {
//				GbestVet.push_back(vetorThreshold.at(i));
//				PbestVet.push_back(vetorThreshold.at(i));
//			}
//
//			//int1 = ((rand() % 10)+ 1) / 10.0;
//
//			for (int I = 0; I < Iterations; I++) {
//				if (I > 5) {
//					if (contaMudanca == 0)
//						break;
//				}
//
//				vector <int> vetorAuxiliar; // vetor para auxiliar nos valores de Xi de cada particula
//
//				for (int i = 0; i < SwarmSize; i++) {
//
//
//					//Particula* p = particulas[i]; // inicializa particula
//												  // Nr. aleatorios
//												  //srand( time(0) );
//												  //randon = ((rand() % 10)+ 1) / 10.0;
//												  //Xi = p->_pbest = (randon) * (ub-lb)+lb;
//												  //Xi = p->_vetThreshold.at(i)
//												  //p->_aptidao = fitness;
//					//p->_centroide = c.centroid1;
//					fitnessAtual = 0;
//					//cout << "_pbest da particula: " << i << " PXi: " << p->_pbest << " Gbest: " << Gbest << " Fitness: " << fitness << endl;
//					//for (int j = 0; j < p->_vetThreshold.size(); j++) {
//					//	Xi = p->_vetThreshold.at(j);
//					//	//atualiza Vi	(equa��o 2.2)
//					//	//Vi = w*Vi + ( c1 * (rand()%1) * (Pbest - img.at<uchar>(i,j))) + (c2 * (rand()%1) * (Gbest - img.at<uchar>(i,j)));
//					//	Vi = w * p->_velocidade +
//					//		(c1 * (rand() % 1) * (PbestVet.at(j) - Xi)) +	//(c1 * (rand()%1) * (Pbest - p->_pbest)) + 
//					//		(c2 * (rand() % 1) * (GbestVet.at(j) - Xi));	//(c2 * (rand()%1) * (Gbest - p->_pbest));
//
//					//														//Atualiza Xi
//					//	Xi = Xi + Vi > 255 ? 255 : Xi + Vi;
//					//	//cout << "Vi: " << Vi << endl;
//
//					//	// Adicionar aquir a formula de fitness ou aptidao
//					//	//fitnessAtual += atualizaFitness(imgRed,Xi,contFitness);
//					//	contFitness++;
//
//					//	vetorAuxiliar.push_back(Xi);
//					//	//p->_centroide = c.centroid1;
//					//	//melhor entre Xi e Pbest//Atualiza melhor Pbest e Gbest
//					//	//if (fitness < p->_aptidao) { //******************************************
//					//}
//
//					fitnessAtual = atualizaFitnessParticula(imgRed, vetorAuxiliar);
//
//					if (fitnessAtual < fitness && fitnessAtual > 0) { //******************************************
//						//p->_aptidao = fitnessAtual;
//						//p->_centroide = c.centroid1;
//						//p->_vetThreshold = vetorAuxiliar;
//						PbestVet = vetorAuxiliar;
//						fitness = fitnessAtual;
//						centroid = c.centroid1;
//						Pbest = fitnessAtual;
//						//cout << "Pebst Atual: " << p->_pbest << endl;
//						//cout << "fitness Atual: " << p->_pbest << endl;
//						if (fitnessAtual < Gbest) {
//							Gbest = fitnessAtual;
//							GbestVet = vetorAuxiliar;
//							//cout << "Gebst Atual: " << Gbest << endl;
//						}
//						//cout << "_pbest da particula: " << i << " Xi: " << p->_pbest << " Gbest: " << Gbest << " Fitness: " << fitness << endl;
//					}
//					else {
//						contaMudanca = 0;
//						//fitness = p->_aptidao;
//					}
//					vetorAuxiliar.clear();
//				}
//			}
//
//			//colocar as imagens de threshold *******************************************8
//			//Binarizar a imagem
//			Mat imgB = imgBin > Threshold;
//
//			int intervalo1 = abs(fitness - centroid);
//			int intervalo2 = abs(Gbest - Pbest);
//
//			//Gera imagem da melhor particula
//			//imgParticula = geraImagemMelhorParticula(GbestVet,imgRed); // cria imagens com intervalo do limiares
//			//pintaImagem(imgRed, GbestVet);	 // cria imagens com a distancia euclidiana do pixels com seu centroide
//
//			//GbestVet.clear();		// Zera o vetor de Gbest
//			PbestVet.clear();		// Zera o vetor de Pbest
//			vetorThreshold.clear();	// Zera vetor de Threshold
//			vetorTemp->clear();	// Zera vetor temporario
//		//	particulas.clear();		// Zera vetor de particulas
//			clusteres.clear();
//			clusterOtsu.clear();
//
//			cout << "Proxima Imagem: " << endl;
//			cout << "Imagens Processadas: " << processar << endl;
//
//			//namedWindow("Imagem Binaria",2);
//			//namedWindow("Img 1024",2);
//			//imshow("Img 1024",imgRed);
//			//waitKey(0);
//			//system("pause");
//
//		}
//	}
//
//	closedir(dir);
//	system("pause");
//
//	return 0;
//}


// mostra o timer
void mostraTimer() {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "Timer Now it's %I:%M%p.: ", timeinfo);
	puts(buffer);

}

//Clacula o thresholde inicial das particulas a partir do Otsu
void calculaThreshold(Mat &img, float Threshold) {
	//Mat imgFitness(img.rows,img.cols,img.type());
	float centroid1 = 0, centroid2 = 0, soma1 = 0, soma2 = 0, fitness = 0, thresholdAnterior = 0;
	int qtdObejtos1 = 0, qtdObejtos2 = 0;
	bool repetil = false;
	//int* qtdObjetos = new int[k];

	if (clusterOtsu.size() == 0) {
		clusterParticula.threshold = Threshold;
		clusterOtsu.push_back(new Cluster(clusterParticula));
	}
	else {
		thresholdAnterior = clusterOtsu.at(clusterOtsu.size() - 1)->threshold;
		if (thresholdAnterior != Threshold) {
			clusterParticula.threshold = Threshold;
			clusterOtsu.push_back(new Cluster(clusterParticula));
		}
		else {
			thresholdeRepetido++; // Contador de threshold repedito
			repetil = true;
		}
	}

	//Vetor de pixel para cada cluster
	vector<Pixel> pixels;
	//vector<Pixel> pixels2;

	// Fazer enquanto os pixels maior q 
	for (int i = 0; i< img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			if (img.at<uchar>(i, j) > 0 && img.at<uchar>(i, j) <= Threshold) {
				//c.pixels[(int)qtdObejtos1] = imgFitness.at<uchar>(i,j);
				Pixel p1;
				p1.x = i;
				p1.y = j;
				p1.valor = img.at<uchar>(i, j);
				p1.lado = 0; // Lado Esquerdo
				pixels.push_back(p1);
				//cout << "pixel " << (int)img.at<uchar>(i,j) << endl;
				//img.at<uchar>(i,j) = 0; 
			}
			else if (img.at<uchar>(i, j) > 0) {
				//c.pixels[(int)qtdObejtos2] = imgFitness.at<uchar>(i,j);
				Pixel p2;
				p2.x = i;
				p2.y = j;
				p2.valor = img.at<uchar>(i, j);
				p2.lado = 1; // Lado Direito
				pixels.push_back(p2);
				//cout << "pixel " << (int)img.at<uchar>(i,j) << endl;
				//img.at<uchar>(i,j) = 1; 
			}
		}

	}

	//** guardar threshold******
	//fitness = 
	calculaCentroide2(pixels, img, repetil, desvioPadraoMedioImagem);
	pixels.clear();
}

//Clacula centroide para vetor de pixel
void calculaCentroide2(vector<Pixel> pixels, Mat &img, bool repetil, float desvioPadraoMedio) {

	Mat imgThreshold1 = Mat::zeros(img.rows, img.cols, img.type());
	Mat imgThreshold2 = Mat::zeros(img.rows, img.cols, img.type());
	Mat imgTemp1 = Mat::zeros(img.rows, img.cols, img.type());
	Mat imgTemp2 = Mat::zeros(img.rows, img.cols, img.type());
	//cout << "valor pixel" << (int)imgThreshold1.at<uchar>(0,0) << " " << (int)imgThreshold2.at<uchar>(0,0) << endl;
	float centroid1 = 0, centroid2 = 0, soma1 = 0, soma2 = 0, Fitness1 = 0, Fitness2 = 0, mediaFit1 = 0, mediaFit2 = 0;
	float limiar1 = 0, limiar2 = 0, limiar11 = 0, limiar22 = 0, mediadiferencaFitness = 0;
	//variaveis para somar X e Y---------
	//float somaPixelX1 = 0, somaPixelX2 = 0,  somaPixelY1 = 0, somaPixelY2 = 0;
	float somaPixelPerimetro1 = 0, somaPixelPerimetro2 = 0, somaPixelArea1 = 0, somaPixelArea2 = 0, circularidade1 = 0, circularidade2 = 0;
	bool vizinhoZero = false;
	//---------------------------------------
	int qtdObejtos1 = 0, qtdObejtos2 = 0, qtdFit1 = 0, qtdFit2 = 0;
	int valorPixelMaior1 = 0, valorPixelMenor1 = 255, valorPixelMaior2 = 0, valorPixelMenor2 = 255, aux = 0;
	float liniarImagem1 = 0, liniarImagem2 = 0;
	//int* qtdObjetos = new int[k];

	for (int i = 0; i < pixels.size(); i++) {

		if (pixels.at(i).valor > 0 && pixels.at(i).lado == 0) {
			//c.pixels[(int)qtdObejtos1] = imgFitness.at<uchar>(i,j);
			soma1 += pixels.at(i).valor;
			qtdObejtos1++;
			/*vizinhoZero = verificaVizinhoZero(img, pixels.at(i).x, pixels.at(i).y,3,3);
			if (vizinhoZero){
			somaPixelPerimetro1++;
			imgTemp1.at<uchar>(pixels.at(i).x, pixels.at(i).y) = 255;
			}*/

			//Modificado para armazenar o menor e maior valor
			if (valorPixelMaior1 > pixels.at(i).valor) {
				if (pixels.at(i).valor < valorPixelMenor1) {
					valorPixelMenor1 = pixels.at(i).valor;
				}
			}
			else {
				valorPixelMaior1 = pixels.at(i).valor;
			}

			imgThreshold1.at<uchar>(pixels.at(i).x, pixels.at(i).y) = pixels.at(i).valor;

		}
		else if (pixels.at(i).valor > 0) {
			//c.pixels[(int)qtdObejtos2] = imgFitness.at<uchar>(i,j);
			soma2 += pixels.at(i).valor;
			qtdObejtos2++;
			/*vizinhoZero = verificaVizinhoZero(img, pixels.at(i).x, pixels.at(i).y,3,3);
			if (vizinhoZero){
			somaPixelPerimetro2++;
			imgTemp2.at<uchar>(pixels.at(i).x, pixels.at(i).y) = 255;
			}*/

			if (valorPixelMaior2 > pixels.at(i).valor) {
				if (pixels.at(i).valor < valorPixelMenor2) {
					valorPixelMenor2 = pixels.at(i).valor;
				}
			}
			else {
				valorPixelMaior2 = pixels.at(i).valor;
			}

			imgThreshold2.at<uchar>(pixels.at(i).x, pixels.at(i).y) = pixels.at(i).valor;
		}

	}

	liniarImagem1 = (valorPixelMaior1 + valorPixelMenor1) / 2;
	liniarImagem2 = (valorPixelMaior2 + valorPixelMenor2) / 2;
	//Grava imagem para Temporaria -----
	//imwrite("C:/op/mestrado_ufma/imagens/imagemPSO/imgThreshold1.jpg",imgThreshold1);
	//imwrite("C:/op/mestrado_ufma/imagens/imagemPSO/imgThreshold2.jpg",imgThreshold2);
	//imwrite(pathDestino+"imgTemp1.jpg",imgTemp1);
	//imwrite(pathDestino+"imgTemp2.jpg",imgTemp2);

	// Calculo da Compacidade ou circularidade
	// C = P^2 / A
	//circularidade1 = pow(somaPixelPerimetro1,2) / qtdObejtos1;
	//circularidade2 = pow(somaPixelPerimetro2,2) / qtdObejtos2;
	//cout << "Compacidade1: " << circularidade1 << endl;
	//cout << "Compacidade2: " << circularidade2 << endl;


	//Calculo do centroide
	centroid1 = qtdObejtos1 == 0 ? 0 : soma1 / qtdObejtos1; //mf
	centroid2 = qtdObejtos2 == 0 ? 0 : soma2 / qtdObejtos2; //mf

															//Calcula o fitnes com a soma da diferenca dos pixels com seu centroid
	for (int i = 0; i< pixels.size(); i++) {
		if (pixels.at(i).valor > 0 && pixels.at(i).lado == 0) {
			//c.pixels[(int)qtdObejtos1] = imgFitness.at<uchar>(i,j);
			Fitness1 += pow((pixels.at(i).valor - centroid1), 2);
			qtdFit1++;
		}
		else if (pixels.at(i).valor > 0) {
			//c.pixels[(int)qtdObejtos2] = imgFitness.at<uchar>(i,j);
			Fitness2 += pow((pixels.at(i).valor - centroid2), 2);
			//cout << "pixel: " << pixels.at(i).valor << endl;
			qtdFit2++;
		}

	}

	//m�dia do fitness = Calculo da variancia [ s = (xi - M)^2 / n-1 ] - Seguido do desvio padr�o
	//n = nr. pixel, xi pixel, M � o centroide (media), 
	mediaFit1 = qtdFit1 == 0 ? 0 : sqrt(Fitness1 / qtdFit1);
	mediaFit2 = qtdFit2 == 0 ? 0 : sqrt(Fitness2 / qtdFit2);

	mediadiferencaFitness = (mediaFit1 + mediaFit2) / 2; // M�dia da Diferenca entre as variancias de cada cluster
														 //cout << "Dif. da varianda: " << mediadiferencaFitness << "lado menor " << endl;

														 //Claucular outro limiar se a variancia maior que 15
	if (mediaFit1 > desvioPadraoMedio) {
		//Otsu otsu1(imgThreshold1);
		//limiar1 = otsu1.OtsuBinariza();
		//limiar11=threshold(imgThreshold1,imgTemp1,valorPixelMenor1,valorPixelMaior1,CV_THRESH_OTSU);
		//limiar11=threshold(imgThreshold1,imgTemp1,1,255,CV_THRESH_OTSU);
		//imwrite(pathDestino+"imgThreshold1.png",imgThreshold1);
		//imwrite(pathDestino+"imgTemp1.png",imgTemp1);
		//cout << "limiar1: " << limiar1 << "lado menor " << endl;
		//pixels.clear();
		//if (repetil!=true){
		//calculaThreshold(imgThreshold1, limiar1);
		//calculaThreshold(imgThreshold1, limiar11);
		calculaThreshold(imgThreshold1, centroid1);
		//repetil = false;
		//}		
	}

	//Claucular outro limiar se a variancia maior que 15
	if (mediaFit2 > desvioPadraoMedio) {
		//Otsu otsu2(imgThreshold2);
		//limiar2 = otsu2.OtsuBinariza();
		//limiar22=threshold(imgThreshold2,imgTemp2,valorPixelMenor2,valorPixelMaior2,CV_THRESH_OTSU);
		//limiar22=threshold(imgThreshold2,imgTemp2,1,255,CV_THRESH_OTSU);
		//imwrite(pathDestino+"imgThreshold2.png",imgThreshold2);
		//imwrite(pathDestino+"imgTemp2.png",imgTemp2);
		//cout << "limiar2: " << limiar2 << "lado maior " << endl;
		//pixels.clear();
		//if (repetil != true){
		//calculaThreshold(imgThreshold2, limiar2);  // o erro esta aqui quando repete o otsu 
		// calculaThreshold(imgThreshold2, limiar22);
		calculaThreshold(imgThreshold2, centroid2);
		//repetil = false;
		//}
	}

	pixels.clear();

	//int xx=0;
	//calcula o ultimo limiar a partir do centroide da imagem acima do primeiro limiar gerado pelo Otsu
	//dividir a media pelo valor do desvio padrao ******************************************
	//if (mediadiferencaFitness > desvioPadraoMedio){
	//	if (mediaFit1 > 0 && mediaFit2 > 0){
	//		if (mediaFit1 > mediaFit2 ){
	//			if (repetil != true){
	//				calculaThreshold(imgThreshold1, centroid1);  // o erro esta aqui quando repete o otsu 
	//			}
	//		}else{
	//			if (repetil != true){
	//				calculaThreshold(imgThreshold2, centroid2);  // o erro esta aqui quando repete o otsu 
	//			}
	//		}
	//	}
	//}

}


//Atualiza o fitness de cada particula
float atualizaFitnessParticula(Mat &img, vector <int> centroide) {

	//Mat imgFitness = img;
	float fitness = 0, qtdElementos = 0, qtdCluster = 0, desvioPadra = 0;
	float variancia = 0, somaDaDiferenca = 0;
	vector <Cluster> clusterParticulas;
	int diferencaCentroide = 0;
	int diferencaMenor = 256;
	int aux = 0, valorPixel, qtdCentroide = centroide.size() - 1;

	for (int i = 0; i<qtdCentroide; i++) {
		Cluster cluster;
		cluster.threshold = 0;
		cluster.centroid1 = i;
		cluster.qtdElementos1 = 0;
		cluster.somaPixels = 0;
		clusterParticulas.push_back(cluster);
	}

	//Adicionado para verificar se o vator de particulas est� vasio*****************************************8
	if (!clusterParticulas.empty()) {

		int vetPixelUsado[256];
		int x = 0;
		while (x < 256) {	//********************* 255 ou 256
			vetPixelUsado[x] = -1;
			x++;
		}

		for (int i = 0; i< img.rows; i++) {
			for (int j = 0; j < img.cols; j++) {
				valorPixel = img.at<uchar>(i, j);
				if (valorPixel > 0) {
					if (vetPixelUsado[valorPixel] == -1) {
						aux = 0;
						qtdElementos = 0;
						somaDaDiferenca = 0;
						vetPixelUsado[valorPixel] == 1;
						for (int k = 0; k < qtdCentroide; k++) {
							//cout << "pixel: " << (int)img.at<uchar>(i,j) << "centroide: " << centroide.at(aux) << endl; 
							//cout << "pixel: " << (int)img.at<uchar>(i,j) << "centroide: " << centroide.at(k+1) << endl; 
							diferencaCentroide = abs(valorPixel - centroide.at(k));

							if (diferencaCentroide < diferencaMenor) {
								aux = k;
								diferencaMenor = diferencaCentroide;
							}
						}

						//cout << "pixel: " << (int)img.at<uchar>(i,j) << "centroide: " << centroide.at(aux) << endl; 
						somaDaDiferenca += pow((float)(diferencaMenor), 2);
						qtdElementos++;

						//Cluster k;
						clusterParticulas.at(aux).qtdElementos1 += qtdElementos;
						clusterParticulas.at(aux).somaPixels += somaDaDiferenca;

					}
				}
			}
		}


		//cout << "pr�xima part�cula" << endl;

		//Calculo da variancia, Desvio padr�o = Fiteness
		for (int x = 0; x < qtdCentroide; x++) {

			//variancia += sqrt(clusterParticulas.at(x).somaPixels / clusterParticulas.at(x).qtdElementos1);
			variancia += clusterParticulas.at(x).qtdElementos1 == 0 ? 0 : (float)(clusterParticulas.at(x).somaPixels / clusterParticulas.at(x).qtdElementos1);
		}


		// Desvio Absoluto m�dio (Sf)
		desvioPadra = sqrt(variancia / qtdCentroide);
		clusterParticulas.clear();
	}
	else {
		cout << "Vetor de particulas vasio: " << endl;
	}

	return desvioPadra;
}


// Fu��o para montar a imagem com a distancia euclidiana do pixels
//Gera imagens da particula Final pelo centroide *************************
//void pintaImagem(Mat &img, vector <int> centroide){
//
//	//imagens temporarias
//	Mat imgTemp1  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp2  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp3  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp4  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp5  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp6  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp7  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp8  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp9  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp10 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp11 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp12 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp13 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp14 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp15 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp16  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp17  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp18  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp19  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp20  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp21  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp22  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp23  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp24  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp25 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp26 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp27 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp28 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp29 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp30 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp31  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp32  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp33  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp34  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp35 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp36 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp37 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp38 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp39 = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgTemp40 = Mat::zeros(img.rows,img.cols,img.type());
//	
//	//imagens finais
//	Mat imgPart1   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart2   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart3   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart4   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart5   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart6   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart7   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart8   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart9   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart10  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart11  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart12  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart13  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart14  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart15  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart16   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart17   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart18   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart19   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart20  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart21   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart22  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart23  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart24   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart25  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart26  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart27  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart28  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart29  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart30  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart31   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart32  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart33  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart34   = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart35  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart36  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart37  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart38  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart39  = Mat::zeros(img.rows,img.cols,img.type());
//	Mat imgPart40  = Mat::zeros(img.rows,img.cols,img.type());
//
//	float fitness=0, qtdElementos=0, qtdCluster=0, desvioPadra=0;
//	float variancia=0, somaDaDiferenca=0;
//	vector <Cluster> clusterParticulas;
//	int diferencaCentroide=0;
//	int diferencaMenor=255;
//	int aux=0, valorPixel, qtdCentroide = centroide.size()-1; 
//	char arquivo[30];
//
//	//variaveis para somar X e Y---------
//	//float somaPixelX1 = 0, somaPixelX2 = 0,  somaPixelY1 = 0, somaPixelY2 = 0;
//	float fatorCircular = 0; vector <float> vetFatorCircular;
//	float perimetro1 = 0, perimetro2 = 0, perimetro3 = 0, perimetro4 = 0, perimetro5 = 0, perimetro6 = 0, perimetro7 = 0, perimetro8 = 0, perimetro9 = 0, perimetro10 = 0, 
//		  perimetro11 = 0, perimetro12 = 0, perimetro13 = 0, perimetro14 = 0, perimetro15 = 0; 
//	float area1 = 0, area2 = 0, area3 = 0, area4 = 0, area5 = 0, area6 = 0, area7 = 0, area8 = 0, area9 = 0, area10 = 0, area11 = 0, area12 = 0, area13 = 0, area14 = 0, area15 = 0;
//	float circularidade1 = 0,	circularidade2 = 0, circularidade3 = 0,	circularidade4 = 0, circularidade5 = 0,	circularidade6 = 0, circularidade7 = 0,	circularidade8 = 0, 
//		  circularidade9 = 0,	circularidade10 = 0, circularidade11 = 0,	circularidade12 = 0, circularidade13 = 0,	circularidade14 = 0, circularidade15 = 0;
//	bool vizinhoZero = false;
//
//	for(int i=0;i<qtdCentroide;i++){
//		Cluster cluster;
//		cluster.threshold = 0;
//		cluster.centroid1= i;
//		cluster.qtdElementos1 = 0;
//		cluster.somaPixels = 0;
//		clusterParticulas.push_back(cluster);
//	}
//
//
//	cout << "Criando imagens, aguarde..." << endl;
//
//	for (int i=0; i< img.rows; i++){
//		for(int j=0; j < img.cols; j++){
//			valorPixel=img.at<uchar>(i,j);
//			if (valorPixel > 0){
//				aux=0;
//				//qtdElementos=0;
//				//somaDaDiferenca=0;
//				diferencaMenor = 255;
//				for (int k = 0; k < qtdCentroide; k++){
//					//cout << "pixel: " << (int)img.at<uchar>(i,j) << "centroide: " << centroide.at(aux) << endl; 
//					//cout << "pixel: " << (int)img.at<uchar>(i,j) << "centroide: " << centroide.at(k+1) << endl; 
//					diferencaCentroide = abs(valorPixel - centroide.at(k));
//
//					if (diferencaCentroide < diferencaMenor){
//						aux = k;
//						diferencaMenor = diferencaCentroide;
//					}
//				}
//									
//				//cout << "pixel: " << (int)img.at<uchar>(i,j) << "centroide: " << centroide.at(aux) << endl; 
//				if (aux == 0){
//					imgPart1.at<uchar>(i,j) = 255;
//					/*area1++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro1++;
//						imgTemp1.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 1){
//					imgPart2.at<uchar>(i,j) = 255;
//					/*area2++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro2++;
//						imgTemp2.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 2){
//					imgPart3.at<uchar>(i,j) = 255;
//					/*area3++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro3++;
//						imgTemp3.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 3){
//					imgPart4.at<uchar>(i,j) = 255;
//					/*area4++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro4++;
//						imgTemp4.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 4){
//					imgPart5.at<uchar>(i,j) = 255;
//					/*area5++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro5++;
//						imgTemp5.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 5){
//					imgPart6.at<uchar>(i,j) = 255;
//					/*area6++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro6++;
//						imgTemp6.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 6){
//					imgPart7.at<uchar>(i,j) = 255;
//				/*	area7++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro7++;
//						imgTemp7.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 7){
//					imgPart8.at<uchar>(i,j) = 255;
//					/*area8++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro8++;
//						imgTemp8.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 8){
//					imgPart9.at<uchar>(i,j) = 255;
//					/*area9++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro9++;
//						imgTemp9.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 9){
//					imgPart10.at<uchar>(i,j) = 255;
//					/*area10++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro10++;
//						imgTemp10.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 10){
//					imgPart11.at<uchar>(i,j) = 255;
//					/*area11++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro11++;
//						imgTemp11.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 11){
//					imgPart12.at<uchar>(i,j) = 255;
//					/*area12++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro12++;
//						imgTemp12.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 12){
//					imgPart13.at<uchar>(i,j) = 255;
//					/*area13++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro13++;
//						imgTemp13.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 13){
//					imgPart14.at<uchar>(i,j) = 255;
//					/*area14++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro14++;
//						imgTemp14.at<uchar>(i, j) = 255;
//					}*/
//				}else if (aux == 14){
//					imgPart15.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 15){
//					imgPart16.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 16){
//					imgPart17.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 17){
//					imgPart18.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 18){
//					imgPart19.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 19){
//					imgPart20.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 20){
//					imgPart21.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 21){
//					imgPart22.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 22){
//					imgPart23.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//
//				}else if (aux == 23){
//					imgPart24.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//			
//				}else if (aux == 24){
//					imgPart25.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 25){
//					imgPart26.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 26){
//					imgPart27.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 27){
//					imgPart28.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//			
//				}else if (aux == 28){
//					imgPart29.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 29){
//					imgPart30.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 30){
//					imgPart31.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 31){
//					imgPart32.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 32){
//					imgPart33.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 33){
//					imgPart34.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 34){
//					imgPart35.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 35){
//					imgPart36.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 36){
//					imgPart37.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 37){
//					imgPart38.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 38){
//					imgPart39.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}else if (aux == 39){
//					imgPart40.at<uchar>(i,j) = 255;
//					/*area15++;
//					vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
//					if (vizinhoZero){
//						perimetro15++;
//						imgTemp15.at<uchar>(i, j) = 255;
//					}*/
//				
//				}				
//			}
//		}
//	}
//
//	//Gera imagens
//	string imagemGerada1 = pathDestino + nomeArquivo+".Part1.png";
//	string imagemGerada2 = pathDestino + nomeArquivo+".Part2.png";
//	string imagemGerada3 = pathDestino + nomeArquivo+".Part3.png";
//	string imagemGerada4 = pathDestino + nomeArquivo+".Part4.png";
//	string imagemGerada5 = pathDestino + nomeArquivo+".Part5.png";
//	string imagemGerada6 = pathDestino + nomeArquivo+".Part6.png";
//	string imagemGerada7 = pathDestino + nomeArquivo+".Part7.png";
//	string imagemGerada8 = pathDestino + nomeArquivo+".Part8.png";
//	string imagemGerada9 = pathDestino + nomeArquivo+".Part9.png";
//	string imagemGerada10 = pathDestino + nomeArquivo+".Part10.png";
//	string imagemGerada11 = pathDestino + nomeArquivo+".Part11.png";
//	string imagemGerada12 = pathDestino + nomeArquivo+".Part12.png";
//	string imagemGerada13 = pathDestino + nomeArquivo+".Part13.png";
//	string imagemGerada14 = pathDestino + nomeArquivo+".Part14.png";
//	string imagemGerada15 = pathDestino + nomeArquivo+".Part15.png";
//	string imagemGerada16 = pathDestino + nomeArquivo+".Part16.png";
//	string imagemGerada17 = pathDestino + nomeArquivo+".Part17.png";
//	string imagemGerada18 = pathDestino + nomeArquivo+".Part18.png";
//	string imagemGerada19 = pathDestino + nomeArquivo+".Part19.png";
//	string imagemGerada20 = pathDestino + nomeArquivo+".Part20.png";
//	string imagemGerada21 = pathDestino + nomeArquivo+".Part21.png";
//	string imagemGerada22 = pathDestino + nomeArquivo+".Part22.png";
//	string imagemGerada23 = pathDestino + nomeArquivo+".Part23.png";
//	string imagemGerada24 = pathDestino + nomeArquivo+".Part24.png";
//	string imagemGerada25 = pathDestino + nomeArquivo+".Part25.png";
//	string imagemGerada26 = pathDestino + nomeArquivo+".Part26.png";
//	string imagemGerada27 = pathDestino + nomeArquivo+".Part27.png";
//	string imagemGerada28 = pathDestino + nomeArquivo+".Part28.png";
//	string imagemGerada29 = pathDestino + nomeArquivo+".Part29.png";
//	string imagemGerada30 = pathDestino + nomeArquivo+".Part30.png";
//	string imagemGerada31 = pathDestino + nomeArquivo+".Part31.png";
//	string imagemGerada32 = pathDestino + nomeArquivo+".Part32.png";
//	string imagemGerada33 = pathDestino + nomeArquivo+".Part33.png";
//	string imagemGerada34 = pathDestino + nomeArquivo+".Part34.png";
//	string imagemGerada35 = pathDestino + nomeArquivo+".Part35.png";
//	string imagemGerada36 = pathDestino + nomeArquivo+".Part36.png";
//	string imagemGerada37 = pathDestino + nomeArquivo+".Part37.png";
//	string imagemGerada38 = pathDestino + nomeArquivo+".Part38.png";
//	string imagemGerada39 = pathDestino + nomeArquivo+".Part39.png";
//	string imagemGerada40 = pathDestino + nomeArquivo+".Part40.png";
//		
//	//cout << imagemGerada1 << endl;
//	//imwrite(imagemGerada1, imgPart1);
//	
//	countNonZero(imgPart1)	> 1 ? imwrite(imagemGerada1, imgPart1)	: cout << countNonZero(imgPart1) << endl;
//	countNonZero(imgPart2)	> 1 ? imwrite(imagemGerada2, imgPart2)	: cout << countNonZero(imgPart2) << endl;
//	countNonZero(imgPart3)	> 1 ? imwrite(imagemGerada3, imgPart3)	: cout << countNonZero(imgPart3) << endl;
//	countNonZero(imgPart4)	> 1 ? imwrite(imagemGerada4, imgPart4)	: cout << countNonZero(imgPart4) << endl;
//	countNonZero(imgPart5)	> 1 ? imwrite(imagemGerada5, imgPart5)	: cout << countNonZero(imgPart5) << endl;
//	countNonZero(imgPart6)	> 1 ? imwrite(imagemGerada6, imgPart6)	: cout << countNonZero(imgPart6) << endl;
//	countNonZero(imgPart7)	> 1 ? imwrite(imagemGerada7, imgPart7)	: cout << countNonZero(imgPart7) << endl;
//	countNonZero(imgPart8)	> 1 ? imwrite(imagemGerada8, imgPart8)	: cout << countNonZero(imgPart8) << endl;
//	countNonZero(imgPart9)	> 1 ? imwrite(imagemGerada9, imgPart9)	: cout << countNonZero(imgPart9) << endl;
//	countNonZero(imgPart10) > 1 ? imwrite(imagemGerada10, imgPart10): cout << countNonZero(imgPart10) << endl;
//	countNonZero(imgPart11) > 1 ? imwrite(imagemGerada11, imgPart11): cout << countNonZero(imgPart11) << endl;
//	countNonZero(imgPart12) > 1 ? imwrite(imagemGerada12, imgPart12): cout << countNonZero(imgPart12) << endl;
//	countNonZero(imgPart13) > 1 ? imwrite(imagemGerada13, imgPart13): cout << countNonZero(imgPart13) << endl;
//	countNonZero(imgPart14) > 1 ? imwrite(imagemGerada14, imgPart14): cout << countNonZero(imgPart14) << endl;
//	countNonZero(imgPart15) > 1 ? imwrite(imagemGerada15, imgPart15): cout << countNonZero(imgPart15) << endl;
//	countNonZero(imgPart16) > 1 ? imwrite(imagemGerada16, imgPart16): cout << countNonZero(imgPart16) << endl;
//	countNonZero(imgPart17) > 1 ? imwrite(imagemGerada17, imgPart17): cout << countNonZero(imgPart17) << endl;
//	countNonZero(imgPart18) > 1 ? imwrite(imagemGerada18, imgPart18): cout << countNonZero(imgPart18) << endl;
//	countNonZero(imgPart19) > 1 ? imwrite(imagemGerada19, imgPart19): cout << countNonZero(imgPart19) << endl;
//	countNonZero(imgPart20) > 1 ? imwrite(imagemGerada20, imgPart20): cout << countNonZero(imgPart20) << endl;
//	countNonZero(imgPart21) > 1 ? imwrite(imagemGerada21, imgPart21): cout << countNonZero(imgPart21) << endl;
//	countNonZero(imgPart22) > 1 ? imwrite(imagemGerada22, imgPart22): cout << countNonZero(imgPart22) << endl;
//	countNonZero(imgPart23) > 1 ? imwrite(imagemGerada23, imgPart23): cout << countNonZero(imgPart23) << endl;
//	countNonZero(imgPart24) > 1 ? imwrite(imagemGerada24, imgPart24): cout << countNonZero(imgPart24) << endl;
//	countNonZero(imgPart25) > 1 ? imwrite(imagemGerada25, imgPart25): cout << countNonZero(imgPart25) << endl;
//	countNonZero(imgPart26) > 1 ? imwrite(imagemGerada26, imgPart26): cout << countNonZero(imgPart26) << endl;
//	countNonZero(imgPart27) > 1 ? imwrite(imagemGerada27, imgPart27): cout << countNonZero(imgPart27) << endl;
//	countNonZero(imgPart28) > 1 ? imwrite(imagemGerada28, imgPart28): cout << countNonZero(imgPart28) << endl;
//	countNonZero(imgPart29) > 1 ? imwrite(imagemGerada29, imgPart29): cout << countNonZero(imgPart29) << endl;
//	countNonZero(imgPart30) > 1 ? imwrite(imagemGerada30, imgPart30): cout << countNonZero(imgPart30) << endl;
//	countNonZero(imgPart31) > 1 ? imwrite(imagemGerada31, imgPart31): cout << countNonZero(imgPart31) << endl;
//	countNonZero(imgPart32) > 1 ? imwrite(imagemGerada32, imgPart32): cout << countNonZero(imgPart32) << endl;
//	countNonZero(imgPart33) > 1 ? imwrite(imagemGerada33, imgPart33): cout << countNonZero(imgPart33) << endl;
//	countNonZero(imgPart34) > 1 ? imwrite(imagemGerada34, imgPart34): cout << countNonZero(imgPart34) << endl;
//	countNonZero(imgPart35) > 1 ? imwrite(imagemGerada35, imgPart35): cout << countNonZero(imgPart35) << endl;
//	countNonZero(imgPart36) > 1 ? imwrite(imagemGerada36, imgPart36): cout << countNonZero(imgPart36) << endl;
//	countNonZero(imgPart37) > 1 ? imwrite(imagemGerada37, imgPart37): cout << countNonZero(imgPart37) << endl;
//	countNonZero(imgPart38) > 1 ? imwrite(imagemGerada38, imgPart38): cout << countNonZero(imgPart38) << endl;
//	countNonZero(imgPart39) > 1 ? imwrite(imagemGerada39, imgPart39): cout << countNonZero(imgPart39) << endl;
//	countNonZero(imgPart40) > 1 ? imwrite(imagemGerada40, imgPart40): cout << countNonZero(imgPart40) << endl;
//
//	
//	//erode(imgOri,imgtemp,ponto,);
//	//namedWindow("Imagem Teste X",2);
//	//imshow("Imagem Original",imgMod);
//	//imshow("Imagem Filtro da Erosao 5",imgtemp);
//	//imshow("Imagem Filtro da Dilatacao 5",imgtemp2);
//	//waitKey(0);
//
//	clusterParticulas.clear();
//	cout << "Fim..." << endl;
//	
//}

//--- Calcula a variancia media do desvio padrao das regioes da imagem kernel x por y
float calculaVarianciaMediaDesvioPadraoImagem(Mat &imgOrigem, int kernelX, int kernelY) {

	Mat imgKernel = Mat::zeros(imgOrigem.rows, imgOrigem.cols, imgOrigem.type());

	vector <int> vetValorPixel;
	int lin = imgOrigem.rows;
	int col = imgOrigem.cols;
	float variancia = 0, desvioPadrao = 0, desvioPadraoMedio = 0, soma = 0, media = 0;
	int qtdPixel = 0, qtdDesvioPadrao = 0;
	bool sair = false;

	for (int y = 0; y < lin; y += kernelY) {
		for (int x = 0; x < col; x += kernelX) {

			for (int i = y; i < y + kernelY; i++) {
				for (int j = x; j < x + kernelX; j++) {
					if (imgOrigem.at<uchar>(i, j) == 0) {
						sair = true;
						break;

					}
					soma += imgOrigem.at<uchar>(i, j);
					qtdPixel++;
					vetValorPixel.push_back(imgOrigem.at<uchar>(i, j));
					//cout << (int)imgOrigem.at<uchar>(i,j) << "=> i, j " << i <<", " << j << endl;
				}
				if (sair)
					break;
			}

			if (sair == false) {
				//cout << "soma: " << soma << endl;
				//Media
				media = soma / qtdPixel;
				//Variacia
				for (int w = 0; w < vetValorPixel.size(); w++) {

					variancia += pow((vetValorPixel[w] - media), 2);
				}
				desvioPadrao += sqrt(variancia / qtdPixel);
				qtdDesvioPadrao++;
				//cout << "soma: " << soma << endl;
			}
			sair = false;
			vetValorPixel.clear();
			soma = 0;
			media = 0;
			qtdPixel = 0;
			variancia = 0;
			//desvioPadrao = 0;
		}
	}

	desvioPadraoMedio = desvioPadrao / qtdDesvioPadrao;
	cout << "desvio: " << desvioPadrao << endl;
	cout << "desvio Padrao Medio: " << desvioPadraoMedio << endl;
	vetValorPixel.clear();

	return desvioPadraoMedio;

}

//Calcula a circularidade da imagem
float circularidade(Mat &img) {

	Mat imgBorda = Mat::zeros(img.rows, img.cols, img.type());

	float perimetro = 0, area = 0, circularidade = 0, fatorCircular = 0, fatorCircularAnterior = 0;
	float PI = 3.14159265358979323846264338327950288;
	bool vizinhoZero;

	for (int i = 1; i< img.rows - 1; i++) {
		for (int j = 1; j < img.cols - 1; j++) {
			if (img.at<uchar>(i, j) > 0) {
				area++;
				vizinhoZero = verificaVizinhoZero(img, i, j, 3, 3);
				if (vizinhoZero) {
					perimetro++;
					imgBorda.at<uchar>(i, j) = 255;
				}
			}
		}
	}

	circularidade = area > 0 ? pow(perimetro, 2) / area : 0;
	//fator de cicularidade
	fatorCircular = perimetro > 0 ? (4 * PI * area) / pow(perimetro, 2) : 0;
	//cout << "Circularidade : " << circularidade << endl;
	//cout << "Fator Circular: " << fatorCircular << endl;
	if (fatorCircular >= 0.5) {
		imwrite(pathDestino + nomeArquivo + ".Forma.png", imgBorda);
	}

	return fatorCircular;

}

// Verifica a visinhan�a do pixel
bool verificaVizinhoZero(Mat &img, float x, float y, int kernelX, int kernelY) {

	Mat imgTeste = Mat::zeros(img.rows, img.cols, img.type());
	int lin = img.rows;
	int col = img.cols;
	int posX = x - 1;
	int posY = y - 1;

	for (int i = 0; i < kernelX; i++) {
		for (int j = 0; j < kernelY; j++) {
			//cout << "Pixel X: " << posX+i << "Pixel Y: " << posY+j << endl;
			if (img.at<uchar>(posX + i, posY + j) == 0) {
				//imgTeste.at<uchar>(posX+i,posY+j) = 0;
				//cout << "Valor Pixel: " << (int)img.at<uchar>(posX + i,posY + j) << endl;
				return true;
			}
			else {
				//imgTeste.at<uchar>(posX+i,posY+j) = 255;
				//cout << "Valor Pixel: " << (int)img.at<uchar>(posX + i,posY + j) << endl;
			}
		}
	}


	//imwrite(pathDestino+".forma."+nomeArquivo,imgTeste);
	return false;
}



//Encontrar circulos na imagem
void encontrarCirculosImagem(Mat &imagem)
{
	//   Mat img = imagem, gray;
	////cvCopy(imagem, img);
	//   //cvtColor(img, gray, CV_BGR2GRAY);
	//   // smooth it, otherwise a lot of false circles may be detected
	//   GaussianBlur( img, gray, Size(9, 9), 2, 2 );
	//   vector<Point> circles;
	//   HoughCircles(gray, circles, CV_HOUGH_GRADIENT,
	//                1, gray.rows/4, 200, 100 );
	//   for( size_t i = 0; i < circles.size(); i++ )
	//   {
	//	Point center(cvRound(circles.at(i).x), cvRound(circles.at(i).y));

	//	int radius = cvRound((circles.at(i).x + circles.at(i).y )) / 2;
	//        // draw the circle center
	//        circle( img, center, 3, Scalar(0,255,0), -1, 8, 0 );
	//        // draw the circle outline
	//        circle( img, center, radius, Scalar(0,0,255), 3, 8, 0 );
	//   }
	//   namedWindow( "circles", 1 );
	//   imshow( "circles", img );
	//   //return 0;
}

//Realce histograma local - CLAHE
Mat realceHistogramaLocal(Mat img) {

	//Mat m= imread("lena.png",CV_LOAD_IMAGE_GRAYSCALE); //input image
	//imshow("lena_GRAYSCALE",m);

	Ptr<CLAHE> clahe = createCLAHE();
	clahe->setClipLimit(4);

	Mat dst;
	clahe->apply(img, dst);
	//imshow("img_CLAHE",dst);
	//waitKey();

	return dst;
}

// Remove funda da imagem
Mat removeFundoImagem(Mat &img, float limiar) {

	Mat imgObjeto = Mat::zeros(img.rows, img.cols, img.type());
	int lin = img.rows;
	int col = img.cols;

	for (int i = 0; i < lin; i++) {
		for (int j = 0; j < col; j++) {

			if (img.at<uchar>(i, j) >= limiar) {
				imgObjeto.at<uchar>(i, j) = img.at<uchar>(i, j);
			}
		}
	}

	return imgObjeto;
}

// Retorna tipo de mama (1-> Nao Densa, 2-> Densa)
int returnTipoDeMama() {

	int tipoMama = 0;

	cout << "Escolha o tipo de Mama: " << endl;
	cout << "1 - Mama Nao Densa " << endl;
	cout << "2 - Mama Densa " << endl;

	cin >> tipoMama;

	return tipoMama;

}