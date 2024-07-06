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
    log_info(logger_entrada_salida, "Tiempo esperado: %u", temporal_gettime(temporal));
    temporal_destroy(temporal);
    enviar_fin_de_instruccion ();
}


void enviar_info_io_a_kernel(){
    t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_interfaz) + string_length(nombre_interfaz)+1);
    buffer_add(paquete->buffer, &configuracion.TIPO_INTERFAZ, sizeof(t_interfaz));
    
    buffer_add_string(paquete->buffer, string_length(nombre_interfaz)+1, nombre_interfaz);
    enviar_paquete(paquete, sockets.socket_kernel);

}


void procesar_io_stdin_read(t_buffer* buffer_kernel, uint32_t pid ) {
    uint32_t direccion_fisica_memoria_read;
    buffer_read(buffer_kernel,&direccion_fisica_memoria_read, sizeof(uint32_t));
    char* input_consola = leer_consola();
    escribir_en_memoria(input_consola, direccion_fisica_memoria_read, pid);
    enviar_fin_de_instruccion ();
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
    enviar_fin_de_instruccion();
}


void recibir_instrucciones (){
    t_instruccion instruccion_a_procesar;
    uint32_t pid ;
    op_code cop;
    while(sockets.socket_kernel!=-1) {
        if (recv(sockets.socket_kernel, &cop, sizeof(op_code), 0) != sizeof(op_code)) {
                log_info(logger_conexiones, "Error en el OpCode!");
                return;
            }
        if (cop != ENTRADASALIDA) {
            log_info(logger_conexiones, "OpCode recibido no corresponde con ENTRADASALIDA");
            return;
        }
        t_buffer* buffer_kernel = recibir_todo_elbuffer(sockets.socket_kernel);
        buffer_read(buffer_kernel,&instruccion_a_procesar,sizeof(t_instruccion));
        char* instruccion_string = string_de_instruccion(instruccion_a_procesar);
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
            procesar_io_fs_create(buffer_kernel,pid);
            break;
            case FS_DELETE:
            procesar_io_fs_delete(buffer_kernel,pid);
            break;
            case FS_TRUNCATE:
            procesar_io_fs_truncate(buffer_kernel,pid);
            break;
            case FS_WRITE:
            procesar_io_fs_write(buffer_kernel,pid);
            break;
            case FS_READ:
            procesar_io_fs_read(buffer_kernel,pid);
            break;
            default:
            log_info(logger_entrada_salida, "Instruccion invalida");
            break;
        }
        free(informar_pid);
        free(instruccion_string);
        buffer_destroy(buffer_kernel);
    }
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


void enviar_fin_de_instruccion () {
    t_paquete* paquete = crear_paquete(ENTRADASALIDA_LIBERADO, sizeof(t_interfaz) + string_length(nombre_interfaz)+1);
    buffer_add_string(paquete->buffer, string_length(nombre_interfaz)+1, nombre_interfaz);
    enviar_paquete(paquete, sockets.socket_kernel);
 }

void procesar_io_fs_create(t_buffer* buffer_kernel, uint32_t pid ){
    usleep(configuracion.TIEMPO_UNIDAD_TRABAJO*1000);
    uint32_t longitud_nombre_archivo = buffer_read_uint32(buffer_kernel);
    char* nombre_archivo = buffer_read_string(buffer_kernel,longitud_nombre_archivo);
    char* path_archivo = string_new();
    string_append(&path_archivo,configuracion.PATH_BASE_DIALFS);
    string_append(&path_archivo,"/");
    string_append(&path_archivo,nombre_archivo);
    char* informar_crear_archivo = string_from_format("PID: %s - Crear Archivo: %s",string_itoa(pid),nombre_archivo); 
    log_info(logger_entrada_salida, informar_crear_archivo);
    free(informar_crear_archivo);
    /*
    IO_FS_CREATE (Interfaz, Nombre Archivo): Esta instrucción solicita al 
    Kernel que mediante la interfaz seleccionada, se cree un archivo en el FS montado en dicha interfaz.
    */
    FILE *file = fopen(path_archivo, "r+");
    if (file != NULL) {
        log_info(logger_entrada_salida, "El archivo ya existe");
        fclose(file);
        return;
    }
    if(contar_bloques_libres(bitmap) == 0){
        log_info(logger_entrada_salida, "No hay mas bloques disponibles");
        fclose(file);
        return;
    }
    if (errno == ENOENT) {
        file = fopen(path_archivo, "w+");
        if (file == NULL) {
            log_info(logger_entrada_salida, "Error al crear el archivo");
            return;
        }
        int fd = fileno(file);
        if (ftruncate(fd, 0) != 0) {
            log_info(logger_entrada_salida, "Error al truncar el archivo");
            fclose(file);
            return;
        }
        fclose(file);
        } else {
            perror("Error al abrir el archivo");
            return;
        }
        int bloque_asignado = primer_bloque_libre(bitmap);
        t_config* config_archivo = config_create(path_archivo);
        config_set_value(config_archivo,"BLOQUE_INICIAL",string_itoa(bloque_asignado));
        config_set_value(config_archivo,"TAMANIO_ARCHIVO","0");
        config_save(config_archivo);
        bitarray_set_bit(bitmap,bloque_asignado);
        msync(bitmap,bitarray_get_max_bit(bitmap),MS_SYNC);
        agregar_archivo_a_indice(fd_indice, nombre_archivo ,bloque_asignado);
        enviar_fin_de_instruccion();
}


