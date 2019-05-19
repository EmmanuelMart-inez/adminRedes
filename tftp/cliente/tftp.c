/* A tftp client implementation.
   Author: Erik Nordström <erikn@it.uu.se>
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>

#include "tftp.h"

extern int h_errno;

#define TFTP_TYPE_GET 0
#define TFTP_TYPE_PUT 1

/* Should cover most needs */
#define MSGBUF_SIZE (TFTP_DATA_HDR_LEN + BLOCK_SIZE)


/*
 * NOTE:
 * In tftp.h you will find definitions for headers and constants. Make
 * sure these are used throughout your code.
 */


/* A connection handle */
struct tftp_conn {
	int type; /* Are we putting or getting? */
	FILE *fp; /* The file we are reading or writing */
	int sock; /* Socket to communicate with server */
	int blocknr; /* The current block number */
	char *fname; /* The file name of the file we are putting or getting */
	char *mode; /* TFTP mode */
	struct sockaddr_in peer_addr; /* Remote peer address */
	socklen_t addrlen; /* The remote address length */
	char msgbuf[MSGBUF_SIZE]; /* Buffer for messages being sent or received */
};

/* Close the connection handle, i.e., delete our local state. */
void tftp_close(struct tftp_conn *tc)
{
	if (!tc)
		return;

	fclose(tc->fp);
	close(tc->sock);
	free(tc);
}

