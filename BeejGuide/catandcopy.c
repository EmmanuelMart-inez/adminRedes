#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (){

	short a = 1;
	char b[10] = "txt.c0r0";
	char c[20];
	sprintf(c, "%d%s", a, b);

	printf("%s\n", c); 

}
//Es importante poner el tamaño exacto de la cadena más 1 espacio para caracter nulo