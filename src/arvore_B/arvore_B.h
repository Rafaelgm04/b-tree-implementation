#ifndef ARVORE_B_H
#define ARVORE_B_H


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


typedef enum{
    FALSE_,
    TRUE_ 
}Bool;

typedef struct SO_NO SO_NO;
typedef struct Arvore_B Arvore_B;


typedef struct SO_NO{
    //unsigned  M;
    SO_NO **p;
    //s[0] guarda M
    int *s;
    //tamanho alocado do vetor 
    unsigned tam_vector;

    

}SO_NO;


typedef struct Arvore_B{

    SO_NO *ptraiz;

    // ordem d da árvore B
    unsigned D;



    int *(*Busca)(Arvore_B *, int);

    Bool (*Insercao)(Arvore_B *, int);

    Bool (*Remocao)(Arvore_B *,int);

    Bool (*Desaloca)(Arvore_B *);

    void (*Implecao)(Arvore_B *);


}Arvore_B;


//deve inicializar a "classe e inicializar os ponteitos para funcao com cada funcao sua e inicilizar o valor D"
Arvore_B init(unsigned D);


int *ArvoreB_busca(Arvore_B *self, int x);

Bool ArvoreB_insercao(Arvore_B *self, int x);

Bool ArvoreB_remocao(Arvore_B *self,int chave);

Bool ArvoreB_desaloca(Arvore_B *self);

void ArvoreB_implecao(Arvore_B *self);

#endif