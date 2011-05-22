/*
 * Util.cpp
 *
 *  Created on: 27/03/2011
 *      Author: pablo
 */

#include "Util.h"

string Util::toUpper(string cadena)
{

    string cadenaUpper;

    for ( unsigned int i = 0 ; i < cadena.size() ; i++ ) {

        char letraActual = (char)toupper(cadena[i]);
        cadenaUpper += letraActual;
    }
    return cadenaUpper;
}

string Util::toLower(string cadena)
{
    string cadenaLower;

    for ( unsigned int i = 0 ; i < cadena.size() ; i++ ) {

        char letraActual = (char)tolower((cadena[i]));
        cadenaLower += letraActual;
    }
    return cadenaLower;
}


string Util::trim(string cadena)
{

	//borra los del principio
	while (cadena.size() > 0 && cadena.at(0) == ' ')
		cadena.erase(0,1);

	//borra los del final
	while (cadena.size() > 0 && cadena.at(cadena.length()-1) == ' ')
		cadena.erase(cadena.length()-1,1);

	return cadena;

}

string Util::sinTilde(string cadena)
{


	string invalidos("\341\351\355\363\372\361\301\311\315\323\332\344\353\357\366\374");
	string reemp;


	unsigned int pos = cadena.find_first_of(invalidos);
	while (pos != string::npos)
	{
		reemp.clear();
		reemp.append(cadena.substr(0,pos));
		reemp.append(convertir(cadena[pos]));
		reemp.append(cadena.substr(pos+1,cadena.size() - pos - 1));
		cadena  = reemp;
		pos = cadena.find_first_of(invalidos);
	}



	return cadena;
}

bool Util::existeArchivo(string archivo)
{
	fstream arc;

	arc.open(archivo.c_str(), ios::in);
	if (arc.is_open())
	{
		arc.close();
		return true;
	}else
		return false;

}


string Util::convertir(char letra)
{
	if (letra == '\341') //a
		return "a";
	if (letra == '\351') //e
		return "e";
	if (letra == '\355') //i
		return "i";
	if (letra == '\363') //o
		return "o";
	if (letra == '\372') //u
		return "u";



	if (letra == '\361') //ñ
		return "ñ";

	if (letra == '\301') //A
			return "A";
	if (letra == '\311') //E
			return "E";
	if (letra == '\315') //I
			return "I";
	if (letra == '\323') //O
			return "O";
	if (letra == '\332') //U
			return "U";

	if (letra == '\344') //ä
			return "a";
	if (letra == '\353') //ë
			return "e";
	if (letra == '\357') //ï
			return "i";
	if (letra == '\366') //ö
			return "o";
	if (letra == '\374') //ü
			return "u";

}

list<string>* Util::getArchivos(string carpeta)
{

	DIR *directorio;
	struct dirent *elemento;
	list<string> *archivos = new list<string>();

	directorio = opendir(carpeta.c_str());

	while (elemento = readdir(directorio))
	{
		if (elemento->d_type == DT_REG)
			archivos->push_back(elemento->d_name);
	}

	return archivos;

}

list<string>* Util::getCarpetas(string carpeta)
{
	DIR *directorio;
	struct dirent *elemento;
	list<string> *carpetas = new list<string>();

	directorio = opendir(carpeta.c_str());

	while (elemento = readdir(directorio))
	{
		if (elemento->d_type == DT_DIR)
			carpetas->push_back(elemento->d_name);
	}

	return carpetas;

}

unsigned int Util::generarIdNuevo()
{
	 fstream archivo;
	 string ruta = Parametros().getParametro(RUTA_ID);
	 unsigned int id;
	 archivo.open(ruta.c_str(), ios::in | ios::out | ios::binary);
	 unsigned int nuevo;
	 if (archivo.is_open())
	 {

		char* buff = new char[sizeof(unsigned int)];

		archivo.read(buff, sizeof(unsigned int));
		memcpy(&id, buff, sizeof(unsigned int));
		nuevo = id + 1;
		archivo.seekp(0,ios::beg);
		memcpy(buff, &nuevo, sizeof(unsigned int));
		archivo.write(buff, sizeof(unsigned int));
		delete [] buff;
		archivo.close();
	 }
	 else {
		 //si el archivo no existe devuelvo un id = 0 y lo escribo
		 archivo.open(ruta.c_str(),ios::out | ios::binary);
		 int aux = 0;
		 archivo.write((char*)&aux,sizeof(unsigned int));
		 archivo.close();
		 nuevo = 0;

	 }

	 return nuevo;

}


void Util::reiniciarId(unsigned int unId)
{
	 fstream archivo;
	 string ruta = Parametros().getParametro(RUTA_ID);
	 archivo.open(ruta.c_str(), ios::in | ios::out | ios::binary);
	 if (archivo.is_open())
	 {
		unsigned int id = unId;
		char* buff = new char[sizeof(unsigned int)];
		memcpy(buff,&id,sizeof(unsigned int));
		archivo.write(buff, sizeof(unsigned int));
		delete [] buff;
		archivo.close();
	 }

}

string Util::DoubleToString(double numero)
{
        stringstream ss;

        ss << numero;
        return ss.str();
}

string Util::UIntToString(unsigned int numero)
{
        stringstream ss;

        ss << numero;
        return ss.str();
}

void Util::levantarCSV(string ruta , list<string> *lista)
{
    string linea;
    int tamanio;
    fstream archivo;
    archivo.open(ruta.c_str(), ios::binary | ios::in);
    archivo.seekg(0, ios::end);
    tamanio = archivo.tellg();
    archivo.seekg(0);

    char* buff = new char[tamanio];
    archivo.read(buff, tamanio);
    linea.assign(buff);
    delete [] buff;


    unsigned int posIni = 0;
    unsigned int posFin;
    bool fin = false;
    string palabra;

    while (!fin)
    {
		posFin = linea.find(',',posIni);
		if (posFin == string::npos) //no encontro la coma (caso de ultima palabra)
		{
			posFin = linea.find('\n',posIni);
			fin = true;
		}

		palabra = linea.substr(posIni, posFin - posIni); //string a almacenar
		lista->push_back(palabra);
		posIni = posFin + 1;
    }
    archivo.close();

}
