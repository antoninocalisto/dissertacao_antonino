#ifndef UTIL_H_
#define UTIL_H_

#include "header.h"

class Util
{
	ImageType3D::Pointer imagePrivate;
	vector<ImageType3D::Pointer> *regionNormal;
	int limiar;
public:
	
	Util();
	Util(ImageType3D::Pointer image);
	~Util();

	//read and write Image
	ImageType3D::Pointer readerImage(string file);
	void saveImage(ImageType3D::Pointer image, string path);
	//cria imagens para o processamento
	ImageType3D::Pointer createImage(ImageType3D::Pointer image);

	ImageType3D::Pointer getImageTextureOriginal(ImageType3D::Pointer original, ImageType3D::Pointer otsu );
	
	//filtros
	ImageType3D::Pointer equalizacaoHistograma(ImageType3D::Pointer image);
	int getMinimum(ImageType3D::Pointer image);
	int getMaximum(ImageType3D::Pointer image);
	ImageType3D::Pointer gassuianFilter(ImageType3D::Pointer image, double variance);
	ImageType3D::Pointer floatToshort(ImageTypeFloat::Pointer imgShort);
	
	//OTSU
	ImageType3D::Pointer otsuClustering(ImageType3D::Pointer image, int numberOfThresholds, bool);
	bool verificaEspecies(ImageType3D::Pointer voi);
	ImageType3D::Pointer wClustering(ImageType3D::Pointer image, int numCluster, bool _arq);

	//svm-file generated
	void geraFileSVM(int rotulo, vector<long double> features, string savePath);
	vector<ImageType3D::Pointer> getRegionsOtsuClustering(ImageType3D::Pointer image, int _nCluster, bool);
	
	//funcoes para ciacao da arvore
	//obtem o numero de especies da imagem
	int getNumberSpecies(ImageType3D::Pointer);
	bool existeElemento(vector<especie>, int);
	int getNumIndividuos(ImageType3D::Pointer, int);
	int getLimiarOtsu();
	void getAllRegions(ImageType3D::Pointer imageClusterizacao, ImageType3D::Pointer imageOriginal);
	vector<ImageType3D::Pointer> *getRegions();
	vector<int> getUnicaEspecie(ImageType3D::Pointer image);
	vector<especie> getArvoresFromTxt(string path);
	
};
#endif
