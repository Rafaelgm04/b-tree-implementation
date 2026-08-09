#include "arvore_B.h"




Arvore_B init(unsigned D){

    Arvore_B p;

    p.D = D;


    p.Busca    = ArvoreB_busca;
    p.Insercao = ArvoreB_insercao;
    p.Remocao  = ArvoreB_remocao;
    p.Desaloca = ArvoreB_desaloca;
    p.Implecao = ArvoreB_implecao;

    p.get      = ArvoreB_get;
    p.set      = ArvoreB_set;
    
    return p;

}


//talvez falazer uma busca binaria depois nas chaves
// *(p->s) == M
static ___busca__(Arvore_B *self,unsigned x, SO_NO *pt, unsigned *f, unsigned *g/*, SO_NO *pai*/){
    SO_NO *p = self->ptraiz;
    pt       = NULL;
    *f        = 0;

    while(p != NULL){
        unsigned i = *g = 1;
        pt = p;
        while(i <= *(p->s)){
            if (x > p->s[i])
                i = *g = i+1;
            else if(x = p->s[i]){
                p  = NULL;
                *f = 1;
                
            }else{
                p = p->p[i-1];
                i = *(p->s) + 2;
            }
        }
        if (i == *(p->s) + 1)
            //pai = p;
            p   = p->p[*(p->s)];
    }

}

//*(p->s) == M
static cisao(Arvore_B *self,SO_NO *pt,SO_NO *pai){

    unsigned meio;

    meio = (self->D + 2)/2;


    SO_NO *p2 = malloc(sizeof(SO_NO));
    //o p0 vai assumir o ponteiro da chave que vai subir
    p2->p    = pt->p + meio;
    //meio mais dois porque a poscao zero tem que ser do tamanho se nao fosse isso seira meio + 1
    p2->s    = pt->s + meio +2;
    *(p2->s) = meio-1;


    unsigned i = 1;
    while(pai->s[i] > pt->s[meio])
        i++;
    
    //a poscao de s[0] nao e usada para guardar chave
    memmove(&pai->s[i+2],&pai->s[i+1],(*(pai->s) - i ) * sizeof(unsigned)  );

    //ver se realmente nao precisa de (i+1)
    memmove(&pai->p[i+2],&pai->p[i+1],(*(pai->s) - i ) * sizeof(unsigned)  );
    
    //alocar o no

    //atuliza coloca a chave que subio no pai com seu ponteiro a pontando para a sengunda pagina gerada
    pai->s[i+1] = pt->s[i+1];
    pai->p[i+1] = p2;

    //atuliza o m da primeira pagina gerada *(pt->s) == M
    *(pt->s) = meio -1;
}

SO_NO *ArvoreB_busca(Arvore_B   *self,unsigned x){
    unsigned f = 0;
    SO_NO *pt  = NULL;
    unsigned g = 0;
    ___busca__(self, x, pt, &f, &g);

    if (f != 1)
        return NULL;
    return pt->s[g];

}

Bool ArvoreB_insercao(Arvore_B *self,SO_NO NO){

    ___busca__();




}

SO_NO ArvoreB_remocao(Arvore_B *self,unsigned chave){}

void ArvoreB_implecao(Arvore_B *self){}

Bool ArvoreB_desaloca(Arvore_B *self,unsigned chave){}

SO_NO *ArvoreB_get(Arvore_B *self, unsigned chave){}

void ArvoreB_set(Arvore_B *self,SO_NO *no){}