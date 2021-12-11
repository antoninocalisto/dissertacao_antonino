#include "ReducaoFalsoPositivo.h"


ReducaoFalsoPositivo::ReducaoFalsoPositivo(void)
{
}

ReducaoFalsoPositivo::ReducaoFalsoPositivo(Mat &img){

	this->image = img;

}



ReducaoFalsoPositivo::~ReducaoFalsoPositivo(void)
{
}


vector <float> ReducaoFalsoPositivo::returnHeightWidth(){

	float height = 0;
	float width  = 0; 
	int valueX = 0;
	int valueY = 0;
	vector <float> vetHeightWidth;
	
	for (int i=1; i< image.rows-1; i++){
		for(int j=1; j < image.cols-1; j++){
			
			if (image.at<uchar>(i,j) > 0){
				
				if (j > valueX){
					width++;
				}

				if  (i > valueY){
					height++;
				}
			}
		}
	}

	vetHeightWidth.push_back(height);
	vetHeightWidth.push_back(width);
	
	return vetHeightWidth;
}