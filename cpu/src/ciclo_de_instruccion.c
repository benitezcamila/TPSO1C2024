#include <ciclo_de_instruccion.h>
#include <tlb.h>
#include <cpu_utils.h>
#include <math.h>

uint32_t PID;
registros_CPU* contexto_registros;
char* linea_de_instruccion;
char** linea_de_instruccion_tokenizada;
uint32_t* longitud_linea_instruccion;
tipo_de_interrupcion motivo_interrupcion;
uint32_t dir_logica = 0;
uint32_t dir_fisica = 0;
uint32_t tamano_pagina; //REVISAR.

void ciclo_de_instruccion(){
    fetch_instruction();
    decode();
    execute();
    check_interrupt();
}

void fetch_instruction(){
    log_info(logger_cpu, "PID: %d - FETCH - Program Counter: %d", PID, contexto_registros->PC);
    solicitar_instruccion_a_memoria();
    contexto_registros->PC += 1;
}

void decode(){
    recibir_instruccion_de_memoria(longitud_linea_instruccion);
    linea_de_instruccion_tokenizada = string_n_split(linea_de_instruccion, *longitud_linea_instruccion, " ");
}

void execute(){
    if(strcmp(linea_de_instruccion_tokenizada[0], "SET") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
        uint32_t valor = atoi(linea_de_instruccion_tokenizada[2]);
        set(linea_de_instruccion_tokenizada[1], valor);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "MOV_IN") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
	    mov_in(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2]);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "MOV_OUT") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
	    mov_out(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2]);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "SUM") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
        sum(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2]);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "SUB") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
        sub(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2]);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "JNZ") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
        uint32_t proxInstruccion = atoi(linea_de_instruccion_tokenizada[2]);
        jump_no_zero(linea_de_instruccion_tokenizada[1], proxInstruccion);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "RESIZE") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
		uint tamanio = atoi(linea_de_instruccion_tokenizada[1]);
		resize(tamanio);
	}
    else if(strcmp(linea_de_instruccion_tokenizada[0], "COPY_STRING") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
		uint32_t tamanio = atoi(linea_de_instruccion_tokenizada[1]);
		copy_string(tamanio);
	}
    else if(strcmp(linea_de_instruccion_tokenizada[0], "IO_GEN_SLEEP") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
        uint32_t unidades_de_trabajo = atoi(linea_de_instruccion_tokenizada[2]);
        io_gen_sleep(linea_de_instruccion_tokenizada[1], unidades_de_trabajo);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "IO_STDIN_READ") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
		io_stdin_read(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2], linea_de_instruccion_tokenizada[3]);
	}
    else if(strcmp(linea_de_instruccion_tokenizada[0], "IO_STDOUT_WRITE") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
		io_stdout_write(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2], linea_de_instruccion_tokenizada[3]);
	}
    else if(strcmp(linea_de_instruccion_tokenizada[0], "IO_FS_CREATE") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
		io_fs_create(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2]);
	}
    else if(strcmp(linea_de_instruccion_tokenizada[0], "IO_FS_DELETE") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
		io_fs_delete(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2]);
	}
    else if(strcmp(linea_de_instruccion_tokenizada[0], "IO_FS_TRUNCATE") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
		io_fs_truncate(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2], linea_de_instruccion_tokenizada[3]);
	}
    else if(strcmp(linea_de_instruccion_tokenizada[0], "IO_FS_WRITE") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
		io_fs_write(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2], linea_de_instruccion_tokenizada[3],
                     linea_de_instruccion_tokenizada[4], linea_de_instruccion_tokenizada[5]);
	}
    else if(strcmp(linea_de_instruccion_tokenizada[0], "IO_FS_READ") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
		io_fs_read(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2], linea_de_instruccion_tokenizada[3],
                     linea_de_instruccion_tokenizada[4], linea_de_instruccion_tokenizada[5]);
	}
    else if(strcmp(linea_de_instruccion_tokenizada[0], "EXIT") == 0){
        loggear_instruccion(*longitud_linea_instruccion);
		exit_process();
	}
}

//NO SÉ SI LE FALTA ALGO. CHECKEAR.
void check_interrupt(){
    if(llego_interrupcion == 1){
        llego_interrupcion = 0;

        switch(motivo_interrupcion){
        case DESALOJO_QUANTUM:
            enviar_contexto_a_kernel(DESALOJO_QUANTUM);
            break;

        default:
            //Loggear error.
            break;
        }
    }
}

