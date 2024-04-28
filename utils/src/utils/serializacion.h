#ifndef SERIALIZACION_H_
#define SERIALIZACION_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "estructuras.h"

/*
typedef enum
{
	MENSAJE,
	PAQUETE,
	COD_OP_CREACION_TABLA,
	COD_OP_DESTRUCCION_TABLA,
	COD_OP_ACCESO_TABLA,
	COD_OP_AMPLIACION_PROCESO,
	COD_OP_REDUCION_PROCESO,
	COD_OP_ACCESO_ESPACIO_USUARIO,
	COD_OP_FETCH,
	COD_OP_INSTRUCCION_EJECUTADA,
	COD_OP_,

}op_code;
*/

typedef enum {
    PCB
}op_code;

typedef struct {
	
    uint32_t size; // Tamaño del payload
    uint32_t offset; // Desplazamiento dentro del payload
    void* stream; // Payload
} t_buffer;

typedef struct
{
	uint32_t codigo_operacion;
	t_buffer* buffer;
} t_paquete;


// Crea un buffer vacío de tamaño size y offset 0
t_buffer *buffer_create(uint32_t size);

// Libera la memoria asociada al buffer
void buffer_destroy(t_buffer *buffer);

// Agrega un stream al buffer en la posición actual y avanza el offset
void buffer_add(t_buffer *buffer, void *data, uint32_t size);

// Guarda size bytes del principio del buffer en la dirección data y avanza el offset
void buffer_read(t_buffer *buffer, void *data, uint32_t size);

// Agrega un uint32_t al buffer
void buffer_add_uint32(t_buffer *buffer, uint32_t data);

// Lee un uint32_t del buffer y avanza el offset
uint32_t buffer_read_uint32(t_buffer *buffer);

// Agrega un uint8_t al buffer
void buffer_add_uint8(t_buffer *buffer, uint8_t data);

// Lee un uint8_t del buffer y avanza el offset
uint8_t buffer_read_uint8(t_buffer *buffer);

// Agrega string al buffer con un uint32_t adelante indicando su longitud
void buffer_add_string(t_buffer *buffer, uint32_t length, char *string);

// Lee un string y su longitud del buffer y avanza el offset
char *buffer_read_string(t_buffer *buffer, uint32_t *lengwhath);

extern desempaquetar_pcb(t_buffer*,t_pcb*);

#endif /* UTILS_H_ */
