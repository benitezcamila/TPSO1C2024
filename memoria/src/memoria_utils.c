#include "main.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>

int inicializar_memoria(){
    return iniciar_servidor( configuracion.PUERTO_ESCUCHA , "MEMORIA");
} 


void escuchar_instrucciones(){

    pthread_t thread_kernel;
    pthread_t thread_cpu;
    pthread_t thread_ES;
    pthread_create(&thread_kernel,
                    NULL,
                    (void*)escucha_kernel,
                    NULL);
                    
    pthread_detach(thread_kernel);
    
    pthread_create(&thread_cpu,
                    NULL,
                    (void*)escucha_cpu,
                    NULL);

    pthread_detach(thread_cpu);

    pthread_create(&thread_ES,
                    NULL,
                    (void*)escucha_E_S,
                    NULL);

    pthread_detach(thread_ES);




    return;

}

void escucha_kernel(){
    int socket_server = inicializar_memoria();
    int *fd_conexion_ptr = malloc(sizeof(int));
    *fd_conexion_ptr = accept(socket_server, NULL, NULL);
    int estado =0;
    while(estado != EXIT_FAILURE){
         estado = atender_cliente(fd_conexion_ptr);
    }   
}

void escucha_cpu(){
    int socket_server = inicializar_memoria();
    int *fd_conexion_ptr = malloc(sizeof(int));
    *fd_conexion_ptr = accept(socket_server, NULL, NULL);
    int estado = 0;
    while(estado != EXIT_FAILURE){
        estado = atender_cliente(fd_conexion_ptr);
    }
}

void escucha_E_S(){
    int socket_server = inicializar_memoria();
    int *fd_conexion_ptr = malloc(sizeof(int));
    *fd_conexion_ptr = accept(socket_server, NULL, NULL);
   int estado = 0;
    while(estado != EXIT_FAILURE){
        estado = atender_cliente(fd_conexion_ptr);
    }
}

 int atender_cliente(int* fd_conexion_ptr){
        // recibo cod_op
        int cod_op  = recibir_operacion(*fd_conexion_ptr);
        return enviar_log(&fd_conexion_ptr, cod_op);     
}

int enviar_log(int fd_conexion_ptr, int cod_op){
    switch (cod_op) {
        
        case COD_OP_CREACION_TABLA:
            // Log de creación de tabla de páginas y destruccion de pagina
           
        case COD_OP_DESTRUCCION_TABLA:
                    //enviar_mensaje("PID: <PID> - Tamaño: <CANTIDAD_PAGINAS>", fd_conexion_ptr);
                    log_info(logger_memoria, "PID: <PID> - Tamaño: <CANTIDAD_PAGINAS>");
            break;
        
        case COD_OP_ACCESO_TABLA:
            // Log de acceso a tabla de páginas
                    //enviar_mensaje("PID: <PID> - Pagina: <PAGINA> - Marco: <MARCO>",fd_conexion_ptr);
                    log_info(logger_memoria, "PID: <PID> - Pagina: <PAGINA> - Marco: <MARCO>");
            break;
        case COD_OP_AMPLIACION_PROCESO:
            // Log de ampliación de proceso
                  //  enviar_mensaje("PID: <PID> - Tamaño Actual: <TAMAÑO_ACTUAL> - Tamaño a Ampliar: <TAMAÑO_A_AMPLIAR>" ,fd_conexion_ptr);
                    log_info(logger_memoria, "PID: <PID> - Tamaño Actual: <TAMAÑO_ACTUAL> - Tamaño a Ampliar: <TAMAÑO_A_AMPLIAR>");
                    
            break;
        case COD_OP_REDUCION_PROCESO:
            // Log de reducción de proceso
                    // enviar_mensaje("PID: <PID> - Tamaño Actual: <TAMAÑO_ACTUAL> - Tamaño a Reducir: <TAMAÑO_A_REDUCIR>",fd_conexion_ptr);
                    log_info(logger_memoria, "PID: <PID> - Tamaño Actual: <TAMAÑO_ACTUAL> - Tamaño a Reducir: <TAMAÑO_A_REDUCIR>");
            break;
        case COD_OP_ACCESO_ESPACIO_USUARIO:
            // Log de acceso a espacio de usuario
                   // enviar_mensaje("PID: <PID> - Accion: <LEER / ESCRIBIR> - Direccion fisica: <DIRECCION_FISICA>", fd_conexion_ptr);
                    log_info(logger_memoria, "PID: <PID> - Accion: <LEER / ESCRIBIR> - Direccion fisica: <DIRECCION_FISICA>");
            break;
        
        case -1:
        	log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE; 

        default:
            // Log para código de operación desconocido
                 //   enviar_mensaje("codigo desconocido", fd_conexion_ptr);
                    log_info(logger_memoria, "codigo desconocido");
            break;
        return EXIT_SUCCESS;
    }
}
