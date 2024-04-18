#include <stdlib.h>
#include <stdio.h>
#include <main.h>

int main(int argc, char* argv[]) {
    iniciar_logger();
    obtener_config();
    inicializar_cpu_dispatch();
    inicializar_cpu_interrupt();
    procesos_cpu();
    return 0;
}