/* Connect to a remote TFTP server. */
struct tftp_conn *tftp_connect(int type, char *fname, char *mode,
			       const char *hostname)
{	printf("\nPA2\n");
    struct addrinfo hints;
    struct addrinfo * res = NULL;
	struct tftp_conn *tc;

	if (!fname || !mode || !hostname)
		return NULL;

	tc = malloc(sizeof(struct tftp_conn));

	if (!tc)
		return NULL;

	/* Create a socket.
	 * Check return value. */

	/* ... */
	/*#.#*/
	printf("\nPA3\n");
	if ((tc->sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		fprintf(stderr, "Error socket create\n");
		perror("\nsocket");
		return NULL;
		//		perror("socket");
		//		exit(1);
	}
	/*#.#*/
	printf("\nPA4\n");
	/*if (type == TFTP_TYPE_PUT)
		tc->fp = fopen(fname, "rb");
	else if (type == TFTP_TYPE_GET)
		tc->fp = fopen(fname, "wb");
	else {
		fprintf(stderr, "Invalid TFTP mode, must be put or get\n");
		return NULL;
	}
	printf("\nPA5\n");
	if (tc->fp == NULL) {
		fprintf(stderr, "File I/O error!\n");
		close(tc->sock);
		free(tc);
		return NULL;
	}*/

    memset(&hints,0,sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    char port_str[5];
    sprintf(port_str, "%d", TFTP_PORT);

    /* get address from host name.
	 * If error, gracefully clean up.*/
    printf("\nPA6\n");
	/* ... */ 	
	/*#.#*/
	int rv;
	if ((rv = getaddrinfo(hostname, port_str, &hints, &res)) != 0) {
		printf("\nPA61\n");
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		//freeaddrinfo(res);
		fclose(tc->fp);
		close(tc->sock);
		free(tc);
		return NULL;
	}
	/*#.#*/

	/* Assign address to the connection handle.
	 * You can assume that the first address in the hostent
	 * struct is the correct one */

	memcpy(&tc->peer_addr, res->ai_addr, res->ai_addrlen);

	tc->addrlen = sizeof(struct sockaddr_in);
	printf("\nPA7\n");
	tc->type = type;
	tc->mode = mode;
	tc->fname = fname;
	tc->blocknr = 0;

	memset(tc->msgbuf, 0, MSGBUF_SIZE);
	printf("\nPA8\n");
	return tc;
}

/*
  Send a read request to the server.
  1. Format message.
  2. Send the request using the connection handle.
  3. Return the number of bytes sent, or negative on error.
 */
int tftp_send_rrq(struct tftp_conn *tc)
{
	/* struct tftp_rrq *rrq; */

        /* ... */
	/*#.#*/
	int numbytes; //Number of bytes send
	struct tftp_rrq *rrq = malloc(TFTP_RRQ_LEN(tc->fname, tc->mode));
	rrq->opcode = OPCODE_RRQ;
	//Packing format
	sprintf(rrq->req, "%s%d%s%d", tc->fname, 0, tc->mode, 0);
	memcpy(tc->msgbuf, rrq, TFTP_RRQ_LEN(tc->fname, tc->mode));
	//printf("req:%s\n", rrq->req);

	
	if ((numbytes = sendto(tc->sock, rrq, TFTP_RRQ_LEN(tc->fname, tc->mode), 0,
			 (struct sockaddr *)&tc->peer_addr, tc->addrlen)) == -1) {
		perror("talker: sendto");
		//exit(1);
		return -1;
	}
	/*#.#*/

        return numbytes;
}
/*

  Send a write request to the server.
  1. Format message.
  2. Send the request using the connection handle.
  3. Return the number of bytes sent, or negative on error.
 */
int tftp_send_wrq(struct tftp_conn *tc)
{
	/* struct tftp_wrq *wrq; */

        /* ... */
	/*#.#*/
	int numbytes; //Number of bytes send
	struct  tftp_wrq *wrq = malloc (TFTP_WRQ_LEN(tc->fname, tc->mode));
	wrq->opcode = OPCODE_WRQ;
	//format pack
	sprintf(wrq->req, "%s%d%s%d", tc->fname, 0, tc->mode, 0);
	memcpy(tc->msgbuf, wrq, sizeof(wrq->req));

	if ((numbytes = sendto(tc->sock, wrq, TFTP_WRQ_LEN(tc->fname, tc->mode), 0,
			 (struct sockaddr *)&tc->peer_addr, tc->addrlen)) == -1) {
		perror("talker: sendto");
		//exit(1);
		return -1;
	}
	/*#.#*/
	
        return numbytes;
}


/*
  Acknowledge reception of a block.
  1. Format message.
  2. Send the acknowledgement using the connection handle.
  3. Return the number of bytes sent, or negative on error.
 */
int tftp_send_ack(struct tftp_conn *tc)
{
	/* struct tftp_ack *ack; */
	/*#.#*/
	int numbytes;
	struct tftp_ack *ack = malloc(TFTP_ACK_HDR_LEN);
	ack->opcode = OPCODE_ACK;
	memcpy(&ack->blocknr, &tc->blocknr, sizeof(u_int16_t));
	printf("\n .   .   .Ack number:%d\n", ack->blocknr); 

	if ((numbytes = sendto(tc->sock, ack, TFTP_ACK_HDR_LEN, 0,
			 (struct sockaddr *)&tc->peer_addr, tc->addrlen)) == -1) {
		perror("talker: sendto ack");
		//exit(1);
		return -1;
	}
	/*#.#*/
        return numbytes;
}

/*
  Send a data block to the other side.
  1. Format message.
  2. Add data block to message according to length argument.
  3. Send the data block message using the connection handle.
  4. Return the number of bytes sent, or negative on error.

  TIP: You need to be able to resend data in case of a timeout. When
  resending, the old message should be sent again and therefore no
  new message should be created. This can, for example, be handled by
  passing a negative length indicating that the creation of a new
  message should be skipped.
 */
int tftp_send_data(struct tftp_conn *tc, int length)
{
	/* struct tftp_data *tdata; */
	/*#.#*/
	if(length >= 0){
		int numbytes=0;
		struct tftp_data *tdata = malloc(TFTP_DATA_HDR_LEN+length);
		tdata->opcode = OPCODE_DATA;
		tdata->blocknr = tc->blocknr;
		memcpy(tdata->data, tc->msgbuf, length);
		if ((numbytes = sendto(tc->sock, tdata, TFTP_DATA_HDR_LEN+length, 0,
				 (struct sockaddr *)&tc->peer_addr, tc->addrlen)) == -1) {
			perror("talker: sendto data");
			//exit(1);
			return -1;
		}
	/*#.#*/
        return numbytes;
    }printf("Re send for timeout\n" );
    	return 0;
}

/*
  Transfer a file to or from the server.

 */
int tftp_transfer(struct tftp_conn *tc)
{
	int retval = 0;
	int len;
	int totlen = 0;
	struct timeval timeout;
	//
	int sposition=0, slen=4, c=0;
	char subs[4];
	short msn;
	//
	int nbytes;
	//

        /* Sanity check */
	if (!tc)
		return -1;

        len = 0;

	/* After the connection request we should start receiving data
	 * immediately */

        /* Set a timeout for resending data. */

        timeout.tv_sec = TFTP_TIMEOUT;
        timeout.tv_usec = 0;

        /* Check if we are putting a file or getting a file and send
         * the corresponding request. */

        /* ... */
        /*#.#*/
        if(tc->type == TFTP_TYPE_GET){
        	printf("we are Getting file: type=1\n");
        	tftp_send_rrq(tc);
        }
        if(tc->type == TFTP_TYPE_PUT){
        	printf("we are Putting file: type=2\n");
        	tftp_send_wrq(tc);
        }
        /*#.#*/


        /*
          Put or get the file, block by block, in a loop.
         */
	do {
		/* 1. Wait for something from the server (using
                 * 'select'). If a timeout occurs, resend last block
                 * or ack depending on whether we are in put or get
                 * mode. */

                /* ... */
				/*#.#*/
				fd_set readfds;			
				FD_ZERO(&readfds);
				FD_SET(tc->sock, &readfds);
				retval = select(tc->sock+1, &readfds, NULL, NULL, &timeout);
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
					if ((nbytes = recv(tc->sock, tc->msgbuf, sizeof(tc->msgbuf), 0)) <= 0){
						if (nbytes == 0) {
						// conexión cerrada
						printf("selectserver: socket %d hung up\n", tc->sock+1);
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
          		 //exit(EXIT_SUCCESS);


				//return 0;  !!!!!!!!!!???? esto ya venia incluido skeleto
				/*#.#*/ 


                /* 2. Check the message type and take the necessary
                 * action. */

               	struct tftp_msg *msn = malloc(nbytes);
               	memcpy(msn, tc->msgbuf, nbytes);

		switch ( msn->opcode ) {
		case OPCODE_DATA:
                        /* Received data block, send ack */
			/*#.#*/
			printf("Opcode:data\n"); 
			struct tftp_data *datamsg = malloc(nbytes);
			memcpy(datamsg, msn, nbytes);
			//memcpy(tc->blocknr, tc->msgbuf+2, 2);
			tftp_send_ack(tc);
			/*#.#*/ 
			break;
		case OPCODE_ACK:
                        /* Received ACK, send next block */
			/*#.#*/ 
			//memcpy(tc->blocknr, tc->msgbuf+2, 2);
			//tftp_send_data(tc, tc->????);
			/*#.#*/ 
			break;
		case OPCODE_ERR:
                        /* Handle error... */
			/*#.#*/ 
			printf("Error #\n");
			//fprintf(stderr, "%s\n", );
            /*#.#*/ 
                        break;
		default:
			/*#.#*/ 
			fprintf(stderr, "\nUnknown message type\n");
			/*#.#*/ 
			goto out;

		}

	} while ( 0/*#.#*//*#.#*/  /* 3. Loop until file is finished */);

	printf("\nTotal data bytes sent/received: %d.\n", totlen);
out:
	return retval;
}

int main (int argc, char **argv)
{

	char *fname = NULL;
	char *hostname = NULL;
	char *progname = argv[0];
	int retval = -1;
	int type = -1;
	struct tftp_conn *tc;

        /* Check whether the user wants to put or get a file. */
	while (argc > 0) {

		if (strcmp("-g", argv[0]) == 0) {
			fname = argv[1];
			hostname = argv[2];

			type = TFTP_TYPE_GET;
			break;
		} else if (strcmp("-p", argv[0]) == 0) {
			fname = argv[1];
			hostname = argv[2];

			type = TFTP_TYPE_PUT;
			break;
		}
		argc--;
		argv++;
	}

        /* Print usage message */
	if (!fname || !hostname) {
		fprintf(stderr, "Usage: %s [-g|-p] FILE HOST\n",
			progname);
		return -1;
	}
	printf("\nPA1\n");
        /* Connect to the remote server */
	tc = tftp_connect(type, fname, MODE_OCTET, hostname);
	printf("\nPA1\n");
	if (!tc) {
		fprintf(stderr, "Failed to connect!\n");
		return -1;
	}
	
    
        /* Transfer the file to or from the server */
	retval = tftp_transfer(tc);

        if (retval < 0) {
                fprintf(stderr, "File transfer failed!\n");
        }

        /* We are done. Cleanup our state. */
	tftp_close(tc);

	return retval;
}