// Función para traducir direcciones lógicas a físicas.
uint32_t mmu(t_TLB* tlb, uint32_t pid){
    uint32_t numero_pagina = floor(dir_logica / tamano_pagina);
    uint32_t desplazamiento = dir_logica - (numero_pagina * tamano_pagina);
    int marco = buscar_en_TLB(tlb, pid, numero_pagina);

    if (marco == -1) { //TLB Miss
        log_info(logger_cpu, "PID: %d - TLB MISS - Pagina: %d", PID, numero_pagina);
        marco = solicitar_marco_a_memoria(numero_pagina); 
    }

    //No sé si este log está bien puesto acá
    log_info(logger_cpu, "PID: %d - OBTENER MARCO - Página: %d - Marco: %d", PID, numero_pagina, marco);
    actualizar_TLB(tlb, pid, numero_pagina, marco);

    return (marco * tamano_pagina) + desplazamiento;
}

//Funciones de instrucciones.
void set(char* registro, uint32_t valor){
    if(strcmp(registro, "AX") == 0){
        contexto_registros->AX = valor;
    }
    else if(strcmp(registro, "BX") == 0){
        contexto_registros->BX = valor;
    }
    else if(strcmp(registro, "CX") == 0){
        contexto_registros->CX = valor;
    }
    else if(strcmp(registro, "DX") == 0){
        contexto_registros->DX = valor;
    }
    else if(strcmp(registro, "EAX") == 0){
        contexto_registros->EAX = valor;
    }
    else if(strcmp(registro, "EBX") == 0){
        contexto_registros->EBX = valor;
    }
    else if(strcmp(registro, "ECX") == 0){
        contexto_registros->ECX = valor;
    }
    else if(strcmp(registro, "EDX") == 0){
        contexto_registros->EDX = valor;
    }
    else if(strcmp(registro, "SI") == 0){
        contexto_registros->SI = valor;
    }
    else if(strcmp(registro, "DI") == 0){
        contexto_registros->DI = valor;
    }
}

void mov_in(char* registro_datos, char* registro_direccion){
    dir_logica = (uint32_t) obtener_contenido_registro(registro_direccion);
    dir_fisica = mmu(tlb, PID);
    
    if(registro_datos[0] == 'E' || registro_datos[1] == 'I'){
        solicitar_leer_en_memoria(dir_fisica, sizeof(uint32_t));
        
        void* datos_de_memoria = malloc(sizeof(uint32_t));
        datos_de_memoria = leer_de_memoria(sizeof(uint32_t));
        
        set(registro_datos, datos_de_memoria);
        log_info(logger_cpu, "PID: %d - Acción: LEER - Dirección Física: %d - Valor: %d", PID, dir_fisica, (uint32_t)datos_de_memoria);
        free(datos_de_memoria);
    }
    else{
        solicitar_leer_en_memoria(dir_fisica, sizeof(uint8_t));
        
        void* datos_de_memoria = malloc(sizeof(uint8_t));
        datos_de_memoria = leer_de_memoria(sizeof(uint8_t));
        
        set(registro_datos, datos_de_memoria);
        log_info(logger_cpu, "PID: %d - Acción: LEER - Dirección Física: %d - Valor: %d", PID, dir_fisica, (uint8_t)datos_de_memoria);
        free(datos_de_memoria);
    }
}

void mov_out(char* registro_direccion, char* registro_datos){
    dir_logica = (uint32_t) obtener_contenido_registro(registro_direccion);
    dir_fisica = mmu(tlb, PID);
    
    if(registro_datos[0] == 'E' || registro_datos[1] == 'I'){
        void* datos_de_registro = malloc(sizeof(uint32_t));
        datos_de_registro = obtener_contenido_registro(registro_datos);

        solicitar_escribir_en_memoria(dir_fisica, datos_de_registro, sizeof(uint32_t));

        free(datos_de_registro);
        //Seguro tenga que usar una función auxiliar para asegurarme que la escritura haya sido exitosa,
        //y ahí mismo hago el loggeo de la escritura.
    }
    else{
        void* datos_de_registro = malloc(sizeof(uint8_t));
        datos_de_registro = obtener_contenido_registro(registro_datos);

        solicitar_escribir_en_memoria(dir_fisica, datos_de_registro, sizeof(uint8_t));

        free(datos_de_registro);
        //Seguro tenga que usar una función auxiliar para asegurarme que la escritura haya sido exitosa,
        //y ahí mismo hago el loggeo de la escritura.
    }
}

