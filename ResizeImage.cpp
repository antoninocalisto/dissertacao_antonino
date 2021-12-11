#include "ResizeImage.h"

ResizeImage::ResizeImage(string pathOrigem, string pathDestino, int altura){
	
	_altura = altura;	
	_path = pathOrigem;
	
	DIR *dir1;	

    struct dirent *lsdir1;

	dir1 = opendir(_path.c_str());	//path Massas	

	int processar = 0;
	int QTD_IMAGENS = 99999999999; 

    //Extrai as caracteristicas Massa
	while ( ( lsdir1 = readdir(dir1) ) != NULL && processar < QTD_IMAGENS) {

		if( strcmp(lsdir1->d_name, ".") != 0 && strcmp(lsdir1->d_name, "..") != 0 ){
			processar++;
			
			printf ("%s\n", lsdir1->d_name);
			_imageName = lsdir1->d_name;
			string arquivoImagem = _path+_imageName;
			//cout << imagemImagem << endl;			

			Mat imgImagem = imread(arquivoImagem, CV_LOAD_IMAGE_GRAYSCALE);
			//PathSystem pathDestino;		
			//pathDestino = pathDestino._pathRedimensionada + 
			//string pathSaida = pathDestino._pathResize+_imageName;
			string pathSaida = pathDestino + _imageName;
			ResizeImage imgResize(pathSaida, imgImagem, altura);			

		}
	}

}

ResizeImage::ResizeImage(string path, Mat &img, int altura){

	_altura = altura;
	_imagemResize;
	_path = path;

	//Mat imgClone = img.clone();

	int lin = img.rows;
	int col = img.cols;

	//Redimensionar a imagem caso seja maior que 1024
	if (col > _altura){
		Size t;
		t.height=altura;
		t.width=col/((float)lin/t.height);
		resize(img,_imagemResize,t); // imgMod é a imagem redimensionada para altura de 512 x proporcional 
	}

	
	imwrite(path,_imagemResize);

}


ResizeImage::~ResizeImage(void){


}


