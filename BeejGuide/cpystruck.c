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

int main (){
	char *fname = "hola";
	char *mode = "octec";
	struct tftp_rrq *r = malloc(TFTP_RRQ_LEN(fname,mode));
	r->opcode = OPCODE_RRQ;
	sprintf(r->req, "%s%s", fname, mode);
	//char *a = malloc(TFTP_RRQ_LEN(fname,mode));
	//memcpy(a, r, TFTP_RRQ_LEN); 
	struct tftp_rrq *rq = malloc(TFTP_RRQ_LEN(fname,mode));
	memcpy(rq, r, TFTP_RRQ_LEN(fname,mode));
	printf("req:%s\n", rq->req);
	printf("opcode:%d\n", rq->opcode);

	return 0;
}