#include "cpu_utils.h"

#define min(a,b) (a<b?a:b)

int ind_contexto_kernel = 0;
sem_t sem_contexto_kernel;
str_sockets sockets;
int llego_interrupcion = 0;
uint32_t tamano_pagina;

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
    solicitar_tamano_pagina();
}

void atender_conexiones(){
    pthread_t ki, kd;
    pthread_create(&ki,NULL,(void*)server_escuchar,(void*)(sockets.socket_server_I));
    pthread_create(&kd,NULL,(void*)server_escuchar,(void*)(sockets.socket_server_D));
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
        case CONTEXTO_EXEC:
            ind_contexto_kernel = 1;
            while(ind_contexto_kernel == 1){
                ciclo_de_instruccion();
            }
            break;

        case INTERRUPT_PROC:
            log_info(logger_cpu, "Se recibió una interrupción de proceso de parte del Kernel.");
            llego_interrupcion = 1;
            recibir_interrupcion_de_kernel();
            break;
        
        default:
            log_info(logger_errores_cpu, "Se recibió un código de operación incorrecto de parte del Kernel. El mismo fue: %d", codigo_op);
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

        recv(sockets.socket_server_D, paquete->buffer->stream, sizeof(uint32_t), MSG_WAITALL);
        PID =  buffer_read_uint32(paquete->buffer);

        recv(sockets.socket_server_D, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
        buffer_read(paquete->buffer, contexto_registros, sizeof(registros_CPU));
    }
    else{
        log_info(logger_errores_cpu, "El contexto de ejecución no fue recibido correctamente. El código de operación recibido fue: %d", paquete->codigo_operacion);
    }
}

//NO SÉ SI ESTÁ BIEN. CHECKEAR.
void recibir_interrupcion_de_kernel(){
    recv(sockets.socket_server_I, &(motivo_interrupcion), sizeof(tipo_de_interrupcion), MSG_WAITALL);
    ind_contexto_kernel = 0;
}

void enviar_contexto_a_kernel(motivo_desalojo motivo){
    ind_contexto_kernel = 0;
    t_paquete* paquete = crear_paquete(CONTEXTO_EXEC, sizeof(registros_CPU) + sizeof(motivo_desalojo));
    buffer_add(paquete->buffer, &motivo, sizeof(motivo_desalojo));
    buffer_add(paquete->buffer, contexto_registros, sizeof(registros_CPU));

    enviar_paquete(paquete, sockets.socket_server_D);
}

void enviar_std_a_kernel(t_instruccion motivo_io, char* nombre_interfaz,
                                void* tamanio_std, uint32_t tamanio_data, uint32_t dir_fisica){
    t_paquete* paquete = crear_paquete(CONTEXTO_EXEC, sizeof(motivo_desalojo) + sizeof(t_instruccion) + sizeof(registros_CPU)
                                        + string_length(nombre_interfaz)+1 + tamanio_data + sizeof(uint32_t));
    motivo_desalojo mot_des = PETICION_IO;
    buffer_add(paquete->buffer, &mot_des, sizeof(motivo_desalojo));
    buffer_add(paquete->buffer, contexto_registros, sizeof(registros_CPU));
    buffer_add(paquete->buffer, &motivo_io, sizeof(t_instruccion));
    buffer_add_string(paquete->buffer, string_length(nombre_interfaz)+1, nombre_interfaz);
    buffer_add_uint32(paquete->buffer, tamanio_data);
    buffer_add(paquete->buffer, tamanio_std, tamanio_data);
    buffer_add_uint32(paquete->buffer, dir_fisica);

    enviar_paquete(paquete, sockets.socket_server_D);
}

