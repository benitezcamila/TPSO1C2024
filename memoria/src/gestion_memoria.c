#include <gestion_memoria.h>
#include <math.h>

void crearTablaPagina(int pid){
    tabla_pagina* tabPagina = malloc(sizeof(tabla_pagina));
    tabPagina->pid = pid;
    tabPagina->paginas = list_create();    
}

void ajustar_tam_proceso(t_buffer* buffer_cpu){
    uint32_t pid = buffer_read_uint32(buffer_cpu);
    uint32_t tam_bytes = buffer_read_uint32(buffer_cpu);

    int cant_frames_requeridos = floor(tam_bytes / configuracion.TAM_PAGINA) + 1; 
    
    // busco proceso en tabla de paginas
    bool pid_iguales(tabla_pagina* tabla){
        return tabla->pid == pid;
    }
    
    
    tabla_pagina* paginas_del_proceso= list_find(tabla_global,(void*) pid_iguales);

    if(paginas_del_proceso->paginas == NULL) asignar_id_pagina(paginas_del_proceso);

    if(list_size(paginas_del_proceso->paginas) > cantFrames) reduccion_del_proceso(paginas_del_proceso, cant_frames_requeridos);

    if(list_size (paginas_del_proceso->paginas) <= cantFrames) ampliacion_del_proceso(paginas_del_proceso, cant_frames_requeridos); 

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
    }
}


int marcoDisponible(){
    for(int i=0; i< cantFrames ; i++){
        if(bitMap[i] == 0) return i;
    }
    exit(EXIT_FAILURE) ;// out of memory 
    }

    


