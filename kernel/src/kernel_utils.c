#include "kernel_utils.h"

str_sockets sockets;

pthread_t conexion_CPU_I,conexion_CPU_D, conexion_memoria;

void establecer_conexion_memoria()
{
    int fd_memoria = crear_conexion(configuracion.IP_MEMORIA, string_itoa(configuracion.PUERTO_MEMORIA),logger_conexiones,KERNEL);
    log_info(logger_conexiones, "Conectado Kernel memoria");
    sockets.socket_memoria = fd_memoria;
 
}
void establecer_conexion_cpu_D()
{
    int fd_cpu_distpach = crear_conexion(configuracion.IP_CPU, string_itoa(configuracion.PUERTO_CPU_DISPATCH),logger_conexiones,KERNEL);
    log_info(logger_conexiones, "Conectado Kernel Cpu_Dispatch");
    sockets.socket_CPU_D = fd_cpu_distpach;
 
}

void establecer_conexion_cpu_I()
{
    int fd_cpu_interrupt = crear_conexion(configuracion.IP_CPU, string_itoa(configuracion.PUERTO_CPU_INTERRUPT),logger_conexiones,KERNEL);
    log_info(logger_conexiones, "Concetado Kernel CPU-interrupt ");
    sockets.socket_CPU_I = fd_cpu_interrupt;
   
}

void inicializar_kernel(){
    sockets.socket_server = iniciar_servidor( string_itoa(configuracion.PUERTO_ESCUCHA));
    log_info(logger_conexiones, "Kernel esta escuchando");
} 

int server_escuchar(t_log* logger, char* server_name, int server_socket) {
    codigo_cliente* cod_cliente = malloc(sizeof(codigo_cliente));
    int cliente_socket = esperar_cliente(server_socket,logger_conexiones,cod_cliente);

    if (cliente_socket != -1) {
        pthread_t hilo;
        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;
        pthread_create(&hilo, NULL, (void*) procesar_conexion, (void*) args);
        pthread_detach(hilo);
        return 1;
    }
    return 0;
}
