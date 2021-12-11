#include "Arvores.h"
#include "MedidasDiversidadeFuncional.h"
#include <itkGDCMImageIOFactory.h>
#include "itkImageDuplicator.h"


Arvores::Arvores(void)
{
	
	_pos = 0;
	this->_nos.pos = 0;
	this->_nos.pai = 0;
	this->_nos.esq = 0;
	this->_nos.dir = 0;
	typedef itk::GDCMImageIO ImageIO;
	typedef itk::Image<short, 3> ImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;
	typedef itk::ImageFileWriter<ImageType> WriterType;
	typedef itk::Image<short, 3> ImageType3D;
	typedef itk::Image<short, 2> ImageType2D;
	typedef itk::ImageFileReader<ImageType> ReaderType;
	typedef itk::ImageFileWriter<ImageType> WriterType;
	typedef itk::ImageFileWriter<ImageType2D> WriterType2;
	typedef unsigned char PixelTypeChar;   // must be signed for CT since Hounsfield units can be < 0 
	typedef Image< PixelTypeChar, 3 > ImageTypeChar;
	
}

MedidasDiversidadeFuncional md;

Arvores::~Arvores(void)
{
}

int Arvores::getMinimumA(ImageType::Pointer image) {
	typedef MinimumMaximumImageCalculator <ImageType> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMinimum();
}

int Arvores::getMaximumA(ImageType3D::Pointer image) {
	typedef MinimumMaximumImageCalculator <ImageType3D> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMaximum();
}

ImageType3D::Pointer Arvores::createImageA(ImageType3D::Pointer image) {

	ImageType3D::Pointer newImage = ImageType3D::New();
	/*ImageType3D::SpacingType spacing;
	spacing = image->GetSpacing();
	ImageType3D::SpacingType spacingOut;
	spacingOut[0] = spacing[0];
	spacingOut[1] = spacing[1];
	spacingOut[2] = spacing[2];*/
	ImageType3D::IndexType start;
	start.Fill(0);
	ImageType3D::RegionType region;
	region.SetSize(image->GetLargestPossibleRegion().GetSize());
	region.SetIndex(start);
	newImage->SetRegions(region);
	newImage->SetSpacing(image->GetSpacing());
	newImage->Allocate();
	newImage->FillBuffer(getMinimumA(image));
	return newImage;
}

// OTSU
double Arvores::wClusteringA(ImageType3D::Pointer image, int numCluster) {
	typedef itk::OtsuMultipleThresholdsImageFilter <ImageType3D, ImageType3D> FilterType;
	FilterType::Pointer otsuFilter = FilterType::New();
	otsuFilter->SetInput(image);
	otsuFilter->SetNumberOfThresholds(numCluster);
	otsuFilter->SetNumberOfHistogramBins(getMaximumA(image) - getMinimumA(image) + 1);
	otsuFilter->Update();
	FilterType::ThresholdVectorType thresholds = otsuFilter->GetThresholds();
	//cout << "\NTAMANHO DO OTSU: " <<thresholds.size() << endl << endl;
	double limiar = thresholds.at(thresholds.size()-1);
	//double limiar = otsuFilter->GetThresholds();
	return limiar;
}

void Arvores::saveImageA(ImageType3D::Pointer image, string path) {
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(path);
	writer->SetInput(image);
	writer->Update();
}

//funcoes para criacao da arvore
int Arvores::getNumberSpecies(ImageType3D::Pointer image, int limite) {
	MedidasDiversidadeFuncional md;

	ImageType3D::Pointer imgRegion;
	ImageType3D::SizeType size;
	size = image->GetLargestPossibleRegion().GetSize();

	int min, max;
	int negativo;
	min = getMinimumA(image);
	int minComparacao = min;
	//Saber se a intensidade é negativa ou não, para comparar la na árvore
	if (minComparacao<0) {
		negativo = 1;
	}
	else {
		negativo = 0;
	}
	max = getMaximumA(image);
	//
	//if (min>0) {
	//	min = (min*-1);
	//}
	int sizeHistograma;

	if (max<0) {
		max = max*(-1);
	}

	if (min<0) {
		sizeHistograma = max - min + 1;
	}
	else if (min >= 0) {
		sizeHistograma = max + 1;
	}

	//int sizeHistograma = max + min + 1;

	int *histograma = new int[sizeHistograma];
	if (min<0) {
		for (int i = 0; i < sizeHistograma; i++) {
			histograma[i] = 0;
		}
	}
	else {
		for (int i = 0; i < max+1; i++){
			histograma[i] = 0;
		}
	}

	if (min < 0)
		min *= -1;

	for (int z = 0; z < size[2]; z++) {
		for (int y = 0; y < size[1]; y++) {
			for (int x = 0; x < size[0]; x++) {
				ImageType3D::IndexType position;
				position[0] = x;
				position[1] = y;
				position[2] = z;
				int value = (int)image->GetPixel(position);
				//todos os voxels começam de 0
				if ((value + min) < 0 || value > sizeHistograma) {
					cout << "DEU MERDA:" << value << endl;
				}

				if (minComparacao >= 0) {
					histograma[value]++;

				}
				else {
					histograma[value + min]++;

				}

			}
		}
	}
	vector <int> histogramaVector;
	for (int i = 0; i < sizeHistograma; i++) {
		histogramaVector.push_back(histograma[i]);
	}
	int contSpecies = 0;
	int min2 = 99999;
	//system("cls");
	//cout<<"Especies->";
	for (int i = 0; i < sizeHistograma; i++)
		if (minComparacao<0) {
			if ((histogramaVector.at(i) > 0) && (i != (limite+min) )) {
				contSpecies++;
				//cout<<(i-min)<<" ";
				if ((i - min) < min2)
					min2 = (i - min);
			}
		}else if ( (histogramaVector.at(i) > 0) && (i!= limite) ) {
			contSpecies++;
			//cout<<(i-min)<<" ";
			if ((i - min) < min2)
				min2 = (i - min);
		}
		//cout<<"\nNumero Especies->"<<contSpecies<<" Max-> "<<max<<" Min->"<<min2<<endl;
	return contSpecies;
}

