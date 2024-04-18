#include "cpu_utils.h"

int socket_server_dispatch;
int socket_server_interrupt;

inicializar_cpu_dispatch(){
    socket_server_dispatch = iniciar_servidor(string_itoa(configuracion.PUERTO_ESCUCHA_DISPATCH));
    log_info(logger_conexiones, "CPU DISPATCH esta escuchando");
    return socket_server_dispatch;
}
inicializar_cpu_interrupt(){
    socket_server_interrupt = iniciar_servidor(string_itoa(configuracion.PUERTO_ESCUCHA_INTERRUPT));
    log_info(logger_conexiones, "CPU INTERRUPT esta escuchando");
    return socket_server_interrupt;
}
void procesos_cpu(){
    pthread_t hilo_memoria, hilo_kernel_dispatch, hilo_kernel_interrupcion;
pthread_create(&hilo_kernel_dispatch, NULL,(void*) escucha_KD, NULL); 
pthread_create(&hilo_kernel_interrupcion, NULL,(void *) escucha_KI, NULL);
pthread_create(&hilo_memoria, NULL,(void *) establecer_conexion_memoria, NULL);
pthread_join(hilo_kernel_interrupcion, NULL);
pthread_join(hilo_kernel_dispatch,NULL);
pthread_join(hilo_memoria,NULL);
}

void establecer_conexion_memoria(){
    int fd_memoria = crear_conexion(configuracion.IP_MEMORIA,string_itoa(configuracion.PUERTO_MEMORIA));
    int cod_op;//Lectura/Escritura Memoria Obtener Marco TLB Hit y TLB Miss
    log_info(logger_conexiones, "Conectado-CPU-memoria");
    //enviar_mensaje("Hola soy la CPU",fd_memoria);
    //send(fd_memoria, &cod_op, sizeof(int), MSG_WAITALL);
}

void escucha_KI(){
    int *fd_conexion_ptr = malloc(sizeof(int));
    *fd_conexion_ptr = esperar_cliente(socket_server_interrupt);
    log_info(logger_conexiones, "Se conecto un cliente KI");
    /*int estado = 0;
    while(estado != EXIT_FAILURE){
        estado = enviar_log_I(fd_conexion_ptr);
    }  */
    free(fd_conexion_ptr);
}

void escucha_KD(){
    socket_server_dispatch = iniciar_servidor(string_itoa(configuracion.PUERTO_ESCUCHA_DISPATCH));
    log_info(logger_conexiones, "CPU Dispatch esta escuchando");
    int *fd_conexion_ptr = malloc(sizeof(int));
    *fd_conexion_ptr = esperar_cliente(socket_server_dispatch);
    log_info(logger_conexiones, "Se conecto un cliente KD");
    int estado = 0;
    /*while(estado != EXIT_FAILURE){
    estado = enviar_log_D(fd_conexion_ptr);
    } */ 
    free(fd_conexion_ptr);
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

 




