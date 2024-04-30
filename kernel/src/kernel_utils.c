#include "kernel_utils.h"

str_sockets sockets;

pthread_t conexion_CPU_I,conexion_CPU_D, conexion_memoria;

void establecer_conexion_memoria()
{
    int fd_memoria = crear_conexion(configuracion.IP_MEMORIA, string_itoa(configuracion.PUERTO_MEMORIA),logger_conexiones);
   // int cod_op;
    log_info(logger_conexiones, "Conectado Kernel memoria");
    sockets.socket_memoria = fd_memoria;
    //enviar_mensaje("Hola soy el kernel", fd_memoria);
    //send(fd_memoria, &cod_op, sizeof(int), MSG_WAITALL);
}
void establecer_conexion_cpu_D()
{
    int fd_cpu_distpach = crear_conexion(configuracion.IP_CPU, string_itoa(configuracion.PUERTO_CPU_DISPATCH),logger_conexiones);
    log_info(logger_conexiones, "Conectado Kernel Cpu_Dispatch");
    sockets.socket_CPU_D = fd_cpu_distpach;
   // int cod_op;
    //enviar_mensaje("Hola soy el kernel", fd_cpu_distpach);
    //send(fd_cpu_distpach, &cod_op, sizeof(int), MSG_WAITALL);
}

void establecer_conexion_cpu_I()
{
    int fd_cpu_interrupt = crear_conexion(configuracion.IP_CPU, string_itoa(configuracion.PUERTO_CPU_INTERRUPT),logger_conexiones);
    log_info(logger_conexiones, "Concetado Kernel CPU-interrupt ");
    sockets.socket_CPU_I = fd_cpu_interrupt;
    //int cod_op;
    //enviar_mensaje("Hola soy el kernel", fd_cpu_interrupt);
    //send(fd_cpu_interrupt, &cod_op, sizeof(int), MSG_WAITALL);
}

void inicializar_kernel(){
    sockets.socket_server = iniciar_servidor( string_itoa(configuracion.PUERTO_ESCUCHA));
    log_info(logger_conexiones, "Kernel esta escuchando");
} 

void escucha_E_S(){
    int *fd_conexion_ptr = malloc(sizeof(int));
    *fd_conexion_ptr = esperar_cliente(sockets.socket_server,logger_conexiones);
    sockets.socket_cliente_E_S = *fd_conexion_ptr;
    log_info(logger_conexiones, "Se conecto un cliente Entrada/SAlida");
    int estado =0;
    /*while(estado != EXIT_FAILURE){
         estado = atender_cliente(fd_conexion_ptr);
    }   */
    free(fd_conexion_ptr);
}


void establecer_conexiones(){
    pthread_create(&conexion_memoria,NULL,(void*)establecer_conexion_memoria,NULL);
    pthread_create(&conexion_CPU_D,NULL,(void*)establecer_conexion_cpu_D,NULL);
    pthread_create(&conexion_CPU_I,NULL,(void*)establecer_conexion_cpu_I,NULL);

    pthread_join(conexion_memoria,NULL);
    pthread_join(conexion_CPU_D,NULL);
    pthread_join(conexion_CPU_I,NULL);
}

/*
void procesos_kernel()
{

    pthread_t thread_memoria, thread_cpu_I, thread_cpu_D, thread_E_S;

    pthread_create(&thread_E_S,
                NULL,
                (void *)escucha_E_S,
                NULL);
    
    pthread_create(&thread_memoria,
                   NULL,
                   (void *)establecer_conexion_memoria,
                   NULL);
    
    pthread_create(&thread_cpu_I,
                   NULL,
                   (void *)establecer_conexion_cpu_I,
                   NULL);

    pthread_create(&thread_cpu_D,
                   NULL,
                   (void *)establecer_conexion_cpu_D,
                   NULL);

    pthread_join(thread_E_S,NULL);
    
    pthread_join(thread_memoria,NULL);

    pthread_join(thread_cpu_I,NULL);

    pthread_join(thread_cpu_D,NULL);
}
*/