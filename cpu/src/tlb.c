#include <tlb.h>
#include <cpu_utils.h>

//Función que inicializa la TLB.
void inicializar_TLB(t_TLB* tlb, uint32_t capacidad, char* algoritmo){
    tlb->entradas = malloc(sizeof(t_entrada_TLB) * capacidad);
    tlb->capacidad = capacidad;
    tlb->count = 0;
    tlb->tiempo_actual = temporal_create(void);
    tlb->algoritmo = string_duplicate(algoritmo);
}

void destruir_TLB(t_TLB* tlb){
	free(tlb->entradas);
    temporal_destroy(tlb->tiempo_actual);
	free(tlb->algoritmo);
	free(tlb);
}

//Función para buscar en la TLB.
uint32_t buscar_en_TLB(t_TLB* tlb, uint32_t PID, uint32_t pagina){
    
    for(int i = 0; i < tlb->count; i++){
        if(tlb->entradas[i].pid == PID && tlb->entradas[i].pagina == pagina){
            if(strcmp(tlb->algoritmo, "LRU") == 0){
                tlb->entradas[i].uso = temporal_gettime(tlb->tiempo_actual);
            }        
                
            log_info(logger_cpu, "PID: %d - TLB HIT - Pagina: %d", PID, pagina);

            return tlb->entradas[i].marco; // TLB Hit
        }
    }
    
    return -1; // TLB Miss
}

//Función para agregar entrada a la TLB.
void actualizar_TLB(t_TLB* tlb, uint32_t pid, uint32_t pagina, uint32_t marco){
    t_entrada_TLB nueva_entrada = {pid, pagina, marco, gettime(tlb->tiempo_actual)};
    
    if(tlb->count < tlb->capacidad){
        tlb->entradas[tlb->count++] = nueva_entrada;
    } else{
        reemplazo_en_TLB(tlb, nueva_entrada);
    }
}

//Algoritmo de reemplazo.
void reemplazo_en_TLB(t_TLB* tlb, t_entrada_TLB nueva_entrada){
    uint32_t idx = 0;
    uint64_t tiempo_min = UINT64_MAX;

    for(uint32_t i = 0; i < tlb->count; i++){
        if(tlb->entradas[i].uso < tiempo_min){
            tiempo_min = tlb->entradas[i].uso;
            idx = i;
        }
    }

    tlb->entradas[idx] = nueva_entrada;
}
