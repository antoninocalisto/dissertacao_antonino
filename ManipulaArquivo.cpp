#include "ManipulaArquivo.h"


ManipulaArquivo::ManipulaArquivo(void)
{
}

ManipulaArquivo::ManipulaArquivo(string path, string arquivo){

	_pathArquivo = path;
	_nomeArquivo = arquivo;

}


ManipulaArquivo::~ManipulaArquivo(void)
{
}


void ManipulaArquivo::gravaNoArquivo(string conteudo){

	// basic file operations
	//ifstream file (_pathArquivo + "estatistica.txt");
	ofstream arquivo;
	//arquivo.open (_pathArquivo + "estatistica.txt", ios::app );
	arquivo.open (_pathArquivo + _nomeArquivo, ios::app );
	
	if (arquivo.is_open()){
		arquivo << conteudo << endl;
		arquivo.close();
	}else{
		
	}
	
}

void ManipulaArquivo::limpaArquivo(string pathArquivo){

	
	ifstream my_file(pathArquivo.c_str());
    if (my_file)
    {
        ofstream myfile;
        myfile.open (pathArquivo.c_str());//, fstream::app|fstream::out);
        myfile.close();
    }

}