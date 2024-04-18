#include <stdlib.h>
#include <stdio.h>
#include <main.h>

int main(int argc, char* argv[]) {
     
    iniciar_logger();
    obtener_config();
    inicializar_memoria();
    escuchar_instrucciones();
    return 0;
}
