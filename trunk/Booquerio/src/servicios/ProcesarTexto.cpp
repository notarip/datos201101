/*
 * ProcesarTexto.cpp
 *
 *  Created on: 21/04/2011
 *      Author: pablo
 */

#include "ProcesarTexto.h"

void ProcesarTexto::procesarTexto(string ruta)
{
	Parser *unParser = new Parser();
	unParser->parsear(ruta);
	Libro *unLibro = unParser->getLibro();
	unParser->~Parser();
	//agregar al arbol primario
	//meter el libro en el arhivo de registros variables

	SinIndice *listas = SinIndice().getInstancia();
	listas->agregarLibroPendienteAutores(unLibro->getId());
	listas->agregarLibroPendienteEditoriales(unLibro->getId());
	listas->agregarLibroPendienteTitulos(unLibro->getId());
	listas->agregarLibroPendientePalabras(unLibro->getId());
}


