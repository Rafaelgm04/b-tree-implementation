#include <stdlib.h>
#include <stdio.h>





typedef struct SO_NO{
    unsigned *P;
    unsigned *S;



}SO_NO;


typedef struct{

    SO_NO *ptr;
    unsigned D;

    //nao sei se vai ter essa, deve inicializar a "classe e inicializar os ponteitos para funcao com cada funcao sua"
    void (*init)(Arvore_B *,unsigned);

    void (*Busca)(Arvore_B *,unsigned);

    void (*insercao)(Arvore_B *,unsigned);

    void (*remocao)(Arvore_B *,unsigned);

    void (*desaloca)(Arvore_B *,unsigned);

    void (*implecao)(Arvore_B *);

}Arvore_B;


//deve inicializar a "classe e inicializar os ponteitos para funcao com cada funcao sua e inicilizar o valor D"
Arvore_B init(unsigned D);