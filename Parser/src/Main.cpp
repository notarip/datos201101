

#include <iostream>
#include "util/Parametros.h"

int main (int argc, char** argv)
{

	string prop = Parametros().getParametro("path datos");
	cout << prop.c_str() << endl;





	return 0;
}
