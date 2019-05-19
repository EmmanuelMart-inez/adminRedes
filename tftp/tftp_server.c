/*
** listener.c -- a datagram sockets "server" demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
//
#include <sys/stat.h>
#include "tftp.h"

#define TFTP_TYPE_GET 0
#define TFTP_TYPE_PUT 1

/* Should cover most needs */
#define MSGBUF_SIZE (TFTP_DATA_HDR_LEN + BLOCK_SIZE)

#define MYPORT "4950"	// the port users will be connecting to

#define MAXBUFLEN 550

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("listener: socket");
			continue;
		}

		// obviar el mensaje "address already in use" (la dirección ya se está
		// usando)
		int yes=1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
		sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}

	freeaddrinfo(servinfo);

	printf("listener: waiting to recvfrom...\n");

	addr_len = sizeof their_addr;
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
		(struct sockaddr *)&their_addr, &addr_len)) == -1) {
		perror("recvfrom");
		exit(1);
	}

	printf("listener: got packet from %s\n",
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s));
	printf("listener: packet is %d bytes long\n", numbytes);
	buf[numbytes] = '\0';

	/*Procesamiento del paquete*/
		//Desempaqueteado_v2.0
		struct tftp_rrq *rrq = malloc(numbytes); //checar
		memcpy(rrq, buf, numbytes);
		printf("listener: packet contains \"%d%s\"\n", rrq->opcode, rrq->req);
		printf("---opcode:%d\n", rrq->opcode);
		printf("---req:%s\n", rrq->req);
		char str[150];
		strcpy(str, rrq->req); //request cadena!!
			//Desempaquetado de request write or read
		//char str[] = "1Beej-Programacion-en-Redes.pdf0octet0";
			//char auxbuf2[80] = "1t0ex0to.txt0octet0";
			//char auxbuf3[80] = "01t0ex0to0.t0xt0octet0";
		int init_size = strlen(str);
		char delim[] = "0";
		char pre[20];
		char copy[20];
		char *ptr = strtok(str, delim); //evitamos el opcode

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
		printf("\n");
		printf("copy: %s\n", copy);
		printf("\nprecopy: %s\n", pre);


		// calcular paquetes
		struct stat st;
		char pre1[20] = "texto.txt";
		//char pre1[50];
		//strcpy(pre1, pre); //fname!
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
		
		//desempaquetado_v1.0
		/*
		short popcode;
		//char *pfname;
		char aux[3];
		char auxbuf[40];
		memcpy(auxbuf, buf, numbytes+1);
		memcpy(aux, buf, 2);
		popcode = atoi(aux);
		//printf("El paquete es de OP:%d", popcode);
		*/

		//Empaquetar paquete -> v2: empaquetar mensaje msg struck
		int i;
		//char archivo[] = "texto.txt";
		FILE *ai, *ao=NULL;
		char buff[512];
		ai = fopen("texto.txt", "rb"); 
		//ao = fopen("out.txt", "wb"); 
		if(ai == NULL || ao != NULL){
			fprintf(stderr, "\nError opening file \n");
			exit(1);
		}/*
		for(i=0; i<totalpaks;i++){
			if(i<packs){ 
				fread(buff, 512, 1, ai);
				//fwrite(buff, 512, 1, ao);
			}else{
				fread(buff, packend, 1, ai);
				//fwrite(buff, packend, 1, ao);
				//Contemplar el envio del '\0' siendo este un posible error, solucion: un +/- 1 en stringlen
			}
		}*/
		fread(buff, packend, 1, ai);
		struct tftp_data *data = malloc(TFTP_DATA_HDR_LEN + packend);
		data->opcode = OPCODE_DATA;
		data->blocknr = 1;
		memcpy(data->data, buff, packend); 
		fclose(ai);
		//Enviar DATA PACKET
		int num;
		if ((num = sendto(sockfd, (const char*)data, TFTP_DATA_HDR_LEN + packend, 0,
			 (struct sockaddr *)&their_addr, addr_len)) == -1) {
			perror("talker: sendto");
			exit(1);
		}
		printf("\nBytes enviados:%d\n", num);

		struct timeval timeout;
		int nbytes;
		int retval;
		/* After the connection request we should start receiving data
	 * immediately */

        /* Set a timeout for resending data. */

        timeout.tv_sec = TFTP_TIMEOUT;
        timeout.tv_usec = 0;

        fd_set readfds;			
				FD_ZERO(&readfds);
				FD_SET(sockfd, &readfds);
				retval = select(sockfd+1, &readfds, NULL, NULL, &timeout);
				// no nos preocupemos de writefds and exceptfds:
				/*if (FD_ISSET(tc->sock+1, &readfds)){
					printf("A block or akc was send!\n");
				}
				else{
					printf("Timed out.\n");
					//resend
					if(tc->type == TFTP_TYPE_GET)
						tftp_send_ack(tc);
					else if(tc->type == TFTP_TYPE_PUT)
						tftp_send_data(tc, -1); //checar re send
				}*/
				if (retval == -1)
					perror("select()");
 				else if(retval){
 					printf("Data is available now.\n");
               		/* FD_ISSET(0, &rfds) will be true. */
					if ((nbytes = recv(sockfd, buf, MAXBUFLEN-1 , 0)) <= 0){
						if (nbytes == 0) {
						// conexión cerrada
						printf("selectserver: socket %d hung up\n", sockfd+1);
						} else {
						perror("recv");
						}
						//close(i); // ¡Hasta luego!
						//FD_CLR(i, &master); // eliminar del conjunto maestro
					}else{
						printf("\nDatos guardados en el buffer tc->msgbuf\n");
						printf("listener: packet is %d bytes long\n", nbytes);
						//tc->msgbuf[nbytes] = '\0';
					}
          		}else{
               		printf("No data within five seconds.\n");
               		//Resend block or ack
               		 	//Es el mismo paquete, "size"
               			//resend = -1;
               	}

		//Calcular packetes/

	close(sockfd);

	return 0;
}
