#include <ciclo_de_instruccion.h>
#include <tlb.h>
#include <cpu_utils.h>
#include <math.h>

registros_CPU* contexto_registros;
char* linea_de_instruccion;
const short max_long_instruccion = 6;
char* linea_de_instruccion_tokenizada[max_long_instruccion];
int llego_interrupcion;
tipo_de_interrupcion motivo_interrupcion;

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

   if(instruccion_usa_mmu){
    mmu(); //ACÁ DEBERÍA TENER UNA VARIABLE QUE GUARDE LA DIRECCIÓN OBTENIDA.
   }
}

void execute(){
   //Revisar si esta comparación está bien hecha.
   if(strcmp(linea_de_instruccion_tokenizada[0], "SET") == 0){
      //Debería loggear qué instrucción se usa.
      int valor = atoi(linea_de_instruccion_tokenizada[2])
      set(linea_de_instruccion_tokenizada[1], valor);
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
      int proxInstruccion = atoi(linea_de_instruccion_tokenizada[2]);
      jump_no_zero(linea_de_instruccion_tokenizada[1], proxInstruccion);
   }
   else if(strcmp(linea_de_instruccion_tokenizada[0], "IO_GEN_SLEEP") == 0){
      //Debería loggear qué instrucción se usa.
      uint32_t unidades_de_trabajo = (uint32_t)atoi(linea_de_instruccion_tokenizada[2])
      io_gen_sleep(linea_de_instruccion_tokenizada[1], unidades_de_trabajo);
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
int mmu(t_TLB* tlb, t_tabla_paginas* tabla, uint32_t pid, uint32_t direc_logica, uint32_t tamano_pagina){
    uint32_t numero_pagina = floor(direc_logica / tamano_pagina);
    uint32_t desplazamiento = direc_logica % tamano_pagina;
    int marco = buscar_en_TLB(tlb, pid, numero_pagina);

    if (marco == -1) { //TLB Miss
        if (numero_pagina < tabla->tamano && tabla->marcos[numero_pagina] != -1) {
            marco = tabla->marcos[numero_pagina];
            agregar_entrada_TLB(tlb, pid, numero_pagina, marco);
        } else {
            return -1;
        }
    }

    return (marco * tamano_pagina) + desplazamiento;
}

//Funciones de instrucciones.
void set(char* registro, int valor){
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

void jump_no_zero(char* registro, int nro_instruccion){
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

void io_gen_sleep(char* nombre_interfaz, uint32_t unidades_de_trabajo){
    ind_contexto_kernel = 0;
    t_paquete* paquete = crear_paquete(IO_GEN_SLEEP, sizeof(registros_CPU) + sizeof(motivo_desalojo));
    buffer_add_string(paquete->buffer, string_length(nombre_interfaz)+1, nombre_interfaz);
    buffer_add_uint32(paquete->buffer, unidades_de_trabajo);

    enviar_paquete(paquete, sockets.socket_server_D);
}

//FUNCIONES AUXILIARES.
void sumar_contenido_registro(char* registro, int valor){
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

void restar_contenido_registro(char* registro, int valor){
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

void instruccion_usa_mmu(){
    if(strcmp(linea_de_instruccion_tokenizada[0], "MOV_IN") == 0
    || strcmp(linea_de_instruccion_tokenizada[0], "MOV_OUT") == 0
    || strcmp(linea_de_instruccion_tokenizada[0], "RESIZE") == 0
    || strcmp(linea_de_instruccion_tokenizada[0], "COPY_STRING") == 0
    || strcmp(linea_de_instruccion_tokenizada[0], "IO_STDIN_READ") == 0
    || strcmp(linea_de_instruccion_tokenizada[0], "IO_STDOUT_WRITE") == 0){
        return true;
    }

    return false;
}