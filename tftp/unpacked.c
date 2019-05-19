
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main (){

		char auxbuf[80] = "1texto.txt0octet0";
		char auxbuf2[80] = "1t0ex0to.txt0octet0";
		char auxbuf3[80] = "01t0ex0to0.t0xt0octet0";
		int a_size = strlen(auxbuf);

		const char s[2] = "0";
   		char *token = strtok(auxbuf+1, s);
   		char *pfname, *pmode;

   		int flagorden = 0;
	 	
	    /* walk through other tokens */
	    while(token  != NULL ) {
	      printf( " %s\n", token );
	     	//a = token;
	     	//memcpy(pfname, token, sizeof(token));
	     	flagorden++;
	        token = strtok(NULL, s);
	        /*if(token != NULL){
	        	// b=token;
	        	//memcpy(pmode, token, strlen(token)+1);
	        	flagorden++;
	        	token = strtok(NULL, s);
	        }*/
	    }
	    /* This loop will show that there are zeroes in the str after tokenizing */
	    int i;
	    printf("\n");
	for (int i = 0; i < a_size; i++)
	{
		printf("%d ", auxbuf[i]); /* Convert the character to integer, in this case
							   the character's ASCII equivalent */
	}
	printf("\n");
	int f=0, r=0;
	for(i=a_size; i>0; i++){
		if(auxbuf[i] == "0" && r < 2){
			
			r++;
		}
	}
	   /* if (flagorden%2 == 1){ //Ordenar las ocurrencias
	    	char aux[80]; 
	    	memcpy(aux, pfname, sizeof(pfname));
	    	memmove(pfname, pmode, sizeof(pfname));
	    	memmove(pmode, aux, sizeof(aux));
	    }*/

	    //printf("cadenas: %s %s\n", pfname, pmode);
	return 0;
}