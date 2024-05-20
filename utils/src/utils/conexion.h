
#ifndef CONEXION_H_
#define CONEXION_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>
#include "serializacion.h"

typedef enum {
    CODIGO,
    OK,
    FALLO
}cod_handshake;


int iniciar_servidor(char*);
int esperar_cliente(int,t_log*,char*);
int recibir_operacion(int);
int crear_conexion(char *, char* ,t_log*,char*);
void liberar_conexion(int);
int enviar_hanshake(int,char*);
int recibir_entero(int);
void* serializar_paquete(t_paquete* paquete, int bytes);

void recibir_mensaje(int,t_log*);
void* serializar_paquete(t_paquete* paquete, int bytes);
t_list* recibir_paquete(int);
void* recibir_buffer(int*, int);
void enviar_mensaje(char* mensaje, int socket_cliente);
void crear_buffer(t_paquete* paquete);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void eliminar_paquete(t_paquete* paquete);


#endif
