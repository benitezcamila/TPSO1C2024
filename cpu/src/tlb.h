#ifndef TLB_H
#define TLB_H

#include <utils/estructuras.h>
#include <commons/string.h>

// Estructura de entrada de la TLB
typedef struct {
    uint32_t pid; // Identificador del proceso
    uint32_t pagina; // Número de página
    uint32_t marco; // Número de marco
    uint32_t uso; // Contador de uso para LRU
    uint32_t tiempo; // Marca de tiempo para FIFO
} Entrada_TLB;

// Estructura de la TLB
typedef struct {
    Entrada_TLB* entradas;
    int capacidad;
    int count;
    int tiempo_actual; // Para llevar el tiempo en FIFO
    char algoritmo[4]; // Algoritmo de reemplazo ("FIFO" o "LRU")
} TLB;

// Estructura de la tabla de páginas
typedef struct {
    uint32_t* marcos;
    uint32_t tamano; // Tamaño de la tabla de páginas (cantidad de páginas)
} TablaPaginas;

int buscar_en_TLB(TLB* tlb, uint32_t pid, uint32_t pagina);
void agregar_entrada_TLB(TLB* tlb, uint32_t pid, uint32_t pagina, uint32_t marco);
void reemplazo_FIFO(TLB* tlb, EntradaTLB nueva_entrada);
void reemplazo_LRU(TLB* tlb, EntradaTLB nueva_entrada);

#endif