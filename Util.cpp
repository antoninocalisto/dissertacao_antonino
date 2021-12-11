#include "Util.h"

Util::Util(void)
{
}
Util::Util(ImageType3D::Pointer image)
{
	imagePrivate = image;
}
Util::~Util(void)
{
	imagePrivate = 0;
}
int Util::getMinimum(ImageType3D::Pointer image){
	typedef MinimumMaximumImageCalculator <ImageType3D> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New ();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMinimum();
}
int Util::getMaximum(ImageType3D::Pointer image){
	typedef MinimumMaximumImageCalculator <ImageType3D> ImageCalculatorFilterType;
	ImageCalculatorFilterType::Pointer imageCalculatorFilter;
	imageCalculatorFilter = ImageCalculatorFilterType::New ();
	imageCalculatorFilter->SetImage(image);
	imageCalculatorFilter->Compute();
	return imageCalculatorFilter->GetMaximum();
}
ImageType3D::Pointer Util::readerImage(string file){
	ImageIOType::Pointer ioImg = ImageIOType::New();
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName(file);
	reader->SetImageIO(ioImg);
	reader->Update();
	return reader->GetOutput();
}
void Util::saveImage(ImageType3D::Pointer image, string path){
	WriterType::Pointer writer = WriterType::New();
	writer->SetFileName(path);
	writer->SetInput(image);
	writer->Update();
}
ImageType3D::Pointer Util::equalizacaoHistograma(ImageType3D::Pointer image){

	ImageType3D::SizeType size;
	size = image->GetLargestPossibleRegion().GetSize();
	int min, max;
	min = getMinimum(image);
	max = getMaximum(image);
	
	int sizeHistograma = max-min+1;

	int *hist = new int[sizeHistograma];
	int *histAcum = new int[sizeHistograma];
	float *histProbabilidade = new float[sizeHistograma];

	// initialize all intensity values to 0
	for(int i = 0; i < sizeHistograma; i++){
		hist[i] = 0;
		histAcum[i] = 0;
	}
	
	int background = min;

	if (min < 0 )
		min *= -1;

	// calculate the no of pixels for each intensity values
   
	ImageType3D::Pointer imgEqualizada = ImageType3D::New();
	imgEqualizada = createImage(image);

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
				hist[ value + min ]++;
			}
			}
		}
	}

	// Caluculate the size of image
    float alpha = (float)(max+min)/(size[1]*size[2]*size[0]);
	
	// Calculate the probability of each intensity
	for(int i = 0; i < sizeHistograma; i++)
        histProbabilidade[i] = (double)hist[i] / sizeHistograma;
	
	//gera o acumulado
	histAcum[0] = hist[0];
    for(int i = 1; i < sizeHistograma; i++)
        histAcum[i] = hist[i] + histAcum[i-1];

	//scala o histrograma
	int *histEscaldado = new int[sizeHistograma];
	for (int i=0; i<sizeHistograma; i++)
		histEscaldado[i] = (int) ((double) histAcum[i] * alpha);

	////gera histograma equalizado
	//float *histEqualizado = new float[sizeHistograma];
	//
	////inicializar
	//for (int i=0; i<sizeHistograma; i++)
	//	histEqualizado[i] = 0;
	//
	//////calcular
	//for (int i=0; i<sizeHistograma; i++)
	//	histEqualizado[histEscaldado[i]] += histProbabilidade[i];

	//int *final = new int[sizeHistograma];
	//for (int i=0; i<sizeHistograma; i++)
	//	final[i] = (int)(histEqualizado[i]*max);

	
	for(int z = 0; z < size[2]; z++){
	for(int y = 0; y < size[1]; y++){
		for(int x = 0; x < size[0]; x++){
			ImageType3D::IndexType position;
			position[0] = x;
			position[1] = y;
			position[2] = z;
			
			int value = (int)image->GetPixel(position);
			
			if (value == background)
				imgEqualizada->SetPixel(position,background);
			else{
				//cout<<value+(min*-1)-min<<endl;
				imgEqualizada->SetPixel(position,(int)histEscaldado[ value + min]-min);
			}
		}
	}
	}
	return imgEqualizada;
}
ImageType3D::Pointer Util::createImage(ImageType3D::Pointer image){
	
	ImageType3D::Pointer newImage = ImageType3D::New();
	ImageType3D::IndexType start;
	start.Fill(0);
	ImageType3D::RegionType region;
	region.SetSize(image->GetLargestPossibleRegion().GetSize());
	region.SetIndex(start);
	newImage->SetRegions(region);
	newImage->SetSpacing(image->GetSpacing());
	newImage->Allocate();
	newImage->FillBuffer(getMinimum(image));
	return newImage;
}
ImageType3D::Pointer Util::wClustering(ImageType3D::Pointer image, int numCluster, bool _arq){
	typedef itk::OtsuMultipleThresholdsImageFilter <ImageType3D, ImageType3D> FilterType;
	FilterType::Pointer otsuFilter = FilterType::New();
	otsuFilter->SetInput(image);
	otsuFilter->SetNumberOfThresholds(numCluster);
	if (_arq)
		otsuFilter->SetNumberOfHistogramBins(getMaximum(image)-getMinimum(image)+1);
	otsuFilter->Update();
	FilterType::ThresholdVectorType thresholds = otsuFilter->GetThresholds();
	limiar = thresholds.at(thresholds.size()-1);
	return otsuFilter->GetOutput();
}
ImageType3D::Pointer Util::getImageTextureOriginal(ImageType3D::Pointer original, ImageType3D::Pointer otsu){
	
	ImageType3D::Pointer newImage = createImage(original);
	
	ImageType3D::SizeType size = original->GetLargestPossibleRegion().GetSize();
	
	int fundo = getMinimum(otsu);

	ImageType3D::IndexType position;
	for(int z = 0; z < size[2]; z++){
	for(int y = 0; y < size[1]; y++){
		for(int x = 0; x < size[0]; x++){
			position[0] = x;
			position[1] = y;
			position[2] = z;
			int value = (int)otsu->GetPixel(position);
			if (value != fundo)
				newImage->SetPixel(position,original->GetPixel(position));
		}
	}
	}
	//saveImage(newImage,"c:/teste.dcm");
	return newImage;
}
void Util::geraFileSVM(int rotulo, vector<long double> features, string savePath){
	ofstream featureFile(savePath.c_str(),ofstream::app);
	featureFile.precision(10);
	featureFile<<rotulo<<" ";
	for (int l = 0; l < features.size(); l++)
		featureFile<<l+1<<":"<<features.at(l)<<" ";
	featureFile<<"\n";
	featureFile.close();
}
//vector<ImageType3D::Pointer> Util::getRegionsOtsuClustering(ImageType3D::Pointer imageOriginal, int _nCluster, bool _arq){
//	
//	vector<ImageType3D::Pointer> _region;
//	ImageType3D::Pointer voiOtsu = createImage(imageOriginal);
//
//	voiOtsu = otsuClustering(imageOriginal,_nCluster, _arq);
//
//	ImageType3D::Pointer imgRegion;
//	ImageType3D::SizeType size;
//	size = imageOriginal->GetLargestPossibleRegion().GetSize();
//
//	int min, max;
//	
//	min = getMinimum(voiOtsu);
//	max = getMaximum(voiOtsu);
//	
//	int sizeHistograma = max-min+1;
//
//	int *histograma = new int[sizeHistograma];
//	for(int i = 0; i < sizeHistograma; i++)
//		histograma[i] = 0;
//	
//	int background = min;
//
//	if (min < 0 )
//		min *= -1;
//    int m=0;
//	for(int z = 0; z < size[2]; z++){
//		m=0;
//		for(int y = 0; y < size[1]; y++){
//			for(int x = 0; x < size[0]; x++){
//				ImageType3D::IndexType position;
//				position[0] = x;
//				position[1] = y;
//				position[2] = z;
//				int value = (int)voiOtsu->GetPixel(position);
//
//				ope(,z) = 
//
//				if (value != background){
//					if ((value + min) < 0 || value > sizeHistograma)
//					cout << "DEU MERDA: " << value << endl;
//					histograma[ value + min ]++;
//				}
//				m=m+1;
//			}
//		}
//
//	}
//
//	int m = -1;
//	for (int h = 0; h<sizeHistograma;h++){
//		if (histograma[h] > 0 && h != background){
//			if (histograma[h]>m)
//				m = h;
//		}
//	}
//
//	for (int h = 0; h<sizeHistograma;h++){
//		if (histograma[h] > 0 && h != background){
//			ImageType3D::Pointer imageAux = createImage(imageOriginal);
//			for(int z = 0; z < size[2]; z++){
//			for(int y = 0; y < size[1]; y++){
//				for(int x = 0; x < size[0]; x++){
//					ImageType3D::IndexType position;
//					position[0] = x;
//					position[1] = y;
//					position[2] = z;
//					int value = voiOtsu->GetPixel(position);
//					if (value == h){
//						imageAux->SetPixel(position,imageOriginal->GetPixel(position));
//					}
//					}
//				}
//			}
//			//saveImage(imageAux,"c:/otsu.dcm");
//			_region.push_back(imageAux);
//			imageAux = 0;
//		}
//	}
//	return _region;
//}
ImageType3D::Pointer Util::gassuianFilter(ImageType3D::Pointer image, double variance){
	typedef itk::DiscreteGaussianImageFilter<ImageType3D, ImageTypeFloat >  filterType;
	filterType::Pointer gaussianFilter = filterType::New();
	gaussianFilter->SetInput( image );
	gaussianFilter->SetVariance(variance);
	gaussianFilter->Update();
	return floatToshort(gaussianFilter->GetOutput());
}
ImageType3D::Pointer Util::floatToshort(ImageTypeFloat::Pointer imgFloat){
	typedef itk::CastImageFilter<ImageTypeFloat, ImageType3D> CastFilterType;
	CastFilterType::Pointer castFilter = CastFilterType::New();
	castFilter->SetInput(imgFloat);
	castFilter->Update();
	return castFilter->GetOutput();
}

