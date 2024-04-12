
#ifndef UTILS_H_
#define UTILS_H_
#include<pthread.h>
void* recibir_buffer(int* size, int socket_cliente);
void recibir_mensaje(int socket_cliente);


#endif /* UTILS_H_ */
