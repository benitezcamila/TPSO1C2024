#include <stdlib.h>
#include <stdio.h>
#include <main.h>

int main(int argc, char* argv[]) {
     
    iniciar_logger();
    obtener_config();
    escucha_kernel();
    return 0;
}
