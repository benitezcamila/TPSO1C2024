#include "gestion_memoria.h"
#include <math.h>

t_dictionary* tabla_global;

void crearTablaPagina(int pid){
    tabla_pagina* tabPagina = malloc(sizeof(tabla_pagina));
    tabPagina->pid = pid;
    tabPagina->paginas = list_create();    
    dictionary_put(tabla_global,string_itoa(pid), tab->tabla_pagina);
}

void ajustar_tam_proceso(t_buffer* buffer_cpu){
    uint32_t pid = buffer_read_uint32(buffer_cpu);
    uint32_t tam_bytes = buffer_read_uint32(buffer_cpu);
    int cant_frames_requeridos = floor(tam_bytes / configuracion.TAM_PAGINA) + 1;


    tabla_pagina* paginas_del_proceso = dictionary_get(tabla_global, string_itoa(pid));

    if(list_size(paginas_del_proceso->paginas) == 0 ) {
        asignar_id_pagina(paginas_del_proceso);
        cantFrames--;
    }

    if(list_size (paginas_del_proceso->paginas) > cantFrames) reduccion_del_proceso(paginas_del_proceso, cant_frames_requeridos); 



    if(list_size (paginas_del_proceso->paginas) < cantFrames) ampliacion_del_proceso(paginas_del_proceso, cant_frames_requeridos); 

    list_add(tabla_global , paginas_del_proceso );
    free(paginas_del_proceso);

}

void asignar_id_pagina(tabla_pagina* tabla_proceso ){
    t_pagina* primer_pagina = malloc(sizeof(t_pagina)); 
    primer_pagina->pid_pagina = 0;
    primer_pagina->numero_marco = marcoDisponible();
    list_add(tabla_proceso->paginas, primer_pagina);
    free(primer_pagina);

}

void reduccion_del_proceso(tabla_pagina* tabla_proceso, int marcos_final){
    
    for(int i = list_size(tabla_proceso->paginas); i > marcos_final;i--){
    
        t_pagina* salida = list_remove(tabla_proceso->paginas, i-1);
        bitMap[salida->numero_marco] = 0;
    }
}


void ampliacion_del_proceso(tabla_pagina* tabla_proceso, int marcos_final){ 
    for(int i = list_size(tabla_proceso->paginas); i<marcos_final; i++){
        t_pagina* entrada = malloc(sizeof(t_pagina));
        entrada->numero_marco = marcoDisponible();
        entrada->offSet = 0;
        entrada->escrita = false;

        list_add(tabla_proceso->paginas, entrada);
    }
}


int marcoDisponible(){
    for(int i=0; i< cantFrames ; i++){
        if(bitMap[i] == 0) return i;
    }
    exit(EXIT_FAILURE) ;// out of memory 
    }

void access_espacio_usuario(t_buffer* buffer) {
    uint32_t pid = buffer_read_uint32(buffer);
    uint32_t accion = buffer_read_uint8(buffer); // 1 escribir, 0 leer -> a lo clock 2.0
    uint32_t direc_fisica = buffer_read_uint32(buffer);
    uint32_t tamananio = buffer_read_uint32 (buffer); // tamanio a leer/escribir

    if(accion == 0 ) leer_espacio_usuario(pid, direc_fisica, tamananio);
    if(accion == 1) escribir_espacio_usuario(pid, direc_fisica, tamananio);

}   

void* leer_espacio_usuario(uint32_t pid,uint32_t direc_fisica, uint32_t tamanio){
    void* valor = malloc(tamanio);
    int nro_marco = floor((direc_fisica+espacioUsuario)/ configuracion.TAM_PAGINA);
    int offSet = (direc_fisica+espacioUsuario) % configuracion.TAM_PAGINA;
    
    tabla_pagina* tabla = dictionary_get(tabla_global, string_itoa(pid));
    bool existe_ese_marco(t_pagina* pagina){
        return pagina->nro_marco == nro_marco;
    }
    t_pagina* pag_a_leer = list_find(tabla, (void*) existe_ese_marco);
    bool no_termino_de_leer = true;
    int tam_a_leer = min(tamanio, (pag_a_leer->tamanio-offSet));
    leer_memoria (direc_fisica, tam_a_leer);
    while(no_termino_de_leer){
    int i =0;
    if(configuracion.TAM_MEMORIA - offSet >tamanio ){
       
        offSet = 0;
        i++;
        pag_a_leer = list_get(tabla,pag_a_leer+i);
        direc_fisica = pag_a_leer->numero_marco * configuracion.TAM_PAGINA;
        tam_a_leer = min(tamanio, (pag_a_leer->tamanio));
        valor =+ leer_memoria (direc_fisica, tam_a_leer);
        
    }else no_termino_de_leer = true;
    }
    return valor;
}
void* leer_memoria(uint32_t dir_fisica, uint32_t tamanio){
	void* valor_leido = malloc(tamanio);
    memcpy(valor_leido, espacio_usuario + dir_fisica, tamanio);
    return valor_leido;

}

