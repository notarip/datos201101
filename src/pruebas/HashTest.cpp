


#include <iostream>
#include "../util/Parametros.h"
#include "../hash/Hash.h"

using namespace std;

int main2(int argc, char** argv)
{
	string name = "palabras";
	Hash *prueba = new Hash(name);


	prueba->~Hash();

	return 0;
}
