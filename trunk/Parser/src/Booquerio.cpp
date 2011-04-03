/*
 * Booquerio.cpp
 *
 *  Created on: 03/04/2011
 *      Author: pablo
 *      Funcionalidad
 * Archivo de configuración (con directorio de almacenamiento)
 * Tomar Texto: ./ejecutable -i "archivo de texto"
 * Procesar Editorial: ./ejecutable -e (procesa los no procesados)
 * Procesar Autor: ./ejecutable –a
 * Procesa Título: ./ejecutable –t
 * Procesa Palabras: ./ejecutable –p
 * Listar Archivos Tomados: ./ejecutable –l (muestra identificador, Título, Autor,
 * Editorial y cantidad de palabras registradas para ese libro).
 * Obtener Archivo: ./ejecutable -o ID_Archivo
 * Quita Archivo: ./ejecutable -q ID_Archivo (se elminan las entradas en los
 * otros índices)
 * Ver Estructura: Genera archivos en forma de texto plano, que describen las
 * estructuras y contenidos de los archivos de almacenamiento y control del
 * sistema.
 * - ./ejecutable -v [-e árbol de Editorial, -a árbol de Autor, -t hash de
 * Título, -p hash de Palabra] "Nombre Archivo"
 * - Nombre y estructuras para los archivos generados:
 * - Archivo de Estructura de control: "Nombre Archivo"_Índice,
 * "Nombre Archivo"_tabla.
 * - Archivos de control de espacios libres: "Nombre
 * Archivo"_libres.
 * - Archivos de bloques de datos: "Nombre Archivo"_datos.
 * - Estructura: para árboles la indicada en teórica. Separadores:
 * Bloques con "|", Registros con ";", atributos con ",".
 *
 */


#define ERROR_DE_LLAMADA -10
//3 parametros
#define TOMAR_TEXTO 10
//2 parametros
#define PROCESAR_EDITORIAL 20
#define PROCESAR_AUTOR 30
#define PROCESAR_TITULO 40
#define PROCESAR_PALABRAS 50
#define LISTAR_ARCHIVOS_TOMADOS 60
//3 parametros
#define OBTENER_ARCHIVO 70
#define QUITAR_ARCHIVO 80
#define VER_ESTRUCTURA_EDITORIAL 90
#define VER_ESTRUCTURA_AUTOR 91
#define VER_ESTRUCTURA_TITULO 92
#define VER_ESTRUCTURA_PALABRA 93




int determinar_operacion(int cant_parm, char** parm)
{

	switch(cant_parm)
	{
	case 2:/*return ope 2 parm*/;break;
	case 3:/*return ope 3 parm*/;break;
	default:/*return error en los parametros*/;break;
	}
	return 0;
}


int main_2 (int argc, char** argv)
{

	int operacion =	determinar_operacion(argc, argv);

	switch(operacion)
	{
	case TOMAR_TEXTO:/**/;break;
	case PROCESAR_AUTOR:/**/;break;
	case PROCESAR_EDITORIAL:/*tomar texto*/;break;
	case PROCESAR_PALABRAS:/*tomar texto*/;break;
	case PROCESAR_TITULO:/*tomar texto*/;break;
	case LISTAR_ARCHIVOS_TOMADOS:/*tomar texto*/;break;
	case OBTENER_ARCHIVO:/*tomar texto*/;break;
	case QUITAR_ARCHIVO:/*tomar texto*/;break;
	case VER_ESTRUCTURA_AUTOR:/*tomar texto*/;break;
	case VER_ESTRUCTURA_EDITORIAL:/*tomar texto*/;break;
	case VER_ESTRUCTURA_PALABRA:/*tomar texto*/;break;
	case VER_ESTRUCTURA_TITULO:/*tomar texto*/;break;
	default: ;break;
	}

	return 0;

}
