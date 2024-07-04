#ifndef E_S_UTILS_H
#define E_S_UTILS_H

#include <utils/conexion.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/temporal.h>
#include <unistd.h>
#include <configuracion/config.h>
#include <pthread.h>
#include "utils/conexion.h"
#include "utils/serializacion.h"

typedef struct {
    int socket_memoria;
    int socket_kernel;
}str_sockets;

extern str_sockets sockets;

//void proceso_E_S();
void establecer_conexion_memoria();
void establecer_conexion_kernel();
//void iniciar_interfaz();

void procesar_io_gen_sleep (t_buffer* );

void enviar_info_io_a_kernel();

void procesar_io_stdin_read(t_buffer* , uint32_t );
char* leer_consola();
void escribir_en_memoria(char* ,uint32_t ,uint32_t ) ;
void procesar_io_stdout_write(t_buffer* , uint32_t );
void recibir_instrucciones ();
char* string_de_instruccion (t_instruccion);
int contar_bloques_libres(t_bitarray*);
int primer_bloque_libre(t_bitarray*);
void procesar_io_fs_create(t_buffer*, uint32_t);
void limpiar_bits(t_bitarray* , off_t , off_t);
bool hay_bits_ocupados(t_bitarray* , int , int );
#endif