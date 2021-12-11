#include "MascaraImagem.h"
#include "MedidasDiversidadeFuncional.h"

MascaraImagem::MascaraImagem(void)
{
}


MascaraImagem::~MascaraImagem(void)
{
}

//Retorna a mascara da borda de uma imagem
Mat MascaraImagem::retornaMascaraBorda(Mat img){

	Mat imgBorda = Mat::zeros(img.rows, img.cols, img.type());
	Funcoes funcoes;

	imgBorda = funcoes.geraBordaImagem(img);
	
	return imgBorda;

}

int MascaraImagem::getMinimumMa(ImageType::Pointer image) {
	typedef MinimumMaximumImageCalculator <ImageType> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMinimum();
}

int MascaraImagem::getMaximumMa(ImageType3D::Pointer image) {
	typedef MinimumMaximumImageCalculator <ImageType3D> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMaximum();
}

ImageType3D::Pointer MascaraImagem::createImageMa(ImageType3D::Pointer image) {

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
	newImage->FillBuffer(getMinimumMa(image));
	return newImage;
}

ImageType3D::Pointer MascaraImagem::retornaMascaraImageLuana(ImageType3D::Pointer img, float scale) {

	ImageType3D::SizeType size = img->GetLargestPossibleRegion().GetSize();
	int cols = size[0];
	int rows = size[1];
	Mat imgOut = Mat::zeros(size[1], size[0], 0); // CV_LOAD_IMAGE_GRAYSCALE = 0
	ImageType3D::Pointer imgOut2 = createImageMa(img);

	MedidasDiversidadeFuncional md;
	//md.saveImage(imgOut2, "ImagemDentroMascara.nrrd");
	//int diferencaLinnha = img.rows - (img.rows * scale);
	//int diferencacoluna = img.cols - (img.cols * scale);

	//Mat imgBinaria = img > 0;
	Mat imgScale;

	//resize(imgBinaria,imgScale, cv::Size(0,0), scale, scale);
	resize(imgOut, imgScale, cv::Size(0, 0), scale, scale);
	int lin = imgScale.rows;
	int col = imgScale.cols;
	double min = getMinimumMa(img);
	/*int diferencaLinnha = (img.rows - lin);
	int diferencacoluna = (img.cols - col);*/
	int diferencaLinnha = (rows - lin);
	int diferencacoluna = (cols - col);
	for (int z = 0; z < size[2]; z++) {
		for (int i = 0; i < lin; i++) {
			for (int j = 0; j < col; j++) {
				ImageType3D::IndexType position;
				position[0] = j;
				position[1] = i;
				position[2] = z;

				ImageType3D::IndexType position2;
				position2[0] = (j + (diferencacoluna / 2));
				position2[1] = (i + (diferencaLinnha / 2));
				position2[2] = z;

				/*if (imgScale.at<uchar>(i, j) > 0) {
				imgOut.at<uchar>(i + (diferencaLinnha / 2), j + (diferencacoluna / 2)) = img.at<uchar>(i + (diferencaLinnha / 2), j + (diferencacoluna / 2));
				}*/
				if (img->GetPixel(position) > min) {
					imgOut2->SetPixel(position2, img->GetPixel(position));
				}
			}
		}
	}

	//geraImageScaled(img, imgScale);

	/*imwrite("f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/imgOriginal.png",img);
	//imwrite("f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/imgBinaria.png",imgBinaria);
	//imwrite("f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/imgScale.png",imgScale);
	//imwrite("f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/imgMascara.png",imgOut);*/
	//md.saveImage(imgOut2,"ImagemCirculoPrimeira.nrrd");
	return imgOut2;

}



//Retorna a mascara de uma imagem de acordo com a escala
ImageType3D::Pointer MascaraImagem::retornaMascaraImage(ImageType3D::Pointer img, float scale){
	
	ImageType3D::SizeType size = img->GetLargestPossibleRegion().GetSize();
	int cols = size[0];
	int rows = size[1];
	Mat imgOut = Mat::zeros(size[1], size[0], 0); // CV_LOAD_IMAGE_GRAYSCALE = 0
	ImageType3D::Pointer imgOut2 = createImageMa(img);
	
	MedidasDiversidadeFuncional md;
	//md.saveImage(imgOut2, "ImagemDentroMascara.nrrd");
	//int diferencaLinnha = img.rows - (img.rows * scale);
	//int diferencacoluna = img.cols - (img.cols * scale);

	//Mat imgBinaria = img > 0;
	Mat imgScale;
	
	//resize(imgBinaria,imgScale, cv::Size(0,0), scale, scale);
	resize(imgOut, imgScale, cv::Size(0, 0), scale, scale);
	int lin = imgScale.rows;
	int col = imgScale.cols;
	double min = getMinimumMa(img);
	/*int diferencaLinnha = (img.rows - lin);
	int diferencacoluna = (img.cols - col);*/
	int diferencaLinnha = (rows - lin);
	int diferencacoluna = (cols - col);
	for (int z = 0; z < size[2]; z++) {
			for (int i = 0; i < lin; i++) {
				for (int j = 0; j < col; j++) {
					ImageType3D::IndexType position;
					position[0] = j;
					position[1] = i;
					position[2] = z;
					
					ImageType3D::IndexType position2;
					position2[0] = (j + (diferencacoluna/2));
					position2[1] = (i + (diferencaLinnha/2));
					position2[2] = z;

					/*if (imgScale.at<uchar>(i, j) > 0) {
						imgOut.at<uchar>(i + (diferencaLinnha / 2), j + (diferencacoluna / 2)) = img.at<uchar>(i + (diferencaLinnha / 2), j + (diferencacoluna / 2));
					}*/
					if (img->GetPixel(position) > min) {
						imgOut2->SetPixel(position2, img->GetPixel(position));
					}
				}
			}
	}

	//geraImageScaled(img, imgScale);

	/*imwrite("f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/imgOriginal.png",img);
	//imwrite("f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/imgBinaria.png",imgBinaria);
	//imwrite("f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/imgScale.png",imgScale);
	//imwrite("f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/imgMascara.png",imgOut);*/
	//md.saveImage(imgOut2,"ImagemCirculoPrimeira.nrrd");
	return imgOut2;

}

//Retorna a mascara Externa de duas imagens 
Mat MascaraImagem::retornaMascaraImageExterna(Mat img1, Mat img2){

	Mat imgOut = Mat::zeros(img1.rows, img1.cols, img1.type());

	//Mat imgOut = img1 - img2;

	//imwrite("f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/imgMascaraExterna.png",imgOut);
	
	return imgOut;

}