ImageType3D::Pointer Arvores::readVolumeMDArvores(string fileName)
{
	itk::GDCMImageIOFactory::RegisterOneFactory();

	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(fileName);
	reader->Update();

	return reader->GetOutput();
}

void Arvores::criaArvore(ImageType3D::Pointer imgOriginal, ImageType3D::Pointer image, float limite, int &position, vector <No> &vectorNos, vector <int> &vetRaiz) {
	Funcoes funcoes1;
	//Mat imgClone  = image.clone();
	
	// Saber se a intensidade é negativa para colocar na árvore
	int negativo = 0;

	typedef itk::ImageDuplicator< ImageType3D > DuplicatorType;
	DuplicatorType::Pointer duplicator = DuplicatorType::New();
	duplicator->SetInputImage(image);
	duplicator->Update();
	ImageType3D::Pointer imgClone = duplicator->GetOutput();
	saveImageA(imgClone, "imgCloneCopiada.nrrd");
	/*cout << "valor de contador Origem: " << md._conta << endl;
	stringstream conta;
	conta << "C:/op/TesteITK/bin/Origem/" << md._conta << ".nrrd";
	saveImageA(imgOriginal, conta.str());*/

	if (md._PassouDireita == 0)
		md._conta = md._conta + 1;

	//saveImageA(imgClone, "imagecLONE_criaArvore.nrrd");
	//funcoes1.saveImage(imgClone,"CriaArvore_imgClone.nrrd");
	
	DuplicatorType::Pointer duplicatorimgOrigem = DuplicatorType::New();
	duplicatorimgOrigem->SetInputImage(imgOriginal);
	duplicatorimgOrigem->Update();
	ImageType3D::Pointer imgOrigem = duplicatorimgOrigem->GetOutput();

	saveImageA(imgOrigem, "imageOrigem_criaArvore.nrrd");
	No no;
	//vector <No> vetorEsq, vetorDir, vetorArvore;
	vector <int> histograma;
	//ImageType3D::IndexType position;
	ImageType3D::SizeType size;
	size = image->GetLargestPossibleRegion().GetSize();

	int lin = size[1];
	int col = size[0];
	//int posicaoAtual = position;
	float limiar = 0, limiarEsq = 0, limiarDir = 0;
	float limiar2 = 0, limiar3 = 0;
	int limiarHistograma = 0;
	int posicaoDoNO = 0;
	int posicaoNoPai = 0;
	//cout << "Deu certo!" << endl;

	Arvores arvoresEpecies;
	//cout << "Gera o threshold da imagem!" << endl;

	typedef itk::ImageDuplicator< ImageType3D > DuplicatorType;
	DuplicatorType::Pointer duplicatorimgEsquerda = DuplicatorType::New();
	duplicatorimgEsquerda->SetInputImage(image);
	duplicatorimgEsquerda->Update();
	ImageType3D::Pointer imgEsquerda = duplicatorimgEsquerda->GetOutput();

	imgEsquerda = funcoes1.geraImageThreshold(imgClone, limite, 0);

	saveImageA(imgEsquerda, "SaidaimageEsquerdaGeradathreshold_criaArvore.nrrd");
	//cout << "Saiu do threshold da imagem!" << endl;
	//saveImageA(imgEsquerda, "Arvores_criaArvore_geraImageThreshold___imgEsquerda.nrrd");
	//funcoes1.saveImage(imgEsquerda,"imgEsquerda.nrrd");
	//stringstream nameImgEsq;
	//nameImgEsq << "F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/6-Arvores-Features/imgEsquerda" << limite << ".png";
	//imwrite(nameImgEsq.str(),imgEsquerda);	
	//Otsu otsuEsq(imgEsquerda);
	//limiarEsq = otsuEsq.OtsuBinariza();	
	//cout << "Limiar: " << limite << " Esquerdo--------------- \n";
	
	int qtdEspecie = getNumberSpecies(imgEsquerda, limite);
	posicaoNoPai = vetRaiz.size() - 1;
	int teste = 9999;
	//cout << "Verificando Quantidade de espécies para entrar no otsu da esquerda... QTD de especies = " << qtdEspecie << endl;
	if (qtdEspecie > 1) {
		//cout << "Entra no otsu!" << endl;
		limiarEsq = wClusteringA(imgEsquerda,2);
		//cout << "Saiu do otsu" << endl;
		//cout << "CriarArvore_limiarEsq: " << limiarEsq << endl;
		//saveImageA(imgEsquerda, "Arvores_criaArvore___imgEsquerda.nrrd");
		cout << "Entra na funcao de remover fundo!" << endl;
		imgEsquerda = funcoes1.removeFundo(imgEsquerda, limite);
		//saveImageA(imgEsquerda, "imgEsquerdaRemoveFundo_criaArvore.nrrd");
		//cout << "Sai da funcao de remover fundo!" << endl;
		//saveImageA(imgEsquerda, "Arvores_criaArvore_removeFundo__imgEsquerda.nrrd");
		//cout << "Entra na funcao de gerar histograma" << endl;
		histograma = funcoes1.getVectorHistograma(imgEsquerda, negativo);
		//cout << "Sai da funcao que gerou o histograma" << endl;
		// quantidade de especies menos (-1) que é o fundo
		//int qtdEspecie = arvoresEpecies.returnQuantidadeEspecies(histograma, limite) ;	
		qtdEspecie = getNumberSpecies(imgEsquerda, limite);
		//cout << "CriarArvore_qtdEspecie: " << qtdEspecie << endl;
		//Monta Subarvore da Esquerda---------------------------------
		//position++;
		
	}
	else {
		limiarEsq = limite;
		histograma = funcoes1.getVectorHistograma(imgEsquerda, negativo);
	}
	
	//if (qtdEspecie > 2 && !verificaRaizUsado(vetRaiz,limiarEsq)) {
	if (qtdEspecie > 2) {
		if (!verificaRaizUsado(vetRaiz, limiarEsq)) {
			vetRaiz.push_back(limiarEsq);
			//posicaoNoPai = vetRaiz.size() - 1;
			position++;
			//this->_pos = position;		// Posicao atual
			//this->_nos.pai = position > 1 ? vetRaiz.at(posicaoNoPai - 1) : 0;	// No pai		
			this->_nos.pai = vetRaiz.at(posicaoNoPai);	// No pai		
			no.pos = 0;

			//if (!verificaRaizUsado(vetRaiz, limiarEsq)) {
				//vetRaiz.push_back(limiarEsq);
				//posicaoNoPai = vetRaiz.size() - 1;
			this->_pos = position;		// Posicao atual
			//this->_nos.pai = position > 1 ? vetRaiz.at(posicaoNoPai -1 ) : 0;	// No pai

			if (limiarEsq > getMinimumA(imgEsquerda)) { //&& !verificaNoUsado(vetRaiz, limiarEsq)){	

				//*************** ver a posicao do vetor de raiz esta vindo o pai errado*******************
				no.pos = _pos;
				//no.pai = _nos.pai;
				no.pai = limite;
				no.esp = limiarEsq;
				no.ind = arvoresEpecies.returnQuantidadePixelsImage(imgOrigem, limiarEsq);
				no.esq = 0;
				no.dir = 0;
				if (!verificaNoUsado(vectorNos, limiarEsq)) {
					vectorNos.push_back(no);
					vectorNos.at(posicaoNoPai).esq = limiarEsq;
				}

				if (no.pos-2>=0) {
					if (no.pai==vectorNos.at(no.pos-2).esp) {
						if (vectorNos.at(no.pos - 2).esp>no.esp) {
							vectorNos.at(no.pos - 2).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 2).esp<no.esp) {
							vectorNos.at(no.pos - 2).dir = no.esp;
						}
					}
					
				
				}
				if (no.pos-1>=0) {
					if (no.pai == vectorNos.at(no.pos - 1).esp) {
						if (vectorNos.at(no.pos - 1).esp>no.esp) {
							vectorNos.at(no.pos - 1).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 1).esp<no.esp) {
							vectorNos.at(no.pos - 1).dir = no.esp;
						}
					}
				}

				posicaoDoNO = vectorNos.size();

				Arvores arvoreEsq;
				//cout << "Chama a funcao de cria arvore da imagem da Esquerda!" << endl;
				/*saveImageA(imgOrigem,"imgOrigemAntesCHamarCriaarvore.nrrd");
				saveImageA(imgEsquerda, "imgEsquerdaAntesCHamarCriaarvore.nrrd");*/
				arvoreEsq.criaArvore(imgOrigem, imgEsquerda, limiarEsq, _pos, vectorNos, vetRaiz);
				arvoreEsq.~Arvores();
				//imgEsquerda.release();

			}
			else {
				/*no.pos = _pos;
				no.pai = vectorNos.at(posicaoNoPai - 1).esp;
				no.esp = limite;
				no.ind = arvoresEpecies.returnQuantidadePixelsImage(imgOrigem, limite);*/
				no.esq = 0;
				no.dir = 0;
				if (!verificaNoUsado(vectorNos, limite)) {
					vectorNos.push_back(no);
				}
				if (no.pos - 2 >= 0) {
					if (no.pai == vectorNos.at(no.pos - 2).esp) {
						if (vectorNos.at(no.pos - 2).esp>no.esp) {
							vectorNos.at(no.pos - 2).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 2).esp<no.esp) {
							vectorNos.at(no.pos - 2).dir = no.esp;
						}
					}


				}
				if (no.pos - 1 >= 0) {
					if (no.pai == vectorNos.at(no.pos - 1).esp) {
						if (vectorNos.at(no.pos - 1).esp>no.esp) {
							vectorNos.at(no.pos - 1).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 1).esp<no.esp) {
							vectorNos.at(no.pos - 1).dir = no.esp;
						}
					}
				}

				//posicaoDoNO = vectorNos.size();										
			}

			//} 
		}

	}
	else {

		if (qtdEspecie > 0) {
			position++;
			this->_pos = position;		// Posicao atual
			//this->_nos.pai = vetRaiz.back();
			this->_nos.pai = limiarEsq;
			int posicaoAtualNo = vetRaiz.size() > 0 ? vetRaiz.size() - 1 : 0;
			int fundoEsquerda = getMinimumA(imgEsquerda);
			// pixels do histograma maior que 0 individuos.   retorna as intensidades do histograma
			vector <int> vetorPixelsHistograma = arvoresEpecies.returnPixelsHistograma(histograma, fundoEsquerda);
			//cout << "Linha 269 - Tamanho do arvoresEpecies.returnPixelsHistograma(histograma) : " << vetorPixelsHistograma.size() << endl;
			
			int fundoNegativo = 0;
			if (!verificaRaizUsado(vetRaiz, _nos.pai)) {

				vectorNos.at(posicaoAtualNo).esq = _nos.pai;
				no.pos = _pos;
				no.pai = vetRaiz.at(posicaoAtualNo);
				no.esp = _nos.pai;
				no.esq = 0;
				no.dir = 0;
				//no.ind = funcoes1.returnQuantidadePixelMesmoValor(imgOrigem,_nos.esp); modifica 
				no.ind = funcoes1.returnQuantidadePixelMesmoValor(imgOrigem, no.esp);
				//cout << "Quantidade pixels mesmo valor : " << no.ind << endl;

				vetRaiz.push_back(_nos.pai);
				vectorNos.push_back(no);
				posicaoAtualNo = vetRaiz.size() - 1;

				// TENHO QUE DIMINUIR MENOS O VALOR DA INTENSIDADE DO FUNDO

				if (vetorPixelsHistograma.size() > 2) {
					if (fundoEsquerda<0) {
						fundoEsquerda = fundoEsquerda*-1;
						fundoNegativo = 1;
					}

					if(fundoNegativo){
						vectorNos.at(posicaoAtualNo).esq = (vetorPixelsHistograma.at(1));
						fundoEsquerda = fundoEsquerda*-1;
						
					}else {
						vectorNos.at(posicaoAtualNo).esq = vetorPixelsHistograma.at(1);
					}

					if (fundoNegativo) {
						vectorNos.at(posicaoAtualNo).dir = (vetorPixelsHistograma.at(2));
						fundoEsquerda = fundoEsquerda*-1;
						
					}else {
						vectorNos.at(posicaoAtualNo).dir = vetorPixelsHistograma.at(2);
					}

				}
				else {

					if (fundoEsquerda<0) {
						fundoEsquerda = fundoEsquerda*-1;
						fundoNegativo = 1;
					}

					if (fundoNegativo) {
						vectorNos.at(posicaoAtualNo).esq = (vetorPixelsHistograma.at(1));
						fundoEsquerda = fundoEsquerda*-1;
					}else {
						vectorNos.at(posicaoAtualNo).esq = vetorPixelsHistograma.at(1);
					}
					//FOI AQUI (0)
					/*vectorNos.at(posicaoAtualNo).esq = vetorPixelsHistograma.at(1);*/
					//vectorNos.at(posicaoAtualNo).dir = vetorPixelsHistograma.at(2);
				}

				if (no.pos - 2 >= 0) {
					if (no.pai == vectorNos.at(no.pos - 2).esp) {
						if (vectorNos.at(no.pos - 2).esp>no.esp) {
							vectorNos.at(no.pos - 2).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 2).esp<no.esp) {
							vectorNos.at(no.pos - 2).dir = no.esp;
						}
					}
				}
				if (no.pos - 1 >= 0) {
					if (no.pai == vectorNos.at(no.pos - 1).esp) {
						if (vectorNos.at(no.pos - 1).esp>no.esp) {
							vectorNos.at(no.pos - 1).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 1).esp<no.esp) {
							vectorNos.at(no.pos - 1).dir = no.esp;
						}
					}
				}


			}
			else {
				/*int valorVerifica;
				if (vetorPixelsHistograma.size() == 1) {

					if (fundoEsquerda < 0) {
						fundoEsquerda = fundoEsquerda*-1;
						fundoNegativo = 1;
					}
					if (fundoNegativo) {
						if (vetorPixelsHistograma.at(0) < fundoEsquerda) {
							vectorNos.at(posicaoAtualNo).esq = (vetorPixelsHistograma.at(1)*-1) - fundoEsquerda;
							fundoEsquerda = fundoEsquerda*-1;
						}

					}else {
							vectorNos.at(posicaoAtualNo).esq = vetorPixelsHistograma.at(0);
						}
				}*/

				if (vetorPixelsHistograma.size() == 1) {

					if (fundoEsquerda < 0) {
						fundoEsquerda = fundoEsquerda*-1;
						fundoNegativo = 1;
					}

					if (fundoNegativo) {
						if ( ((vetorPixelsHistograma.at(0))) <vectorNos.at(posicaoNoPai).esp) {
							vectorNos.at(posicaoNoPai).esq = ((vetorPixelsHistograma.at(0)));
							fundoEsquerda = fundoEsquerda*-1;
						}
						else if ( ( ((vetorPixelsHistograma.at(0))) ) > vectorNos.at(posicaoNoPai).esp) {
							vectorNos.at(posicaoNoPai).dir = ((vetorPixelsHistograma.at(0)));
							fundoEsquerda = fundoEsquerda*-1;
						}
					
					}else {
						if (vetorPixelsHistograma.at(0)<vectorNos.at(posicaoNoPai).esp) {
							vectorNos.at(posicaoNoPai).esq = vetorPixelsHistograma.at(0);
						}
						else if (vetorPixelsHistograma.at(0) > vectorNos.at(posicaoNoPai).esp) {
							vectorNos.at(posicaoNoPai).dir = vetorPixelsHistograma.at(0);
						}
					}
				
				}else if (vetorPixelsHistograma.size() > 1) {

					if (fundoEsquerda < 0) {
						fundoEsquerda = fundoEsquerda*-1;
						fundoNegativo = 1;
					}

					if (fundoNegativo) {
						if ( ((vetorPixelsHistograma.at(1))) <vectorNos.at(posicaoNoPai).esp) {
							vectorNos.at(posicaoNoPai).esq = ((vetorPixelsHistograma.at(1)));
							fundoEsquerda = fundoEsquerda*-1;
						}
						else if ( ( ((vetorPixelsHistograma.at(1))) ) > vectorNos.at(posicaoNoPai).esp) {
							vectorNos.at(posicaoNoPai).dir = ((vetorPixelsHistograma.at(1)));
							fundoEsquerda = fundoEsquerda*-1;
						}
					
					}else {
						if (vetorPixelsHistograma.at(1)<vectorNos.at(posicaoNoPai).esp) {
							vectorNos.at(posicaoNoPai).esq = vetorPixelsHistograma.at(1);
						}
						else if (vetorPixelsHistograma.at(1) > vectorNos.at(posicaoNoPai).esp) {
							vectorNos.at(posicaoNoPai).dir = vetorPixelsHistograma.at(1);
						}
					}
				}
				/*if (no.pos - 2 >= 0) {
					if (no.pai == vectorNos.at(no.pos - 2).esp) {
						if (vectorNos.at(no.pos - 2).esp>no.esp) {
							vectorNos.at(no.pos - 2).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 2).esp<no.esp) {
							vectorNos.at(no.pos - 2).dir = no.esp;
						}
					}


				}
				if (no.pos - 1 >= 0) {
					if (no.pai == vectorNos.at(no.pos - 1).esp) {
						if (vectorNos.at(no.pos - 1).esp>no.esp) {
							vectorNos.at(no.pos - 1).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 1).esp<no.esp) {
							vectorNos.at(no.pos - 1).dir = no.esp;
						}
					}
				}*/

				//vectorNos.at(posicaoNoPai).esq = vetorPixelsHistograma.size() == 1 ? vetorPixelsHistograma.at(0) : vetorPixelsHistograma.at(1);
				position--;
			}
			fundoNegativo = 0;
			int valorminimoImagemEsquerda = getMaximumA(imgEsquerda);
			for (int i = 0; i < vetorPixelsHistograma.size(); i++) {
				if (valorminimoImagemEsquerda<0) {
					valorminimoImagemEsquerda = valorminimoImagemEsquerda*-1;
					fundoNegativo = 1;
				}
				int valorDoindividuo = vetorPixelsHistograma.at(i);
				if (!verificaRaizUsado(vetRaiz, (vetorPixelsHistograma[i])) && (vetorPixelsHistograma[i]) != valorminimoImagemEsquerda) {
					if (fundoNegativo) {
						valorDoindividuo = vetorPixelsHistograma.at(i);
						valorminimoImagemEsquerda = valorminimoImagemEsquerda*-1;
					}
					int individo = valorDoindividuo;
					int qtdIndivido = returnQuantidadePixelsImage(imgOrigem, individo);
					//cout << "Quantidade de individuos daquele valor: " << endl;
					vetRaiz.push_back(individo);

					position++;
					this->_pos = position;		// Posicao atual
					no.pos = _pos;
					no.pai = _nos.pai;
					no.esp = individo;
					no.ind = qtdIndivido;
					no.esq = 0;
					no.dir = 0;
					vectorNos.push_back(no);

					if (no.pos - 2 >= 0) {
						if (no.pai == vectorNos.at(no.pos - 2).esp) {
							if (vectorNos.at(no.pos - 2).esp>no.esp) {
								vectorNos.at(no.pos - 2).esq = no.esp;
							}
							else if (vectorNos.at(no.pos - 2).esp<no.esp) {
								vectorNos.at(no.pos - 2).dir = no.esp;
							}
						}


					}
					if (no.pos - 1 >= 0) {
						if (no.pai == vectorNos.at(no.pos - 1).esp) {
							if (vectorNos.at(no.pos - 1).esp>no.esp) {
								vectorNos.at(no.pos - 1).esq = no.esp;
							}
							else if (vectorNos.at(no.pos - 1).esp<no.esp) {
								vectorNos.at(no.pos - 1).dir = no.esp;
							}
						}
					}


				}
			}
			vetorPixelsHistograma.clear();
		}
	}

	//imgEsquerda.release();

	//Monta Subarvore da Direita -----------------------------------
	//cout << "Limiar: " << limite << " --------------- \n";
	//histograma = funcoes1.returnHistogramaImage(imgClone);

	//cout << "Entrou na imagem para gerar o lado direito" << endl;
	//saveImageA(imgOrigem, "imgDireitaPegandoOrigem.nrrd");

	typedef itk::ImageDuplicator< ImageType3D > DuplicatorType;
	DuplicatorType::Pointer duplicatorimgDireita = DuplicatorType::New();
	duplicatorimgDireita->SetInputImage(image);
	duplicatorimgDireita->Update();
	ImageType3D::Pointer imgDireita = duplicatorimgDireita->GetOutput();

	
    //### PROBLEMA DA RECURSIVIDADE
	/*
	if (md._conta > 1){
		// Lendo o path das imagens recursivas
		stringstream pathRecusivoDireita;
	pathRecusivoDireita << "C:/op/TesteITK/bin/Origem/";
	vector <string> imagensDireita;
	vector <string> imagensDireitaOrdenado;
	String pathRecusivoDireita2 = pathRecusivoDireita.str();
	imagensDireita = funcoes1.returnVetorArquivos(pathRecusivoDireita2.c_str());
	cout << "\n" << "Tamanho do vetor de Imagens da Direita: " << imagensDireita.size() << endl;

	for (int i = 0; i < imagensDireita.size(); i++) {
		stringstream pathTeste;
		pathTeste << pathRecusivoDireita.str() << i + 1 << ".nrrd";
		imagensDireitaOrdenado.push_back(pathTeste.str());
		cout << pathTeste.str() << endl;
	}

	vector <vector <string>> vetorPathRecusivoDireita;
	vetorPathRecusivoDireita.push_back(imagensDireitaOrdenado);
	cout << "\n" << "Passo1" << endl;
	vector <string> vetorDeArquivosDireita = vetorPathRecusivoDireita.back();
	cout << "\n" << "Passo2" << endl;
	stringstream pathVolumeDadireita;
	cout << "\n" << "Tamanho do arquivo da direita: " << vetorDeArquivosDireita.size() << endl;
	pathVolumeDadireita << vetorDeArquivosDireita.at((md._conta - 2));
	/*for (int i = 0;i<vetorDeArquivosDireita.size();i++) {
		stringstream pathTeste;
		pathTeste << vetorDeArquivosDireita.at(i);
		cout << pathTeste.str() << endl;
	}
	cout << "\n" << "Nome do arquivo da direita que está sendo lido: " << pathVolumeDadireita.str() << endl;
	imgDireita = readVolumeMDArvores(pathVolumeDadireita.str());
	md._conta = md._conta - 1;
	*/
	// ##############  FIM DO PROBLEMA DA RECURSIVIDADE
	saveImageA(imgClone, "ImgClone.nrrd");
	// criando os nós do lado direito
	imgDireita = funcoes1.geraImageThreshold(imgClone, limite, 1);
	saveImageA(imgDireita, "imgDireitageraImageThreshold.nrrd");
	//imgDireita = funcoes1.geraImageThreshold(imgOriginal,limite,1);

	//cout << "Saiu da imagem que gera o lado direito (gera image THreshold)" << endl;
	stringstream nameImgDir;
	//nameImgDir << "F:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/6-Arvores-Features/imgDireita" << limite << ".png";
	//imwrite(nameImgDir.str(),imgDireita);
	//Otsu otsuDir(imgDireita);
	//limiarDir = otsuDir.OtsuBinariza();	
	qtdEspecie = getNumberSpecies(imgDireita, limite);
	//cout << "CriarArvore_qtdEspecie: " << qtdEspecie << endl;

	//cout << "Limiar: " << limite << "Direito--------------- \n";
	
	md._PassouDireita = 1;
	if (qtdEspecie > 1) {
		//cout << "Entra no otsu da iamgem da direita" << endl;
		//saveImageA(imgDireita,"imgDireitaAntesDeEntrarNoOtsu.nrrd");
		//saveImageA(imgDireita, "imgDireitaAntesDeEntrarNoOtsu.nrrd");
		
		limiarDir = wClusteringA(imgDireita, 2);
		cout << "Saiu dele" << endl;
	
	imgDireita = funcoes1.removeFundo(imgDireita,limite);
	
	histograma = funcoes1.getVectorHistograma(imgDireita, negativo);

	qtdEspecie = arvoresEpecies.returnQuantidadeEspecies(histograma, limite);
	}
	else if (qtdEspecie == 1) {
		limiarDir = limite;
		histograma = funcoes1.getVectorHistograma(imgDireita, negativo);
	}

	int fundoDireita = getMinimumA(imgDireita);
	int fundoDireitaNegativo = 0;
	//if (qtdEspecie > 2 && !verificaRaizUsado(vetRaiz,limiarDir)) {	
	/*} else*/ if (qtdEspecie >= 2 ){
		if (!verificaRaizUsado(vetRaiz,limiarDir)) {
		
			vetRaiz.push_back(limiarDir);
			position++;

			if (limiarDir > getMinimumA(imgDireita)){
						
				//vectorNos.at(posicaoNoPai - 1).dir = limiarDir;
				//vectorNos.at(posicaoNoPai - 1).esq = limiarEsq;
				//position++;
				//this->_pos = vectorNos.size() + 1;
				this->_pos = position;
				no.pos = _pos;
				//no.pai = vectorNos.at(posicaoNoPai).esp;				
				no.pai = limite;				
				no.esp = limiarDir;
				no.dir = 0;
				no.esq = 0;
				no.ind = arvoresEpecies.returnQuantidadePixelsImage(imgOrigem, limiarDir);

				if (!verificaNoUsado(vectorNos,no.esp)){
					vectorNos.push_back(no);
					//posicaoNoPai++;
					if (limiarDir>no.pai) {
						vectorNos.at(posicaoNoPai).dir = limiarDir;
					}
				}				

				if (no.pos - 2 >= 0) {
					if (no.pai == vectorNos.at(no.pos - 2).esp) {
						if (vectorNos.at(no.pos - 2).esp>no.esp) {
							vectorNos.at(no.pos - 2).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 2).esp<no.esp) {
							vectorNos.at(no.pos - 2).dir = no.esp;
						}
					}


				}
				if (no.pos - 1 >= 0) {
					if (no.pai == vectorNos.at(no.pos - 1).esp) {
						if (vectorNos.at(no.pos - 1).esp>no.esp) {
							vectorNos.at(no.pos - 1).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 1).esp<no.esp) {
							vectorNos.at(no.pos - 1).dir = no.esp;
						}
					}
				}


				Arvores arvoreDir;
				//cout << "Chama a funcao de criar arvore da imagem da direita" << endl;
				arvoreDir.criaArvore(imgOrigem, imgDireita,limiarDir,_pos,vectorNos, vetRaiz);	
				arvoreDir.~Arvores();
				//imgDireita.release();

		
			} else {
									
				vectorNos.at(posicaoNoPai).dir = 0;		
			
			}				
		}
	} else {

		if (qtdEspecie > 0) {
			position++;
			this->_pos = position;		// Posicao atual
			//this->_nos.pai = vetRaiz.back();
			this->_nos.pai = limiarDir;
		
			int posicaoAtualNo = vetRaiz.size() > 0 ? vetRaiz.size() - 1 : 0;

			vector <int> vetorPixelsHistograma = arvoresEpecies.returnPixelsHistograma(histograma, fundoDireita);

			if (!verificaRaizUsado(vetRaiz, _nos.pai)) {

				vectorNos.at(posicaoAtualNo).dir = _nos.pai;
				no.pos = _pos;
				no.pai = vetRaiz.at(posicaoAtualNo);
				no.esp = _nos.pai;
				no.esq = 0;
				no.dir = 0;
				//no.ind = funcoes1.returnQuantidadePixelMesmoValor(imgOrigem,_nos.esp);
				no.ind = funcoes1.returnQuantidadePixelMesmoValor(imgOrigem, no.esp);
		
				vetRaiz.push_back(_nos.pai);			
				vectorNos.push_back(no);
				posicaoAtualNo = vetRaiz.size() - 1;

					if (vetorPixelsHistograma.size() > 2 ){
						if (fundoDireita<0) {
							fundoDireita = fundoDireita*-1;
							fundoDireitaNegativo = 1;
						}

						if (fundoDireitaNegativo) {
							vectorNos.at(posicaoAtualNo).esq = (vetorPixelsHistograma.at(1));
							vectorNos.at(posicaoAtualNo).dir = (vetorPixelsHistograma.at(2));
							fundoDireita = fundoDireita*-1;

						}
						else {
							vectorNos.at(posicaoAtualNo).esq = vetorPixelsHistograma.at(1);
							vectorNos.at(posicaoAtualNo).dir = vetorPixelsHistograma.at(2);
						}
						
					} else {
						if (fundoDireita<0) {
							fundoDireita = fundoDireita*-1;
							fundoDireitaNegativo = 1;
						}

						if (fundoDireitaNegativo) {
							vectorNos.at(posicaoAtualNo).dir = vetorPixelsHistograma.at(1);
							fundoDireita = fundoDireita*-1;
						}else {
							vectorNos.at(posicaoAtualNo).dir = vetorPixelsHistograma.at(1);
						}
						
						//vectorNos.at(posicaoAtualNo).dir = vetorPixelsHistograma.at(2);
					}
				
				if (no.pos - 2 >= 0) {
					if (no.pai == vectorNos.at(no.pos - 2).esp) {
						if (vectorNos.at(no.pos - 2).esp>no.esp) {
							vectorNos.at(no.pos - 2).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 2).esp<no.esp) {
							vectorNos.at(no.pos - 2).dir = no.esp;
						}
					}


				}
				if (no.pos - 1 >= 0) {
					if (no.pai == vectorNos.at(no.pos - 1).esp) {
						if (vectorNos.at(no.pos - 1).esp>no.esp) {
							vectorNos.at(no.pos - 1).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 1).esp<no.esp) {
							vectorNos.at(no.pos - 1).dir = no.esp;
						}
					}
				}


			} else {
				// FALTA ESSE
				if (fundoDireita<0) {
					fundoDireita = fundoDireita*-1;
					fundoDireitaNegativo = 1;
				}

				if (fundoDireitaNegativo) {
					if (vetorPixelsHistograma.size() == 1) {
						vectorNos.at(posicaoNoPai).dir = vetorPixelsHistograma.at(0);
						fundoDireita = fundoDireita * -1;
					}else {
						vectorNos.at(posicaoNoPai).dir = vetorPixelsHistograma.at(1);
						fundoDireita = fundoDireita * -1;
					}

				}else {
					vectorNos.at(posicaoNoPai).dir = vetorPixelsHistograma.size() == 1 ? vetorPixelsHistograma.at(0) : vetorPixelsHistograma.at(1);
				}
				

				/*if (no.pos - 2 >= 0) {
					if (no.pai == vectorNos.at(no.pos - 2).esp) {
						if (vectorNos.at(no.pos - 2).esp>no.esp) {
							vectorNos.at(no.pos - 2).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 2).esp<no.esp) {
							vectorNos.at(no.pos - 2).dir = no.esp;
						}
					}


				}
				if (no.pos - 1 >= 0) {
					if (no.pai == vectorNos.at(no.pos - 1).esp) {
						if (vectorNos.at(no.pos - 1).esp>no.esp) {
							vectorNos.at(no.pos - 1).esq = no.esp;
						}
						else if (vectorNos.at(no.pos - 1).esp<no.esp) {
							vectorNos.at(no.pos - 1).dir = no.esp;
						}
					}
				}*/


				position--;
			}
				


			/*
			
			if (valorFundoImagemDireita<0) {
					fundoDireitaNegativo = 1;
				}
				int valorDoindividuo = vetorPixelsHistograma.at(i);
				if (!verificaRaizUsado(vetRaiz, vetorPixelsHistograma[i]) && vetorPixelsHistograma[i] != valorFundoImagemDireita) {
					if (fundoDireitaNegativo) {
						valorFundoImagemDireita = valorFundoImagemDireita*-1;
						valorDoindividuo = vetorPixelsHistograma.at(i) - valorFundoImagemDireita;
						valorFundoImagemDireita = valorFundoImagemDireita*-1;
					}
					int individo = valorDoindividuo;
			
			
			*/

			int valorFundoImagemDireita = getMinimumA(imgDireita);
			
			for (int i=0; i < vetorPixelsHistograma.size(); i++){
			
				if (valorFundoImagemDireita<0) {
					fundoDireitaNegativo = 1;
				}
				int valorDoindividuo = vetorPixelsHistograma.at(i);
				if (!verificaRaizUsado(vetRaiz, (vetorPixelsHistograma[i])) && (vetorPixelsHistograma[i] ) != valorFundoImagemDireita) {
					if (fundoDireitaNegativo) {
						valorFundoImagemDireita = valorFundoImagemDireita*-1;
						valorDoindividuo = vetorPixelsHistograma.at(i);
						valorFundoImagemDireita = valorFundoImagemDireita*-1;
					}
					int individo = valorDoindividuo;
					int qtdIndivido = returnQuantidadePixelsImage(imgOrigem,individo);
					vetRaiz.push_back(individo);	
				
					position++;
					this->_pos = position;		// Posicao atual

					no.pos = _pos;
					no.pai = _nos.pai;
					no.esp = individo;
					no.ind = qtdIndivido;
					no.esq = 0;
					no.dir = 0;		
					vectorNos.push_back(no);
					
					if (no.pos - 2 >= 0) {
						if (no.pai == vectorNos.at(no.pos - 2).esp) {
							if (vectorNos.at(no.pos - 2).esp>no.esp) {
								vectorNos.at(no.pos - 2).esq = no.esp;
							}
							else if (vectorNos.at(no.pos - 2).esp<no.esp) {
								vectorNos.at(no.pos - 2).dir = no.esp;
							}
						}


					}
					if (no.pos - 1 >= 0) {
						if (no.pai == vectorNos.at(no.pos - 1).esp) {
							if (vectorNos.at(no.pos - 1).esp>no.esp) {
								vectorNos.at(no.pos - 1).esq = no.esp;
							}
							else if (vectorNos.at(no.pos - 1).esp<no.esp) {
								vectorNos.at(no.pos - 1).dir = no.esp;
							}
						}
					}

				
				}
			}		
			vetorPixelsHistograma.clear();
		}		
	}

	//imgEsquerda.release();
	//imgEsquerda = nullptr;
	//imgDireita.release();
	//imgDireita= nullptr;
	//imgClone.release();
	//imgClone	= nullptr;
	//imgOrigem.release();
	//histograma.clear();
	//image.release();
	//image = nullptr;
	
	funcoes1.~Funcoes();
	arvoresEpecies.~Arvores();
	//imgOriginal.release();
	//imgOriginal = nullptr;
	
	//int x=0;
	//for 
	//return vetorNos;
}

