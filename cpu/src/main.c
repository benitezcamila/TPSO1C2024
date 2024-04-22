#include <stdlib.h>
#include <stdio.h>
#include <main.h>

int main(int argc, char* argv[]) {
    pthread_t levantar_server, conexion, escucha;
    iniciar_logger();
    obtener_config();
    pthread_create(&levantar_server,NULL,(void*)iniciar_server_kernel,NULL);
    pthread_create(&conexion,NULL,(void*)establecer_conexion_memoria,NULL);
    pthread_create(&escucha,NULL,(void*)escuchar_conexiones,NULL);

    pthread_join(levantar_server,NULL);
    pthread_join(conexion,NULL);
    pthread_join(escucha,NULL);
    return 0;
}
