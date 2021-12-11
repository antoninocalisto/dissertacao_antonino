#include "Funcoes.h"
#include "itkImageDuplicator.h"
//#include "header.h"

Funcoes::Funcoes(void)
{
}


Funcoes::~Funcoes(void)
{
}
//
//Mat Funcoes::returnCircleROI(Mat &image){ // Retorna um circulo contendo a ROI
//
//	Mat imageROICircle = image.clone();
//
//	int thickness	= -1;
//	int lineType	= 8;
//	int raio		= 0;
//
//	Funcoes funcoes;	
//	Point pontoCentral = funcoes.returnPointCenter(imageROICircle);
//	Point pontoXY = funcoes.returnPointXY(imageROICircle,1);
//	raio = (int)funcoes.returnDistancePointXY(pontoCentral, pontoXY);
//	
//	
//	circle( imageROICircle, pontoCentral, raio, Scalar( 255 ), thickness, lineType );
//
//	PathSystem path;
//	string pathOut = path._pathROICirclular +"imgRecortadaCirculo.png";
//	imwrite(pathOut,imageROICircle);
//	
//	return imageROICircle;
//
//}

//------------------------------------------------------------------------------
//Retorna o Ponto central da imagem
//Point Funcoes::returnPointCenter(Mat &image){
//		
//	Point ponto;
//	int lin = image.rows;
//	int col = image.cols;
//	int menorX = 0;
//	int maiorX = 0;
//	int menorY = 0;
//	int maiorY = 0;
//	bool primeiro=true;
//
//	for (int i=0; i < lin; i++){
//		for(int j=0; j < col; j++){
//			if (image.at<uchar>(i,j) > 0){
//				if (primeiro){
//					primeiro = false;
//					menorX = maiorX = j;
//					menorY = maiorY = i;
//				}
//				
//				if (j < menorX ){
//					menorX = j;
//				}else if(j > maiorX){
//					maiorX = j;
//				}
//
//				if (i < menorY ){
//					menorY = i;
//				}else if(i > maiorY ){
//					maiorY = i;
//				}
//			}
//		}
//	}
//
//	int Xmedio = (maiorX + menorX) / 2;
//	int Ymedio = (maiorY + menorY) / 2;
//	
//	ponto.x = Xmedio;
//	ponto.y = Ymedio;
//
//	return ponto;
//}
//
////------------------------------------------------------------------------------
//Retorna os Ponto X e Y, minumos ou maximos
////Tipo: 1 = minimos, 2 = maximos
//Point Funcoes::returnPointXY(Mat &image, int tipo){
//	
//
//	Point ponto;
//	int lin = image.rows;
//	int col = image.cols;
//	int menorX = 0;
//	int maiorX = 0;
//	int menorY = 0;
//	int maiorY = 0;
//	bool primeiro=true;
//
//	for (int i=0; i < lin; i++){
//		for(int j=0; j < col; j++){
//			if (image.at<uchar>(i,j) > 0){
//				if (primeiro){
//					primeiro = false;
//					menorX = maiorX = j;
//					menorY = maiorY = i;
//				}
//				
//				if (j < menorX ){
//					menorX = j;
//				}else if(j > maiorX){
//					maiorX = j;
//				}
//
//				if (i < menorY ){
//					menorY = i;
//				}else if(i > maiorY ){
//					maiorY = i;
//				}
//			}
//		}
//	}
//
//	if (tipo == 1) {
//		ponto.x = menorX;
//		ponto.y = menorY;
//	} else {
//		ponto.x = maiorX;
//		ponto.y = maiorY;
//	}
//
//	return ponto;
//}

//------------------------------------------------------------------------------
////Retorna a distancia entre dois Ponto X e Y
//float Funcoes::returnDistancePointXY(Point p1, Point p2){
//	
//
//	float x1 = p1.x;
//	float x2 = p2.x;
//	float y1 = p1.y;
//	float y2 = p2.y;
//
//	float distancia = sqrt( pow(x2-x1,2) + pow(y2-y1,2));
//	
//	return distancia;
//}

//------------------------------------------------------------------------------
//Retorna a ROI de interesse da imagem
//
//int getMinimum(ImageType3D::Pointer image) {
//	typedef MinimumMaximumImageCalculator <ImageType3D> ImageCalculatorFilterType;
//	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
//	imageCalculatorFilter = ImageCalculatorFilterType::New();
//	imageCalculatorFilter->SetImage(image);
//	imageCalculatorFilter->Compute();
//	return imageCalculatorFilter->GetMinimum();
//}
//
//int getMaximum(ImageType3D::Pointer image) {
//	typedef MinimumMaximumImageCalculator <ImageType3D> ImageCalculatorFilterType;
//	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
//	imageCalculatorFilter = ImageCalculatorFilterType::New();
//	imageCalculatorFilter->SetImage(image);
//	imageCalculatorFilter->Compute();
//	return imageCalculatorFilter->GetMaximum();
//}