int primer_bloque_libre(t_bitarray* bitmap) {
    int primer_bit_libre;
    for (int i=0;i<bitarray_get_max_bit(bitmap);i++){
        if(!bitarray_test_bit(bitmap,i)){
            primer_bit_libre=i;
            return primer_bit_libre;
        }
    }
    return -1;
}

int contar_bloques_libres(t_bitarray* bitmap) {
    int bits_libres=0;
    for (int i=0;i<bitarray_get_max_bit(bitmap);i++){
        if(!bitarray_test_bit(bitmap,i)){
            bits_libres+=1;
        }
    }
    return bits_libres;
}

int obtener_primer_bloque_de_archivo (char * nombre_archivo){
    char* path_archivo = string_new();
    string_append(&path_archivo,configuracion.PATH_BASE_DIALFS);
    string_append(&path_archivo,"/");
    string_append(&path_archivo,nombre_archivo);
    t_config* config_achivo = config_create(path_archivo);
    int bloque_inicial = config_get_int_value(config_achivo,"BLOQUE_INICIAL");
    free(path_archivo);
    return bloque_inicial;
}

int obtener_tamanio_de_archivo (char * nombre_archivo){
    char* path_archivo = string_new();
    string_append(&path_archivo,configuracion.PATH_BASE_DIALFS);
    string_append(&path_archivo,"/");
    string_append(&path_archivo,nombre_archivo);
    t_config* config_achivo = config_create(path_archivo);
    int tamanio_archivo = config_get_int_value(config_achivo,"TAMANIO_ARCHIVO");
    config_destroy(path_archivo);
    free(path_archivo);
    return tamanio_archivo;
}

int obtener_nuevo_bloque_final (char * nombre_archivo, uint32_t tamanio_nuevo){
    int bloque_inicial = obtener_primer_bloque_de_archivo(nombre_archivo);
    int bloque_final = obtener_ultimo_bloque_de_archivo(nombre_archivo);
    int nuevo_bloque_final = tamanio_nuevo / configuracion.BLOCK_SIZE;
    if (tamanio_nuevo%configuracion.BLOCK_SIZE!=0) {
        nuevo_bloque_final+=1;
    }
    nuevo_bloque_final+=bloque_inicial;
    if(nuevo_bloque_final>configuracion.BLOCK_COUNT){
        log_info(logger_entrada_salida, "Error: el bloque final supera el limite del FS");
        return -1;
    }
    return nuevo_bloque_final;
}

int obtener_ultimo_bloque_de_archivo (char * nombre_archivo){
    char* path_archivo = string_new();
    string_append(&path_archivo,configuracion.PATH_BASE_DIALFS);
    string_append(&path_archivo,"/");
    string_append(&path_archivo,nombre_archivo);
    t_config* config_achivo = config_create(path_archivo);
    int bloque_inicial = config_get_int_value(config_achivo,"BLOQUE_INICIAL");
    int tamanio_archivo = config_get_int_value(config_achivo,"TAMANIO_ARCHIVO");
    int bloque_final = tamanio_archivo / configuracion.BLOCK_SIZE;
    if (tamanio_archivo%configuracion.BLOCK_SIZE!=0) {
        bloque_final+=1;
    }
    bloque_final+=bloque_inicial;
    config_destroy(config_achivo);
    free(path_archivo);
    return bloque_final;
}

