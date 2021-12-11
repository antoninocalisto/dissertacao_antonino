#ifndef QUANTIZACAO_H
#define QUANTIZACAO_H

#include "header.h"
class Quantizacao{

	public:
		Quantizacao(ImageType3D::Pointer image, int bit);
		~Quantizacao();
		void executa();
		void executa2();
		ImageType3D::Pointer getVOI();

	private:
		ImageType3D::Pointer voi;
		ImageType3D::Pointer voiQuantizada;
		int bit;

};

#endif