/*
 * pruebasFrontCodding.cpp
 *
 *  Created on: 22/05/2011
 *      Author: pablo
 */

#include <iostream>
#include "../archivos/Bloque.h"
#include "../util/Util.h"

int main(int argc, char** argv)
{

	list<string> lista;
	Util().levantarCSV("./archivos/conf/editoriales.csv",&lista);
	Bloque *unBloque = new Bloque();

	for (list<string>::iterator it = lista.begin(); it != lista.end(); it++)
	{
		unBloque->agregarRegistro(new Registro(*it));
	}




		Bloque* bloqueFontCoding= new Bloque();
		bloqueFontCoding->setAtributoBloque(unBloque->getAtributoBloque());
		bloqueFontCoding->setSiguiente(unBloque->getSiguiente());

		list<Registro>* listaReg= unBloque->obtenerRegistros();
		list<Registro>::iterator itRegistros= listaReg->begin();
		if(itRegistros!=listaReg->end()){
			Registro unRegistro(*itRegistros);

			itRegistros++;
			bloqueFontCoding->agregarRegistro(unRegistro);
			unsigned int coincidencias;
			string previo= unRegistro.getString();
			char* stringFront;
			string actual= "";
			bool coincidio= true;
			unsigned int caracteresNoCoincidentes= 0;
			while(itRegistros!=listaReg->end()){
				coincidio= true;
				Registro unRegistro(*itRegistros);
				stringFront= NULL;
				actual=unRegistro.getString();
				coincidencias= 0;
				while(coincidencias<actual.size()&&coincidencias<previo.size()&&coincidio){
					if(actual.at(coincidencias)==previo.at(coincidencias)){
						coincidencias++;
					}
					else{
						coincidio=false;
					}
				}
			caracteresNoCoincidentes= actual.size()-coincidencias;
			stringFront=new char[caracteresNoCoincidentes+1];
			stringFront[caracteresNoCoincidentes]='\0';
			//copy(dest,cuanto,desde donde)
			actual.copy(stringFront,caracteresNoCoincidentes,coincidencias);
			previo=actual;
			unRegistro.setString(stringFront);
			if (coincidencias>0)
				unRegistro.agregarAtribEntero(coincidencias);
			bloqueFontCoding->agregarRegistro(unRegistro);
			itRegistros++;
			delete []stringFront;
		}
		}


	for (list<string>::iterator it = lista.begin(); it != lista.end(); it++)
		cout << *it << ",";
	cout << endl;

	list<Registro> *listaR = bloqueFontCoding->obtenerRegistros();

	for (list<Registro>::iterator it = listaR->begin(); it != listaR->end(); it++)
	{
		int coinsidencias = (*it).getAtributosEnteros() ? (*it).getAtributosEnteros()->front():0;
		cout << coinsidencias << " | " << (*it).getString() << endl;
	}


	return 0;
}