void procesar_io_fs_delete(t_buffer* buffer_kernel, uint32_t pid){
    /*
    IO_FS_DELETE (Interfaz, Nombre Archivo): Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, 
    se elimine un archivo en el FS montado en dicha interfaz
    */
    usleep(configuracion.TIEMPO_UNIDAD_TRABAJO*1000);
    uint32_t longitud_nombre_archivo = buffer_read_uint32(buffer_kernel);
    char* nombre_archivo = buffer_read_string(buffer_kernel,longitud_nombre_archivo);
    char* path_archivo = string_new();
    string_append(&path_archivo,configuracion.PATH_BASE_DIALFS);
    string_append(&path_archivo,"/");
    string_append(&path_archivo,nombre_archivo);
    char* informar_eliminar_archivo = string_from_format("PID: %s - Eliminar Archivo: %s",string_itoa(pid),nombre_archivo); 
    log_info(logger_entrada_salida, informar_eliminar_archivo);
    free(informar_eliminar_archivo);
    int primer_bit_archivo = obtener_primer_bloque_de_archivo(nombre_archivo);
    int ultimo_bit_archivo = obtener_ultimo_bloque_de_archivo(nombre_archivo);
    if (remove(path_archivo) == 0) {
        log_info(logger_entrada_salida, "Archivo eliminado");
        limpiar_bits(bitmap,primer_bit_archivo,ultimo_bit_archivo);
    } else {
        log_info(logger_entrada_salida, "Error al eliminar el archivo");
    }
    eliminar_archivo_de_indice(fd_indice,nombre_archivo);
    free(nombre_archivo);
    free(path_archivo);
    enviar_fin_de_instruccion();
}

int calcular_cantidad_de_bloques (int tamanio){
    int bloques_totales = tamanio / configuracion.BLOCK_SIZE;
    if(tamanio%configuracion.BLOCK_SIZE!=0){
        bloques_totales+=1;
    }
    return bloques_totales;
}


void limpiar_bits(t_bitarray* bitmap, off_t bit_inicial, off_t bit_final) {
    for (off_t i=bit_inicial;i<bit_final;i++){
        bitarray_clean_bit(bitmap,i);
        log_info(logger_entrada_salida, "Bit %i eliminado",i);
    }
    msync(bitmap,bitarray_get_max_bit(bitmap),MS_SYNC);
}

void setear_bits(t_bitarray* bitmap, off_t bit_inicial, off_t bit_final) {
    for (off_t i=bit_inicial;i<bit_final;i++){
        bitarray_set_bit(bitmap,i);
        log_info(logger_entrada_salida, "Bit %i seteado",i);
    }
    msync(bitmap,bitarray_get_max_bit(bitmap),MS_SYNC);
}
/*
void procesar_io_fs_delete_prueba(char* nombre_archivo){
    char* path_archivo = string_new();
    string_append(&path_archivo,configuracion.PATH_BASE_DIALFS);
    string_append(&path_archivo,"/");
    string_append(&path_archivo,nombre_archivo);
    char* informar_eliminar_archivo = string_from_format("PID: %s - Eliminar Archivo: %s","prueba",nombre_archivo); 
    log_info(logger_entrada_salida, informar_eliminar_archivo);
    free(informar_eliminar_archivo);
    int primer_bit_archivo = obtener_primer_bloque_de_archivo(nombre_archivo);
    int ultimo_bit_archivo = obtener_ultimo_bloque_de_archivo(nombre_archivo);
    if (remove(path_archivo) == 0) {
        log_info(logger_entrada_salida, "Archivo eliminado");
        limpiar_bits(bitmap,primer_bit_archivo,ultimo_bit_archivo);
    } else {
        log_info(logger_entrada_salida, "Error al eliminar el archivo");
    }
    free(path_archivo);
}
*/

