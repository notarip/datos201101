/*
 * pruebaParseoFrase.cpp
 *
 *  Created on: 16/06/2011
 *      Author: pablo
 */

#include <iostream>
#include <map>
#include <string>
#include <list>
#include "../parser/Parser.h"

int main_kkkk(int argc, char** argv)
{

	Parser *p = new Parser();

	list<string> *l= p->parsearFrase("conversasion extraña");


	for (list<string>::iterator it = l->begin(); it != l->end(); it++)
		cout << *it << endl;



	return 0;
}

