
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



typedef enum
{
	MENSAJE,
	PAQUETE,
	COD_OP_CREACION_TABLA,
	COD_OP_DESTRUCCION_TABLA,
	COD_OP_ACCESO_TABLA,
	COD_OP_AMPLIACION_PROCESO,
	COD_OP_REDUCION_PROCESO,
	COD_OP_ACCESO_ESPACIO_USUARIO,
	COD_OP_FETCH,
	COD_OP_INSTRUCCION_EJECUTADA,
	COD_OP_,

}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

int iniciar_servidor(char*,char*);
int esperar_cliente(int);

int recibir_operacion(int);
void* recibir_buffer(int*, int);
void recibir_mensaje(int);
t_list* recibir_paquete(int);
void* serializar_paquete(t_paquete* paquete, int bytes);
int crear_conexion(char *ip, char* puerto);
void enviar_mensaje(char* mensaje, int socket_cliente);
void crear_buffer(t_paquete* paquete);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
void liberar_conexion(int socket_cliente);

#endif
