#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (){
	char s[30] = "01texto.txt0octect0";
	printf("Tam:%ld",strlen(s));
	short op=0;
	char aux[3];
	memcpy(aux, s, 2);
	printf("\nAux:%s\n", aux);
	printf("\n1:%c,2:%c\n", aux[0], aux[1]);
	op=atoi(aux);
	printf("\nOPcode:%d", op);

	char fname[10];
	memcpy(fname, s+2, 9);
	printf("\nfName:%s\n", fname);
	

	char mode[8];
	memcpy(mode, s+4+10, 7);
	printf("\nmodo:%s\n", mode);
	return 0;
}
//Es importante poner el tamaño exacto de la cadena más 1 espacio para caracter nulo