/*
 * pruebasFrontCodding.cpp
 *
 *  Created on: 22/05/2011
 *      Author: pablo
 */

#include <iostream>
#include "../archivos/Bloque.h"
#include "../util/Util.h"

Bloque* deshacerFrontCoding(Bloque* unBloque) {

	Bloque* bloqueDescomprimido= new Bloque();
	bloqueDescomprimido->setAtributoBloque(unBloque->getAtributoBloque());
	bloqueDescomprimido->setSiguiente(unBloque->getSiguiente());
	list<Registro>* listaReg= unBloque->obtenerRegistros();
	list<Registro>::iterator itRegistros= listaReg->begin();
	Registro unRegistro(*itRegistros);
	string previo= itRegistros->getString();
	*itRegistros++;
	bloqueDescomprimido->agregarRegistro(unRegistro);
	unsigned int coincidencias;
	string actual="";
	while(itRegistros!=listaReg->end()){
		Registro unRegistro(*itRegistros);
		actual="";
		if(unRegistro.getAtributosEnteros()->size()>0){
			coincidencias= unRegistro.getAtributosEnteros()->front();
			unRegistro.getAtributosEnteros()->pop_front();
		}
		else
			coincidencias=0;
		actual.append(previo,0,coincidencias);
		previo=unRegistro.getString();
		actual.append(previo);
		unRegistro.setString(actual);
		bloqueDescomprimido->agregarRegistro(unRegistro);
		previo= actual;
		itRegistros++;
	}
	return bloqueDescomprimido;
}



int main_ppp(int argc, char** argv)
{

	list<string> lista;
	Util().levantarCSV("./archivos/pruebas/testFront.csv",&lista);
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
		int coinsidencias = (*it).getAtributosEnteros()->size() > 0 ? (*it).getAtributosEnteros()->front():0;
		cout << coinsidencias << " | " << (*it).getString() << endl;
	}

	cout << "-----------------------------------" << endl;
	unBloque = deshacerFrontCoding(bloqueFontCoding);
	listaR = unBloque->obtenerRegistros();
	list<string>::iterator itOri = lista.begin();
	string ok;
	int good = 0;
	int	bad = 0;
	for (list<Registro>::iterator it = listaR->begin(); it != listaR->end(); it++)
	{
		int coinsidencias = (*it).getAtributosEnteros()->size() > 0 ? (*it).getAtributosEnteros()->front():0;
		if ((*it).getString() == (*itOri))
		{
			ok = " ---> GOOD";
			good++;
		}
		else
		{
			ok = " ---> BAD";
			bad++;
		}
		cout << coinsidencias << " | " << (*it).getString() << " = " <<(*itOri)  << ok << endl;
		itOri++;
	}

	cout << "Errores: " << bad << "  Sin error: " << good;

	return 0;
}

