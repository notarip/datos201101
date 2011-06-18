#include "../ProcesadorConsultas/archivoNormasInf.h"
#include <iostream>
#include <map>
#include "../hash/Hash.h"
using namespace std;


int main321(int argc, char** argv){
	string ruta="archivos/datos/hash_palabras";
	Hash hash(ruta);

	map<string,unsigned int>* mapa= hash.recuperacionComprensiva();
	map<string,unsigned int>::iterator it= mapa->begin();
	while (it!= mapa->end()){

		cout << it->first <<"; "<< it->second <<endl;
		it++;
	}
}
