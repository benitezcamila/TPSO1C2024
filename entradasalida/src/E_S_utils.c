#include <E_S_utils.h>

str_sockets sockets;
char* nombre_interfaz;


void establecer_conexion_kernel(){
    motivo_desalojo instruccion_a_procesar;
    int fd_kernel = crear_conexion(configuracion.IP_KERNEL ,string_itoa(configuracion.PUERTO_KERNEL),logger_entrada_salida,"Entrada Salida");
    sockets.socket_kernel = fd_kernel;
    log_info(logger_entrada_salida, "Conectado Entrada/Salida-Kernel");
    enviar_info_io_a_kernel();
    log_info(logger_entrada_salida, "Info enviada");
    recibir_instrucciones();
}
void establecer_conexion_memoria(){
    int fd_memoria = crear_conexion(configuracion.IP_MEMORIA ,string_itoa(configuracion.PUERTO_MEMORIA),logger_entrada_salida,"Entrada Salida");
    sockets.socket_memoria = fd_memoria;
    log_info(logger_entrada_salida, "Conectado Entrada/Salida-Memoria");
}


void procesar_io_gen_sleep (t_buffer* buffer_kernel) {
    uint32_t milisegundos_de_espera;
    buffer_read (buffer_kernel,&milisegundos_de_espera, sizeof(uint32_t));
    t_temporal *temporal = temporal_create();
	usleep(milisegundos_de_espera*1000);
	temporal_stop(temporal);
    log_info(logger_entrada_salida, "Tiempo esperado:");
    log_info(logger_entrada_salida,  string_itoa(temporal_gettime(temporal)));
    temporal_destroy(temporal);
}


void enviar_info_io_a_kernel(){
    t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_interfaz) + string_length(nombre_interfaz)+1+sizeof(uint32_t));
    buffer_add(paquete->buffer, &configuracion.TIPO_INTERFAZ, sizeof(t_interfaz));
    
    buffer_add_string(paquete->buffer, string_length(nombre_interfaz)+1, nombre_interfaz);
    enviar_paquete(paquete, sockets.socket_kernel);

}


void procesar_io_stdin_read(t_buffer* buffer_kernel, uint32_t pid ) {
    uint32_t direccion_fisica_memoria_read;
    buffer_read(buffer_kernel,&direccion_fisica_memoria_read, sizeof(uint32_t));
    char* input_consola = leer_consola();

    escribir_en_memoria(input_consola, direccion_fisica_memoria_read, pid);


}

char* leer_consola(){
	char* leido;
	leido = readline("> ");
    log_info(logger_entrada_salida,leido);
    return leido;	
}


void escribir_en_memoria(char* input_consola,uint32_t pid,uint32_t direccion_fisica_memoria_write) {
    uint32_t tamanio = strlen(input_consola)+1 ;
    uint32_t accion =1 ;//escribir
    t_paquete* paquete = crear_paquete(ACCESS_ESPACIO_USUARIO_ES, tamanio * sizeof(char) + sizeof(uint32_t) * 4);



    buffer_add_uint32(paquete->buffer, pid);
    buffer_add_uint32(paquete->buffer, accion); 
    buffer_add_uint32(paquete->buffer, direccion_fisica_memoria_write);
    buffer_add_uint32(paquete->buffer, tamanio * sizeof(char) );
    buffer_add(paquete->buffer, input_consola, tamanio * sizeof(char)); 

    enviar_paquete(paquete, sockets.socket_memoria);//peticion escritura memoria.. si pone ok! escribio correctamente

    eliminar_paquete(paquete);
    }

