#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <stdint.h>

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
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;



#endif /* UTILS_H_ */