/*
int Util::getNumberSpecies(ImageType3D::Pointer image){

ImageType3D::SizeType size = image->GetLargestPossibleRegion().GetSize();
int background = getMinimum(image);
int min, max;
min = getMinimum(image);
max = getMaximum(image);
int sizeHistograma = max-min+1;

int *histograma = new int[sizeHistograma];

for(int i = 0; i < sizeHistograma; i++)
histograma[i] = 0;

if (min < 0 )
min *= -1;

for(int z = 0; z < size[2]; z++){
for(int y = 0; y < size[1]; y++){
for(int x = 0; x < size[0]; x++){
ImageType3D::IndexType position;
position[0] = x;
position[1] = y;
position[2] = z;

int value = (int)image->GetPixel(position);
//todos os voxels começam de 0
if (value != background){
if ((value + min) < 0 || value > sizeHistograma)
cout << "DEU MERDA: " << value << endl;
histograma[ value + min ]++;
}
}
}
}
int contSpecies = 0;
int min2 = 99999;
//system("cls");
//cout<<"Especies->";
for(int i = 0; i < sizeHistograma; i++)
if (histograma[i] > 0){
contSpecies++;
//cout<<(i-min)<<" ";
if ((i-min) < min2)
min2 = (i-min);
}
//cout<<"\nNumero Especies->"<<contSpecies<<" Max-> "<<max<<" Min->"<<min2<<endl;
return contSpecies;
}
*/