void sum(char* registro1, char* registro2){
    if(strcmp(registro2, "AX") == 0){
        sumar_contenido_registro(registro1, (int)contexto_registros->AX);
    }
    else if(strcmp(registro2, "BX") == 0){
        sumar_contenido_registro(registro1, (int)contexto_registros->BX);
    }
    else if(strcmp(registro2, "CX") == 0){
        sumar_contenido_registro(registro1, (int)contexto_registros->CX);
    }
    else if(strcmp(registro2, "DX") == 0){
        sumar_contenido_registro(registro1, (int)contexto_registros->DX);
    }
    else if(strcmp(registro2, "EAX") == 0){
        sumar_contenido_registro(registro1, (int)contexto_registros->EAX);
    }
    else if(strcmp(registro2, "EBX") == 0){
        sumar_contenido_registro(registro1, (int)contexto_registros->EBX);
    }
    else if(strcmp(registro2, "ECX") == 0){
        sumar_contenido_registro(registro1, (int)contexto_registros->ECX);
    }
    else if(strcmp(registro2, "EDX") == 0){
        sumar_contenido_registro(registro1, (int)contexto_registros->EDX);
    }
    else if(strcmp(registro2, "SI") == 0){
        sumar_contenido_registro(registro1, (int)contexto_registros->SI);
    }
    else if(strcmp(registro2, "DI") == 0){
        sumar_contenido_registro(registro1, (int)contexto_registros->DI);
    }
}

void sub(char* registro1, char* registro2){
    if(strcmp(registro2, "AX") == 0){
        restar_contenido_registro(registro1, (int)contexto_registros->AX);
    }
    else if(strcmp(registro2, "BX") == 0){
        restar_contenido_registro(registro1, (int)contexto_registros->BX);
    }
    else if(strcmp(registro2, "CX") == 0){
        restar_contenido_registro(registro1, (int)contexto_registros->CX);
    }
    else if(strcmp(registro2, "DX") == 0){
        restar_contenido_registro(registro1, (int)contexto_registros->DX);
    }
    else if(strcmp(registro2, "EAX") == 0){
        restar_contenido_registro(registro1, (int)contexto_registros->EAX);
    }
    else if(strcmp(registro2, "EBX") == 0){
        restar_contenido_registro(registro1, (int)contexto_registros->EBX);
    }
    else if(strcmp(registro2, "ECX") == 0){
        restar_contenido_registro(registro1, (int)contexto_registros->ECX);
    }
    else if(strcmp(registro2, "EDX") == 0){
        restar_contenido_registro(registro1, (int)contexto_registros->EDX);
    }
    else if(strcmp(registro2, "SI") == 0){
        restar_contenido_registro(registro1, (int)contexto_registros->SI);
    }
    else if(strcmp(registro2, "DI") == 0){
        restar_contenido_registro(registro1, (int)contexto_registros->DI);
    }
}

void jump_no_zero(char* registro, uint32_t nro_instruccion){
    if(strcmp(registro, "AX") == 0){
        if(contexto_registros->AX != 0){
            contexto_registros->PC = nro_instruccion;
        }
    }
    else if(strcmp(registro, "BX") == 0){
        if(contexto_registros->BX != 0){
            contexto_registros->PC = nro_instruccion;
        }
    }
    else if(strcmp(registro, "CX") == 0){
        if(contexto_registros->CX != 0){
            contexto_registros->PC = nro_instruccion;
        }
    }
    else if(strcmp(registro, "DX") == 0){
        if(contexto_registros->DX != 0){
            contexto_registros->PC = nro_instruccion;
        }
    }
    else if(strcmp(registro, "EAX") == 0){
        if(contexto_registros->EAX != 0){
            contexto_registros->PC = nro_instruccion;
        }
    }
    else if(strcmp(registro, "EBX") == 0){
        if(contexto_registros->EBX != 0){
            contexto_registros->PC = nro_instruccion;
        }
    }
    else if(strcmp(registro, "ECX") == 0){
        if(contexto_registros->ECX != 0){
            contexto_registros->PC = nro_instruccion;
        }
    }
    else if(strcmp(registro, "EDX") == 0){
        if(contexto_registros->EDX != 0){
            contexto_registros->PC = nro_instruccion;
        }
    }
    else if(strcmp(registro, "SI") == 0){
        if(contexto_registros->SI != 0){
            contexto_registros->PC = nro_instruccion;
        }
    }
    else if(strcmp(registro, "DI") == 0){
        if(contexto_registros->DI != 0){
            contexto_registros->PC = nro_instruccion;
        }
    }
}

