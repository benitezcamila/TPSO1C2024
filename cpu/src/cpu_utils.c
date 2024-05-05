#include "cpu_utils.h"

struct sockets sockets;

void iniciar_server_kernel(){
    pthread_t dispatch, interrupt;
    pthread_create(&dispatch,NULL,(void*)inicializar_cpu_dispatch,NULL);
    pthread_create(&interrupt,NULL,(void*)inicializar_cpu_interrupt,NULL);
    pthread_join(dispatch,NULL);
    pthread_join(interrupt,NULL);
}

void inicializar_cpu_dispatch(){
    sockets.socket_server_D = iniciar_servidor(string_itoa(configuracion.PUERTO_ESCUCHA_DISPATCH));
    log_info(logger_conexiones, "CPU DISPATCH esta escuchando");
}
    
void inicializar_cpu_interrupt(){
    sockets.socket_server_I = iniciar_servidor(string_itoa(configuracion.PUERTO_ESCUCHA_INTERRUPT));
    log_info(logger_conexiones, "CPU INTERRUPT esta escuchando");
    
}

void establecer_conexion_memoria(){
    int fd_memoria = crear_conexion(configuracion.IP_MEMORIA,string_itoa(configuracion.PUERTO_MEMORIA),logger_conexiones,CPU);
    int cod_op;//Lectura/Escritura Memoria Obtener Marco TLB Hit y TLB Miss
    log_info(logger_conexiones, "Conectado-CPU-memoria");
    sockets.socket_memoria = fd_memoria;
    //enviar_mensaje("Hola soy la CPU",fd_memoria);
    //send(fd_memoria, &cod_op, sizeof(int), MSG_WAITALL);
}

void escuchar_conexiones(){
    pthread_t ki, kd;
    pthread_create(&ki,NULL,(void*) escucha_KI,NULL);
    pthread_create(&kd,NULL,(void*) escucha_KD,NULL);
    pthread_join(ki,NULL);
    pthread_join(kd,NULL);
}

void escucha_KI(){
    int *fd_conexion_ptr = malloc(sizeof(int));
    *fd_conexion_ptr = esperar_cliente(sockets.socket_server_I,logger_conexiones);
    log_info(logger_conexiones, "Se conecto un cliente KI");
    sockets.socket_cliente_KI = *fd_conexion_ptr;
    /*int estado = 0;
    while(estado != EXIT_FAILURE){
        estado = enviar_log_I(fd_conexion_ptr);
    }  */
    free(fd_conexion_ptr);
}

void escucha_KD(){
    int *fd_conexion_ptr = malloc(sizeof(int));
    *fd_conexion_ptr = esperar_cliente(sockets.socket_server_D,logger_conexiones);
    log_info(logger_conexiones, "Se conecto un cliente KD");
    sockets.socket_cliente_KD = *fd_conexion_ptr;
    int estado = 0;
    /*while(estado != EXIT_FAILURE){
    estado = enviar_log_D(fd_conexion_ptr);
    } */ 
    free(fd_conexion_ptr);
}


/*
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

*/

/*
void procesos_cpu(){
    pthread_t hilo_memoria, hilo_kernel_dispatch, hilo_kernel_interrupcion;
pthread_create(&hilo_kernel_dispatch, NULL,(void*) escucha_KD, NULL); 
pthread_create(&hilo_kernel_interrupcion, NULL,(void *) escucha_KI, NULL);
pthread_create(&hilo_memoria, NULL,(void *) establecer_conexion_memoria, NULL);
pthread_join(hilo_kernel_interrupcion, NULL);
pthread_join(hilo_kernel_dispatch,NULL);
pthread_join(hilo_memoria,NULL);
}
*/



