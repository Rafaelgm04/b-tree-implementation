#ifndef ARVORE_B_H
#define ARVORE_B_H


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

typedef enum{
    FALSE = 0,TRUE = 1
}Bool;


typedef struct SO_NO{
    //unsigned  M;
    unsigned **p;
    //s[0] guarda M
    unsigned *s;
    //tamanho alocado do vetor 
    unsigned tam_vector;



}SO_NO;


typedef struct Arvore_B{

    SO_NO *ptraiz;
    //sempre d +1
    unsigned D;


    SO_NO *(*Busca)(Arvore_B *,unsigned/*, SO_NO, unsigned, unsigned*/);

    Bool (*Insercao)(Arvore_B *,SO_NO);

    SO_NO (*Remocao)(Arvore_B *,unsigned);

    Bool (*Desaloca)(Arvore_B *,unsigned);

    void (*Implecao)(Arvore_B *);

    SO_NO *(*get)(Arvore_B *,unsigned);

    void (*set)(Arvore_B *,SO_NO *);

}Arvore_B;


//deve inicializar a "classe e inicializar os ponteitos para funcao com cada funcao sua e inicilizar o valor D"
Arvore_B init(unsigned D);


SO_NO *ArvoreB_get(Arvore_B *self, unsigned chave);

void ArvoreB_set(Arvore_B *self,SO_NO *no);

SO_NO *ArvoreB_Busca(Arvore_B *self,unsigned chave/*, SO_NO pt, unsigned f, unsigned g*/);

Bool ArvoreB_insercao(Arvore_B *self,SO_NO NO);

SO_NO ArvoreB_remocao(Arvore_B *self,unsigned chave);

Bool ArvoreB_desaloca(Arvore_B *self,unsigned chave);

void ArvoreB_implecao(Arvore_B *self);

#endif