void solicitar_create_delete_fs_a_kernel(t_instruccion motivo_io, char* nombre_interfaz, char* nombre_archivo){
    t_paquete* paquete = crear_paquete(CONTEXTO_EXEC, sizeof(motivo_desalojo) + sizeof(t_instruccion) + sizeof(registros_CPU)
                                        + string_length(nombre_interfaz)+1 + string_length(nombre_archivo)+1);
    motivo_desalojo mot_des = PETICION_IO;
    buffer_add(paquete->buffer, &mot_des, sizeof(motivo_desalojo));
    buffer_add(paquete->buffer, contexto_registros, sizeof(registros_CPU));
    buffer_add(paquete->buffer, &motivo_io, sizeof(t_instruccion));
    buffer_add_string(paquete->buffer, string_length(nombre_interfaz)+1, nombre_interfaz);
    buffer_add_string(paquete->buffer, string_length(nombre_archivo)+1, nombre_archivo);

    enviar_paquete(paquete, sockets.socket_server_D);
}

void solicitar_truncate_fs_a_kernel(t_instruccion motivo_io, char* nombre_interfaz, char* nombre_archivo,
                                    void* tamanio_fs, uint32_t tamanio_data){
    t_paquete* paquete = crear_paquete(CONTEXTO_EXEC, sizeof(motivo_desalojo) + sizeof(t_instruccion) + sizeof(registros_CPU)
                                        + string_length(nombre_interfaz)+1 + string_length(nombre_archivo)+1
                                        + tamanio_data);
    motivo_desalojo mot_des = PETICION_IO;
    buffer_add(paquete->buffer, &mot_des, sizeof(motivo_desalojo));
    buffer_add(paquete->buffer, contexto_registros, sizeof(registros_CPU));
    buffer_add(paquete->buffer, &motivo_io, sizeof(t_instruccion));
    buffer_add_string(paquete->buffer, string_length(nombre_interfaz)+1, nombre_interfaz);
    buffer_add_string(paquete->buffer, string_length(nombre_archivo)+1, nombre_archivo);
    buffer_add_uint32(paquete->buffer, tamanio_data);
    buffer_add(paquete->buffer, tamanio_fs, tamanio_data);

    enviar_paquete(paquete, sockets.socket_server_D);
}

void solicitar_write_read_fs_a_kernel(t_instruccion motivo_io, char* nombre_interfaz, char* nombre_archivo,
                                        void* tamanio_fs, uint32_t tamanio_data1, uint32_t dir_fisica,
                                        void* puntero_archivo, uint32_t tamanio_data2){
    t_paquete* paquete = crear_paquete(CONTEXTO_EXEC, sizeof(motivo_desalojo) + sizeof(t_instruccion) + sizeof(registros_CPU)
                                        + string_length(nombre_interfaz)+1 + string_length(nombre_archivo)+1
                                        + sizeof(uint32_t) + tamanio_data1 + tamanio_data2);
    motivo_desalojo mot_des = PETICION_IO;
    buffer_add(paquete->buffer, &mot_des, sizeof(motivo_desalojo));
    buffer_add(paquete->buffer, contexto_registros, sizeof(registros_CPU));
    buffer_add(paquete->buffer, &motivo_io, sizeof(t_instruccion));
    buffer_add_string(paquete->buffer, string_length(nombre_interfaz)+1, nombre_interfaz);
    buffer_add_string(paquete->buffer, string_length(nombre_archivo)+1, nombre_archivo);
    buffer_add_uint32(paquete->buffer, tamanio_data1);
    buffer_add(paquete->buffer, tamanio_fs, tamanio_data1);
    buffer_add_uint32(paquete->buffer, dir_fisica);
    buffer_add_uint32(paquete->buffer, tamanio_data2);
    buffer_add(paquete->buffer, puntero_archivo, tamanio_data2);

    enviar_paquete(paquete, sockets.socket_server_D);
}

void solicitar_tamano_pagina(){
    op_code* codigo_operacion;
    *codigo_operacion = SOLICITUD_TAMANO_PAGINA;

    send(sockets.socket_memoria, codigo_operacion, sizeof(op_code),0);

    free(codigo_operacion);
    
    int cod_op = recibir_operacion(sockets.socket_memoria);
    
    if(cod_op == TAMANO_PAGINA){
        t_buffer* buffer_memoria = recibir_todo_elbuffer(sockets.socket_memoria);

        tamanio_pagina =  buffer_read_uint32(buffer_memoria);
    }
    else{
        log_info(logger_errores_cpu, "El código de operación recibido no fue el esperado de memoria. El mismo fue: %d", cod_op);
    }
}

