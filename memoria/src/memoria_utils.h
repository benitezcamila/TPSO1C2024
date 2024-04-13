#ifndef MEMORIA_UTILS_H
#define MEMORIA_UTILS_H

int inicializar_memoria();
void escuchar_instrucciones();
void escucha_kernel();
void escucha_cpu();
void escucha_E_S();
int atender_cliente(int* fd_conexion_ptr);
int enviar_log(int fd_conexion_ptr, int cod_op);
#endif