void resize(uint32_t tamanio){
    t_paquete* paquete = crear_paquete(AJUSTAR_TAMANIO, sizeof(uint32_t) * 2);
    buffer_add_uint32(paquete->buffer, PID);
    buffer_add_uint32(paquete->buffer, tamanio);
    
    enviar_paquete(paquete, sockets.socket_memoria);

    recibir_respuesta_resize_memoria(PID);
}

void copy_string(uint32_t tamanio){
    dir_logica = (uint32_t) obtener_contenido_registro("SI");
    dir_fisica = mmu(tlb, PID);
    void* datos_de_memoria = malloc(tamanio);

    solicitar_leer_en_memoria(dir_fisica, tamanio);
    datos_de_memoria = leer_de_memoria(tamanio);

    dir_logica = (uint32_t) obtener_contenido_registro("DI");
    dir_fisica = mmu(tlb, PID);

    solicitar_escribir_en_memoria(dir_fisica, datos_de_memoria, tamanio);
}

void wait(char* nombre_recurso){
    ind_contexto_kernel = 0;

    t_paquete* paquete = crear_paquete(CONTEXTO_EXEC, sizeof(motivo_desalojo) + sizeof(registros_CPU)
                                        + string_length(nombre_recurso)+1);
    motivo_desalojo mot = PETICION_RECURSO;
    buffer_add(paquete->buffer, &mot, sizeof(motivo_desalojo));
    buffer_add(paquete->buffer, contexto_registros, sizeof(registros_CPU));
    buffer_add_string(paquete->buffer, string_length(nombre_recurso)+1, nombre_recurso);

    enviar_paquete(paquete, sockets.socket_server_D);
}

void signal(char* nombre_recurso){
    ind_contexto_kernel = 0;

    t_paquete* paquete = crear_paquete(CONTEXTO_EXEC, sizeof(motivo_desalojo) + sizeof(registros_CPU)
                                        + string_length(nombre_recurso)+1);
    motivo_desalojo mot = SIGNAL_RECURSO;
    buffer_add(paquete->buffer, &mot, sizeof(motivo_desalojo));
    buffer_add(paquete->buffer, contexto_registros, sizeof(registros_CPU));
    buffer_add_string(paquete->buffer, string_length(nombre_recurso)+1, nombre_recurso);

    enviar_paquete(paquete, sockets.socket_server_D);
}

void io_gen_sleep(char* nombre_interfaz, uint32_t unidades_de_trabajo){
    ind_contexto_kernel = 0;

    t_paquete* paquete = crear_paquete(CONTEXTO_EXEC, sizeof(motivo_desalojo) + sizeof(registros_CPU)
                                        + string_length(nombre_interfaz)+1 + sizeof(uint32_t));
    motivo_desalojo mot = IO_GEN_SLEEP;                                    
    buffer_add(paquete->buffer, &mot, sizeof(motivo_desalojo));
    buffer_add(paquete->buffer, contexto_registros, sizeof(registros_CPU));
    buffer_add_string(paquete->buffer, string_length(nombre_interfaz)+1, nombre_interfaz);
    buffer_add_uint32(paquete->buffer, unidades_de_trabajo);

    enviar_paquete(paquete, sockets.socket_server_D);
}