void solicitar_instruccion_a_memoria(){
    t_paquete* paquete = crear_paquete(SOLICITUD_INSTRUCCION, sizeof(uint32_t));
    buffer_add_uint32(paquete->buffer, contexto_registros->PC);

    enviar_paquete(paquete, sockets.socket_memoria);
}

void recibir_instruccion_de_memoria(uint32_t* longitud_linea_instruccion){
    t_paquete* paquete = malloc(sizeof(t_paquete));
    recv(sockets.socket_memoria, &(paquete->codigo_operacion), sizeof(op_code), MSG_WAITALL);

    if(paquete->codigo_operacion == INSTRUCCION){
        recv(sockets.socket_memoria, &(paquete->buffer->size), sizeof(uint32_t), MSG_WAITALL);
        paquete->buffer->stream = malloc(paquete->buffer->size);

        recv(sockets.socket_memoria, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
        linea_de_instruccion = buffer_read_string(paquete->buffer, longitud_linea_instruccion);
    }
    else{
        log_info(logger_errores_cpu, "El código de operación recibido de Memoria no fue una instrucción. El código de operación recibido fue: %d", paquete->codigo_operacion);
    }

    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);
}

void recibir_respuesta_resize_memoria(uint32_t PID){
    int op_code = recibir_operacion(sockets.socket_memoria);

    switch (op_code){
    case NO_RESIZE:
        enviar_contexto_a_kernel(OUT_OF_MEMORY);
        break;
    
    case RESIZE_SUCCESS:
        log_info(logger_cpu, "El resize del proceso %d fue exitoso.", PID);
        break;

    default:
        log_info(logger_errores_cpu, "Ocurrió un error recibiendo la respuesta de Memoria tras Resize. El código de operación recibido fue: %d", op_code);
        break;
    }
}

uint32_t cantidad_paginas_que_ocupa(uint32_t tamanio, uint32_t desplazamiento){
    return ((tamanio - tamano_pagina + desplazamiento) / tamano_pagina) + 1;
}

void* leer_en_memoria_mas_de_una_pagina(t_buffer buffer_auxiliar, uint32_t tamanio_auxiliar, uint32_t tamanio_total){
    uint32_t direccion_fisica = mmu(tlb, dir_logica);
    uint32_t offset = direccion_fisica % tamano_pagina;
    uint32_t tamanio_a_leer = min(tamanio_auxiliar, tamano_pagina-offset);
    
    buffer_add(buffer_auxiliar, solicitar_leer_en_memoria(dir_fisica, tamanio_a_leer), tamanio_a_leer);
    tamanio_auxiliar =- tamanio_a_leer;
    
    if(tamanio_auxiliar >= 0){
        auxiliar = floor(dir_logica)+ 1;    
        dir_logica = auxiliar;
        leer_en_memoria_mas_de_una_pagina(buffer_auxiliar, tamanio_auxiliar, tamanio_total);    
    }
    else{
        void* retorno = malloc(tamanio_total);
        buffer_read(buffer_auxiliar, retorno, tamanio_total);
        return retorno;
    }
}

void escribir_en_memoria_mas_de_una_pagina(t_buffer buffer_auxiliar, uint32_t tamanio_total){
    uint32_t direccion_fisica = mmu(tlb, dir_logica);
    uint32_t offset = direccion_fisica % tamano_pagina;
    uint32_t tamanio_a_escribir = min(tamanio_auxiliar, tamano_pagina-offset);
    void* data_a_escribir = malloc(tamanio_a_escribir);
    
    buffer_read(buffer_auxiliar, data_a_escribir, tamanio_a_escribir);
    tamanio_auxiliar -= tamanio_a_escribir;
    solicitar_escribir_en_memoria(direccion_fisica, data_a_escribir, tamanio_a_escribir);
    
    if(tamanio_auxiliar > 0){
        auxiliar = floor(dir_logica)+1;
        dir_logica = auxiliar;
        escribir_en_memoria_mas_de_una_pagina(buffer_auxiliar, tamanio_auxiliar);
    }
    else{
        return;
    } 
}