//

int Funcoes::getMinimumFuncoes(ImageType3D::Pointer image) {
	typedef MinimumMaximumImageCalculator <ImageType> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMinimum();
}

ImageType3D::Pointer Funcoes::createImageFuncoes(ImageType3D::Pointer image) {

	ImageType3D::Pointer newImage = ImageType3D::New();
	ImageType3D::IndexType start;
	start.Fill(0);
	ImageType3D::RegionType region;
	region.SetSize(image->GetLargestPossibleRegion().GetSize());
	region.SetIndex(start);
	newImage->SetRegions(region);
	newImage->SetSpacing(image->GetSpacing());
	newImage->Allocate();
	newImage->FillBuffer(getMinimumFuncoes(image));
	return newImage;
}

void Funcoes::saveIntensidadesImagem(vector< vector<double> > vetorDeCaracteristicas) {

	PathSystem path;
	string pathFeats = path._pathFeaters + "ValoresDaIntensidadeDaImagem.txt";

	//ManipulaArquivo file(path._pathFeature.c_str(),"caracteristica.txt");	
	//file.limpaArquivo(pathFeats);

	ofstream myfile;
	myfile.open(pathFeats.c_str(), ios::app); //fstream::app|fstream::out appends to existing file
											  //    myLogFile.open (fileFeatsLog.c_str());

	int numFeat = 1, totalFeat = 1, limiteFeature = vetorDeCaracteristicas.size() * vetorDeCaracteristicas.at(0).size();
	int distNum = 1;
	int testeTamanhoDaLinha = 30;
	int outraFatia = 30;
	int contalinha = 1;
	//    int glcmNum = 0;

	//myfile<<startX<<"."<<startY<<"*"<<glcmNum<<"."<<distNum<<"#";
	//myfile << rotulo << " ";

	for (int i = 0; i<vetorDeCaracteristicas.size(); i++) {

		for (int j = 0; j<vetorDeCaracteristicas[i].size(); j++) {

			double vl = vetorDeCaracteristicas[i][j];

			myfile << vl << " "; totalFeat++; //contador de colunas
											  //cout << rotulo << " " << numFeat-1<<":"<<vl<<" " << totalFeat-1;

											  //if(numFeat > 144) // (numFeat > 9) ou (numFeat > 36)
			if (totalFeat > testeTamanhoDaLinha) { // (numFeat > 9) ou (numFeat > 36)
				myfile << "\n";
				totalFeat = 1;
				contalinha++;
			}
			if (contalinha>outraFatia) {
				myfile << "\n#FIMDAFATIA#\n";
				totalFeat = 1;
				contalinha = 1;
			}
		}

	}
	myfile.close();
}



// menorX,menorY,width,height
ImageType3D::Pointer Funcoes::createImageRect(ImageType3D::Pointer image, int width, int height, int contaFatias) {
	ImageType3D::Pointer newImage = ImageType3D::New();
	ImageType3D::IndexType start;
	start.Fill(0);
	ImageType3D::RegionType region;
	ImageType3D::SizeType size;
	
	//ImageType3D::SizeType sizeOriginal;
	size = image->GetLargestPossibleRegion().GetSize();
	size[1] = height;
	size[0] = width;
	size[2] = contaFatias;
	region.SetSize(size);
	region.SetIndex(start);
	//cout << "region.GetSize():" << region.GetSize() << endl;
	newImage->SetRegions(region);
	newImage->SetSpacing(image->GetSpacing());
	newImage->Allocate();
	newImage->FillBuffer(getMinimumFuncoes(image));
	return newImage;
}