void io_stdin_read(char* nombre_interfaz, char* registro_direccion, char* registro_tamanio){
    ind_contexto_kernel = 0;
    dir_logica = (uint32_t) obtener_contenido_registro(registro_direccion);
    dir_fisica = mmu(tlb, PID);

    if(registro_tamanio[0] == 'E' || registro_tamanio[1] == 'I'){
        void* tamanio = malloc(sizeof(uint32_t));
        tamanio = obtener_contenido_registro(registro_tamanio);

        enviar_std_a_kernel(IO_STDIN_READ, nombre_interfaz, tamanio, sizeof(uint32_t), dir_fisica);

        free(tamanio);
    }
    else{
        void* tamanio = malloc(sizeof(uint8_t));
        tamanio = obtener_contenido_registro(registro_tamanio);

        enviar_std_a_kernel(IO_STDIN_READ, nombre_interfaz, tamanio, sizeof(uint8_t), dir_fisica);

        free(tamanio);
    }
}

void io_stdout_write(char* nombre_interfaz, char* registro_direccion, char* registro_tamanio){
    ind_contexto_kernel = 0;
    dir_logica = (uint32_t) obtener_contenido_registro(registro_direccion);
    dir_fisica = mmu(tlb, PID);

    if(registro_tamanio[0] == 'E' || registro_tamanio[1] == 'I'){
        void* tamanio = malloc(sizeof(uint32_t));
        tamanio = obtener_contenido_registro(registro_tamanio);

        enviar_std_a_kernel(IO_STDOUT_WRITE, nombre_interfaz, tamanio, sizeof(uint32_t), dir_fisica);

        free(tamanio);
    }
    else{
        void* tamanio = malloc(sizeof(uint8_t));
        tamanio = obtener_contenido_registro(registro_tamanio);

        enviar_std_a_kernel(IO_STDOUT_WRITE, nombre_interfaz, tamanio, sizeof(uint8_t), dir_fisica);

        free(tamanio);
    }
}

void io_fs_create(char* nombre_interfaz, char* nombre_archivo){
    ind_contexto_kernel = 0;

    solicitar_create_delete_fs_a_kernel(IO_FS_CREATE, nombre_interfaz, nombre_archivo);
}

void io_fs_delete(char* nombre_interfaz, char* nombre_archivo){
    ind_contexto_kernel = 0;

    solicitar_create_delete_fs_a_kernel(IO_FS_DELETE, nombre_interfaz, nombre_archivo);
}

void io_fs_truncate(char* nombre_interfaz, char* nombre_archivo, char* registro_tamanio){
    ind_contexto_kernel = 0;

    if(registro_tamanio[0] == 'E' || registro_tamanio[1] == 'I'){
        void* tamanio = malloc(sizeof(uint32_t));
        tamanio = obtener_contenido_registro(registro_tamanio);

        solicitar_truncate_fs_a_kernel(IO_FS_TRUNCATE, nombre_interfaz, nombre_archivo, tamanio, sizeof(uint32_t));

        free(tamanio);
    }
    else{
        void* tamanio = malloc(sizeof(uint8_t));
        tamanio = obtener_contenido_registro(registro_tamanio);

        solicitar_truncate_fs_a_kernel(IO_FS_TRUNCATE, nombre_interfaz, nombre_archivo, tamanio, sizeof(uint8_t));

        free(tamanio);
    }
}

