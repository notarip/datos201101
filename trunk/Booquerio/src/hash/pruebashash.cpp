#include "Hash.h"

int main(){



	Hash hash("unhash");


	Registro* reg0=new Registro("0");
	for (int i=0; i<500;i++){
		reg0->agregarAtribEntero(i);
	}

	hash.insertar(reg0);


	Registro* reg1=new Registro("1");
	for (int i=0; i<500;i++){
		reg1->agregarAtribEntero(i);
	}

	hash.insertar(reg1);

	hash.mostrar();


	Registro* reg2=new Registro("2");
	for (int i=0; i<500;i++){
		reg2->agregarAtribEntero(i);
	}

	hash.insertar(reg2);

	hash.mostrar();


	Registro* reg3=new Registro("3");
	for (int i=0; i<500;i++){
		reg3->agregarAtribEntero(i);
	}

	hash.insertar(reg3);


	Registro* reg4=new Registro("4");
	for (int i=0; i<500;i++){
		reg4->agregarAtribEntero(i);
	}

	hash.insertar(reg4);

	hash.mostrar();


	Registro* reg5=new Registro("6");
	for (int i=0; i<500;i++){
		reg5->agregarAtribEntero(i);
	}

	hash.insertar(reg5);


	hash.mostrar();

	hash.borrar("0");
	hash.borrar("4");

	hash.mostrar();


}
