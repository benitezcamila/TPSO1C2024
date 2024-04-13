#include "main.h"
#include <configuracion/config.h>
#include <bits/pthreadtypes.h>
#include <config.h>


#define IP_MEMORIA 201291

#define PUERTO_MEMORIA 012012

#define PUERTO_ESCUCHA 1000402

int inicializar_cpu(){
    return inicializar_servidor(PUERTO_ESCUCHA, "CPU - inicio");//retorna fd
}
void procesos_cpu(){
    pthread_t hilo_memoria, hilo_kernel_dispatch, hilo_kernel_interrupcion;
pthread_create(&hilo_memoria, NULL,(void *) establecer_conexion_memoria, NULL);
pthread_create(&hilo_kernel_dispatch, NULL,(void*) escucha_KD, NULL); 
pthread_create(&hilo_kernel_interrupcion, NULL,(void *) escucha_KI, NULL);
 
}

void establecer_conexion_memoria(){
    int fd_memoria = crear_conexion(IP_MEMORIA,PUERTO_MEMORIA);
    int cod_op;//Lectura/Escritura Memoria Obtener Marco TLB Hit y TLB Miss
    log_info(logger_cpu, "Conectado-CPU-memoria");
    send(fd_memoria, &cod_op, sizeof(int), MSG_WAITALL);

}

void escucha_KI(){
    int socket_server = inicializar_cpu();
    int *fd_conexion_ptr = malloc(sizeof(int));
    *fd_conexion_ptr = accept(socket_server, NULL, NULL);
    int estado = 0;
    while(estado != EXIT_FAILURE){
        estado = atender_cliente(fd_conexion_ptr);
    }  
}
void escucha_KD(){
    int socket_server = inicializar_cpu();
    int *fd_conexion_ptr = malloc(sizeof(int));
    *fd_conexion_ptr = accept(socket_server, NULL, NULL);
    int estado = 0;
    while(estado != EXIT_FAILURE){
    estado = enviar_log_D(fd_conexion_ptr);
    }  
}
int enviar_log_D( int fd_conexion_ptr){
    int cod_op = recibir_operacion(fd_conexion_ptr);
    if(cod_op == COD_OP_FETCH ) log_info(logger_cpu, "PID: <PID> - FETCH - Program Counter: <PROGRAM_COUNTER>");
    else return EXIT_FAILURE;
    return 0;
}
int enviar_log_I( int fd_conexion_ptr){
    int cod_op = recibir_operacion(fd_conexion_ptr);
    if(cod_op == COD_OP_INSTRUCCION_EJECUTADA ) log_info(logger_cpu, "PID: <PID> - Ejecutando: <INSTRUCCION> - <PARAMETROS>");
    else return EXIT_FAILURE;
    return 0;
}

 




