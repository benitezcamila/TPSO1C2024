#include <tlb.h>
#include <cpu_utils.h>

t_TLB* tlb;

//Función para buscar en la TLB.
int buscar_en_TLB(t_TLB* tlb, uint32_t pid, uint32_t pagina){
    for (int i = 0; i < tlb->count; ++i) {
        if (tlb->entradas[i].pid == pid && tlb->entradas[i].pagina == pagina) {
            if (strcmp(tlb->algoritmo, "LRU") == 0) {
                tlb->entradas[i].uso = tlb->tiempo_actual++;
            }
    
        log_info(logger_cpu, "PID: %d - TLB HIT - Pagina: %d", PID, pagina);
        return tlb->entradas[i].marco; // TLB Hit
        }
    }
    
    return -1; // TLB Miss
}

//Función para agregar entrada a la TLB.
void actualizar_TLB(t_TLB* tlb, uint32_t pid, uint32_t pagina, uint32_t marco) {
    t_entrada_TLB nueva_entrada = {pid, pagina, marco, 0, tlb->tiempo_actual++};

    if (tlb->count < tlb->capacidad) {
        tlb->entradas[tlb->count++] = nueva_entrada;
    } else {
        if (strcmp(tlb->algoritmo, "FIFO") == 0) {
            reemplazo_FIFO(tlb, nueva_entrada);
        } else if (strcmp(tlb->algoritmo, "LRU") == 0) {
            reemplazo_LRU(tlb, nueva_entrada);
        }
    }
}

//Algoritmos de reemplazo.
void reemplazo_FIFO(t_TLB* tlb, t_entrada_TLB nueva_entrada) {
    int idx = 0;
    int tiempo_min = 1; //REVISAR

    for (int i = 0; i < tlb->count; ++i) {
        if (tlb->entradas[i].tiempo < tiempo_min) {
            tiempo_min = tlb->entradas[i].tiempo;
            idx = i;
        }
    }

    tlb->entradas[idx] = nueva_entrada;
}

void reemplazo_LRU(t_TLB* tlb, t_entrada_TLB nueva_entrada) {
    int idx = 0;
    int uso_min = 1; //REVISAR

    for (int i = 0; i < tlb->count; ++i) {
        if (tlb->entradas[i].uso < uso_min) {
            uso_min = tlb->entradas[i].uso;
            idx = i;
        }
    }

    tlb->entradas[idx] = nueva_entrada;
}