#include <memoria_utils.h>

str_sockets sockets;

void inicializar_memoria(){
    sockets.socket_server = iniciar_servidor( string_itoa(configuracion.PUERTO_ESCUCHA));
    log_info(logger_conexiones, "Memoria esta escuchando");
    
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
        case 1:
            /* code */
            break;
        
        default:
            break;
        }
    }
    free(nombre_cliente);
}



// checkpoint 2, capaz de leer las instrucciones y enviarlas al cpu

// leer las instrucciones del path
void leer_instrucciones_del_path(){
    
}



// envia la instruccion siguiente
void enviar_instrucciones_cpu(){



}



















//  int atender_cliente(int* fd_conexion_ptr){
      // recibo cod_op
//         int cod_op  = recibir_operacion(*fd_conexion_ptr);
//         return enviar_log(*fd_conexion_ptr, cod_op);     
// }

// int enviar_log(int fd_conexion_ptr, int cod_op){
//     switch (cod_op) {
        
//         case COD_OP_CREACION_TABLA:
// Log de creacion de tabla de paginas y destruccion de pagina
           
//         case COD_OP_DESTRUCCION_TABLA:
//enviar_mensaje("PID: <PID> - Tamanio: <CANTIDAD_PAGINAS>", fd_conexion_ptr);
//                     log_info(logger_memoria, "PID: <PID> - Tamaño: <CANTIDAD_PAGINAS>");
//             break;
        
//         case COD_OP_ACCESO_TABLA:
        // Log de acceso a tabla de paginas
         //enviar_mensaje("PID: <PID> - Pagina: <PAGINA> - Marco: <MARCO>",fd_conexion_ptr);
        //log_info(logger_memoria, "PID: <PID> - Pagina: <PAGINA> - Marco: <MARCO>");
        //             break;
//         case COD_OP_AMPLIACION_PROCESO:
        // Log de ampliacion de proceso
        //  enviar_mensaje("PID: <PID> - Tamanio Actual: <TAMANIO_ACTUAL> - Tamanio a Ampliar: <TAMANIO_A_AMPLIAR>" ,fd_conexion_ptr);
        //  log_info(logger_memoria, "PID: <PID> - Tamaño Actual: <TAMAÑO_ACTUAL> - Tamaño a Ampliar: <TAMAÑO_A_AMPLIAR>");
                    
//             break;
//         case COD_OP_REDUCION_PROCESO:
            // Log de reducciOn de proceso
            // enviar_mensaje("PID: <PID> - Tamanio Actual: <TAMANIO_ACTUAL> - Tamanio a Reducir: <TAMANIO_A_REDUCIR>",fd_conexion_ptr);
            //log_info(logger_memoria, "PID: <PID> - Tamaño Actual: <TAMAÑO_ACTUAL> - Tamaño a Reducir: <TAMAÑO_A_REDUCIR>");
            //             break;
            //         case COD_OP_ACCESO_ESPACIO_USUARIO:
           // Log de acceso a espacio de usuario
            // enviar_mensaje("PID: <PID> - Accion: <LEER / ESCRIBIR> - Direccion fisica: <DIRECCION_FISICA>", fd_conexion_ptr);
            // log_info(logger_memoria, "PID: <PID> - Accion: <LEER / ESCRIBIR> - Direccion fisica: <DIRECCION_FISICA>");
            //break;
        
            //         case -1:
            //         	log_error(logger_memoria, "el cliente se desconecto. Terminando servidor");
            // 			return EXIT_FAILURE; 

            //         default:
            //             // Log para codigo de operacion desconocido
            //                  //   enviar_mensaje("codigo desconocido", fd_conexion_ptr);
            //                     log_info(logger_memoria, "codigo desconocido");
            //             break;
            //     }
            //         return EXIT_SUCCESS;
            // }

/*
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
*/