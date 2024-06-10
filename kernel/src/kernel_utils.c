#include "kernel_utils.h"

str_sockets sockets;
sem_t sem_escuchar;
t_dictionary* dicc_io;

pthread_t conexion_CPU_I,conexion_CPU_D, conexion_memoria;

void establecer_conexion_memoria()
{
    int fd_memoria = crear_conexion(configuracion.IP_MEMORIA, string_itoa(configuracion.PUERTO_MEMORIA),logger_conexiones,"KERNEL");
    log_info(logger_conexiones, "Conectado Kernel memoria");
    sockets.socket_memoria = fd_memoria;
 
}

void atender_escuchas(){
    while(1){
    sem_wait(&sem_escuchar);
    pthread_t escuchar;
    pthread_create(&escuchar,NULL,(void*)server_escuchar,NULL);
    pthread_join(escuchar,NULL);
    }
}

void establecer_conexion_cpu_D()
{
    int fd_cpu_distpach = crear_conexion(configuracion.IP_CPU, string_itoa(configuracion.PUERTO_CPU_DISPATCH),logger_conexiones,"KERNEL_D");
    log_info(logger_conexiones, "Conectado Kernel Cpu_Dispatch");
    sockets.socket_CPU_D = fd_cpu_distpach;
 
}

void establecer_conexion_cpu_I()
{
    int fd_cpu_interrupt = crear_conexion(configuracion.IP_CPU, string_itoa(configuracion.PUERTO_CPU_INTERRUPT),logger_conexiones,"KERNEL_I");
    log_info(logger_conexiones, "Concetado Kernel CPU-interrupt ");
    sockets.socket_CPU_I = fd_cpu_interrupt;
   
}

void inicializar_kernel(){
    sockets.socket_server = iniciar_servidor( string_itoa(configuracion.PUERTO_ESCUCHA));
    //socket_server queda en listen() 
    log_info(logger_conexiones, "Kernel esta escuchando");
} 

int server_escuchar() {
    char* nom_cliente = malloc(20);

    int cliente_socket = esperar_cliente(sockets.socket_server,logger_conexiones,nom_cliente); //conecta, valida con handshake y devuelve socket
    //log_info(logger_conexiones, "Conectado 1");
    //sem_wait(&sem_escuchar); //semaforo escuchar, repetido?
    //log_info(logger_conexiones, "No bloqueado");
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




void procesar_conexion(void* void_args) {
    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    int cliente_socket = args->fd;
    char* nombre_cliente = args->cliente_name;
    free(args);

    op_code cop;
    while (cliente_socket != -1) {

        if (recv(cliente_socket, &cop, sizeof(op_code), 0) != sizeof(op_code)) {
            log_info(logger_conexiones, "%s DISCONNECT!", nombre_cliente);
            free(nombre_cliente);
            return;
        }

        switch (cop)
        {
        case CONTEXTO_EXEC:
        recibir_contexto_exec(pcb_en_ejecucion);
        break;

        case ENTRADASALIDA:
            recibir_info_io(cliente_socket);
            break;
        default:
            log_info(logger_conexiones,"no estas mandando nada");
            break;
        }
    }
    free(nombre_cliente);
}

void establecer_conexiones(){
    establecer_conexion_cpu_D();
    establecer_conexion_cpu_I();
    establecer_conexion_memoria();
}

void recibir_info_io(int cliente_socket){
    log_info(logger_conexiones,"me mandaste la info de entradasalida");
    /* agregar la info al diccionario (con mutex?) */            
    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->buffer= malloc(sizeof(t_buffer));
    recv(cliente_socket, &(paquete->buffer->size), sizeof(uint32_t), MSG_WAITALL);
    paquete->buffer->stream = malloc(paquete->buffer->size);
    recv(cliente_socket, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
    /*serializo*/
    t_interfaz tipo_interfaz;
    buffer_read(paquete->buffer,&tipo_interfaz,sizeof(t_interfaz));
    uint32_t long_nombre;
    char* nombre_interfaz = buffer_read_string(paquete->buffer,&long_nombre);
    log_info(logger_conexiones,"Tipo de interfaz recibida");
    log_info(logger_conexiones,string_itoa(tipo_interfaz));
    log_info(logger_conexiones,"Nombre de interfaz recibida");
    log_info(logger_conexiones,nombre_interfaz);

    free(long_nombre);
    free(nombre_interfaz);
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);
    //dictionary_put(dicc_io,string_itoa(nombre_interfaz),tipo_interfaz);

}