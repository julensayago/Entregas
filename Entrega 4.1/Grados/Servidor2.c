#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	
	
	memset(&serv_adr, 0, sizeof(serv_adr)); // inicia serv_addr a 0
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); //htonl formatea numero recibido al formato necesario
	serv_adr.sin_port = htons(9000);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	int i;
	// Bucle infinito
	for (;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		
		int terminar =0;
		while (terminar ==0)
		{
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido\n");
			peticion[ret]='\0'; //Marca fin de string, no escribe lo posterior en el buffer
			printf ("Peticion: %s\n",peticion);
			char *p = strtok( peticion, "/");
			int codigo = atoi (p);
			char nombre[20];
			int grados;
			int resultado;
			
			if (codigo !=0)
			{
				p = strtok( NULL, "/");
				strcpy (nombre, p);
				grados=atof(nombre);
				// Ya tenemos el nombre
				printf ("Codigo: %d, Grados: %d\n", codigo, grados);
			}
			
			if (codigo ==0) //peticion de desconexion
				terminar=1;
			
			else if (codigo ==1) //cambiar de fahrenheit a celsius
			{
				resultado=(grados-32)*5/9;
				sprintf(respuesta, "Grados en Celsius: %d",resultado);
			}
			
			else if (codigo ==2) //cambiar de celsius a fahrenheit
			{
				resultado=(grados*9/5)+32;
				sprintf(respuesta, "Grados en Fahrenheit: %d",resultado);
			}
				if (codigo !=0)
				{
					printf ("Respuesta: %s\n", respuesta);
					write (sock_conn,respuesta, strlen(respuesta));
				}
		}
		//fin servicio para este cliente
		close(sock_conn); 
	}
}