// Retorna a quantidade de especies do histograma-----------------------------
int Arvores::returnQuantidadeEspecies(vector <int> histograma, int pixelFundo){

	
	int qtdEspecies = 0;
	
	for(int i=1 ; i < histograma.size(); i++){
		//cout<< "histograma gerado: " <<histograma.at(i) << "e posição: "<< i <<endl;
		if (histograma.at(i) > 0 && i != pixelFundo)
			qtdEspecies++;	
	}

	return qtdEspecies;

}

// Encontra um vetor de pixel > 0 apartir do hitograma
vector <int> Arvores::returnPixelsHistograma(vector <int> histograma, int fundo){
		
	vector <int> vetorPixel;

	for (int i=0; i < histograma.size(); i++){
		
		if (fundo<0) {
			if (i==0) {
				if (histograma.at(i) > 0) {
					vetorPixel.push_back(((i*-1)+fundo));
				}
			}else {
				if (histograma.at(i) > 0) {
					vetorPixel.push_back((i + fundo));
				}
			}
		}else if (histograma.at(i) > 0){
			vetorPixel.push_back(i);			
		}

	}

	return vetorPixel;
}

// Retorna a quatidade de individos (pixels) apartir do hitograma
int Arvores::returnQuantidadePixelsHistograma(vector <int> histograma, int individo){
		
	int qtdIndividos;

	for (int i=0; i < histograma.size(); i++){
		if ( i == individo){
			qtdIndividos = histograma.at(i);			
		}

	}

	return qtdIndividos;
}