ImageType3D::Pointer Funcoes::cutImage(ImageType3D::Pointer image, bool saveImage) {

	ImageType3D::Pointer imageContorno = image;
	ImageType3D::SizeType size = image->GetLargestPossibleRegion().GetSize();
	int intensidadeMinima = (getMinimumFuncoes(image));
	
	int lin = size[1];
	int col = size[0];
	int menorX = 0;
	int maiorX = 0;
	int menorY = 0;
	int maiorY = 0;
	int contaFatias = 0;
	int AtivaFatia = 0;
	int AtivaLinha = 0;
	bool primeiro = true;
	Mat imagem = Mat::zeros(lin,col,1);

	for (int z = 0; z < size[2]; z++) {
		AtivaFatia = 0;
		for (int i = 0; i < lin; i++) { 
			for (int j = 0; j < col; j++) {
				ImageType3D::IndexType position;
				position[0] = j;
				position[1] = i;
				position[2] = z;
					if ( ((int)image->GetPixel(position) > intensidadeMinima) ) {
						
						if (AtivaFatia==0) {
							contaFatias++;
							AtivaFatia=1;
						}

						//if (((int)image->GetPixel(position) > intensidadeMinima) && (i>190) && (j>120)) {
						if (primeiro) {
							primeiro = false;
							menorX = j;
							maiorX = j;
							menorY = i;
							maiorY = i;
						}

						if (j < menorX) {
							menorX = j;
						}
						else if (j > maiorX) {
							maiorX = j;
						}

						if (i < menorY) {
							menorY = i;
						}
						else if (i > maiorY) {
							maiorY = i;
						}
					}
			}
		}
    } // Fim Z

	int width  = maiorX - menorX + 1;
	int height = maiorY - menorY + 1;

	//	cout << "Maior X: "<< maiorX << "Maior Y"<< maiorY << "menor X:" << menorX << "Menor Y: " << menorY << endl;

	//Mat imgsaida(image,Rect(menorX,menorY,width,height));
	Mat imgsaida2(imagem, Rect(menorX, menorY, width, height));
	//cout << "Linhas da imagem: " << imgsaida2.rows << " Colunas da imagem: " << imgsaida2.cols << endl;
	ImageType3D::Pointer imgsaida = createImageRect(image, imgsaida2.cols, imgsaida2.rows, contaFatias);
	Util util;
	//util.saveImage(imgsaida, "imagemCorteDentro.nrrd");
	/*ImageType3D::SizeType size2 = imgsaida->GetLargestPossibleRegion().GetSize();
	cout << "Linhas da imagem Original: " << size2[1] << " Colunas da imagem: " << size2[0] << endl*/;
	
	int jc = 0, ic = 0, zc=0;
	ImageType3D::IndexType positionD;
	ImageType3D::IndexType positionCopia;
	ImageType3D::SizeType sizeCopia = imgsaida->GetLargestPossibleRegion().GetSize();
	
	for (int z = 0; z < size[2]; z++) {
		AtivaFatia = 0;
		for (int i = 0; i < lin; i++) {
			AtivaLinha = 0;
			for (int j = 0; j < col; j++) {
				positionD[0] = j;
				positionD[1] = i;
				positionD[2] = z;
					if( (((j>=menorX) && (i>=menorY)) && ((j<=maiorX) && (i<=maiorY))) ) {
						if (AtivaLinha == 0) {
							AtivaLinha = 1;
						}
						
						if ( (image->GetPixel(positionD)) > intensidadeMinima) {
							if (AtivaFatia==0) {
								AtivaFatia = 1;
							}
							positionCopia[0] = jc;
							positionCopia[1] = ic;
							positionCopia[2] = zc;
							imgsaida->SetPixel(positionCopia,image->GetPixel(positionD));
						}

						jc++;
					}
			}
			//cout << "VALOR DO JC (COMPRIMENTO DA IMAGEM): " << jc << endl;
			jc = 0;
			if (AtivaLinha == 1) {
				ic++;
			}
			
		}
		if (AtivaFatia==1) {
			zc++;
		}
		ic = 0;
		jc = 0;
	}

	//if (saveImage) imwrite(_pathFeats+"imgRecortada.png",imgsaida);;
   
	//util.saveImage(imgsaida, "imagemCorteDentro2.nrrd");
	return imgsaida;

}

int* Funcoes::getHistogram(ImageType3D::Pointer image) {
	Util tools;
	ImageType3D::SizeType size;
	size = image->GetLargestPossibleRegion().GetSize();
	int min, max;
	min = tools.getMinimum(image);
	max = tools.getMaximum(image);
	int sizeHistograma = max - min + 1;

	int *hist2 = new int[4125];

	for (int i = 0; i < 4125; i++)
		hist2[i] = 0;

	int background = min;

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
				if (value != background) {
					if(value>0){
						hist2[value]++;
					}else {
						value = value * (-1);
						hist2[value+3095]++;
					}
				}
			}
		}
	}
	//print histogram
	/*for (int h = 0; h < sizeHistograma; h++)
	cout<<hist[h] <<endl;*/
	int root = sizeHistograma - 1;
    return hist2;
}


