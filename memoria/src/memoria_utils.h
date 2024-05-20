#ifndef MEMORIA_UTILS_H
#define MEMORIA_UTILS_H

#include <utils/conexion.h>
#include "configuracion/config.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <semaphore.h>
#define maxiumLineLength 256


typedef struct {
    int socket_server;
    int socket_cliente_CPU;
    int socket_cliente_kernel;
    int socket_cliente_E_S;
}str_sockets;

extern char* path_kernel;

extern str_sockets sockets;
extern sem_t sem_escuchar;
extern char rutaKernel[256];
extern int socket_server;
void inicializar_memoria();
void atender_escuchas();
void escuchar_instrucciones();
int server_escuchar();
void procesar_conexion(void*);
int enviar_log(int, int);
t_list* leer_instrucciones_del_path();

typedef struct {
    int pid;
    t_list* instruccionesParaCpu;
}procesoListaInst;
extern  procesoListaInst* procesos; 

struct paquetePcb{
    int pid;
};
char* instruccionActual (procesoListaInst* proceso, int ip );
extern struct paquetePcb kernelPcb;
bool pidIguales(procesoListaInst* proceso);
void enviar_instruccion(char*);
extern t_list* listaDeProcesos;
procesoListaInst* buscar_procesoPorId(int pid);
#endif