/*
 * Parametros.h
 *
 *  Created on: 25/03/2011
 *      Author: pablo
 */

#ifndef PARAMETROS_H_
#define PARAMETROS_H_

#define RUTA_CONFIG "./archivos/conf/config.propiedades"
#define CARPETA_DATOS "path_datos"
#define CARPETA_LIBROS "path_libros"
#define ARCHIVO_STOPWORDS "path_stopWords"
#define ARCHIVO_EDITORIALES "path_editoriales"
#define FACTOR_EDITORIALES "factor_editoriales"
#define CARPETA_ARC_LIBROS "path_arc_libros"
#define CARPETA_SALIDA "path_salidas"


#include <string>
#include <iostream>
#include <fstream>
using namespace std;
/*
 * Levanta propiedades de un archivo de configuracion
 * llamado config.ini que esta en el mismo directorio
 * de la aplicacion.
 * Formato:
 *
 * PROPIEDAD_1=datos de la propiedad 1;
 * PROPIEDAD_2=datos de la propiedad 2;
 * */
class Parametros {
public:
	/*
	 *Recibe por parametro el id de la propiedad que se
	 *Recibe necesita (ej. PROPIEDAD_1) y devuelve el valor
	 *de la propiedada (ej. datos de la propiedad 1)
	 * */
	static string getParametro (string idParametro);
private:
	static string buscarParametro(fstream *archivo, string idParametro);
};

#endif /* PARAMETROS_H_ */