void procesar_io_fs_truncate(t_buffer* buffer_kernel, uint32_t pid ){
    /*
    IO_FS_TRUNCATE (Interfaz, Nombre Archivo, Registro Tamaño): Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, 
    se modifique el tamaño del archivo en el FS montado en dicha interfaz, actualizando al valor que se encuentra en el registro indicado 
    por Registro Tamaño.
    */
    usleep(configuracion.TIEMPO_UNIDAD_TRABAJO*1000);
    uint32_t longitud_nombre_archivo = buffer_read_uint32(buffer_kernel);
    char* nombre_archivo = buffer_read_string(buffer_kernel,longitud_nombre_archivo);

    char* path_archivo = string_new();
    string_append(&path_archivo,configuracion.PATH_BASE_DIALFS);
    string_append(&path_archivo,"/");
    string_append(&path_archivo,nombre_archivo);

    uint32_t tamanio_nuevo = buffer_read_uint32(buffer_kernel);
    char* informar_truncar_archivo = string_from_format("PID: %s - Truncar Archivo: %s - Tamaño %i",string_itoa(pid),nombre_archivo,tamanio_nuevo); 
    log_info(logger_entrada_salida, informar_truncar_archivo);
    free(informar_truncar_archivo);
    int bloque_inicial_archivo = obtener_primer_bloque_de_archivo(nombre_archivo);
    int pre_bloque_final_archivo = obtener_ultimo_bloque_de_archivo(nombre_archivo);
    int post_bloque_final_archivo = obtener_nuevo_bloque_final (nombre_archivo,tamanio_nuevo); 

    if((post_bloque_final_archivo > pre_bloque_final_archivo) ||  post_bloque_final_archivo == -1) { //-1 representa que se excede de bloque maximo
        if(post_bloque_final_archivo!=-1 && !hay_bits_ocupados(bitmap, bloque_inicial_archivo,post_bloque_final_archivo)){ 
            //si hay espacio para extender, ocupo bits
            setear_bits(bitmap,bloque_inicial_archivo,post_bloque_final_archivo);
        } else{
            //si tenemos bloques suficientes, compactar, sino devolver error
            if(contar_bloques_libres(bitmap)>calcular_cantidad_de_bloques(tamanio_nuevo)) { 
                int nuevo_bloque_inicial = compactar_y_acomodar_al_final(bloques,bitmap,bloque_inicial_archivo,pre_bloque_final_archivo,pid);
                //actualizar_valores
                bloque_inicial_archivo = nuevo_bloque_inicial;
                post_bloque_final_archivo = obtener_nuevo_bloque_final (nombre_archivo,tamanio_nuevo);
                setear_bits(bitmap,bloque_inicial_archivo,post_bloque_final_archivo);
            } else {
                log_info(logger_entrada_salida, "Error al truncar: no hay suficiente espacio libre en FS");
                return;
            }
        }
    }
    if (post_bloque_final_archivo < pre_bloque_final_archivo) {
        //https://github.com/sisoputnfrba/foro/issues/3993 no es necesario limpiar, puede quedar basura en el archivo
        limpiar_bits(bitmap,bloque_inicial_archivo+1,post_bloque_final_archivo);
    } 
    t_config* config_archivo = config_create(path_archivo);
    config_set_value(config_archivo,"BLOQUE_INICIAL",string_itoa(bloque_inicial_archivo));
    config_set_value(config_archivo,"TAMANIO_ARCHIVO",string_itoa(tamanio_nuevo));
    config_save(config_archivo);
    config_destroy(config_archivo);
    free(nombre_archivo);
    free(path_archivo);
}

bool hay_bits_ocupados(t_bitarray* bitmap, int inicio, int fin){
    for (int i=inicio; i<fin;i++) {
        if(bitarray_test_bit(bitmap,i)){
            return true;
        }
    }
    return false;
}





