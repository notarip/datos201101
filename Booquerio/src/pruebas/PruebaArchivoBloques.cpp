#include <iostream>
#include "../archivos/ArchivoBloques.h"
#include "../archivos/Bloque.h"
#include "../archivos/Registro.h"
#include "string.h"

using namespace std;

int main2() {

	ArchivoBloques arch("testArchivoBloques", 128);

	/* CREO UN PAR DE REGISTROS*/

	Registro reg("El planeta de los simios", 2);
	reg.agregarId(8);
	reg.agregarReferencia(5);
	// tamanio = 24 + 2*4 + 2*1 = 34 bytes
	Registro reg2("La vida es bella", 1);
	reg2.agregarReferencia(20);
	reg2.agregarReferencia(25);
	// tamanio = 16 + 1*4 + 2*2 = 24 bytes


	Registro reg3("Harry Potter y la Piedra Filosofal", 3);
	// tamanio = 34 + 1*4 = 38 bytes

	Registro reg4(4, 300);
	// tamanio = 4 + 2 = 6 bytes

	Registro reg5("El Codigo Da Vinci");
	reg5.agregarReferencia(410);
	// tamanio = 18 + 2*1 = 20 bytes

	/*ARMO UN PAR DE BLOQUES*/

	Bloque bloq1(reg);
	bloq1.agregarRegistro(reg2);
	// tamanio = 4 + 16*2 + 34 + 24 = 94 bytes

	Bloque bloq2(reg3);
	bloq2.agregarRegistro(reg4);
	bloq2.agregarRegistro(reg5);
	// tamanio = 4 + 16*3 + 20 + 6 + 38= 116 bytes

	Bloque bloq3(reg);
	bloq3.agregarRegistro(reg2);
	bloq3.agregarRegistro(reg3);
	bloq3.agregarRegistro(reg4);
	bloq3.agregarRegistro(reg5);
	// tamanio = 4 + 16*5 + 34 + 24 + 20 + 6 + 38 = 206 bytes  ===> no entra

	/*PRUEBA DE TAMANIOS*/

	cout << "PRUEBA DE TAMANIOS" << endl << endl;

	cout <<"ocupacion bloque1 : " <<arch.getOcupacionBloque(&bloq1) << " bytes" << endl;
	cout <<"ocupacion bloque2 : " <<arch.getOcupacionBloque(&bloq2) << " bytes" << endl;
	cout <<"ocupacion bloque3 : " <<arch.getOcupacionBloque(&bloq3) << " bytes" << endl;


	if (arch.getOcupacionBloque(&bloq1) == (float)94 / 128) {
		cout << "bloque1 							[OK]" << endl;
	} else {
		cout << "bloque1 							[WRONG]" << endl;
	}
	if (arch.getOcupacionBloque(&bloq2) == (float)116 / 128) {
		cout << "bloque2								[OK]" << endl;
	} else {
		cout << "bloque2 								[WRONG]" << endl;
	}
	if (arch.getOcupacionBloque(&bloq3) == (float)206 / 128) {
		cout << "bloque3 							[OK]" << endl;
	} else {
		cout << "bloque3 							[WRONG]" << endl;
	}

	arch.grabarBloque(&bloq1, 0);
	arch.grabarBloque(&bloq2, 1);
	arch.grabarBloque(&bloq2, 4);

	try {
		arch.grabarBloque(&bloq3,3);

	} catch (ExceptionBloque& e) {
		cout << e.what() << endl;
		cout << "TRANKI ESTA OK QUISISTE GUARDAR UN BLOQUE MUY GRANDE" << endl << endl;

	}


	/* LEVANTO LOS BLOQUES PARA VER QUE ONDA*/


	Bloque* bloq1Restored = arch.recuperarBloque(0);
	Bloque* bloq2Restored = arch.recuperarBloque(1);
	Bloque* bloq2Restoredtwo = arch.recuperarBloque(4);


	/*VERIFICACION DE LOS DATOS*/

	cout << "VERIFICACION DE LOS DATOS" << endl << endl;

	/************************************************************************************************/
	cout << "BLOQUE-1 " << endl;

	list<Registro>* listreg = bloq1Restored->obtenerRegistros();
	list<Registro>::iterator it = listreg->begin();

	list<unsigned int>::iterator itIds;
	list<unsigned int>::iterator itIdsRestored;

	list<unsigned short int>::iterator itRefs;
	list<unsigned short int>::iterator itRefsRestored;

	cout << "registro1" << endl;
	Registro reg1Restored = *it;
	it++;

	if (reg1Restored.getString() == reg.getString()) {
		cout << "string1 							[OK]" << endl;
	} else {
		cout << "string1 							[WRONG]" << endl;
	}

	itIds = reg.getIdentificadores()->begin();
	itIdsRestored = reg1Restored.getIdentificadores()->begin();

	while (itIds != reg.getIdentificadores()->end()) {
		if ((*itIds) == (*itIdsRestored)) {
			cout << "id 								[OK]" << endl;
		} else {
			cout << "id 								[WRONG]" << endl;
		}
		itIds++;
		itIdsRestored++;
	}

	itRefs = reg.getReferencias()->begin();
	itRefsRestored = reg1Restored.getReferencias()->begin();

	while (itRefs != reg.getReferencias()->end()) {
		if ((*itRefs) == (*itRefsRestored)) {
			cout << "ref 								[OK]" << endl;
		} else {
			cout << "ref 								[WRONG]" << endl;
		}
		itRefs++;
		itRefsRestored++;
	}

	cout << endl;

	cout << "registro2" << endl;
	Registro reg2Restored = *it;
	it++;

	if (reg2Restored.getString() == reg2.getString()) {
		cout << "string2 							[OK]" << endl;
	} else {
		cout << "string2 							[WRONG]" << endl;
	}

	itIds = reg2.getIdentificadores()->begin();
	itIdsRestored = reg2Restored.getIdentificadores()->begin();

	while (itIds != reg2.getIdentificadores()->end()) {
		if ((*itIds) == (*itIdsRestored)) {
			cout << "id 								[OK]" << endl;
		} else {
			cout << "id 								[WRONG]" << endl;
		}
		itIds++;
		itIdsRestored++;
	}

	itRefs = reg2.getReferencias()->begin();
	itRefsRestored = reg2Restored.getReferencias()->begin();

	while (itRefs != reg2.getReferencias()->end()) {
		if ((*itRefs) == (*itRefsRestored)) {
			cout << "ref 								[OK]" << endl;
		} else {
			cout << "ref 								[WRONG]" << endl;
		}
		itRefs++;
		itRefsRestored++;
	}

	cout << endl;

	/************************************************************************************************/


	cout << "BLOQUE-2 " << endl;

	listreg = bloq2Restored->obtenerRegistros();
	it = listreg->begin();

	cout << "registro1" << endl;
	reg1Restored = *it;
	it++;

	if (reg1Restored.getString() == reg3.getString()) {
		cout << "string1 							[OK]" << endl;
	} else {
		cout << "string1 							[WRONG]" << endl;
	}

	itIds = reg3.getIdentificadores()->begin();
	itIdsRestored = reg1Restored.getIdentificadores()->begin();

	while (itIds != reg3.getIdentificadores()->end()) {
		if ((*itIds) == (*itIdsRestored)) {
			cout << "id 								[OK]" << endl;
		} else {
			cout << "id 								[WRONG]" << endl;
		}
		itIds++;
		itIdsRestored++;
	}

	cout << endl;

	cout << "registro2" << endl;
	reg2Restored = *it;
	it++;

	if (reg2Restored.getString() == reg4.getString()) {
		cout << "string2 							[OK]" << endl;
	} else {
		cout << "string2 							[WRONG]" << endl;
	}

	itIds = reg4.getIdentificadores()->begin();
	itIdsRestored = reg2Restored.getIdentificadores()->begin();

	while (itIds != reg4.getIdentificadores()->end()) {
		if ((*itIds) == (*itIdsRestored)) {
			cout << "id 								[OK]" << endl;
		} else {
			cout << "id 								[WRONG]" << endl;
		}
		itIds++;
		itIdsRestored++;
	}

	itRefs = reg4.getReferencias()->begin();
	itRefsRestored = reg2Restored.getReferencias()->begin();

	while (itRefs != reg4.getReferencias()->end()) {
		if ((*itRefs) == (*itRefsRestored)) {
			cout << "ref 								[OK]" << endl;
		} else {
			cout << "ref 								[WRONG]" << endl;
		}
		itRefs++;
		itRefsRestored++;
	}

	cout << endl;


	/************************************************************************************************/
	cout << "registro3" << endl;
	Registro reg3Restored = *it;


	if (reg3Restored.getString() == reg5.getString()) {
		cout << "string2 							[OK]" << endl;
	} else {
		cout << "string2 							[WRONG]" << endl;
	}

	itIds = reg5.getIdentificadores()->begin();
	itIdsRestored = reg3Restored.getIdentificadores()->begin();

	while (itIds != reg5.getIdentificadores()->end()) {
		if ((*itIds) == (*itIdsRestored)) {
			cout << "id 								[OK]" << endl;
		} else {
			cout << "id 								[WRONG]" << endl;
		}
		itIds++;
		itIdsRestored++;
	}

	itRefs = reg5.getReferencias()->begin();
	itRefsRestored = reg3Restored.getReferencias()->begin();

	while (itRefs != reg5.getReferencias()->end()) {
		if ((*itRefs) == (*itRefsRestored) ) {
			cout << "ref 								[OK]" << endl;
		} else {
			cout << "ref 								[WRONG]" << endl;
		}
		itRefs++;
		itRefsRestored++;
	}

	cout << endl << endl;



	delete bloq1Restored;
	delete bloq2Restored;
	delete bloq2Restoredtwo;
	return 0;
}
