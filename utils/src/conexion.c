#include "conexion.h"

int iniciar_servidor(char* puerto){
	int socket_servidor;

	struct addrinfo hints, *servinfo;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, puerto, &hints, &servinfo);

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

	// Asociamos el socket a un puerto
	bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

	// Escuchamos las conexiones entrantes
	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);
	return socket_servidor;
}

int esperar_cliente(int socket_servidor,t_log* log_conexiones,char* nom_cliente){
	int socket_cliente = accept(socket_servidor, NULL, NULL);

	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->offset = 0;
	cod_handshake* msg_recibido = malloc(sizeof(cod_handshake));
	uint32_t* len_cliente = malloc(sizeof(uint32_t));
	recv(socket_cliente, &(paquete->codigo_operacion),sizeof(op_code),MSG_WAITALL);
	
	if(paquete->codigo_operacion == HANDSHAKE){
		recv(socket_cliente,&(paquete->buffer->size),sizeof(uint32_t),MSG_WAITALL);
		paquete->buffer->stream = malloc(paquete->buffer->size);
		recv(socket_cliente, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
		char* string_aux = buffer_read_string(paquete->buffer,len_cliente);
		strcpy(nom_cliente,string_aux);
		free(string_aux);
		buffer_read(paquete->buffer,msg_recibido,sizeof(cod_handshake));
	}
	else{
		log_info(log_conexiones,"Codigo de operacion incorrecto en el Handshake");
	}

	log_info(log_conexiones,"Se conecto cliente %s",nom_cliente );
	
	if(*msg_recibido ==  CODIGO){
		cod_handshake* codigo = malloc(sizeof(cod_handshake));
		*codigo = OK;
		send(socket_cliente,codigo,sizeof(cod_handshake),0);
		log_info(log_conexiones,"El codigo del Handshake es correcto");
		free(codigo);
	}
	else{
		cod_handshake* codigo = malloc(sizeof(cod_handshake));;
		*codigo = FALLO;
		send(socket_cliente,codigo,sizeof(cod_handshake),0);
		log_info(log_conexiones,"El codigo del Handshake es incorrecto");
		free(codigo);
	}

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
	free(len_cliente);

	return socket_cliente;
}


int recibir_operacion(int socket_cliente){
	int cod_op;
		if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
			return cod_op;
		else{
			close(socket_cliente);

			return -1;
		}
}


int crear_conexion(char *ip, char* puerto,t_log* log_conexiones,char* nom_cliente){
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	// Ahora que tenemos el socket, vamos a conectarlo
	while(1){
		if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == 0){
			break;
		}
		else{
			sleep(1);
		}
	}

	int bytes_enviados = enviar_hanshake(socket_cliente,nom_cliente);
	cod_handshake* msg_recibido = malloc(sizeof(cod_handshake));
	recv(socket_cliente,msg_recibido,sizeof(cod_handshake),MSG_WAITALL);
	
	
	if(*msg_recibido == OK){
		log_info(log_conexiones,"El handshake fue exitoso");
	}
	else if (*msg_recibido == FALLO){
		log_info(log_conexiones,"El handshake fallo del lado del servidor");
	}
	else{
		log_info(log_conexiones,"El handshake fallo por alguna razon");
	}

	freeaddrinfo(server_info);
	free(msg_recibido);

	return socket_cliente;
}


void liberar_conexion(int socket_cliente){
	close(socket_cliente);
}

int enviar_hanshake(int socket,char* nom_cliente){
    t_paquete* paquete = malloc(sizeof(t_paquete));
	cod_handshake* codigo = malloc(sizeof(cod_handshake));
	*codigo = CODIGO;
	
    paquete->codigo_operacion = HANDSHAKE;
	uint32_t len_cliente = string_length(nom_cliente)+1;
    paquete->buffer = buffer_create(sizeof(cod_handshake)+sizeof(uint32_t)+len_cliente);
	buffer_add_string(paquete->buffer,len_cliente,nom_cliente);
	buffer_add(paquete->buffer,codigo,sizeof(cod_handshake));
	void* a_enviar = a_enviar_create(paquete);

	int bytes = send(socket,a_enviar,paquete->buffer->size + sizeof(uint32_t) + sizeof(op_code),0);

	free(codigo);
	free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);
	free(a_enviar);
	
	return bytes;
}

t_buffer* recibir_todo_elbuffer(int socket_conexion ){
	uint32_t size;
	if(recv(socket_conexion, &size, sizeof(uint32_t), MSG_WAITALL)> 0){
		t_buffer* buffer = buffer_create(size);
		if(recv(socket_conexion, buffer->stream, buffer->size,MSG_WAITALL )>0){
			return buffer;
		
		}}else{
		printf("Pifiaste negro");
		exit(EXIT_FAILURE);
		}return NULL;
}

