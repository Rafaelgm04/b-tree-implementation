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
//realocar para um novo tamanho deve ser no minimo m+1 ou d+1 eu acho tem que ver se e isso mesmo
static Bool func_realoc(SO_NO *pt){


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
static unsigned novaCapacidade(unsigned atual, unsigned D)
{
    unsigned maximo = D + 1;

    if (atual >= maximo)
        return maximo;

    if (atual == 0)
        return 1;

    double ocupacao = (double)atual / maximo;

    /*
        ocupacao = 0%   -> crescimento ≈ 100%
        ocupacao = 100% -> crescimento = 15%
    */
    double porcentagem = 1.0 - (0.85 * ocupacao);

    unsigned nova =
        atual + (unsigned)ceil(atual * porcentagem);

    if (nova > maximo)
        nova = maximo;

    return nova;
}

//*(p->s) == M

static Bool cisao(Arvore_B *self,SO_NO *pt,SO_NO *pai){

    unsigned M = pt->s[0];
    unsigned meio = (self->D + 2) / 2;
    unsigned qtdDireita = M - meio;
    unsigned chaveSobe = pt->s[meio];


    SO_NO *p2 = malloc(sizeof(*p2));

    if (p2 == NULL)
        return FALSE;
    /*
    //o p0 vai assumir o ponteiro da chave que vai subir
    p2->p    = pt->p + meio;
    //meio mais dois porque a poscao zero tem que ser do tamanho se nao fosse isso seira meio + 1
    p2->s    = pt->s + meio +2;
    *(p2->s) = meio-1;
        B.O na hora do  free melhor ter memoria nova
    */
    
    //caso eu nao queira alocar de inteiro mudar para acho que meio + uma quantidade 
    p2->tam_vector = novaCapacidade(qtdDireita, self->D);
    p2->s  = malloc((p2->tam_vector + 1) * sizeof(p2->s[0]));
    p2->p  = malloc((p2->tam_vector + 1) * sizeof(SO_NO *));

    if (p2->s == NULL || p2->p == NULL) {
        free(p2->s);
        free(p2->p);
        free(p2);
        return FALSE;
    }
    *p2->s = qtdDireita;

    //copia os dados para o p2
    memcpy(&p2->s[1], &pt->s[meio + 1], qtdDireita * sizeof p2->s[0] );
    memcpy(&p2->p[0], &pt->p[meio], (qtdDireita + 1) * sizeof p2->p[0]);

    //talves dar um resaize no pt

    unsigned i = 1;

    //procura o lugar que vai colocar a chave que vai subir no pai 
    while(i <= *(pai->s) && pai->s[i] < chaveSobe)
        i++;
    
    //se o vetor de pai estiver com m = tamanho alocado dar um realoc nele
    if (pai->s[0] >= pai->tam_vector) {

        //se der errado desaloca p2 para nao dar vazamento de memoria 
        if (!func_realoc(pai)) {

            free(p2->s);
            free(p2->p);
            free(p2);

            return FALSE;
            }
    }

    //a poscao de s[0] nao e usada para guardar chave
    memmove(&pai->s[i+1],&pai->s[i],(*(pai->s) - i + 1) * sizeof(pai->s[0]/*tamanho do tipo da chave */)  );


    memmove(&pai->p[i+1],&pai->p[i],(*(pai->s) - i + 1) * sizeof(pai->p[0]/*tamanho do tipo do ponteiro usado em SO_NO */)  );
    


    //atuliza coloca a chave que subio no pai com seu ponteiro a pontando para a sengunda pagina gerada
    pai->s[i] = pt->s[meio];
    pai->p[i] = p2;
    //aumenta numero de chave do pai
    (*(pai->s))++;


    //atuliza o m da primeira pagina gerada *(pt->s) == M
    *(pt->s) = meio -1;

    return TRUE;
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

    //___busca__();




}

SO_NO ArvoreB_remocao(Arvore_B *self,unsigned chave){}

void ArvoreB_implecao(Arvore_B *self){}

Bool ArvoreB_desaloca(Arvore_B *self,unsigned chave){}

SO_NO *ArvoreB_get(Arvore_B *self, unsigned chave){}

void ArvoreB_set(Arvore_B *self,SO_NO *no){}