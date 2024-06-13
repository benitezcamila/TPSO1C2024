#include <ciclo_de_instruccion.h>
#include <tlb.h>
#include <cpu_utils.h>
#include <math.h>

uint32_t PID;
registros_CPU* contexto_registros;
char* linea_de_instruccion;
const short max_long_instruccion = 6;
char* linea_de_instruccion_tokenizada[max_long_instruccion];
int llego_interrupcion;
tipo_de_interrupcion motivo_interrupcion;
int dir_logica = 0;
uint32_t dir_fisica = 0;

void ciclo_de_instruccion(){
    fetch_instruction();
    decode();
    execute();
    check_interrupt();
}

void fetch_instruction(){
    solicitar_instruccion_a_memoria();
    contexto_registros.PC += 1;
}

void decode(){
    recibir_instruccion_de_memoria();
    linea_de_instruccion_tokenizada = string_n_split(linea_de_instruccion, max_long_instruccion, " ");
}

void execute(){
    //Revisar si esta comparación está bien hecha.
    if(strcmp(linea_de_instruccion_tokenizada[0], "SET") == 0){
         //Debería loggear qué instrucción se usa.
        uint32_t valor = atoi(linea_de_instruccion_tokenizada[2]);
        set(linea_de_instruccion_tokenizada[1], valor);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "MOV_IN") == 0){
        //Debería loggear qué instrucción se usa.
	    mov_in(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2]);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "MOV_OUT") == 0){
        //Debería loggear qué instrucción se usa.
	    mov_out(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2]);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "SUM") == 0){
        //Debería loggear qué instrucción se usa.
        sum(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2]);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "SUB") == 0){
        //Debería loggear qué instrucción se usa.
        sub(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2]);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "JNZ") == 0){
        //Debería loggear qué instrucción se usa.
        uint32_t proxInstruccion = atoi(linea_de_instruccion_tokenizada[2]);
        jump_no_zero(linea_de_instruccion_tokenizada[1], proxInstruccion);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "RESIZE") == 0){
        //Debería loggear qué instrucción se usa.
		int tamano = atoi(linea_de_instruccion_tokenizada[1]);
		resize(tamano);
	}
    else if(strcmp(linea_de_instruccion_tokenizada[0], "COPY_STRING") == 0){
        //Debería loggear qué instrucción se usa.
		int tamano = atoi(linea_de_instruccion_tokenizada[1]);
		copy_string(tamano);
	}
    else if(strcmp(linea_de_instruccion_tokenizada[0], "IO_GEN_SLEEP") == 0){
        //Debería loggear qué instrucción se usa.
        uint32_t unidades_de_trabajo = (uint32_t)atoi(linea_de_instruccion_tokenizada[2])
        io_gen_sleep(linea_de_instruccion_tokenizada[1], unidades_de_trabajo);
    }
    else if(strcmp(linea_de_instruccion_tokenizada[0], "IO_STDIN_READ") == 0){
        //Debería loggear qué instrucción se usa.
		io_stdin_read(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2], linea_de_instruccion_tokenizada[3]);
	}
    else if(strcmp(linea_de_instruccion_tokenizada[0], "IO_STDOUT_WRITE") == 0){
        //Debería loggear qué instrucción se usa.
		io_stdout_write(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2], linea_de_instruccion_tokenizada[3]);
	}
    else if(strcmp(linea_de_instruccion_tokenizada[0], "EXIT") == 0){
        //Debería loggear qué instrucción se usa.
		exit_process();
	}
    //Y así...
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

// Función para traducir direcciones lógicas a físicas
uint32_t mmu(t_TLB* tlb, uint32_t pid, uint32_t tamano_pagina){
    uint32_t numero_pagina = floor(dir_logica / tamano_pagina);
    uint32_t desplazamiento = dir_logica - (numero_pagina * tamano_pagina);
    int marco = buscar_en_TLB(tlb, pid, numero_pagina);

    //REVISAR ESTA PARTE
    if (marco == -1) { //TLB Miss
        if (numero_pagina < tabla->tamano && tabla->marcos[numero_pagina] != -1) {
            marco = tabla->marcos[numero_pagina];
            agregar_entrada_TLB(tlb, pid, numero_pagina, marco);
        } else {
            return -1; //Esto no debería ser posible técnicamente
        }
    }

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
    dir_logica = obtener_contenido_registro(registro_direccion);
    dir_fisica = mmu(tlb, PID, tamano_pagina); //No sé de dónde saco tlb y tamano_pagina.

    solicitar_leer_en_memoria(dir_fisica, es_registro_de_4_bytes(registro_datos));
    leer_de_memoria_y_registrar(registro_datos);
}

void mov_out(char* registro_direccion, char* registro_datos){
    dir_logica = obtener_contenido_registro(registro_direccion);
    dir_fisica = mmu(tlb, PID, tamano_pagina); //No sé de dónde saco tlb y tamano_pagina.

    uint32_t datos_de_registro = obtener_contenido_registro(registro_datos);

    solicitar_escribir_en_memoria(dir_fisica, datos_de_registro);
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

void resize(int tamanio){
    t_paquete* paquete = crear_paquete(AJUSTAR_TAMANIO, sizeof(uint32_t) * 2);
    buffer_add_uint32(paquete->buffer, PID);
    buffer_add_uint32(paquete->buffer, tamanio);

    enviar_paquete(paquete, sockets.socket_memoria);
}

void copy_string(){
    //
}

void io_gen_sleep(char* nombre_interfaz, uint32_t unidades_de_trabajo){
    ind_contexto_kernel = 0;
    t_paquete* paquete = crear_paquete(IO_GEN_SLEEP, sizeof(registros_CPU) + sizeof(motivo_desalojo));
    buffer_add_string(paquete->buffer, string_length(nombre_interfaz)+1, nombre_interfaz);
    buffer_add_uint32(paquete->buffer, unidades_de_trabajo);

    enviar_paquete(paquete, sockets.socket_server_D);
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

int obtener_contenido_registro(char* registro){
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

bool es_registro_de_4_bytes(char* registro){
    if(strcmp(registro, "AX") == 0 || strcmp(registro, "BX") == 0
            || strcmp(registro, "CX") == 0 || strcmp(registro, "DX") == 0){
        return false;
    }

    return true;
}

void leer_de_memoria_y_registrar(char* registro_datos){
    uint32_t datos_de_memoria;

    t_paquete* paquete = malloc(sizeof(t_paquete));
    recv(sockets.socket_memoria, &(paquete->codigo_operacion), sizeof(op_code), MSG_WAITALL);

    if(paquete->codigo_operacion == RESPUESTA_LECTURA_MEMORIA){
        recv(sockets.socket_memoria, &(paquete->buffer->size), sizeof(uint32_t), MSG_WAITALL);
        paquete->buffer->stream = malloc(paquete->buffer->size);

        recv(sockets.socket_memoria, paquete->buffer->stream, sizeof(uint32_t), MSG_WAITALL);
        buffer_read_uint32(paquete->buffer, &datos_de_memoria); //Recibo un uint32_t porque puede contener un uint8_t.
                                                                //así que no debería haber problema a la hora de registrarlo.

        set(registro_datos, datos_de_memoria);
    }
    else{
        //Loggear error.
    }
}