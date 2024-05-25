#include "cpu_utils.h"

int ind_contexto_kernel = 0;
sem_t sem_contexto_kernel;
str_sockets sockets;

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
    int fd_memoria = crear_conexion(configuracion.IP_MEMORIA,string_itoa(configuracion.PUERTO_MEMORIA),logger_conexiones,"CPU");
    log_info(logger_conexiones, "Conectado-CPU-memoria");
    sockets.socket_memoria = fd_memoria;
}

void atender_conexiones(){
    pthread_t ki, kd;
    pthread_create(&ki,NULL,(void*)server_escuchar,(void*)sockets.socket_server_I);
    pthread_create(&kd,NULL,(void*)server_escuchar,(void*)sockets.socket_server_D);
    pthread_join(ki,NULL);
    pthread_join(kd,NULL);
}

int server_escuchar(int server_socket) {
    char* nom_cliente = malloc(20);
    int cliente_socket = esperar_cliente(server_socket,logger_conexiones,nom_cliente);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->fd = cliente_socket;
        args->cliente_name = nom_cliente;
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
        pthread_detach(hilo);
        return 1;

    }
    return 0;
}

//PROBABLEMENTE HAYA QUE CAMBIAR COSAS.
void procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    int cliente_socket = args->fd;
    char* nombre_cliente = args->cliente_name;
    free(args);

    op_code codigo_op;
    while (cliente_socket != -1) {

        if (recv(cliente_socket, &codigo_op, sizeof(op_code), 0) != sizeof(op_code)) {
            log_info(logger_conexiones, "%s DISCONNECT!", nombre_cliente);
            free(nombre_cliente);
            
            return;
        }

        switch (codigo_op) {
        case CONTEXTO_EJECUCION:
            ind_contexto_kernel = 1;
            while(ind_contexto_kernel == 1){
                ciclo_de_instruccion();
            }
            break;

        case INTERRUPT_PROC:
            //Loggear.
            recibir_interrupcion_de_kernel();
            enviar_contexto_a_kernel();
            break;
        
        default:
            //Loggear un error.
            break;
        }
        
    }

    free(nombre_cliente);
}

void recibir_contexto_ejecucion(){
    t_paquete* paquete = malloc(sizeof(t_paquete));
    recv(sockets.socket_server_D, &(paquete->codigo_operacion), sizeof(op_code), MSG_WAITALL);

    if(paquete->codigo_operacion == CONTEXTO_EXEC){
        recv(sockets.socket_server_D, &(paquete->buffer->size), sizeof(uint32_t), MSG_WAITALL);
        paquete->buffer->stream = malloc(paquete->buffer->size);

        recv(sockets.socket_CPU_D, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
        buffer_read(paquete->buffer, contextoRegistros, sizeof(registros_CPU));
    }
    else{
        //Loggear error?
    }
}

void solicitar_instruccion_a_memoria(){
    t_paquete* paquete = crear_paquete(SOLICITUD_INSTRUCCION, sizeof(uint32_t));
    buffer_add_uint32(paquete->buffer, contextoRegistros->PC);

    enviar_paquete(paquete, sockets.socket_memoria);
}

void recibir_instruccion_de_memoria(){
    t_paquete* paquete = malloc(sizeof(t_paquete));
    recv(sockets.socket_memoria, &(paquete->codigo_operacion), sizeof(op_code), MSG_WAITALL);

    if(paquete->codigo_operacion == INSTRUCCION){
        recv(sockets.socket_memoria, &(paquete->buffer->size), sizeof(uint32_t), MSG_WAITALL);
        paquete->buffer->stream = malloc(paquete->buffer->size);

        uint32_t longitud_linea_instruccion = 0;

        recv(sockets.socket_memoria, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
        linea_de_instruccion = buffer_read_string(paquete->buffer, &longitud_linea_instruccion);
    }
    else{
        //Loggear error?
    }
}

void enviar_contexto_a_kernel(motivo_desalojo motivo){
    t_paquete* paquete = crear_paquete(CONTEXTO_EXEC, sizeof(registros_CPU) + sizeof(motivo_desalojo));
    buffer_add(paquete->buffer, &motivo, sizeof(motivo_desalojo));
    buffer_add(paquete->buffer, contextoRegistros, sizeof(registros_CPU));

    enviar_paquete(paquete, sockets.socket_server_D);
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



