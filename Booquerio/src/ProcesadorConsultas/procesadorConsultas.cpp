/*
 * procesadorConsultas.cpp
 *
 *  Created on: 09/06/2011
 *      Author: rjc
 */

#include "procesadorConsultas.h"

ProcesadorConsultas::ProcesadorConsultas() {
	carpetaRaiz = Parametros().getParametro(CARPETA_DATOS);
}

ProcesadorConsultas::~ProcesadorConsultas() {

}

float ProcesadorConsultas::calcularPesoxProximidad(list<string> terminos, unsigned int id){
	unsigned int cantidadTerminos = terminos.size();
	list<unsigned int> vectorListas[cantidadTerminos];
	list<unsigned int>::iterator vectorIteradores[cantidadTerminos];

	//levanto las apariciones de terminos de cada documento


	string pathArbolTerminos = carpetaRaiz + PREFIJO_ARBOL_TERMINOS;
	stringstream conversor;
	conversor << id;
	string docId;
	conversor >> docId;
	pathArbolTerminos += docId;

	ArbolBMasAlfabetico* arbolDeTerminos = new ArbolBMasAlfabetico(
				pathArbolTerminos, 6144);
	resultadoOperacion result(OK);

	list<string>::iterator itTerminos = terminos.begin();
	Registro* regEncontrado;
	unsigned int unOffset;
	unsigned int n = 0;
	string unTermino;

	while ( itTerminos != terminos.end()){
		unTermino = *itTerminos;
		regEncontrado = arbolDeTerminos->buscarRegistro(unTermino, &result);
		unOffset = regEncontrado->getReferenciai(1);
		ListasIds().obtenerListaIds(unOffset, &vectorListas[n]);
		n++;
		itTerminos++;
	}

	//Tengo las listas con las ocurrencias de cada uno de los terminos
	//Fabrico iteradores para ellos

	for ( unsigned int i = 0 ; i< cantidadTerminos ; i++){
		vectorIteradores[i] = vectorListas[i].begin();
	}

	// voy a ir calculando secuencias optimas para cada unos de las posicionse
	// del ultimo termino

	float pesoDocumento = 0;

	while ( vectorIteradores[cantidadTerminos-1] != vectorListas[cantidadTerminos-1].end()){
		list<unsigned int> unaSecuencia;
		unaSecuencia.push_back(*vectorIteradores[cantidadTerminos-1]);
		list<unsigned int> secOptima;

		ArmarSecuenciaOptima(unaSecuencia,&secOptima,cantidadTerminos,vectorListas);

		if (secOptima.size() !=0 ){
			pesoDocumento += 1 / pow(obtenerSeparacion(secOptima),2);

			//limpiar listas

			for(unsigned int j=0; j<cantidadTerminos-1; j++){
				while(vectorListas[j].front()<=(*vectorIteradores[cantidadTerminos-1])){
					vectorListas[j].pop_front();
				}
			}
		}
		vectorIteradores[cantidadTerminos-1]++;
	}

	delete arbolDeTerminos;

	return pesoDocumento;
}

void ProcesadorConsultas::ArmarSecuenciaOptima(list<unsigned int> secuencia, list<unsigned int>* max , unsigned int cantListas, list<unsigned int> vectorListas[]){
	//salida recursion
	if (secuencia.size() == cantListas){
		unsigned int pesoSecuencia = this->obtenerSeparacion(secuencia);
		if (pesoSecuencia < this->obtenerSeparacion(*max)){
			max->clear();
			*max= secuencia;
		}
		return;
	}

	unsigned int listaActual = cantListas -1 - secuencia.size();
	unsigned int limite = secuencia.back();
	list<unsigned int>::iterator it;

	it= vectorListas[listaActual].begin();

	while ( it != vectorListas[listaActual].end() && *it < limite ){
		secuencia.push_back(*it);
		this->ArmarSecuenciaOptima(secuencia,max,cantListas,vectorListas);
		secuencia.pop_back();
		it++;

	}


}

unsigned int ProcesadorConsultas:: obtenerSeparacion(list<unsigned int> unaSecuencia){

	if (unaSecuencia.size() == 0){
		return UINT_MAX;
	}

	list<unsigned int>::iterator it,fin;
	unaSecuencia.sort();
	it = unaSecuencia.begin();
	fin = unaSecuencia.end();
	fin--;
	unsigned int resultado = 0;
	unsigned int unaResta,aux;


	while (it != fin){
		aux = *it;
		it++;
		unaResta = *it-aux;
		resultado += unaResta;
	}

	return resultado;
}


