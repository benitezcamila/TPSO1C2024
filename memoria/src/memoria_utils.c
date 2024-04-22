#include <memoria_utils.h>

int socket_server;

int inicializar_memoria(){
    socket_server = iniciar_servidor( string_itoa(configuracion.PUERTO_ESCUCHA));
    log_info(logger_conexiones, "Memoria esta escuchando");
    return socket_server;//eventualmente innecesario veremos xd
} 

void escuchar_instrucciones(){

    pthread_t thread_kernel;
    pthread_t thread_cpu;
    pthread_t thread_ES;
    pthread_create(&thread_kernel,
                    NULL,
                    (void*)escucha_kernel,
                    NULL);
                    

    
    pthread_create(&thread_cpu,
                    NULL,
                    (void*) escucha_cpu,
                    NULL);



    pthread_create(&thread_ES,
                    NULL,
                    (void*) escucha_E_S,
                    NULL);
                    
    pthread_join(thread_cpu,NULL);
    pthread_join(thread_ES,NULL);
    pthread_join(thread_kernel,NULL);

    return;

}

void escucha_kernel(){
    int *fd_conexion_ptr = malloc(sizeof(int));
    *fd_conexion_ptr = esperar_cliente(socket_server);
    log_info(logger_conexiones, "Se conecto un cliente kernel");
    int estado =0;
    /*while(estado != EXIT_FAILURE){
         estado = atender_cliente(fd_conexion_ptr);
    }   */
    free(fd_conexion_ptr);
}

void escucha_cpu(){
    int *fd_conexion_ptr = malloc(sizeof(int));
    *fd_conexion_ptr = esperar_cliente(socket_server);
    log_info(logger_conexiones, "Se conecto un cliente cpu");
    int estado = 0;
    /*while(estado != EXIT_FAILURE){
        estado = atender_cliente(fd_conexion_ptr);
    }*/
    free(fd_conexion_ptr);
}

void escucha_E_S(){
    int *fd_conexion_ptr = malloc(sizeof(int));
    *fd_conexion_ptr = esperar_cliente(socket_server);
    log_info(logger_conexiones, "Se conecto un cliente entrada/salida");
    int estado = 0;
    /*while(estado != EXIT_FAILURE){
        estado = atender_cliente(fd_conexion_ptr);
    }*/
    free(fd_conexion_ptr);
}

 int atender_cliente(int* fd_conexion_ptr){
        // recibo cod_op
        int cod_op  = recibir_operacion(*fd_conexion_ptr);
        return enviar_log(*fd_conexion_ptr, cod_op);     
}

int enviar_log(int fd_conexion_ptr, int cod_op){
    switch (cod_op) {
        
        case COD_OP_CREACION_TABLA:
            // Log de creacion de tabla de paginas y destruccion de pagina
           
        case COD_OP_DESTRUCCION_TABLA:
                    //enviar_mensaje("PID: <PID> - Tamanio: <CANTIDAD_PAGINAS>", fd_conexion_ptr);
                    log_info(logger_memoria, "PID: <PID> - Tamaño: <CANTIDAD_PAGINAS>");
            break;
        
        case COD_OP_ACCESO_TABLA:
            // Log de acceso a tabla de paginas
                    //enviar_mensaje("PID: <PID> - Pagina: <PAGINA> - Marco: <MARCO>",fd_conexion_ptr);
                    log_info(logger_memoria, "PID: <PID> - Pagina: <PAGINA> - Marco: <MARCO>");
            break;
        case COD_OP_AMPLIACION_PROCESO:
            // Log de ampliacion de proceso
                  //  enviar_mensaje("PID: <PID> - Tamanio Actual: <TAMANIO_ACTUAL> - Tamanio a Ampliar: <TAMANIO_A_AMPLIAR>" ,fd_conexion_ptr);
                    log_info(logger_memoria, "PID: <PID> - Tamaño Actual: <TAMAÑO_ACTUAL> - Tamaño a Ampliar: <TAMAÑO_A_AMPLIAR>");
                    
            break;
        case COD_OP_REDUCION_PROCESO:
            // Log de reducciOn de proceso
                    // enviar_mensaje("PID: <PID> - Tamanio Actual: <TAMANIO_ACTUAL> - Tamanio a Reducir: <TAMANIO_A_REDUCIR>",fd_conexion_ptr);
                    log_info(logger_memoria, "PID: <PID> - Tamaño Actual: <TAMAÑO_ACTUAL> - Tamaño a Reducir: <TAMAÑO_A_REDUCIR>");
            break;
        case COD_OP_ACCESO_ESPACIO_USUARIO:
            // Log de acceso a espacio de usuario
                   // enviar_mensaje("PID: <PID> - Accion: <LEER / ESCRIBIR> - Direccion fisica: <DIRECCION_FISICA>", fd_conexion_ptr);
                    log_info(logger_memoria, "PID: <PID> - Accion: <LEER / ESCRIBIR> - Direccion fisica: <DIRECCION_FISICA>");
            break;
        
        case -1:
        	log_error(logger_memoria, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE; 

        default:
            // Log para codigo de operacion desconocido
                 //   enviar_mensaje("codigo desconocido", fd_conexion_ptr);
                    log_info(logger_memoria, "codigo desconocido");
            break;
    }
        return EXIT_SUCCESS;
}
