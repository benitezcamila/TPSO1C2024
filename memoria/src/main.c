#include <stdlib.h>
#include <stdio.h>
#include <main.h>

int main(int argc, char* argv[]) {
     
    pthread_t levantar_server,escuchar;

    iniciar_logger();
    obtener_config();

    pthread_create(&levantar_server,NULL,(void*)inicializar_memoria,NULL);
    pthread_create(&escuchar,NULL,(void*)server_escuchar,(void*)sockets.socket_server);
  
    pthread_join(levantar_server,NULL);
    pthread_join(escuchar,NULL);


    return 0;
}