//------------------------------------------------------------------------------
//// Gera imagem a partir de um limiar - Tipo: 0 = menor que o limiar, 1 = Maior ou igual ao limiar
ImageType::Pointer Funcoes::geraImageThreshold(ImageType::Pointer image, float threshold, int tipo) {
	Funcoes funcoes;
	//funcoes.saveImage(image, "geraImageThreshold_image.nrrd");

	typedef itk::ImageDuplicator< ImageType3D > DuplicatorType;
	DuplicatorType::Pointer duplicatorimgDivision = DuplicatorType::New();
	duplicatorimgDivision->SetInputImage(image);
	duplicatorimgDivision->Update();
	ImageType3D::Pointer imgDivision = duplicatorimgDivision->GetOutput();

	//funcoes.saveImage(imgDivision,"geraImageThreshold_imgDivision.nrrd");
	//Mat imgDivision(image.rows,image.cols,image.type());
	ImageType3D::SizeType size;
	saveImage(image, "Imagem Dentro do gera ImageThreshold.nrrd");
	size = image->GetLargestPossibleRegion().GetSize();
	int lin = size[1];
	int col = size[0];
	int fundo = getMinimumF(image);
	//cout << "Entrou na função de geraImageThreshold Internamente!" << endl;
	for (int z = 0; z < size[2]; z++) {
		for (int i = 0; i < lin; i++) {
			for (int j = 0; j < col; j++) {
				ImageType3D::IndexType position;
				position[0] = j;
				position[1] = i;
				position[2] = z;
				//cout << "Valor do pixels na posicao: "<< position << " ="  << image->GetPixel(position) << endl;
				if (tipo == 0) {
					if ((int)image->GetPixel(position) <= threshold && (int)image->GetPixel(position) > fundo ) {
						imgDivision->SetPixel(position, (int)image->GetPixel(position)); 
						
					}
					else
						//imgDivision.at<uchar>(i, j) = threshold;
					    imgDivision->SetPixel(position, threshold);
				}
				else {
					if ( (int)image->GetPixel(position) > threshold && (int)image->GetPixel(position) > fundo ) {
						//imgDivision.at<uchar>(i, j) = (int)image->GetPixel(position);
						imgDivision->SetPixel(position, (int)image->GetPixel(position));
					}
					else
						//imgDivision.at<uchar>(i, j) = threshold;
						imgDivision->SetPixel(position, threshold);
				}
			}
		}
	}
	//cout << "Saiu da funcao geraimagethreshold internamente!" << endl;
	return imgDivision;

}

//------------------------------------------------------------------------------
// Retorna a quantidade de pixel do mesmo valor
int Funcoes::returnQuantidadePixelMesmoValor(ImageType3D::Pointer image, int valor){
	
	ImageType3D::SizeType size;
	size = image->GetLargestPossibleRegion().GetSize();
	int lin = size[1];
	int col = size[0];
/*
	int lin = image.rows;
	int col = image.cols;*/
	int qtdPixel = 0;
	
	for (int z = 0; z < size[2]; z++) {
		for (int i = 0; i < lin; i++) {
			for (int j = 0; j < col; j++) {
				ImageType3D::IndexType position;
				position[0] = col;
				position[1] = lin;
				position[2] = z;
				if ((int)image->GetPixel(position) == valor) {
					qtdPixel++;
				}
			}
		}
	}

	return qtdPixel;

}

void Funcoes::saveImage(ImageType3D::Pointer image, string path) {
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(path);
	writer->SetInput(image);
	writer->Update();
}


//------------------------------------------------------------------------------
//Algoritmo otsu modificado
float Funcoes::Otsu2(ImageType3D::Pointer img){
	
	//saveImage(img, "teste.nrrd");
	//compute histogram first
	//cv::Mat imageh; //image edited to grayscale for histogram purpose
	 //to delete and uncomment below;
	//cv::cvtColor(img, imageh, CV_BGR2GRAY);

	int histSize[1] = {4125}; // number of bins
	float hranges[2] = {0, 3095.0}; // min andax pixel value
	const float* ranges[1] = {hranges};
	int channels[1] = {0}; // only 1 channel used

	////TRANSFORMAR IMAGEM 3D EM 2D
	ImageType3D::SizeType size = img->GetLargestPossibleRegion().GetSize();
	Mat tipo; // Só pra usar como parâmetro no tipo
	Mat imageh = Mat::zeros((size[0]*size[1]),size[2], tipo.type());
	int m = 0;
	for (int z = 0; z < size[2]; z++) {
		m = 0;
		for (int y = 0; y < size[1]; y++) {
			for (int x = 0; x < size[0]; x++) {
				ImageType3D::IndexType position;
				position[0] = x;
				position[1] = y;
				position[2] = z;
				imageh.at<uchar>(m,z) = (int)img->GetPixel(position);
				m = m + 1;
			}
		}
    }

	//imwrite("imagehOtsu.png",imageh);
    //cv::MatND hist;
	// Compute histogram
	int *hist = getHistogram(img);
	//calcHist(&imageh, 1, channels, cv::Mat(), hist, 1, histSize, ranges);
	//IplImage* im = new IplImage(imageh);//assign the image to an IplImage pointer
	//IplImage* finalIm = cvCreateImage(cvSize(im->width, im->height), IPL_DEPTH_8U, 1);
	//double otsuThreshold= cvThreshold(im, finalIm, -1000, 3095, cv::THRESH_BINARY | cv::THRESH_OTSU );

	//cout<<"opencv otsu gives "<<otsuThreshold<<endl;

	int totalNumberOfPixels= imageh.total();
	//cout << "Tota de pixels da imagem: " << totalNumberOfPixels << endl;
	//cout<<"total number of Pixels is " <<totalNumberOfPixels<< endl;

	long double sum = 0;
	for (int t=0 ; t<3095 ; t++) 
	{
		//sum += t * hist.at<float>(t);
		sum += t * hist[t];
		//cout << hist.at<float>(t) << endl;
		cout << hist[t] << endl;
	}
	//cout<<"sum is "<<sum<<endl;
	//hist.at<float>(0) = 0;

	float sumB = 0; //sum of background
	int wB = 0; // weight of background
	int wF = 0; //weight of foreground

	float varMax = 0;
	int threshold1 = 0;
	int threshold2 = 0;

	//run an iteration to find the maximum value of the between class variance(as between class variance shld be maximise)
	for (int t = 0; t<3095; t++)
	{
		   //cout << "t: " << hist.at<float>(t)	;
		   //wB += hist.at<float>(t);               // Weight Background
		   wB += hist[t];               // Weight Background
		   if (wB == 0) continue;

		   wF = totalNumberOfPixels - wB;                 // Weight Foreground
		   if (wF == 0) break;

		   //sumB += (float) (t * hist.at<float>(t));
		   sumB += (float)(t * hist[t]);

		   float mB = sumB / wB;            // Mean Background
		   float mF = (sum - sumB) / wF;    // Mean Foreground

		   // Calculate Between Class Variance
		   float varBetween = (float)wB * (float)wF * (mB - mF) * (mB - mF);

		   // Check if new maximum found
		   if (varBetween >= varMax) {
			   threshold1 = t;
			   if (varBetween > varMax) {
				   threshold2 = t;
			   }
			   varMax = varBetween;
			}
	}

    //cout<<"threshold value is: "<< hist.at<float>(0) << endl;;
	//cout<<"threshold value is: "<< hist.at<float>(22) << endl;;
	/*if (threshold2 > threshold1)
		return threshold1;
	else
		if (threshold2 > 0)
			return threshold2;
		else
			return threshold1;
	*/
	//return (threshold1 + threshold2) /2;
	return threshold1;

}

