#include <stdio.h>
#include <string.h>
//
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
//NOOOO sirve para archivos con 0 intermedios dado que este es el separador
//mejorar en version siguiente
int main()
{
	//Desempaquetado de request write or read
	char str[] = "1Beej-Programacion-en-Redes.pdf0octet0";
		//char auxbuf2[80] = "1t0ex0to.txt0octet0";
		//char auxbuf3[80] = "01t0ex0to0.t0xt0octet0";
	int init_size = strlen(str);
	char delim[] = "0";
	char pre[20];
	char copy[20];
	char *ptr = strtok(str+1, delim); //evitamos el opcode

	while(ptr != NULL)
	{
		//printf("'%s'\n", ptr);
		strcpy(pre, ptr);
		ptr = strtok(NULL, delim);
		if(ptr != NULL){
			//printf("'%s'\n", ptr);
			strcpy(copy, ptr);
			ptr = strtok(NULL, delim);
		}
	}

	/* This loop will show that there are zeroes in the str after tokenizing */
	/*for (int i = 0; i < init_size; i++)
	{
		printf("%d ", str[i]); /* Convert the character to integer, in this case
							   the character's ASCII equivalent 
	}*/
	printf("\n");
	printf("copy: %s\n", copy);
	printf("\nprecopy: %s\n", pre);

	// calcular paquetes
	struct stat st;
	char pre1[20] = "textoprueba.txt";
	stat(pre1, &st);
	int size = st.st_size;
	printf("El tamaño es: %d\n", size);
	int packs=0, packend=0;
	packs = size/512; 
	packend = size%512;
	int totalpaks=0;
	if(packend > 0)
		totalpaks = packs+1;
	else
		totalpaks = packs;
	printf("\nCada paquete de datos es de maximo tamaño:512 bytes,total:%d, total de paquetes completos:%d y 1 paquete de:%d\n",totalpaks, packs, packend);
	//leer y escribir los paquetes en un archivo
	int i;
	//char archivo[] = "texto.txt";
	FILE *ai, *ao;
	char buff[512];
	ai = fopen("textoprueba.txt", "rb"); 
	ao = fopen("out.txt", "wb"); 
	if(ai == NULL || ao == NULL){
		fprintf(stderr, "\nError opening file \n");
		exit(1);
	}
	for(i=0; i<totalpaks;i++){
		if(i<packs){ 
			fread(buff, 512, 1, ai);
			fwrite(buff, 512, 1, ao);
		}else{
			fread(buff, packend, 1, ai);
			fwrite(buff, packend, 1, ao);
			//Contemplar el envio del '\0' siendo este un posible error, solucion: un +/- 1 en stringlen
		}
	}
	fclose(ai);
	fclose(ao);
	return 0;
}