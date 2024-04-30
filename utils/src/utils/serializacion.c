#include "serializacion.h"

// Crea un buffer vacío de tamaño size y offset 0.
t_buffer *buffer_create(uint32_t size){
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = size;
	buffer->offset = 0;
	buffer->stream = malloc(size);

	return buffer;
}

// Libera la memoria asociada al buffer.
void buffer_destroy(t_buffer *buffer){
	free(buffer->stream);
	free(buffer);
}

// Agrega un stream al buffer en la posición actual y avanza el offset.
void buffer_add(t_buffer *buffer, void *data, uint32_t size){
	memcpy(buffer->stream + buffer->offset, data, size);
	buffer->offset += size;
}

// Guarda size bytes del principio del buffer en la dirección data y avanza el offset.
void buffer_read(t_buffer *buffer, void *data, uint32_t size){
	memcpy(data, buffer->stream, size); //?
	buffer->offset += size;
}

// Agrega un uint32_t al buffer
void buffer_add_uint32(t_buffer *buffer, uint32_t data){
	buffer_add(buffer, &data, sizeof(uint32_t));
}

// Lee un uint32_t del buffer y avanza el offset
uint32_t buffer_read_uint32(t_buffer *buffer){
	uint32_t data = -1;

	buffer_read(buffer, &data, sizeof(uint32_t));

	return data;
}

// Agrega un uint8_t al buffer
void buffer_add_uint8(t_buffer *buffer, uint8_t data){
	buffer_add(buffer, &data, sizeof(uint8_t));
}

// Lee un uint8_t del buffer y avanza el offset
uint8_t buffer_read_uint8(t_buffer *buffer){
	uint8_t data = 0;

	buffer_read(buffer, &data, sizeof(uint8_t));

	return data;
}

// Agrega string al buffer con un uint32_t adelante indicando su longitud
void buffer_add_string(t_buffer *buffer, uint32_t length, char *string){
	buffer_add_uint32(buffer,length);
	buffer_add(buffer, string, length);
}

// Lee un string y su longitud del buffer y avanza el offset
char *buffer_read_string(t_buffer *buffer, uint32_t *length){
	char *data = NULL;
	*length = buffer_read_uint32(buffer);
	buffer_read(buffer, data, *length);

	return data;
}

void* a_enviar_create(t_paquete* paquete){
	void* a_enviar = malloc(paquete->buffer->size + sizeof(uint32_t) + sizeof(op_code));
	int offset = 0;
	memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(op_code));
	offset += sizeof(op_code);
	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
	offset += sizeof(uint32_t);
	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);
	return a_enviar;

}