//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/highgui/highgui.hpp"

//using namespace cv;

//
//void Funcoes::kmeansClustering(Mat img){
//  
//	Mat src = img;
//	int cont =0;
//	Mat samples(src.rows*src.cols,1, CV_8U);
//	for( int y = 0; y < src.rows; y++ )
//		for( int x = 0; x < src.cols; x++ ){
//			samples.at<uchar>(y + x*src.rows) = src.at<uchar>(y,x);
//		}
//
//  int clusterCount = 3;
//  Mat labels;
//  int attempts = 2;
//  Mat centers;
//  kmeans(samples, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 10, 1.0), attempts, KMEANS_PP_CENTERS, centers );
//
//
//  Mat new_image( src.size(), src.type() );
//  for( int y = 0; y < src.rows; y++ )
//    for( int x = 0; x < src.cols; x++ )
//    { 
//      int cluster_idx = labels.at<int>(y + x*src.rows,0);
//      new_image.at<Vec3b>(y,x)[0] = centers.at<float>(cluster_idx, 0);
//      new_image.at<Vec3b>(y,x)[1] = centers.at<float>(cluster_idx, 1);
//      new_image.at<Vec3b>(y,x)[2] = centers.at<float>(cluster_idx, 2);
//    }
//  imshow( "clustered image", new_image );
//  waitKey( 0 );
//}

//------------------------------------------------------------------------------
//Retorna o histograma da imagem


int Funcoes::getMinimumF(ImageType::Pointer image) {
	typedef MinimumMaximumImageCalculator <ImageType> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMinimum();
}
int Funcoes::getMaximumF(ImageType3D::Pointer image) {
	typedef MinimumMaximumImageCalculator <ImageType3D> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMaximum();
}


vector <int> Funcoes::getVectorHistograma(ImageType3D::Pointer imageoriginal, int &negativo)
{

ImageType3D::Pointer imgRegion;
ImageType3D::SizeType size;
size = imageoriginal->GetLargestPossibleRegion().GetSize();

int min, max;
min = getMinimumF(imageoriginal);
int minComparacao = min;
//Saber se a intensidade é negativa ou não, para comparar la na árvore
if (minComparacao<0) {
	negativo = 1;
}
else {
	negativo = 0;
}
max = getMaximumF(imageoriginal);
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
else if (min>=0) {
	sizeHistograma = max + 1;
}

//int sizeHistograma = max + min + 1;

int *histograma = new int[sizeHistograma];
	
if (minComparacao<0) {
	for (int i = 0; i < sizeHistograma; i++)
		histograma[i] = 0;
}
else {
	for (int i = 0; i < max+1; i++)
		histograma[i] = 0;
}

	if (min < 0 )
		min *= -1;

	for(int z = 0; z < size[2]; z++){
		for(int y = 0; y < size[1]; y++){
			for (int x = 0; x < size[0]; x++) {
				ImageType3D::IndexType position;
				position[0] = x;
				position[1] = y;
				position[2] = z;
				int value = (int)imageoriginal->GetPixel(position);
				//todos os voxels começam de 0
				if ((value + min) < 0 || value > sizeHistograma) {
					cout << "DEU MERDA:" << value << endl;
				}

				if (minComparacao>=0) {
					histograma[value]++;
					
				}
				else {
					histograma[value + min]++;
					
				}

			}
		}
	}
	vector <int> histogramaVector;
	for (int i = 0; i < sizeHistograma; i++){
		histogramaVector.push_back(histograma[i]);
	}

	return histogramaVector;
}