void compactar_y_acomodar_al_final(void **bloques, t_bitarray *bitmap, int bloque_inicial, int bloque_final, uint32_t pid) {
    /*
    DialFS - Inicio Compactación: “PID: <PID> - Inicio Compactación.” 
    DialFS - Fin Compactación: “PID: <PID> - Fin Compactación.”
    */
    char* informar_inicio_compactacion = string_from_format("PID: %s - Inicio Compactación",string_itoa(pid)); 
    log_info(logger_entrada_salida, informar_inicio_compactacion);
    free(informar_inicio_compactacion);
    int indice_auxiliar = 0;
    int cantidad_a_mover = bloque_final - bloque_inicial +1;
    void **bloques_auxiliares = malloc(cantidad_a_mover * sizeof(void *));
    
    //Guardo temporalmente los bloques que quiero dejar al final y marco como libre el bitmap en esas posiciones
    for (int j = 0; j < cantidad_a_mover; ++j) {
        int i = bloque_inicial+j;
        bloques_auxiliares[j] = malloc(configuracion.BLOCK_SIZE);
        memcpy(bloques_auxiliares[j], bloques[i], configuracion.BLOCK_SIZE);
        bitarray_clean_bit(bitmap,i); 

    }
    msync(bitmap,bitarray_get_max_bit(bitmap),MS_SYNC);

    // Muevo todos los bloques ocupados a las primeras posiciones libres
    for (int i = 0; i < configuracion.BLOCK_COUNT; ++i) {
        if (bitarray_test_bit(bitmap, i)) {
            if (i != indice_auxiliar) { //si el bit no se encuentra en la primer posicion libre (indice auxiliar comienza en 0)
                
                void *bloque_desplazado = bloques[i];
                void *posicion_libre = bloques[indice_auxiliar];
                memcpy(posicion_libre, bloque_desplazado, configuracion.BLOCK_SIZE);
                bitarray_set_bit(bitmap, indice_auxiliar);
                bitarray_clean_bit(bitmap, i);
                //MODIFICO CONFIG DE ARCHIVO USANDO ARCHIVO DE INDICE
                modificar_bloque_inicial_indice(bloque_desplazado,indice_auxiliar);
            }
            indice_auxiliar++;
        }
    }
    msync(bitmap,bitarray_get_max_bit(bitmap),MS_SYNC);
    msync(bloques,tamanio_memoria_bloques,MS_SYNC);

    // Muevo los bloques almacenados temporalmente a las últimas posiciones libres
    for (size_t j = 0; j < cantidad_a_mover; ++j) {
        if (indice_auxiliar < configuracion.BLOCK_COUNT) {
            void *posicion_libre = bloques[indice_auxiliar];
            memcpy(posicion_libre, bloques_auxiliares[j], configuracion.BLOCK_SIZE);
            bitarray_set_bit(bitmap, indice_auxiliar);
            modificar_bloque_inicial_indice(bloque_inicial,indice_auxiliar);
            indice_auxiliar++;
        }
        free(bloques_auxiliares[j]); 
    }
    free(bloques_auxiliares);
    msync(bitmap,bitarray_get_max_bit(bitmap),MS_SYNC);
    msync(bloques,tamanio_memoria_bloques,MS_SYNC);
    // Limpio los bloques que quedan al final de la compactacion (si hubiere)
    for (size_t i = indice_auxiliar; i < configuracion.BLOCK_COUNT; ++i) {
        bitarray_clean_bit(bitmap, i);
    }
    msync(bitmap,bitarray_get_max_bit(bitmap),MS_SYNC);
    usleep(configuracion.RETRASO_COMPACTACION);
    char* informar_fin_compactacion = string_from_format("PID: %s - Fin Compactación",string_itoa(pid)); 
    log_info(logger_entrada_salida, informar_fin_compactacion);
    free(informar_fin_compactacion);
}

 void agregar_archivo_a_indice(int fd_indice, char* nombre_archivo, int bloque) {
    FILE* archivo_indice = fdopen(fd_indice, "a");
    if(archivo_indice == NULL) {
        log_info(logger_entrada_salida,"Error al abrir archivo de indices");
        return;
    }
    t_indice indice_del_archivo;
    indice_del_archivo.indice_de_bloques = bloque;
    strncpy(indice_del_archivo.nombre_archivo,nombre_archivo,PATH_MAX);
    fwrite(&indice_del_archivo,sizeof(t_indice),1,archivo_indice);
    fclose(archivo_indice);
 }

 void eliminar_archivo_de_indice(int fd_indice, char* nombre_archivo) {
    FILE* archivo_indice = fdopen(fd_indice, "rb");
    if(archivo_indice==NULL) {
        log_info(logger_entrada_salida,"Error al abrir archivo de indices");
        return;
    }
    int fd_archivo_temporal = open("indice_temporal.dat", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd_archivo_temporal == -1) {
        log_info(logger_entrada_salida,"Error al crear archivo de indices auxiliar");
        close(fd_indice);
        return;
    }
    FILE* archivo_temporal = fdopen(fd_archivo_temporal,"wb");
    if(archivo_temporal==NULL){
        log_info(logger_entrada_salida,"Error al abrir archivo de indices auxiliar");
        fclose(archivo_indice);
        close(fd_archivo_temporal);
        return;
    }
    t_indice indice_auxiliar;
    while(fread(&indice_auxiliar,sizeof(t_indice),1,archivo_indice)) { //recorro archivo de indices
        if(strcmp(indice_auxiliar.nombre_archivo,nombre_archivo)!=0) { //si el indice no coincide, lo grabo en el archivo temporal
            fwrite(&indice_auxiliar,sizeof(indice_auxiliar),1,archivo_temporal);
        }
    }
    fclose(archivo_indice);
    fclose(archivo_temporal);
    if(remove("indice.dat") == 0) {
        log_info(logger_entrada_salida,"Archivo de indices original borrado");
    }
    rename("indice_temporal.dat","indice.dat"); //quizas podria usar cosntances para evitar problemas debuggeando
    return;
 }

 int buscar_archivo_en_indice(int fd_indice, int bloque_inicial, char* nombre_archivo) { //devuelve -1 si hay error y -2 si no encuentra el archivo
    FILE* archivo_indices = fdopen(fd_indice, "rb");
    if(archivo_indices == NULL){
        log_info(logger_entrada_salida,"Error al abrir archivo de indices");
        return -1;
    }
    t_indice indice_del_archivo;
    while (fread(&indice_del_archivo,sizeof(t_indice),1,archivo_indices)==1){
        if(indice_del_archivo.indice_de_bloques == bloque_inicial) {
            strcpy(nombre_archivo,indice_del_archivo.nombre_archivo);
            fclose(archivo_indices);
            return 0;
        }
    }
    fclose(archivo_indices);
    return -2;
 }

 void modificar_bloque_inicial_indice (int bloque_inicial, int bloque_modificado) {
    char* nombre_del_archivo = string_new();
    if(buscar_archivo_en_indice(fd_indice,bloque_inicial,&nombre_del_archivo)==-2) { //si el bit no corresponde a un bloque inicial de archivo, no hago nada
        return;
    }
    char* path_archivo = string_new();
    string_append(&path_archivo,configuracion.PATH_BASE_DIALFS);
    string_append(&path_archivo,"/");
    string_append(&path_archivo,nombre_del_archivo);    
    t_config* config_archivo = config_create(path_archivo);
    config_set_value(config_archivo,"BLOQUE_INICIAL",string_itoa(bloque_modificado));
    config_save(config_archivo);
    config_destroy(config_archivo);
    free(nombre_del_archivo);
    free(path_archivo);
 }

