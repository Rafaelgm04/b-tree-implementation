#include <stdio.h>
#include <stdlib.h>
#include "arvore_B.h"




int main(){

    unsigned D =2;

    Arvore_B arvore = init(D);
    

    arvore.Busca(&arvore,10);

    return 0;
}