//funcoes para criacao da arvore
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
int Util::getNumIndividuos(ImageType3D::Pointer image, int value){
	ImageType3D::SizeType size = image->GetLargestPossibleRegion().GetSize();
	
	int cont = 0;
	for(int z = 0; z < size[2]; z++){
	for(int y = 0; y < size[1]; y++){
		for(int x = 0; x < size[0]; x++){
			ImageType3D::IndexType position;
			position[0] = x;
			position[1] = y;
			position[2] = z;
			int pixel = (int)image->GetPixel(position);
			if (pixel == value)
				cont++;
			}
		}
	}
	return cont;
}
bool Util::existeElemento(vector<especie> vetor, int elemento){
	for (int i = 0; i < vetor.size(); i++){
		if (vetor.at(i).esp == elemento)
			return false;
	}
	return true;
}
int Util::getLimiarOtsu(){
	return limiar;
}
void Util::getAllRegions(ImageType3D::Pointer imageClusterizacao, ImageType3D::Pointer imageOriginal)
{
	
	regionNormal = new vector<ImageType3D::Pointer>;
	
	ImageType3D::Pointer imgRegion;
	ImageType3D::SizeType size;
	size = imageOriginal->GetLargestPossibleRegion().GetSize();

	
	int min, max;
	min = getMinimum(imageClusterizacao);
	max = getMaximum(imageClusterizacao);
	int sizeHistograma = max-min+1;

	int *histograma = new int[sizeHistograma];
	for(int i = 0; i < sizeHistograma; i++)
		histograma[i] = 0;
	
	int background = getMinimum(imageOriginal);

	if (min < 0 )
		min *= -1;

	for(int z = 0; z < size[2]; z++){
	for(int y = 0; y < size[1]; y++){
		for(int x = 0; x < size[0]; x++){
			ImageType3D::IndexType position;
			position[0] = x;
			position[1] = y;
			position[2] = z;
			int value = (int)imageClusterizacao->GetPixel(position);
			//todos os voxels começam de 0
			if ((value + min) < 0 || value > sizeHistograma)
				cout << "DEU MERDA: " << value << endl;
				histograma[ value + min ]++;
			}
		}
	}
	/*cout<<sizeHistograma<<"\n";
	for (int h = 0; h<sizeHistograma;h++)
		cout<<h<<" "<<histograma[h]<<" ";
	cout<<endl;*/
	for (int h = 0; h<sizeHistograma;h++){
		if (histograma[h] > 0 && h != min){
			//cout<<h<<histograma[h]<<endl;
			ImageType3D::Pointer imageAux = createImage(imageOriginal);
			for(int z = 0; z < size[2]; z++){
				for(int y = 0; y < size[1]; y++){
				for(int x = 0; x < size[0]; x++){
					ImageType3D::IndexType position;
					position[0] = x;
					position[1] = y;
					position[2] = z;
					int value = imageClusterizacao->GetPixel(position);
					if (value == h){
						imageAux->SetPixel(position,imageOriginal->GetPixel(position));
					}
				}
					
				}
			}
			//saveImage(imageAux,"f:/teste.dcm");
			regionNormal->push_back(imageAux);
			imageAux = 0;
		}
	}

}
vector<ImageType3D::Pointer>* Util::getRegions(){
	return regionNormal;
}
vector<int> Util::getUnicaEspecie(ImageType3D::Pointer image){
	
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
			if (value != background){
				if ((value + min) < 0 || value > sizeHistograma)
				cout << "DEU MERDA: " << value << endl;
				histograma[ value + min ]++;
			}
			}
		}
	}

	int numEspecies = 0;
	//int especie = -1;
	vector<int> esp;

	for (int h = 0; h<sizeHistograma; h++){
		if (histograma[h] > 0){
			numEspecies++;
			//cout<<h<<endl;
			esp.push_back((h-min));
		}
	}
	if (esp.size() > 1){
		cout<<"ERRO"<<endl;
	}
	return esp;
}

vector<especie> Util::getArvoresFromTxt(string path){
	vector<especie> limiares;
	
	ifstream file(path.c_str());
	if (file.is_open()){
		for (string line; getline(file, line); )
			{
				especie aux;
				string buf;
				stringstream ss(line);
				vector<string> tokens;
				while (ss >> buf)
					tokens.push_back(buf);
				aux.esp = stoi(tokens.at(0));
				aux.ind = stoi(tokens.at(1));
				//cout<<aux.esp<<endl;
				//cout<<aux.ind<<endl;
				limiares.push_back(aux);
			}
	}
	else{
		cout<<"Falha ao abrir o arquivo: "<<path<<endl;
	}
	//cout<<limiares.size();
	return limiares;
}