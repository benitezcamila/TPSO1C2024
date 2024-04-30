#include <stdlib.h>
#include <stdio.h>
#include <main.h>

int main(int argc, char* argv[]) {
     
    pthread_t levantar_server, e_s, cpu, kernel;

    iniciar_logger();
    obtener_config();

    pthread_create(&levantar_server,NULL,(void*)inicializar_memoria,NULL);
    pthread_create(&kernel,NULL,(void*)escucha_kernel,NULL);
    pthread_create(&cpu,NULL,(void*)escucha_cpu,NULL);
    pthread_create(&e_s,NULL,(void*)escucha_E_S,NULL);

    pthread_join(levantar_server,NULL);
    pthread_join(e_s,NULL);
    pthread_join(cpu,NULL);
    pthread_join(kernel,NULL);

    return 0;
}