vector <int> Funcoes::returnHistogramaImage(ImageType3D::Pointer imageh) {

	//TRANSFORMAR IMAGEM 3D EM 2D
	ImageType3D::SizeType size = imageh->GetLargestPossibleRegion().GetSize();
	Mat tipo; // Só pra usar como parâmetro no tipo
	Mat image = Mat::zeros((size[0] * size[1]), size[2], tipo.type());
	int m = 0;
	for (int z = 0; z < size[2]; z++) {
		m = 0;
		for (int y = 0; y < size[1]; y++) {
			for (int x = 0; x < size[0]; x++) {
				ImageType3D::IndexType position;
				position[0] = x;
				position[1] = y;
				position[2] = z;
				image.at<uchar>(m, z) = (int)imageh->GetPixel(position);
				m = m + 1;
			}
		}
	}
	
	int lin = image.rows;
	int col = image.cols;
	int pixelAtual = 0, pixelAnterior = 0;


	int histData[4125];
	vector <int> histograma;

	for ( int i = 0; i < 4125; i++ )
		histData[i] = 0	;

	int x = 0;
	int n = image.total();

	while (x < n) {
		int h = image.data[x];
		//	cout << h << endl;
		histData[h]++;
		//	cout << "histogramaData: " << histData[h] << endl;
		//cout << "hostdata: " << histData[h] << endl;
		x++;
	}	
	
	//cout << "pixel: " << endl;
	for (int i=0; i < 4125; i++ ){
		histograma.push_back(histData[i]);
		if (histData[i] > 0) {
			//cout << "pixel: " << i << "-> " << histData[i] ;
			//cout << i << " ";
		}
	}
	//cout << "\n";
	return histograma;
}

//------------------------------------------------------------------------------
// Retorna um vetor com os arquivos do diretório
vector <string> Funcoes::returnVetorArquivos(string diretorio){

	int processar=0, contaPixel = 0;
	int QTD_IMAGENS = 999999999;
	vector <string> vetorArquivos;

	DIR *dir;
    struct dirent *lsdir;
	
	dir = opendir(diretorio.c_str());
	//dir = opendir("f:/op/1-PSO/3_Sem_fundo/Nao_densas/");
	
	//cout << "Gerando o vetor de arquivos, Aguarde! " << endl;

    /* print all the files and directories within directory */
    while ( ( lsdir = readdir(dir) ) != NULL && processar < QTD_IMAGENS) {
	    //cout << "d_name " << lsdir->d_name << endl;
		if( strcmp(lsdir->d_name, ".") != 0 && strcmp(lsdir->d_name, "..") != 0 ){
			processar++;
			//printf ("%s\n", lsdir->d_name);
			string nomeArquivo = lsdir->d_name;
			string imagem = diretorio+nomeArquivo; // strcat(path,nomeArquivo);
			vetorArquivos.push_back(imagem);
		}
	}

	return vetorArquivos;
}

//------------------------------------------------------------------------------
//Salva arquivo de caracteristica
void Funcoes::saveFeatures(vector< vector<double> > vetorDeCaracteristicas, int rotulo){

	PathSystem path;
	string pathFeats = path._pathFeaters + "caracteristicasFuncionaisQuantizadosCirculosAneis.txt";
	
	//ManipulaArquivo file(path._pathFeature.c_str(),"caracteristica.txt");	
	//file.limpaArquivo(pathFeats);

    ofstream myfile;
    myfile.open (pathFeats.c_str(), ios::app); //fstream::app|fstream::out appends to existing file
//    myLogFile.open (fileFeatsLog.c_str());
 
	int numFeat = 1, totalFeat = 1, limiteFeature = vetorDeCaracteristicas.size() * vetorDeCaracteristicas.at(0).size();
    int distNum = 1;
//    int glcmNum = 0;

    //myfile<<startX<<"."<<startY<<"*"<<glcmNum<<"."<<distNum<<"#";
	myfile<< rotulo << " " ;

    for(int i=0;i<vetorDeCaracteristicas.size();i++){
        
		for(int j=0;j<vetorDeCaracteristicas[i].size();j++){
    
			double vl = vetorDeCaracteristicas[i][j];

            myfile<<numFeat++<<":"<<vl<<" "; totalFeat++;
			//cout << rotulo << " " << numFeat-1<<":"<<vl<<" " << totalFeat-1;

            //if(numFeat > 144) // (numFeat > 9) ou (numFeat > 36)
			if(numFeat > limiteFeature) { // (numFeat > 9) ou (numFeat > 36)
                myfile<<"\n";
    
			}
		}
		
	}
	myfile.close();
}