void io_fs_write(char* nombre_interfaz, char* nombre_archivo, char* registro_direccion,
                char* registro_tamanio, char* registro_puntero_archivo){
    ind_contexto_kernel = 0;
    dir_logica = (uint32_t) obtener_contenido_registro(registro_direccion);
    dir_fisica = mmu(tlb, PID);

    if(registro_tamanio[0] == 'E' || registro_tamanio[1] == 'I'){
        void* tamanio = malloc(sizeof(uint32_t));
        tamanio = obtener_contenido_registro(registro_tamanio);

        if(registro_puntero_archivo[0] == 'E' || registro_puntero_archivo[1] == 'I'){
            void* puntero_registro = malloc(sizeof(uint32_t));
            puntero_registro = obtener_contenido_registro(registro_puntero_archivo);

            solicitar_write_read_fs_a_kernel(IO_FS_WRITE, nombre_interfaz, nombre_archivo, tamanio, sizeof(uint32_t),
                                            dir_fisica, puntero_registro, sizeof(uint32_t));
        
            free(puntero_registro);
        }
        else{
            void* puntero_registro = malloc(sizeof(uint8_t));
            puntero_registro = obtener_contenido_registro(registro_puntero_archivo);

            solicitar_write_read_fs_a_kernel(IO_FS_WRITE, nombre_interfaz, nombre_archivo, tamanio, sizeof(uint32_t),
                                            dir_fisica, puntero_registro, sizeof(uint8_t));
        
            free(puntero_registro);
        }

        free(tamanio);
    }
    else{
        void* tamanio = malloc(sizeof(uint8_t));
        tamanio = obtener_contenido_registro(registro_tamanio);

        if(registro_puntero_archivo[0] == 'E' || registro_puntero_archivo[1] == 'I'){
            void* puntero_registro = malloc(sizeof(uint32_t));
            puntero_registro = obtener_contenido_registro(registro_puntero_archivo);

            solicitar_write_read_fs_a_kernel(IO_FS_WRITE, nombre_interfaz, nombre_archivo, tamanio, sizeof(uint8_t),
                                            dir_fisica, puntero_registro, sizeof(uint32_t));
        
            free(puntero_registro);
        }
        else{
            void* puntero_registro = malloc(sizeof(uint8_t));
            puntero_registro = obtener_contenido_registro(registro_puntero_archivo);

            solicitar_write_read_fs_a_kernel(IO_FS_WRITE, nombre_interfaz, nombre_archivo, tamanio, sizeof(uint8_t),
                                            dir_fisica, puntero_registro, sizeof(uint8_t));

            free(puntero_registro);
        }

        free(tamanio);
    }
}

void io_fs_read(char* nombre_interfaz, char* nombre_archivo, char* registro_direccion,
                char* registro_tamanio, char* registro_puntero_archivo){
    ind_contexto_kernel = 0;
    dir_logica = (uint32_t) obtener_contenido_registro(registro_direccion);
    dir_fisica = mmu(tlb, PID);

    if(registro_tamanio[0] == 'E' || registro_tamanio[1] == 'I'){
        void* tamanio = malloc(sizeof(uint32_t));
        tamanio = obtener_contenido_registro(registro_tamanio);

        if(registro_puntero_archivo[0] == 'E' || registro_puntero_archivo[1] == 'I'){
            void* puntero_registro = malloc(sizeof(uint32_t));
            puntero_registro = obtener_contenido_registro(registro_puntero_archivo);

            solicitar_write_read_fs_a_kernel(IO_FS_READ, nombre_interfaz, nombre_archivo, tamanio, sizeof(uint32_t),
                                            dir_fisica, puntero_registro, sizeof(uint32_t));
        
            free(puntero_registro);
        }
        else{
            void* puntero_registro = malloc(sizeof(uint8_t));
            puntero_registro = obtener_contenido_registro(registro_puntero_archivo);

            solicitar_write_read_fs_a_kernel(IO_FS_READ, nombre_interfaz, nombre_archivo, tamanio, sizeof(uint32_t),
                                            dir_fisica, puntero_registro, sizeof(uint8_t));
        
            free(puntero_registro);
        }

        free(tamanio);
    }
    else{
        void* tamanio = malloc(sizeof(uint8_t));
        tamanio = obtener_contenido_registro(registro_tamanio);

        if(registro_puntero_archivo[0] == 'E' || registro_puntero_archivo[1] == 'I'){
            void* puntero_registro = malloc(sizeof(uint32_t));
            puntero_registro = obtener_contenido_registro(registro_puntero_archivo);

            solicitar_write_read_fs_a_kernel(IO_FS_READ, nombre_interfaz, nombre_archivo, tamanio, sizeof(uint8_t),
                                            dir_fisica, puntero_registro, sizeof(uint32_t));
        
            free(puntero_registro);
        }
        else{
            void* puntero_registro = malloc(sizeof(uint8_t));
            puntero_registro = obtener_contenido_registro(registro_puntero_archivo);

            solicitar_write_read_fs_a_kernel(IO_FS_READ, nombre_interfaz, nombre_archivo, tamanio, sizeof(uint8_t),
                                            dir_fisica, puntero_registro, sizeof(uint8_t));

            free(puntero_registro);
        }

        free(tamanio);
    }
}

void exit_process(){
    ind_contexto_kernel = 0;

    enviar_contexto_a_kernel(PROCESS_EXIT);
}

