#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
int main (){

struct stat st;
stat("Beej-Programacion-en-Redes.pdf", &st);
int size = st.st_size;
printf("El tamaño es: %d\n", size);
	return 0;
}