float ProcesadorConsultas::calcularPeso(string termino, unsigned int documento,
		float pesoGlobal) {

	archivoNormasInf archivoNormas;
	unsigned int norma= archivoNormas.obtenerNorma(documento);

	//Obtengo la cantidad de apariciones del termino

	string pathArbolTerminos = carpetaRaiz + PREFIJO_ARBOL_TERMINOS;
	stringstream conversor;
	conversor << documento;
	string docId;
	conversor >> docId;
	pathArbolTerminos += docId;

	ArbolBMasAlfabetico* arbolDeTerminos = new ArbolBMasAlfabetico(
			pathArbolTerminos, TAMANIO_BLOQUE);
	resultadoOperacion result(OK);
	Registro* regEncontrado = arbolDeTerminos->buscarRegistro(termino, &result);
	unsigned int offset= regEncontrado->getReferenciai(1);
	list<unsigned int> listaPosiciones;
	ListasIds().obtenerListaIds(offset, &listaPosiciones);
	unsigned int aij = listaPosiciones.size();
	delete arbolDeTerminos;

	return (aij * pesoGlobal / norma);
}

void ProcesadorConsultas::consultaUnitaria(string termino) {
	float pesoGlobal;
	string pathHash = carpetaRaiz + "hash_palabras";
	Hash hashTerminos(pathHash);
	Registro* regPalabra = hashTerminos.buscar(termino);
	if (regPalabra == NULL) {
		cout << "La Busqueda no Obtuvo Resultados" << endl;
	} else {
		string pathArbolPrim = carpetaRaiz + "bmas_primario";
		ArbolBMasNumerico* arbolPrimario = new ArbolBMasNumerico(pathArbolPrim,
				TAMANIO_BLOQUE_BMAS_NUMERICO);
		resultadoOperacion result(OK);
		Registro* regActual = arbolPrimario->buscarRegistroNumerico(0, &result);
		unsigned int N = 1;
		while (regActual != NULL) {
			regActual = arbolPrimario->siguiente();
			N++;
		}
		delete arbolPrimario;

		unsigned int offset = regPalabra->getAtributosEnteros()->front();
		list<unsigned int> listaIdsConsulta;
		ListasIds().obtenerListaIds(offset, &listaIdsConsulta);
		unsigned int ni = listaIdsConsulta.size();
		list<unsigned int>::iterator it = listaIdsConsulta.begin();

		list<unsigned int> listaDocs;
		list<float> listaPesos;
		float unPeso;
		list<unsigned int>::iterator itDocs;
		list<float>::iterator itPesos;

		pesoGlobal = log10(N / ni);
		while (it != listaIdsConsulta.end()) {
			unPeso = calcularPeso(termino, *it, pesoGlobal);

			//CHEKEAR SI NO HACE FALTA HACER OTRA MULTIPLICACION EN LA FORMULA

			itDocs = listaDocs.begin();
			itPesos = listaPesos.begin();

			while (unPeso < *itPesos) {
				itPesos++;
				itDocs++;
			}
			listaDocs.insert(itDocs, *it);
			listaPesos.insert(itPesos, unPeso);

			it++;

		}
		//aca tengo 2 listas con los contenidos ordenados por pesos, una de pesos y otras de doc

		/*TERMINAR DE REVISAR COMO IMPRIMIOS LA INFO*/

	}
}