void* solicitar_leer_en_memoria(uint32_t dir_fisica, uint32_t tamanio){
    t_paquete* paquete = crear_paquete(ACCESS_ESPACIO_USUARIO_CPU, sizeof(uint32_t) * 4);
    
    buffer_add_uint32(paquete->buffer, PID);
    buffer_add_uint32(paquete->buffer, (uint32_t) 0 ); // 1 escribir, 0 leer
    buffer_add_uint32(paquete->buffer, dir_fisica);
    buffer_add_uint32(paquete->buffer, tamanio);

    enviar_paquete(paquete, sockets.socket_memoria);
    return leer_de_memoria(tamanio);

}

void* leer_de_memoria(uint32_t tamanio){
    t_paquete* paquete = malloc(sizeof(t_paquete));
    recv(sockets.socket_memoria, &(paquete->codigo_operacion), sizeof(op_code), MSG_WAITALL);

    if(paquete->codigo_operacion == RESPUESTA_LECTURA_MEMORIA){
        recv(sockets.socket_memoria, &(paquete->buffer->size), sizeof(uint32_t), MSG_WAITALL);
        paquete->buffer->stream = malloc(paquete->buffer->size);

        void* datos_de_memoria = malloc(tamanio);
        
        if(tamanio == sizeof(uint32_t)){
            recv(sockets.socket_memoria, paquete->buffer->stream, tamanio, MSG_WAITALL);
            *(uint32_t*)datos_de_memoria = buffer_read_uint32(paquete->buffer);
        }
        else if(tamanio == sizeof(uint8_t)){
            recv(sockets.socket_memoria, paquete->buffer->stream, tamanio, MSG_WAITALL);
            *(uint8_t*)datos_de_memoria = buffer_read_uint8(paquete->buffer);
        }

        free(paquete->buffer->stream);
        free(paquete->buffer);
        free(paquete);

        return datos_de_memoria;
    }
    else{
        log_info(logger_errores_cpu, "Ocurrió un error tras realizar una lectura de Memoria. El código de operación recibido fue: %d", paquete->codigo_operacion);
        free(paquete);

        return NULL;
    }
}

void solicitar_escribir_en_memoria(uint32_t dir_fisica, void* datos_de_registro, uint32_t tamanio){
    t_paquete* paquete = crear_paquete(ACCESS_ESPACIO_USUARIO_CPU, sizeof(uint32_t) * 4 + tamanio);
    
    buffer_add_uint32(paquete->buffer, PID);
    buffer_add_uint32(paquete->buffer, (uint32_t) 1 ); // 1 escribir, 0 leer
    buffer_add_uint32(paquete->buffer, dir_fisica);
    
    buffer_add_uint32(paquete->buffer, tamanio);
    buffer_add(paquete->buffer, datos_de_registro, tamanio);

    enviar_paquete(paquete, sockets.socket_memoria);
}

int solicitar_marco_a_memoria(uint32_t numero_pagina){
    t_paquete* paquete = crear_paquete(ACCESO_TABLA_PAGINAS, sizeof(uint32_t) * 2);
    buffer_add_uint32(paquete->buffer, PID);
    buffer_add_uint32(paquete->buffer, numero_pagina);

    enviar_paquete(paquete, sockets.socket_memoria);
    
    int op_code = recibir_operacion(sockets.socket_memoria);
    
    if(op_code == MARCO_BUSCADO ){
        t_buffer* buffer_memoria = recibir_todo_elbuffer(sockets.socket_memoria);

        return buffer_read_uint32(buffer_memoria);
    }

    return -1;
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