//Retorna a imagem quantizada
ImageType3D::Pointer Funcoes::quantization(ImageType3D::Pointer imgOrig, int bit)
{
	Funcoes funcoes;
	//Mat img = Mat::zeros(imgOrig.size(),imgOrig.type());
	ImageType3D::Pointer img = createImageFuncoes(imgOrig);
	//funcoes.saveImage(imgOrig, "Funcoes_quantization_752_imgOrig.nrrd");
    int i,j;
    int rangeColors = pow(2.0,8); //fixed max amount
    int newRangeColors = pow(2.0,bit);

    int interval = rangeColors/newRangeColors;
	ImageType3D::SizeType size;
	size = imgOrig->GetLargestPossibleRegion().GetSize();

	for (i = 0; i < size[2]; i++)
	{
		for (i = 0; i < size[1]; i++)
		{
			for (j = 0; j < size[0]; j++)
			{
				ImageType3D::IndexType position;
				position[0] = size[0];
				position[1] = size[1];
				position[2] = size[2];
				int value = imgOrig->GetPixel(position);
				value /= interval; //removes the fractional part
				// getMaximumF getMinimumF
				//img.at<uchar>(i,j) = amt_pix>1 ? (value * interval) : value > 0 ? value : 255;
				if (bit == 1){
					if(value>getMinimumF(imgOrig)){
						img->SetPixel(position, getMaximumF(imgOrig));
					}
					else {
						img->SetPixel(position, getMinimumF(imgOrig));
					}
				}else{
					img->SetPixel(position, (value * interval));
					//img.at<uchar>(i, j) = (value * interval);
				}
				//if (value > 0){
				//	cout << (int)img.at<uchar>(i,j) << value << endl;
				//}
			}
		}
	}
    return img;
}

//------------------------------------------------------------------------------
// Remove o funda da imagem
ImageType3D::Pointer Funcoes::removeFundo(ImageType3D::Pointer image, float valorPixel) {

	//ImageType3D::Pointer imgDivision = Mat::zeros(image.rows,image.cols,image.type());
	typedef itk::ImageDuplicator< ImageType3D > DuplicatorType;
	DuplicatorType::Pointer duplicatorimgDivision = DuplicatorType::New();
	duplicatorimgDivision->SetInputImage(image);
	duplicatorimgDivision->Update();
	ImageType3D::Pointer imgDivision = duplicatorimgDivision->GetOutput();
	//ImageType3D::Pointer imgDivision = image;
	//Mat imgDivision(image.rows,image.cols,image.type());
	ImageType3D::SizeType size;

	int lin = size[1];
	int col = size[0];
	for (int z = 0; z < size[2]; z++) {
		for (int i = 0; i < lin; i++) {
			for (int j = 0; j < col; j++) {
				ImageType3D::IndexType position;
				position[0] = size[0];
				position[1] = size[1];
				position[2] = size[2];
				int value = image->GetPixel(position);
				if (image->GetPixel(position) != valorPixel) {
					imgDivision->SetPixel(position, image->GetPixel(position));
					//imgDivision.at<uchar>(i, j) = image->GetPixel(position);
				}
			}
		}
	}

	return imgDivision;

}

// Gera a borda da Imagem
Mat Funcoes::geraBordaImagem(Mat image) {

	Mat imgClone = Mat::zeros(image.rows,image.cols,image.type());
	int lin = image.rows;
	int col = image.cols;
	bool temVizinhos = false;

	for (int i =0; i < lin; i++){
		for(int j = 0; j < col; j++) {
			if (image.at<uchar>(i,j) > 0){
				temVizinhos = verificarVizinhoZero(image, i, j);
				if (temVizinhos){
					imgClone.at<uchar>(i,j) = 255;
				}
			}

		}
	}

	return imgClone;
}

//Retorna se o pixel tem vizinho com valor zero
bool Funcoes::verificarVizinhoZero(Mat image, int lin, int col) {

	
	for (int i=lin; i < lin+3; i++){
		for(int j=col; j < col+3; j++){
			if (image.at<uchar>(i-1, j-1) == 0){
				return true;
			}
		}
	}

	return false;
}

