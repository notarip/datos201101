/*
 * Parser.cpp
 *
 *  Created on: 26/03/2011
 *      Author: pablo
 */

#include "Parser.h"

Parser::Parser() {


}


Parser::~Parser()
{

	stopWords.clear();

}

int Parser::parsear(string archivo)
{

	fstream archLibro;

	archLibro.open(archivo.c_str(), ios::binary | ios::in);
	if (archLibro.is_open())
	{
		procesarLibro(&archLibro,archivo);
		archLibro.close();
		unsigned int id = Util().generarIdNuevo();
		this->libro = new Libro(id,this->titulo,this->editorial,this->autor,this->texto);
		return EXITOSO;
	}
	return ARCHIVO_INVALIDO;
}

set<string> *Parser::obtenerPalabras(Libro unLibro)
{
	this->texto = unLibro.getTexto();

	return this->obtenerPalabras();

}

set<string> *Parser::obtenerPalabras()
{

	fstream archStopW;
	string path_stopW = Parametros().getParametro(TAG_PATH_STOP_WORDS);

	archStopW.open(path_stopW.c_str(), ios::binary | ios::in);
	if (archStopW.is_open())
	{
		levantarCSV(&archStopW,&stopWords);
		archStopW.close();
	}


	return this->procesarPalabras();

}

void Parser::procesarLibro(fstream *archLibro, string archivo)
{

    string linea;
    int tamanio;

    archLibro->seekg(0, ios::end);
	tamanio = archLibro->tellg();
	archLibro->seekg(0);

	char* buff = new char[tamanio];
	archLibro->read(buff, tamanio);

	linea.assign(buff);

	delete [] buff;


	this->obtenerAutorTitulo(archivo);
	this->obtenerEditorial();

	//levanta texto

	this->texto = linea;
}


void Parser::obtenerAutorTitulo(string nombre)
{
	unsigned int posBarra = nombre.find_last_of("/\\");
	nombre.erase(0, posBarra+1);

	unsigned int  posGuion = nombre.find('-',0);
	string autor = nombre.substr(0,posGuion - 1);
	string titulo = nombre.substr(posGuion + 1, nombre.length() - posGuion - 5);

	this->autor = Util().toLower(Util().trim(autor));
	this->titulo = Util().toLower(Util().trim(titulo));

}



Libro *Parser::getLibro()
{
	return this->libro;
}

void Parser::obtenerEditorial()
{


	string rutaEd = Parametros().getParametro(ARCHIVO_EDITORIALES);
	list<string> editoriales;
	fstream arcEd;

	arcEd.open(rutaEd.c_str(), ios::binary | ios::in);
	if (arcEd.is_open())
	{
		levantarCSV(&arcEd,&editoriales);
		arcEd.close();
	}

	unsigned int factor = atoi(Parametros().getParametro(FACTOR_EDITORIALES).c_str());
	if (factor == 0)
		factor = 1;


	unsigned int nroEditorial = (((unsigned int)(this->autor.at(0))) - 97)/factor; // en ascii a = 97
	unsigned int i = 0;
	for (list<string>::iterator it = editoriales.begin(); it != editoriales.end(); it++ )
	{
		if (i == nroEditorial)
		{
			this->editorial = Util().toLower((*it));
			break;
		}else
			i++;
	}
}

set<string> *Parser::procesarPalabras()
{
	set<string> *palabras = new set<string>();

	unsigned int posIni = 0;
	unsigned int posFin;
	unsigned int largo = this->texto.length();
	string palabra;

	while (posFin <= largo)
	{
		posFin = encontrarFinPalabra(posIni);
		palabra = this->texto.substr(posIni,posFin - posIni);
		procesarPalabra(palabra, palabras);
		posIni = posFin + 1;
	}

	return palabras;
}


void Parser::procesarPalabra(string palabra, set<string>* palabras)
{


	//reemplaza los inavalidos por espacios
	unsigned int pos = palabra.find_first_of(INVALIDOS);
	while (pos != string::npos)
	{
		palabra[pos] = ' ';
		pos = palabra.find_first_of(INVALIDOS, pos+1);
	}

	//reemplaza los  \n por espacios
	for (unsigned int j = 0; j < palabra.size();j++)
			if (palabra.at(j) == '\n' || palabra.at(j) == '\r' || palabra.at(j) == '\0')
					palabra[j] = ' ';

	//cambia tilde
	palabra = Util().sinTilde(palabra);

	//a minuscula
	palabra = Util().toLower(palabra);


	//saco espacios al pricipio y al final
	palabra = Util().trim(palabra);

	if (palabra.size() != 0)
	{
		//analiza si quedaron dos o mas palabras (ejemplo: www fi uba ar)
		//quedo un espacio en el medio
		//esto funciona por que antes hice el trim
		unsigned int posFin = palabra.find(' ',0);
		string palabraLimpia;

		while (posFin != string::npos)
		{
			palabraLimpia = palabra.substr(0,posFin+1);
			this->guardarPalabra(palabraLimpia, palabras);
			palabra.erase(0,posFin+1);
			posFin = palabra.find(' ',0);
		}

		this->guardarPalabra(palabra, palabras);
	} //palabra.size() != 0
}


void Parser::guardarPalabra(string palabra, set<string> *palabras)
{
	if (palabra[0] != '0' && atoi(palabra.c_str()) == 0)
		if(!this->buscarStopWord(palabra))
			palabras->insert(palabra);
}

bool Parser::buscarStopWord(string palabra)
{
	return (bool)this->stopWords.count(palabra);
}

unsigned int Parser::encontrarFinPalabra(unsigned int posIni)
{

	unsigned int posFinEspacio = this->texto.find(' ',posIni);
	unsigned int posFinEndL = this->texto.find('\n',posIni);
	unsigned int posFinEndF = this->texto.find('\0',posIni);

	if (posFinEspacio != string::npos && posFinEndL != string::npos)
		return posFinEspacio < posFinEndL ? posFinEspacio:posFinEndL;
	else
		if (posFinEspacio != string::npos)
			return posFinEspacio;
		else
			if (posFinEndL != string::npos )
				return posFinEndL;
			else
				return posFinEndF;

}


void Parser::levantarCSV(fstream *archivo, list<string> *lista)
{
    string linea;
    int tamanio;
    archivo->seekg(0, ios::end);
    tamanio = archivo->tellg();
    archivo->seekg(0);

    char* buff = new char[tamanio];
    archivo->read(buff, tamanio);
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

}

void Parser::levantarCSV(fstream *archivo, set<string> *set)
{
    string linea;
    int tamanio;
    archivo->seekg(0, ios::end);
    tamanio = archivo->tellg();
    archivo->seekg(0);

    char* buff = new char[tamanio];
    archivo->read(buff, tamanio);
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
		set->insert(palabra);
		posIni = posFin + 1;
    }

}

void Parser::listarStopWords()
{

	for (set<string>::iterator it = stopWords.begin(); it != stopWords.end(); it++)
		cout << (*it).c_str() << endl;

}


void Parser::listarLibro(bool conTexto)
{
	cout << this->autor << endl;
	cout << this->editorial << endl;
	cout << this->titulo << endl;
	if (conTexto)
		cout << this->texto << endl;

}



