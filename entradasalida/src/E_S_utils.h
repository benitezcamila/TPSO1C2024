#ifndef E_S_UTILS_H
#define E_S_UTILS_H

#include <utils/conexion.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/temporal.h>
#include <unistd.h>
#include <configuracion/config.h>
#include <pthread.h>

typedef struct {
    int socket_memoria;
    int socket_kernel;
}str_sockets;

 

extern str_sockets sockets;
extern char* nombre_interfaz;

//void proceso_E_S();
void establecer_conexion_memoria();
void establecer_conexion_kernel();
//void iniciar_interfaz();
#endif