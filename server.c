#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[])
{
	if (argc > 1) {
		int server_socket, client_socket, longitud_cliente, puerto;

		puerto = atoi(argv[1]);

		struct sockaddr_in server;
		struct sockaddr_in client;

		server.sin_family = AF_INET; /*Aqui se define que se usa TCP/IP*/
		server.sin_port = htons(puerto);
		server.sin_addr.s_addr = INADDR_ANY;
		bzero( &(server.sin_zero), 8);

		if((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			perror("No pude abrir el socket\n");

			return -1;
		}

		if(bind(server_socket, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
			printf("No pude abrir el puerto %s\n", argv[1]);

			return -2;
		}

		if(listen(server_socket, 5) == -1) {
			perror("No pude ponerme en modo escucha\n");

			return -3;
		}
		longitud_cliente = sizeof(struct sockaddr_in);
		
		printf("Esperando clientes...\n");
		if((client_socket = accept(server_socket, (struct sockaddr *) &client, &longitud_cliente)) == -1) {
			printf("No pudimos aceptar una conexion\n");

			return -1;
		}

		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(client.sin_addr), str, INET_ADDRSTRLEN);

		printf("Se conecto un cliente desde %s:%d. Los saludo\n", str, client.sin_port);
		send(client_socket, "Bienvenido a mi servidor.\n", 26, 0);

		printf("Saludo enviado!\n");
		shutdown(client_socket, 2);
		shutdown(server_socket, 2);
	} else {
		printf("Por favor, indique el puerto\n");

		return -5;
	}
}