void procesar_io_stdout_write(t_buffer* buffer_kernel, uint32_t pid) {
    //leer el valor que se encuentra en la o las direcciones físicas pedidas y mostrar el resultado por pantalla
    /*
    IO_STDOUT_WRITE (Interfaz, Registro Dirección, Registro Tamaño):
     Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se lea desde la posición de memoria indicada por 
     la Dirección Lógica almacenada en el Registro Dirección, un tamaño indicado por el Registro Tamaño y se imprima por pantalla.
    */

    uint32_t direccion_fisica_memoria_write;
    uint32_t tamanio_a_leer;

    uint32_t accion =0;    
    tamanio_a_leer = buffer_read_uint32(buffer_kernel);
    direccion_fisica_memoria_write = buffer_read_uint32(buffer_kernel);
    t_paquete* paquete = crear_paquete(ACCESS_ESPACIO_USUARIO_ES, sizeof(uint32_t) * 4);
    buffer_add_uint32(paquete->buffer, pid);
    buffer_add_uint32(paquete->buffer, accion);
    buffer_add_uint32(paquete->buffer, direccion_fisica_memoria_write);
    buffer_add_uint32(paquete->buffer, tamanio_a_leer);
    enviar_paquete(paquete, sockets.socket_memoria);
    
    eliminar_paquete(paquete);
    
    op_code codigo_leido;
    
    recv(sockets.socket_memoria,&codigo_leido , sizeof(op_code), 0);
    t_buffer* memoria = recibir_todo_elbuffer (sockets.socket_memoria);
    uint32_t* length = malloc(sizeof(uint32_t));
    char* mostrar_de_memoria = buffer_read_string(memoria, length); 
    
    printf(mostrar_de_memoria);
    free(length);
     
}


void recibir_instrucciones (){
    t_instruccion instruccion_a_procesar;
    uint32_t pid ;
    if (recv(sockets.socket_kernel, &instruccion_a_procesar, sizeof(t_instruccion), 0) != sizeof(t_instruccion)) {
            log_info(logger_entrada_salida, "Instruccion invalida");
            return;
        }
    char* instruccion_string = string_de_instruccion(instruccion_a_procesar);
    t_buffer* buffer_kernel = recibir_todo_elbuffer(sockets.socket_kernel);
    pid = buffer_read_uint32(buffer_kernel);
    char* informar_pid = string_from_format("PID: %s - Operacion: %s",string_itoa(pid),instruccion_string); 
    log_info(logger_entrada_salida, informar_pid);
    
    switch (instruccion_a_procesar) {
        case GEN_SLEEP:
        procesar_io_gen_sleep(buffer_kernel);
        break;
        case STDIN_READ:
        procesar_io_stdin_read(buffer_kernel,pid);
        break;
        case STDOUT_WRITE:
        procesar_io_stdout_write(buffer_kernel,pid);
        break;
        case FS_CREATE:
        //procesar_io_fs_create();
        break;
        case FS_DELETE:
        //procesar_io_fs_delete();
        break;
        case FS_TRUNCATE:
        //procesar_io_fs_truncate();
        break;
        case FS_WRITE:
        //procesar_io_fs_write();
        break;
        case FS_READ:
        //procesar_io_fs_read();
        break;
        default:
        log_info(logger_entrada_salida, "Instruccion invalida");
        break;
    }
    free(informar_pid);
    free(instruccion_string);
    buffer_destroy(buffer_kernel);
    return;
}


char* string_de_instruccion (t_instruccion instruccion_a_procesar){
    char* instruccion_en_string;
    switch(instruccion_a_procesar) {
        case GEN_SLEEP:
        instruccion_en_string = string_from_format("GEN_SLEEP");
        break;
        case STDIN_READ:
        instruccion_en_string = string_from_format("STDIN_READ");
        break;
        case STDOUT_WRITE:
        instruccion_en_string = string_from_format("STDOUT_WRITE");
        break;
        case FS_CREATE:
        instruccion_en_string = string_from_format("FS_CREATE");
        break;
        case FS_DELETE:
        instruccion_en_string = string_from_format("FS_DELETE");
        break;
        case FS_TRUNCATE:
        instruccion_en_string = string_from_format("FS_TRUNCATE");
        break;
        case FS_WRITE:
        instruccion_en_string = string_from_format("FS_WRITE");       
        break;
        case FS_READ:
        instruccion_en_string = string_from_format("FS_READ");
        break;
    }
    return instruccion_en_string;
}


/*void proceso_E_S(){
    pthread_t hilo_kernel, hilo_memoria;
    pthread_create(&hilo_memoria, NULL,(void *) establecer_conexion_memoria, NULL);
    pthread_create(&hilo_kernel, NULL,(void*) establecer_conexion_kernel, NULL); 
    
    pthread_join(hilo_memoria);
    pthread_join(hilo_kernel);
}
*/
