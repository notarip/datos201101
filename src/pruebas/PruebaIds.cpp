/*
 * PruebaIds.cpp
 *
 *  Created on: 21/04/2011
 *      Author: pablo
 */

#include <iostream>
#include "../util/Util.h"

int main88(int argc, char** argv)
{

	Util().reiniciarId(0);

	cout << Util().generarIdNuevo() << endl;

	cout << Util().generarIdNuevo() << endl;

	cout << Util().generarIdNuevo() << endl;



	Util().reiniciarId(5);

	cout << Util().generarIdNuevo() << endl;

	cout << Util().generarIdNuevo() << endl;

	cout << Util().generarIdNuevo() << endl;


	return 0;
}