void ProcesadorConsultas::consultaPorTerminosCercanos(
		list<string> listaTerminos) {

	//itero por todos los terminos buscando los documentos que los contengan
	list<string>::iterator itTerminos = listaTerminos.begin();
	list<unsigned int> idsTodos;
	unsigned int unOffset;
	list<unsigned int> unaListaIds;
	string unTermino = *itTerminos;
	bool primeraVez = true;

	while (itTerminos != listaTerminos.end()) {
		unTermino = *itTerminos;
		string pathHash = carpetaRaiz + "hash_palabras";
		Hash hashTerminos(pathHash);
		cout << "busco termino :" << unTermino << endl;
		Registro* regPalabra = hashTerminos.buscar(unTermino);
		//si un termino no tiene documentos asociados entonces la busqueda es imposible
		if (regPalabra == NULL) {
			cout << "La Busqueda no Obtuvo Resultados con todas esas palabras"<< endl;
			return;
		} else {
			unOffset = regPalabra->getAtributosEnteros()->front();
			ListasIds().obtenerListaIds(unOffset, &unaListaIds);
			if (primeraVez) {
				//si es la primera vez entonces la lista de todos los ids es la que acabo de levantar
				idsTodos = unaListaIds;
			}
			list<unsigned int >::iterator it = unaListaIds.begin();
			cout << "lista encontrada de esta palabra " << endl;
			while ( it != unaListaIds.end() ){
				cout << *it << '-';
				it++;
			}
			cout << endl;



			// la nueva lista de todos los ids que cumplen tener todos los terminos es la interseccion
			// del actual(unaListaIds) con el que venia siendo la interseccion previa(idsTodos)
			idsTodos = resolverInterseccion(idsTodos, unaListaIds);
		}

		list<unsigned int >::iterator it = idsTodos.begin();
		cout << "Ids hasta ahora " << endl;
		while ( it != idsTodos.end() ){
			cout << *it << '-';
			it++;
		}
		cout << endl;

		itTerminos++;
		primeraVez = false;
		unaListaIds.clear();
	}
	//a la salida de este while tengo en idsTodos los documentos que contienen a todos los terminos
	if ( idsTodos.size() == 0) {
		//busqueda no satisfactoria
		cout << "La Busqueda no Obtuvo Resultados con todas esas palabras" << endl;
		return;
	}



	//Ahora debo iterar por estos documentos y asignar un peso a cada uno de ellos
	list<unsigned int>::iterator itDocsARankear = idsTodos.begin();

	float unPeso;
	list<unsigned int> listaDocsRankeados;
	list<float> listaPesos;
	list<unsigned int>::iterator itDocs;
	list<float>::iterator itPesos;
	while ( itDocsARankear != idsTodos.end()){
		cout<<"calculo peso del doc "<< *itDocsARankear <<endl;
		unPeso = this->calcularPesoxProximidad(listaTerminos,*itDocsARankear);
		cout<<"ok calculado ... dio"<< unPeso << *itDocsARankear <<endl;
		itDocs = listaDocsRankeados.begin();
		itPesos = listaPesos.begin();

		while (unPeso < *itPesos) {
			itPesos++;
			itDocs++;
		}
		listaDocsRankeados.insert(itDocs, *itDocsARankear);
		listaPesos.insert(itPesos, unPeso);

		itDocsARankear++;
	}

	//aca tengo 2 listas con los contenidos ordenados por pesos, una de pesos y otras de doc

	imprimirConsulta(listaDocsRankeados);

}

list<unsigned int> ProcesadorConsultas::resolverInterseccion(list<unsigned int> vieja, list<unsigned int> nueva) {

	list<unsigned int> resultado;
	vieja.sort();
	nueva.sort();

	list<unsigned int>::iterator itNuevo = nueva.begin();
	list<unsigned int>::iterator itViejo = vieja.begin();
	bool encontrado = false;
	bool imposible = false;

	unsigned int aVerificar;

	while (itViejo != vieja.end()) {
		encontrado = false;
		imposible = false;
		aVerificar = *itViejo;

		while (!encontrado && !imposible) {
			if (*itNuevo == aVerificar)
				encontrado = true;
			else {
				if (*itNuevo > aVerificar)
					imposible = true;
				else
					itNuevo++;

			}
		}

		if (encontrado) {
			resultado.push_back(*itViejo);
		}
		itViejo++;
	}

	return resultado;

}

void ProcesadorConsultas::imprimirConsulta(list<unsigned int> docRankeados){
	list<unsigned int>::iterator itRanking= docRankeados.begin();
	cout<<"La consulta ha arrojado resultados en los siguientes libros, "
			"ordenados de acuerdo a relevancia"<<endl<<endl;
	while(itRanking!=docRankeados.end()){
		cout<<*itRanking<<endl;
		itRanking++;
	}
}

void ProcesadorConsultas::procesar(list<string> terminos){
	if (terminos.size()==0)
		cout<<"No se encontraron resultados para la consulta"<<endl;
	else if (terminos.size()==1)
		consultaUnitaria(terminos.front());
		else consultaPorTerminosCercanos(terminos);
}

