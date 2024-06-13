#include <memoria_utils.h>

str_sockets sockets;
sem_t sem_escuchar;
t_list* listaDeProcesos;

void inicializar_memoria(){
    sockets.socket_server = iniciar_servidor( string_itoa(configuracion.PUERTO_ESCUCHA));
    log_info(logger_conexiones, "Memoria esta escuchando");
    espacio_usuario = malloc(sizeof(configuracion.TAM_MEMORIA));
    
    cantFrames = configuracion.TAM_MEMORIA/ configuracion.TAM_PAGINA;
    
  

    memset(bitMap, 0 , sizeof(int) * cantFrames);
    tabla_global = dictionary_create();

} 

void iniciar_proceso(t_buffer* bufferDeKernel){
    procesoListaInst* procesos = malloc(sizeof(procesoListaInst));

// ------------ cargar buffer kernel -------//
    procesos->pid = buffer_read_uint32(bufferDeKernel);
    procesos->instruccionesParaCpu = list_create();
    uint32_t sizeDelProceso = buffer_read_uint8(bufferDeKernel);    

// ------------ leo archivo instrucciones ---------//    
    procesos->instruccionesParaCpu = leer_instrucciones_del_path(buffer_read_string(bufferDeKernel,&sizeDelProceso));

    cargarEnTablaDePaginas(procesos->pid);
    agregarProcesoALaCola(procesos);


    free(procesos);
}

void atender_escuchas(){
    while(1){
    sem_wait(&sem_escuchar);
    pthread_t escuchar;
    pthread_create(&escuchar,NULL,(void*)server_escuchar,NULL);
    pthread_join(escuchar,NULL);
    }
}

int server_escuchar() {
    char* nom_cliente = malloc(20);
    int cliente_socket = esperar_cliente(sockets.socket_server,logger_conexiones,nom_cliente);
    sem_post(&sem_escuchar);
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
    void* a_enviar;//espacio_usuario
     op_code cop;
    while (cliente_socket != -1) {

        if (recv(cliente_socket, &cop, sizeof(op_code), 0) != sizeof(op_code)) {
            log_info(logger_conexiones, "%s DISCONNECT!", nombre_cliente);
            free(nombre_cliente);
            return;
        }

        switch (cop)
        {
        case ENVIAR_INSTRUCCION:
            t_buffer* buffer_de_cpu = recibir_todo_elbuffer(sockets.socket_cliente_CPU);
            enviar_instrucciones_cpu(buffer_de_cpu);
            log_info(logger_memoria, "Inicio envio de instrucciones");
            break;
        case INICIAR_PROCESO:
            t_buffer* buffer_de_kernel = recibir_todo_elbuffer(sockets.socket_cliente_kernel);
            iniciar_proceso(buffer_de_kernel);
            log_info(logger_memoria, "Inicio proceso");
            break;
    

        case AJUSTAR_TAMANIO:
            buffer_de_cpu = recibir_todo_elbuffer(sockets.socket_cliente_CPU);
            ajustar_tam_proceso( buffer_de_cpu);
            log_info(logger_memoria, "Ajusto tamanio proceso");
            break;

        case FINALIZAR_PROCESO:
             buffer_de_kernel = recibir_todo_elbuffer(sockets.socket_cliente_kernel);
             finalizar_proceso(buffer_de_kernel);
             log_info(logger_memoria, "Finalizo el proceso");
            break;
        case ACCESS_ESPACIO_USUARIO_ES:
            t_buffer* buffer_ENTRADA_SALIDA = recibir_todo_elbuffer(sockets.socket_cliente_E_S);
            a_enviar = access_espacio_usuario(buffer_ENTRADA_SALIDA);
            if(a_enviar == NULL) enviar_hanshake(sockets.socket_cliente_E_S,"ok!");//lectura
            enviar_paquete((t_paquete*)a_enviar, sockets.socket_cliente_E_S); //escritura
            break;
        case ACCESS_ESPACIO_USUARIO_CPU:
            buffer_de_cpu = recibir_todo_elbuffer(sockets.socket_cliente_CPU);
            a_enviar = access_espacio_usuario(buffer_ENTRADA_SALIDA);
            if(a_enviar == NULL) enviar_hanshake(sockets.socket_cliente_CPU,"ok!");//escritura
            enviar_paquete((t_paquete*)a_enviar, sockets.socket_cliente_CPU);//lectura
        break;

        case ACCESO_TABLA_PAGINAS:
            buffer_de_cpu = recibir_todo_elbuffer(sockets.socket_cliente_CPU);
            a_enviar = buscar_marco_pagina (buffer_de_cpu);
            enviar_paquete((t_paquete*) a_enviar, sockets.socket_cliente_CPU);

        default:
            break;
        }
    }
    free(nombre_cliente);
}



// checkpoint 2, capaz de leer las instrucciones y enviarlas al cpu

// leer las instrucciones del path
t_list* leer_instrucciones_del_path(char* rutaKernel) {
   

    // Concatenar la ruta de configuracion.PATH_INSTRUCCIONES
    strcat(rutaKernel, configuracion.PATH_INSTRUCCIONES); 

    // Abrir el archivo
   
    FILE* archivo_instrucciones = fopen(rutaKernel, "rb");
   
    if (archivo_instrucciones == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    char* aux = malloc(sizeof(char) * maxiumLineLength);
    t_list* instruccionesParaCPu = list_create();
    while (fgets(aux, maxiumLineLength, archivo_instrucciones)) {
        list_add(instruccionesParaCPu, aux );

    }
    fclose(archivo_instrucciones);
    free(aux);
return instruccionesParaCPu;   

}

void agregarProcesoALaCola(procesoListaInst* proceso){
    list_add(listaDeProcesos, proceso);
}


procesoListaInst* buscar_procesoPorId(int pid){
        bool pidIguales(  procesoListaInst* proceso){
   return proceso->pid == pid;
}
        return list_find(listaDeProcesos, (void*) pidIguales);       
}


char* instruccionActual (procesoListaInst* proceso, int ip ){

if(ip<0 || list_size(proceso->instruccionesParaCpu) < ip ){
    exit(EXIT_FAILURE);
    return NULL;
}

return list_get(proceso->instruccionesParaCpu,ip); 
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