void procesar_io_fs_write(buffer_kernel,pid){
    /*
        IO_FS_WRITE (Interfaz, Nombre Archivo, Registro Dirección, Registro Tamaño, Registro Puntero Archivo): 
        Esta instrucción solicita al Kernel que mediante la interfaz seleccionada, se lea desde Memoria la cantidad de bytes indicadas por el 
        Registro Tamaño a partir de la dirección lógica que se encuentra en el Registro Dirección y se escriban en el archivo a partir del valor 
        del Registro Puntero Archivo.
    */
    usleep(configuracion.TIEMPO_UNIDAD_TRABAJO*1000);

    uint32_t longitud_nombre_archivo = buffer_read_uint32(buffer_kernel);
    char* nombre_archivo = buffer_read_string(buffer_kernel,longitud_nombre_archivo);
    uint32_t tamanio_bytes_escritura = buffer_read_uint32(buffer_kernel);
    uint32_t bytes_a_escribir;
    uint32_t dir_fisica = buffer_read_uint32(buffer_kernel);
    buffer_read(buffer_kernel,&bytes_a_escribir, tamanio_bytes_escritura);
    uint32_t tamanio_puntero_archivo = buffer_read_uint32(buffer_kernel);
    uint32_t offset_archivo;
    buffer_read(buffer_kernel,&offset_archivo, tamanio_puntero_archivo);
    char* informar_leer_archivo = string_from_format("PID: %s - Escribir Archivo: %s - Tamaño a Escribir: %i - Puntero Archivo: %i",string_itoa(pid),nombre_archivo,bytes_a_leer,offset_archivo); 
    log_info(logger_entrada_salida, informar_leer_archivo);
    free(informar_leer_archivo);

    char* path_archivo = string_new();
    string_append(&path_archivo,configuracion.PATH_BASE_DIALFS);
    string_append(&path_archivo,"/");
    string_append(&path_archivo,nombre_archivo);
    t_config* config_archivo = config_create(path_archivo);
    int bloque_inicial_archivo = config_get_int_value(path_archivo,"BLOQUE_INICIAL");
    config_destroy(config_archivo);
    /*
    https://github.com/sisoputnfrba/foro/issues/4038 lo indica al reves
    */
    
    uint32_t accion =0;    
    t_paquete* paquete = crear_paquete(ACCESS_ESPACIO_USUARIO_ES, sizeof(uint32_t) * 4);
    buffer_add_uint32(paquete->buffer, pid);
    buffer_add_uint32(paquete->buffer, accion);
    buffer_add_uint32(paquete->buffer, dir_fisica);
    buffer_add_uint32(paquete->buffer, bytes_a_escribir);
    enviar_paquete(paquete, sockets.socket_memoria);
    eliminar_paquete(paquete);
    op_code codigo_leido;
    recv(sockets.socket_memoria,&codigo_leido , sizeof(op_code), 0);
    t_buffer* memoria = recibir_todo_elbuffer (sockets.socket_memoria);
    uint32_t* length = malloc(sizeof(uint32_t));
    char* info_de_memoria = buffer_read_string(memoria, length); 
    escribir_en_fs(bloque_inicial_archivo,offset_archivo,tamanio_bytes_escritura,&info_de_memoria);
    free(length);
    free(path_archivo);
    enviar_fin_de_instruccion();
}


