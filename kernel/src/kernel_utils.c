#include "main.h"
#include <config.h>
#define PUERTO_ESCUCHA 300021
#define IP_MEMORIA ""
#define PUERTO_MEMORIA ""

#define IP_CPU ""
#define PUERTO_CPU_DISPATCH ""
#define PUERTO_CPU_INTERRUPT ""

int iniciar_kernel(){
    return iniciar_servidor(PUERTO_ESCUCHA, "KERNEL");
}

void procesos_kernel(){
    
    pthread_t thread_memoria,thread_cpu;
    
    pthread_create(&thread_memoria,
                    NULL,
                    (void*)conexion_memoria,
                    NULL);
                    
    pthread_detach(thread_memoria);
    
    pthread_create(&thread_cpu,
                    NULL,
                    (void*)conexion_cpu,
                    NULL);
                    
    pthread_detach(thread_cpu);

}

void conexion_memoria(){
    int fd_memoria = crear_conexion(IP_MEMORIA,PUERTO_MEMORIA);
    int cod_op;
    log_info(logger_kernel, "Conectado-memoria");
    send(fd_memoria, &cod_op, sizeof(int), MSG_WAITALL);

}
void conexion_cpu(){
    int fd_cpu_distpach = crear_conexion(IP_CPU,PUERTO_CPU_DISPATCH);
    log_info(logger_kernel, "CONEXION CPU-distpach ");
    int fd_cpu_interrupt = crear_conexion(IP_CPU,PUERTO_CPU_INTERRUPT);
    log_info(logger_kernel, "CONEXION CPU-interrupt ");
}