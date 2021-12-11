#include "quantizacao.h"
#include "Util.h"

Quantizacao::Quantizacao(ImageType3D::Pointer image, int bits){
	voi = image;
	bit =  bits;
	executa2();
	
	//executa();
	//Executa a Quantização Uniforme da Imagem (ROI).
}

Quantizacao::~Quantizacao(){
	voiQuantizada = 0;
}


ImageType3D::Pointer Quantizacao::getVOI(){
	return voiQuantizada;
}

void Quantizacao::executa(){


	WriterType::Pointer writer = WriterType::New();
	
	//----------------------------------------------------------------------------------------------
	
	typedef itk::MinimumMaximumImageCalculator <ImageType3D>    ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
		
	int fundo;
	int max;
		
	imageCalculatorFilter = ImageCalculatorFilterType::New ();
	imageCalculatorFilter->SetImage(voi);
	imageCalculatorFilter->Compute();
	
	fundo = imageCalculatorFilter->GetMinimum();
	max = imageCalculatorFilter->GetMaximum();

	int height = voi->GetLargestPossibleRegion().GetSize()[1];

	int width = voi->GetLargestPossibleRegion().GetSize()[0];

	int depth = voi->GetLargestPossibleRegion().GetSize()[2];

	//int fundo2;

	//fundo2 = fundo;

	//if(fundo<0)
		//fundo2 = fundo*-1;
	
	int MIN = fundo;//+fundo2;
	int MAX = max;//+fundo2;

	//bit+1;

	//----------- Calcula o Máximo de Níveis de Cinza, de acordo com os BIT's da imagem ------------
	int fator = (int) pow(2.0, 16 - (bit));
	//----------------------------------------------------------------------------------------------
		
	
	//-------------------------------------- Imagem de resultado --------------------------------------
	

	voiQuantizada = ImageType3D::New();

	ImageType3D::IndexType start;
	start[0] = 0; // first index on X
	start[1] = 0; // first index on Y
	start[2] = 0; // first index on Z
	
	ImageType3D::SizeType size;
	size[0] = width; // size along X
	size[1] = height; // size along Y
	size[2] = depth; // size along Z
	
	ImageType3D::RegionType region;
	region.SetSize(size);
	region.SetIndex(start);
		
	voiQuantizada->SetRegions(region);
	voiQuantizada->Allocate();
	//-------------------------------------------------------------------------------------------------
	
	if (bit == 16){
		voiQuantizada = voi;
		return;
	}

	for (int z = 0; z < depth; z++) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < height; j++) {

				itk::Index<3> center;	
				long centerId[] ={i,j,z};
				center.SetIndex(centerId);

				//Obtém o Nível de Cinza do Pixel (j, i) da ROI.
				int pixel = voi->GetPixel(center);

				//-------------------------------- Pixel de FUNDO não é quantizado ---------------------------------
				if (pixel == fundo) {
					voiQuantizada->SetPixel(center, fundo);
					continue;
				}
				//-------------------------------------------------------------------------------------------------------

				int pixelQuant;
				//ORIGINAL
				//pixelQuant = pixel / fator;
				
				/*
				pixel +=fundo2;

				//-------------------------- Pixel quantizado para o intervalo [0, niveis - 1] --------------------------*/
				int niveis = pow(2.0,bit);

				pixelQuant = (niveis - 1) * (pixel - MIN) / (MAX - MIN);
				
				//-------------------------------------------------------------------------------------------------------

				/*s
				//--------------------------- Pixel ESCALADO para o intervalo [0, MAX - MIN] ----------------------------
			
				pixelQuant *= (int) ((MAX - MIN) * 1.0 / (niveis - 1));
				//-------------------------------------------------------------------------------------------------------
			

				//----------------------------- Pixel TRANSLADADO para o intervalo [MIN, MAX] ---------------------------
				pixelQuant += MIN;
				//-------------------------------------------------------------------------------------------------------

				pixelQuant -= fundo2;
				*/

				voiQuantizada->SetPixel(center, pixelQuant);
				
			}
		}
	}
}

void Quantizacao::executa2(){
	Util *tools = new Util();

	ImageType3D::SizeType size = voi->GetLargestPossibleRegion().GetSize();

	voiQuantizada = tools->createImage(voi);

	//-------------------------------------------------------------------------------------------------
	int newMax, newMin, oldMax, oldMin;

	newMax = tools->getMaximum(voi);
	newMin = tools->getMinimum(voi);

	oldMin = 0; 
	oldMax = (pow(2.0,bit)) -1; 

	//busca os novos maximos e minimos, q eu chamei de oldMin e oldMax
	for (int z = 0; z < size[2]; z++) {
		for (int i = 0; i < size[1]; i++) {
			for (int j = 0; j < size[0]; j++) {

				itk::Index<3> center;	
				long centerId[] ={j,i,z};
				center.SetIndex(centerId);

				//Obtém o Nível de Cinza do Pixel (j, i) da ROI.
				int pixel = voi->GetPixel(center);
				
				if (pixel == newMin) continue;
				
				int pixelQuant, pixel2;
				int niveis = pow(2.0,bit);

				pixel2 = int((niveis - 1) * (pixel - newMin) / (newMax - newMin));
				
				double scale;
				
				scale = (double)(pixel2-oldMin)/(double)(oldMax-oldMin);
				
				pixelQuant = ((newMax - newMin)*scale)+newMin;
				
				voiQuantizada->SetPixel(center, pixelQuant);
				
			}
		}
	}
}