void escribir_en_fs (int indice_bloques, uint32_t offset, uint32_t tamanio, void* data) {
    uint32_t bytes_escritos = 0;
    uint32_t bytes_a_escribir = tamanio;
    uint32_t offset_de_bloques = offset/configuracion.BLOCK_SIZE;
    uint32_t offset_dentro_de_bloque = offset % configuracion.BLOCK_SIZE;
    uint32_t nuevo_indice = indice_bloques + offset_de_bloques;


    while(bytes_escritos<tamanio){
        //si lo que tengo que escribir supera lo que tengo remanente en el bloque
        if(bytes_a_escribir > configuracion.BLOCK_SIZE - offset_dentro_de_bloque) {
            bytes_a_escribir = configuracion.BLOCK_SIZE - offset_de_bloques;
        }
        memcpy(bloques[nuevo_indice] + offset_dentro_de_bloque, data + bytes_escritos, bytes_a_escribir);
        bytes_escritos +=bytes_a_escribir;
        nuevo_indice ++;
        offset_dentro_de_bloque = 0;
    }
}


void procesar_io_fs_read(buffer_kernel,pid){
    usleep(configuracion.TIEMPO_UNIDAD_TRABAJO*1000);
    uint32_t longitud_nombre_archivo = buffer_read_uint32(buffer_kernel);
    char* nombre_archivo = buffer_read_string(buffer_kernel,longitud_nombre_archivo);
    uint32_t tamanio_bytes_escritura = buffer_read_uint32(buffer_kernel);
    uint32_t bytes_a_escribir;
    buffer_read(buffer_kernel,&bytes_a_escribir, tamanio_bytes_escritura);
    uint32_t tamanio_puntero_archivo = buffer_read_uint32(buffer_kernel);
    uint32_t offset_archivo;
    buffer_read(buffer_kernel,&offset_archivo, tamanio_puntero_archivo);
    char* informar_leer_archivo = string_from_format("PID: %s - Leer Archivo: %s - Tamaño a Leer: %i - Puntero Archivo: %i",string_itoa(pid),nombre_archivo,bytes_a_leer,offset_archivo); 
    log_info(logger_entrada_salida, informar_leer_archivo);
    free(informar_leer_archivo);
    //no veo registro de memoria donde escribir
    enviar_fin_de_instruccion();
}