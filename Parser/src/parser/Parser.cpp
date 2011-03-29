/*
 * Parser.cpp
 *
 *  Created on: 26/03/2011
 *      Author: pablo
 */

#include "Parser.h"

Parser::Parser() {

	fstream archStopW;
	string path_stopW = Parametros().getParametro(TAG_PATH_STOP_WORDS);

	archStopW.open(path_stopW.c_str(), ios::binary | ios::in);
	if (archStopW.is_open())
	{
		levantarStopWords(&archStopW);
		archStopW.close();
	}

}

Parser::~Parser() {

	stopWords.clear();

}

int Parser::parsear(string archivo)
{

	fstream archLibro;

	archLibro.open(archivo.c_str(), ios::binary | ios::in);
	if (archLibro.is_open())
	{
		procesarLibro(&archLibro);
		archLibro.close();

	}
	return ARCHIVO_INVALIDO;
}


list<string> *Parser::obtenerPalabras()
{

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

	unsigned int posIni = 0;
	unsigned int posTag;
	unsigned int posFin;



	//levanta el autor
	posTag = linea.find("AUTOR:",posIni);
	posTag += 6; //lenght de AUTOR:
	posFin = linea.find('\n',posTag);
	this->autor = linea.substr(posTag, posFin - posTag);
	posIni = posFin + 1;

	//levanta la editorial
	posTag = linea.find("EDITORIAL:",posIni);
	posTag += 10; //lenght de EDITORIAL:
	posFin = linea.find('\n',posTag);
	this->editorial = linea.substr(posTag, posFin - posTag);
	posIni = posFin + 1;

	//levanta el titulo
	posTag = linea.find("TITULO:",posIni);
	posTag += 7; //lenght de TITULO:
	posFin = linea.find('\n',posTag);
	this->titulo = linea.substr(posTag, posFin - posTag);
	posIni = posFin + 1;

	//levanta texto
	posFin = linea.find('\0',posTag);
	this->texto = linea.substr(posIni, posFin - posIni);

    delete [] buff;
}



list<string> *Parser::procesarPalabras()
{
	list<string> *palabras = new list<string>();

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


void Parser::procesarPalabra(string palabra, list<string>* palabras)
{

	/*Ver aca problema con los tildes
	 *
	 */
	//sin tildes
	//	palabra = Util().sinTilde(palabra);


	//reemplaza los inavalidos por espacios
	unsigned int pos = palabra.find_first_of(INVALIDOS);
	while (pos != string::npos)
	{
		palabra[pos] = ' ';
		pos = palabra.find_first_of(INVALIDOS, pos+1);
	}


/* Ver aca poblema con los \n
 *
	//reemplaza los  \n por espacios
	pos = palabra.find('\n', 0);
	while (pos != string::npos)
	{
		palabra[pos] = ' ';
		pos = palabra.find('\n',pos +1);
	}

	for (int i =0 ; i < palabra.length();i++)
	{
		if (palabra[i] == '\n')
			palabra[i] == ' ';
	}

	//reemplaza los  \n por espacios
	pos = palabra.find('\r', 0);
	while (pos != string::npos)
	{
		palabra[pos] = ' ';
		pos = palabra.find('\r',pos +1);
	}
*/

	//a minuscula
	palabra = Util().toLower(palabra);



	//analiza si quedaron dos palabras (ejemplo: potter dragon)

	unsigned int posIni = 0;
	unsigned int posFin = palabra.find(' ',posIni);

	//eliminar los del principio
	while (posFin != string::npos && posFin == posIni)
	{
		palabra.erase(posFin,1);
		posFin = palabra.find(' ',posIni);
	}

	//elimina los del final
	posFin = palabra.find_last_of(' ');
	while (posFin != string::npos && posFin == palabra.length()-1)
	{
		palabra.erase(posFin,1);
		posFin = palabra.find_last_of(' ');
	}

	//quedo un espacio en el medio ?
	posFin = palabra.find(' ',0);
	string palabraLimpia;

	if (posFin != string::npos)
	{
		palabraLimpia = palabra.substr(0,posFin-1);
		this->guardarPalabra(palabraLimpia, palabras);
		palabra.erase(0,posFin-1);
	}

	this->guardarPalabra(palabra, palabras);

}


void Parser::guardarPalabra(string palabra, list<string> *palabras)
{
	if(!this->buscarStopWord(palabra))
		palabras->push_back(palabra);
}

bool Parser::buscarStopWord(string palabra)
{

	for (list<string>::iterator it = this->stopWords.begin(); it != this->stopWords.end(); it++)
		if ((*it).compare(palabra.c_str()) == 0)
			return true;

	return false;
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

void Parser::levantarStopWords(fstream *archStopW)
{

    string linea;
    int tamanio;

    archStopW->seekg(0, ios::end);
    tamanio = archStopW->tellg();
    archStopW->seekg(0);

    char* buff = new char[tamanio];
    archStopW->read(buff, tamanio);
    linea.assign(buff);


    unsigned int posIni = 0;
    unsigned int posFin;
    bool fin = false;
    string stopWord;

    while (!fin)
    {
		posFin = linea.find(',',posIni);
		if (posFin == string::npos) //no encontro la coma (caso de ultima palabra)
		{
			posFin = linea.find('\n',posIni);
			fin = true;
		}

		stopWord = linea.substr(posIni, posFin - posIni); //string a almacenar
		this->stopWords.push_back(stopWord);
		posIni = posFin + 1;
    }

    delete [] buff;

}


void Parser::listarStopWords()
{

	for (list<string>::iterator it = stopWords.begin(); it != stopWords.end(); it++)
		cout << (*it).c_str() << endl;

}


void Parser::listarLibro()
{
	cout << this->autor << endl;
	cout << this->editorial << endl;
	cout << this->titulo << endl;
	cout << this->texto << endl;

}

void Parser::obtenerAutorTitulo(string nombre)
{
}



void Parser::obtenerEditorial()
{
}


