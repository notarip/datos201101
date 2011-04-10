


#include <iostream>
#include "../util/Parametros.h"
#include "../hash/Hash.h"

using namespace std;

int main(int argc, char** argv)
{
	string name = "palabras";
	Hash *prueba = new Hash();
	if (prueba->crear(name) != 0)
		prueba->abrir(name);

	prueba->~Hash();

	return 0;
}