// Verifica se o no Raiz foi usado
bool Arvores::verificaRaizUsado(vector <int> vetorDeNos, int no){
		
	for (int i=0; i < vetorDeNos.size(); i++){
		if ( vetorDeNos[i] == no){
			return true;			
		}

	}

	return false;
}

// Verifica se o no foi usado
bool Arvores::verificaNoUsado(vector <No> vetorDeNos, int no){
		
	for (int i=0; i < vetorDeNos.size(); i++){
		if ( vetorDeNos.at(i).esp == no){
			return true;			
		}

	}

	return false;
}

//Retorna a quantida de eletemtos de um pixels da imagem
int Arvores::returnQuantidadePixelsImage(ImageType3D::Pointer img, int pixel){
	ImageType3D::SizeType size;
	size = img->GetLargestPossibleRegion().GetSize();
    int lin = size[1];
	int col = size[0];
	int qtdPixels=0;
	for (int z = 0; z < size[2]; z++){
		for (int i = 0; i < lin; i++)  {
			for (int j = 0; j < col; j++) {
				ImageType3D::IndexType position;
				position[0] = j;
				position[1] = i;
				position[2] = z;
				if ((int)img->GetPixel(position) == pixel) {
					qtdPixels++;
					 
				}
			}
		}
	}
	//cout<< "Quantidade Pixels Arvores_returnQuantidadePixelsImage_ : " << qtdPixels << endl;
	return qtdPixels;
}