//FUNCIONES AUXILIARES.
void sumar_contenido_registro(char* registro, uint32_t valor){
    if(strcmp(registro, "AX") == 0){
        contexto_registros->AX += valor;
    }
    else if(strcmp(registro, "BX") == 0){
        contexto_registros->BX += valor;
    }
    else if(strcmp(registro, "CX") == 0){
        contexto_registros->CX += valor;
    }
    else if(strcmp(registro, "DX") == 0){
        contexto_registros->DX += valor;
    }
    else if(strcmp(registro, "EAX") == 0){
        contexto_registros->EAX += valor;
    }
    else if(strcmp(registro, "EBX") == 0){
        contexto_registros->EBX += valor;
    }
    else if(strcmp(registro, "ECX") == 0){
        contexto_registros->ECX += valor;
    }
    else if(strcmp(registro, "EDX") == 0){
        contexto_registros->EDX += valor;
    }
    else if(strcmp(registro, "SI") == 0){
        contexto_registros->SI += valor;
    }
    else if(strcmp(registro, "DI") == 0){
        contexto_registros->DI += valor;
    }
}

void restar_contenido_registro(char* registro, uint32_t valor){
    if(strcmp(registro, "AX") == 0){
        contexto_registros->AX -= valor;
    }
    else if(strcmp(registro, "BX") == 0){
        contexto_registros->BX -= valor;
    }
    else if(strcmp(registro, "CX") == 0){
        contexto_registros->CX -= valor;
    }
    else if(strcmp(registro, "DX") == 0){
        contexto_registros->DX -= valor;
    }
    else if(strcmp(registro, "EAX") == 0){
        contexto_registros->EAX -= valor;
    }
    else if(strcmp(registro, "EBX") == 0){
        contexto_registros->EBX -= valor;
    }
    else if(strcmp(registro, "ECX") == 0){
        contexto_registros->ECX -= valor;
    }
    else if(strcmp(registro, "EDX") == 0){
        contexto_registros->EDX -= valor;
    }
    else if(strcmp(registro, "SI") == 0){
        contexto_registros->SI -= valor;
    }
    else if(strcmp(registro, "DI") == 0){
        contexto_registros->DI -= valor;
    }
}

//Esto puede explotar.
void* obtener_contenido_registro(char* registro){
    if(strcmp(registro, "AX") == 0){
        return contexto_registros->AX;
    }
    else if(strcmp(registro, "BX") == 0){
        return contexto_registros->BX;
    }
    else if(strcmp(registro, "CX") == 0){
        return contexto_registros->CX;
    }
    else if(strcmp(registro, "DX") == 0){
        return contexto_registros->DX;
    }
    else if(strcmp(registro, "EAX") == 0){
        return contexto_registros->EAX;
    }
    else if(strcmp(registro, "EBX") == 0){
        return contexto_registros->EBX;
    }
    else if(strcmp(registro, "ECX") == 0){
        return contexto_registros->ECX;
    }
    else if(strcmp(registro, "EDX") == 0){
        return contexto_registros->EDX;
    }
    else if(strcmp(registro, "SI") == 0){
        return contexto_registros->SI;
    }
    else if(strcmp(registro, "DI") == 0){
        return contexto_registros->DI;
    }
}

void loggear_instruccion(int cant_parametros){
    switch(cant_parametros){
        case 0:
        log_info(logger_cpu, "PID: %d - Ejecutando: %s", PID, linea_de_instruccion_tokenizada[0]);

        case 1:
        log_info(logger_cpu, "PID: %d - Ejecutando: %s - %s", PID, linea_de_instruccion_tokenizada[0],
                linea_de_instruccion_tokenizada[1]);

        case 2:
        log_info(logger_cpu, "PID: %d - Ejecutando: %s - %s %s", PID, linea_de_instruccion_tokenizada[0],
                linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2]);

        case 3:
        log_info(logger_cpu, "PID: %d - Ejecutando: %s - %s %s %s", PID, linea_de_instruccion_tokenizada[0],
                linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2],
                linea_de_instruccion_tokenizada[3]);

        case 5:
        log_info(logger_cpu, "PID: %d - Ejecutando: %s - %s %s %s %s %s", PID, linea_de_instruccion_tokenizada[0],
                linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2],
                linea_de_instruccion_tokenizada[3], linea_de_instruccion_tokenizada[4],
                linea_de_instruccion_tokenizada[5]);

        default:
        //Loggear error.
    }
}