#include "estruturas.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define TEMPO_DE_ESPERA 30 //TEMPO DE ESPERA ATÉ ENCONTRAR O ÁRBITRO "VIVO"
