#include <stdlib.h>
#include <stdio.h>
#include <main.h>

int main(int argc, char* argv[]) {

    pthread_t levantar_kernel, escuchar_conexiones, establecer_conexion;

    iniciar_logger();
    obtener_config();
    pthread_create(&levantar_kernel,NULL,(void*)inicializar_kernel,NULL);
    pthread_create(&establecer_conexion,NULL,(void*)establecer_conexiones,NULL);
    pthread_create(&escuchar_conexiones,NULL,(void*)escucha_E_S,NULL);

    int curioso1 = sockets.socket_server;
    int curioso2 = sockets.socket_CPU_D;
    int curioso3 = sockets.socket_CPU_I;
    int curioso4 = sockets.socket_memoria;

    pthread_join(levantar_kernel,NULL);
    pthread_join(establecer_conexion,NULL);
    pthread_join(escuchar_conexiones,NULL);

    

    return 0;
}