//Realce histograma local - CLAHE
Mat Funcoes::realceHistogramaLocal(Mat img){

	//Mat m= imread("lena.png",CV_LOAD_IMAGE_GRAYSCALE); //input image
	//imshow("lena_GRAYSCALE",m);

	Ptr<CLAHE> clahe = createCLAHE();
	clahe->setClipLimit(4);

	Mat dst;
	clahe->apply(img,dst);
	//imshow("img_CLAHE",dst);
	//waitKey();

	return dst;
}

//Unir dois arquivos
void Funcoes::unity2File(string arquivo1, string arquivo2){

	ifstream file1(arquivo1), file2(arquivo2);
	ofstream fileOut;

	string path = "f:/op/mestrado_ufma/imagens/imagemPSO/resultados/PSO-nao_densas/5-PSO-Features/";
	string nameArquivo = "FD-N-R-LBP.txt";
	
	fileOut.open(path+nameArquivo);
	//file1.open(arquivo1);
	//file2.open(arquivo2);

	string conteudoUnido;

	if (file1.is_open() || file2.is_open()){
		string conteudo1;
		while (getline(file1,conteudo1)){
			string conteudo2;
			//bool x = getline(file2,conteudo2);
			bool x =(bool)(getline(file1, conteudo1));
			conteudoUnido = conteudo1 + conteudo2;

			ManipulaArquivo manipulaArquivo(path,nameArquivo);
			manipulaArquivo.gravaNoArquivo(conteudoUnido);
			//cout << conteudoUnido << endl;

		}
	}else {
			cout << "arquivo inexistente! " << endl;
	}


}

//------------------------------------------------------------------------------
// Retorna a quantidade de pixel da imagem
int Funcoes::returnQuantidadePixelDaImagem(Mat image){

	int lin = image.rows;
	int col = image.cols;
	int qtdPixel = 0;
	
	for(int i=0; i < lin; i++){
		for(int j = 0; j < col; j++){
			
			if (image.at<uchar>(i,j) > 0 ){
				qtdPixel++;
			}			
		}
	}

	return qtdPixel;

}

////Retorna a distancia entre duas ROIs
//float Funcoes::returnDistance2ROIs(Mat imgROI1, Mat imgROI2){
//	Funcoes funcoes;
//
//	Point p1 = funcoes.returnPointCenter(imgROI1);
//	Point p2 = funcoes.returnPointCenter(imgROI2);
//
//	float distancia = sqrt(pow((float)(p2.x - p1.x),2) + pow((float)(p2.y - p1.y),2));
//
//	return distancia;
//
//}

//--- Calcula a variancia ou Desvio padrão da imagem -> 1 = Variancia; 2 = Desvio Padrao
float Funcoes::calculaVarianciaDesvioPadraoImagem(Mat &imgOrigem, int tipo) {

	Mat imgKernel = Mat::zeros(imgOrigem.rows,imgOrigem.cols,imgOrigem.type()); 

	vector <int> vetValorPixel;
	int lin = imgOrigem.rows;
	int col = imgOrigem.cols;
	float variancia = 0, desvioPadrao = 0, soma = 0;
	float somaPixel = 0, mediaPixel = 0;
	int qtdPixel = 0, qtdDesvioPadrao = 0;

	for (int i=0; i < lin; i++){
		for(int j=0; j <  col; j++){
			if (imgOrigem.at<uchar>(i,j) > 0){
				somaPixel += imgOrigem.at<uchar>(i,j);
				qtdPixel++;
				vetValorPixel.push_back(imgOrigem.at<uchar>(i,j));
			}

		}

	}

	// Valor medio dos pixels
	mediaPixel = somaPixel / qtdPixel;


	//Calcula a variancia	
	qtdPixel = 0;
	for (int i=0; i < vetValorPixel.size(); i++){
		soma += pow((vetValorPixel.at(i) - mediaPixel),2);
		qtdPixel++;
	}

	variancia = soma / qtdPixel;
	desvioPadrao = sqrt(variancia);
	
	//cout << "Variancia: " << variancia << endl;
	//cout << "desvio Padrao: " << desvioPadrao << endl;
	vetValorPixel.clear();
	
	if (tipo == 1){
		return variancia;
	}

	return desvioPadrao;

}  

// GeraROI com os pixels originais da imagem
Mat Funcoes::geraROIPixelOriginal(Mat imgOriginalDDSM, Mat imgOvelay){

	Mat imgROIOriginal = Mat::zeros(imgOvelay.rows,imgOvelay.cols,imgOvelay.type());

	int lin = imgOvelay.rows;
	int col = imgOvelay.cols;
	double area = 0;

	for (int i = 0; i < lin; i++ ){
		for(int j = 0; j < col; j++){
			if (imgOvelay.at<uchar>(i,j) > 0){
				imgROIOriginal.at<uchar>(i,j) = imgOriginalDDSM.at<uchar>(i,j);
			}
		}
	}


	return imgROIOriginal;

}