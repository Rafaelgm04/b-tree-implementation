#include "arvore_B.h"




Arvore_B init(unsigned D){

    Arvore_B p;

    p.D      = D;

    p.ptraiz = NULL;

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
//para chaves e d+1
//para os ponteiro e d+2 
//verificar
//ver se e isso mesmo depois!!
inline static unsigned novaCapacidade(unsigned atual, unsigned D){

    //D + 1 porque qunado fica em estado "inrregular" ele pode ficar com +1 para facilitar a proxima operacao
    unsigned maximo = 2 * D + 1;

    if (atual >= maximo)
        return maximo;

    //ver se e isso mesmo
    if (atual == 0)
        return 2;

    double ocupacao = (double)atual / maximo;

    double porcentagem = 1.0 - (0.85 * ocupacao);

    unsigned nova = atual + (unsigned)ceil(atual * porcentagem);

    if (nova > maximo)
        nova = maximo;

    return nova;
}

static Bool func_realoc(Arvore_B *self, SO_NO *pt)
{
    unsigned nova = novaCapacidade(pt->tam_vector, self->D);

    if(nova <= pt->tam_vector)
        return FALSE;


    unsigned *novo_s = malloc((nova + 1) * sizeof(novo_s[0]));

    if(novo_s == NULL)
        return FALSE;

    SO_NO **novo_p = NULL;

    if(pt->p != NULL){

        novo_p = malloc((nova + 1) * sizeof(novo_p[0]));

        if(novo_p == NULL){
            free(novo_s);
            return FALSE;
        }
    }

    //s[0] + m chaves
    memcpy(novo_s, pt->s, (pt->s[0] + 1) * sizeof(novo_s[0]));

    //pagina interna possui m+1 ponteiros
    if(pt->p != NULL){

        memcpy(novo_p, pt->p, (pt->s[0] + 1) * sizeof(novo_p[0])
        );
    }

    free(pt->s);
    free(pt->p);

    pt->s = novo_s;
    pt->p = novo_p;

    pt->tam_vector = nova;

    return TRUE;
}

//talvez falazer uma busca binaria depois nas chaves
// *(p->s) == M
static void ___busca___(Arvore_B *self, unsigned x, SO_NO **pt, unsigned *f, unsigned *g)
{
    SO_NO *p = self->ptraiz;

    *pt = NULL;
    *f = 0;

    while(p != NULL){

        unsigned m = *p->s;
        unsigned i = *g = 1;

        *pt = p;

        while(i <= m){

            if(x > p->s[i]){

                i = *g = i + 1;

            }else if(x == p->s[i]){

                p = NULL;      
                *f = 1;
                i = m + 2;

            }else{
                if(p->p != NULL)
                    p = p->p[i - 1];
                else
                    p = NULL;

                i = m + 2;
            }
        }

        if(i == m + 1){
            if(p->p != NULL)
                p = p->p[m];
            else
                p = NULL;
        }
    }
}

static SO_NO *aloca(void){
    SO_NO *temp = malloc(sizeof(*temp));

    if(temp == NULL){
        fprintf(stderr, "Erro: malloc falhou.\n");
        exit(EXIT_FAILURE);
    }

    temp->tam_vector = 2;

    temp->s = calloc(temp->tam_vector + 1, sizeof(temp->s[0]));

    if(temp->s == NULL){
        free(temp);
        fprintf(stderr, "Erro: calloc falhou.\n");
        exit(EXIT_FAILURE);
    }

    temp->s[0] = 0;

    //Pagina inicialmente folha
    temp->p = NULL;

    return temp;
}


static Bool resize_chaves(Arvore_B *self, SO_NO *NO){

}

static void insere_chave(Arvore_B *self, SO_NO *NO, unsigned x){
    unsigned i = 1;
    unsigned m = NO->s[0];

    //Procura a posicao correta de x
    while (i <= m && NO->s[i] < x)
        i++;

    //Se nao permite chaves repetidas 
    if (i <= m && NO->s[i] == x)
        return;
    
    //Garante espaco para mais uma chave 
    if(m >= NO->tam_vector && m <= 2 * self->D){

        if(!resize_chaves(self, NO))
            return;
        }
    //abre espaco para x 
    if (i <= m) {
        memmove(&NO->s[i + 1], &NO->s[i], (m - i + 1) * sizeof(NO->s[0]));
    }

    //Insere a chave
    NO->s[i] = x;

    //Atualiza m
    NO->s[0]++;
}

//*(p->s) == M
//acho que nao trata o caso quando nao tem pai no caso e o pt raiz// pai == NULL
static void cisao(Arvore_B *self,SO_NO *pt,SO_NO *pai){

    unsigned M = pt->s[0];

    //o tamanho qunado for nessesario fazer uma cisao sempre vais er impar 
    unsigned meio       = self->D + 1;
    unsigned qtdDireita = M - meio;
    unsigned chaveSobe  = pt->s[meio];


    SO_NO *p2 = malloc(sizeof(*p2));

    if (p2 == NULL)
        return /*FALSE*/;

    p2->tam_vector = novaCapacidade(qtdDireita, self->D);
    /*
    //o p0 vai assumir o ponteiro da chave que vai subir
    p2->p    = pt->p + meio;
    //meio mais dois porque a poscao zero tem que ser do tamanho se nao fosse isso seira meio + 1
    p2->s    = pt->s + meio +2;
    *(p2->s) = meio-1;
        B.O na hora do  free melhor ter memoria nova
    */
    
    p2->s = malloc((p2->tam_vector + 1) * sizeof(p2->s[0]));

    if(p2->s == NULL){
        free(p2);
        return;
    }

    p2->p = NULL;

    if(pt->p != NULL){

        p2->p = malloc(
            (p2->tam_vector + 1) * sizeof(p2->p[0])
        );

        if(p2->p == NULL){
            free(p2->s);
            free(p2);
            return;
        }

        memcpy(
            &p2->p[0],
            &pt->p[meio],
            (qtdDireita + 1) * sizeof(p2->p[0])
        );
    }
    *p2->s = qtdDireita;

    //copia os dado para o p2
    memcpy(&p2->s[1], &pt->s[meio + 1], qtdDireita * sizeof p2->s[0] );


    //talves dar um resaize no pt

    unsigned i = 1;

    //caso o pai nao exista 
    if(pai == NULL){

        SO_NO *novaRaiz = malloc(sizeof(*novaRaiz));

        if(novaRaiz == NULL){
            /* tratar erro */
            return;
        }

        novaRaiz->tam_vector = 2 * self->D + 1;

        novaRaiz->s = calloc(novaRaiz->tam_vector + 1, sizeof(novaRaiz->s[0]));

        novaRaiz->p = calloc(novaRaiz->tam_vector + 1, sizeof(novaRaiz->p[0]));

        if(novaRaiz->s == NULL || novaRaiz->p == NULL){

            free(novaRaiz->s);
            free(novaRaiz->p);
            free(novaRaiz);

            return;
        }

        novaRaiz->s[0] = 1;
        novaRaiz->s[1] = chaveSobe;

        novaRaiz->p[0] = pt;
        novaRaiz->p[1] = p2;

        pt->s[0] = meio - 1;

        self->ptraiz = novaRaiz;

        return;
    }


    //procura o lugar que vai colocar a chave que vai subir no pai 
    while(i <= *(pai->s) && pai->s[i] < chaveSobe)
        i++;
    
    //se o vetor de pai estiver com m = tamanho alocado dar um realoc nele
    if (pai->s[0] >= pai->tam_vector) {

        //se der errado desaloca p2 para nao dar vazamento de memoria 
        if (!func_realoc(self, pai)) {

            free(p2->s);
            free(p2->p);
            free(p2);

            return /*FALSE*/;
            }
    }

    //a poscao de s[0] nao e usada para guardar chave
    //arruma para entrar a chave no meio
    memmove(&pai->s[i+1],&pai->s[i],(*(pai->s) - i + 1) * sizeof(pai->s[0]/*tamanho do tipo da chave */)  );


    memmove(&pai->p[i+1],&pai->p[i],(*(pai->s) - i + 1) * sizeof(pai->p[0]/*tamanho do tipo do ponteiro usado em SO_NO */)  );
    


    //atuliza coloca a chave que subio no pai com seu ponteiro a pontando para a sengunda pagina gerada
    pai->s[i] = chaveSobe;
    pai->p[i] = p2;
    //aumenta numero de chave do pai
    (*(pai->s))++;


    //atuliza o m da primeira pagina gerada *(pt->s) == M
    *(pt->s) = meio -1;

    return /*TRUE*/;
}

//ver o caso que a arvore esta vasia
static Bool ___ArvoreB_insercao___(Arvore_B *self, SO_NO *NO_atu, unsigned x, SO_NO *pai){


    unsigned i = 0;
    while(i < *NO_atu->s && x > NO_atu->s[i + 1])
        i++;

    if(i < *NO_atu->s && x == NO_atu->s[i + 1])
        return FALSE;

        
    //se for folha 
    if(NO_atu->p == NULL){
        insere_chave(self, NO_atu, x);
        if(*NO_atu->s > 2 * self->D)
            cisao(self,NO_atu,pai);
        return TRUE;
    }
    

    Bool temp =  ___ArvoreB_insercao___(self, NO_atu->p[i], x, NO_atu);

    if(*NO_atu->s > 2 * self->D)
        cisao(self,NO_atu,pai);

    return temp;
}

static void ___redistribuicao___(Arvore_B *self, SO_NO *NO_atu, SO_NO *pai){


}

static void ___comcatenacao___(){



}

static void ___redistribuicao___(){



}

//retornar 1 se for comcatenacao ou 0 se for redistribuicao
inline static void ___comcatenacao_ou_redistribuicao____(SO_NO *NO_atu, SO_NO *pai){

}
//acha o sucessor da sub arvore e remove e faz as operavao que precisa ate o NO_atua 
static unsigned ___remove___(SO_NO *NO_atu, SO_NO *pai, unsigned i){

    if(*NO_atu->p == NULL){
        //remove a chave que foi substituida
        unsigned temp = NO_atu->s[1];
        memmove(&NO_atu->s[1],&NO_atu->s[2],(*NO_atu->s - 1) * sizeof(*NO_atu->s));
        (*NO_atu->s)--;
        ___comcatenacao_ou_redistribuicao____(NO_atu,pai);
        return temp;
    }   
    
    unsigned temp;
    //para retornar o sucessor
    temp = ___remove___(*NO_atu->p,NO_atu, i);
    
    //verifica se precisa de alguma operacao se precisar faz
    ___comcatenacao_ou_redistribuicao____(NO_atu,pai);

    return temp;
}

static unsigned ___ArvoreB_remocao___(Arvore_B *self, SO_NO *NO_atu, unsigned x, SO_NO *pai){

    unsigned i = 0;
    unsigned temp_ret;
    while(i < *NO_atu->s && x > NO_atu->s[i + 1])
        i++;

    
    if(i < *NO_atu->s && x == NO_atu->s[i + 1]){
        //remove a chave se for folha o no
        temp_ret = NO_atu->s[i + 1];

        if(NO_atu->p == NULL){
            //remove a chave
            memmove(&NO_atu->s[i+1],&NO_atu->s[i+2],(*NO_atu->s - 1) * sizeof(*NO_atu->s));
            //decrementa o m
            (*NO_atu->s)--;
        }else{
            //chama a funcao que vai remover o sucessor e rotar ele e vai verificar se precica fazer alguma opercao de comcatenacao_ou_substitui ate o ponto do NO_atual
            NO_atu->s[i + 1 ] = ___remove___(NO_atu->p[i + 1],NO_atu,i);
        }
        //retorna a chave retirada
        return temp_ret;
    }        


    temp_ret = ___ArvoreB_remocao___(self, NO_atu->p[i],x,NO_atu);

    //verifica se essa pagina precisa de alguma operacao se precisar faz ela inline talves fazer dpi aqui mesmo para otmizar
    ___comcatenacao_ou_redistribuicao____(NO_atu,pai);

    //retorna a chave mas caso fosse uma struuct de dasdos retornaria ela 
    return temp_ret;
}

unsigned *ArvoreB_busca(Arvore_B   *self, unsigned x){
    unsigned f = 0;
    SO_NO *pt  = NULL;
    unsigned g = 0;
    ___busca___(self, x, &pt, &f, &g);

    if (f != 1)
        return NULL;
    return &pt->s[g];

}

Bool ArvoreB_insercao(Arvore_B *self,unsigned x){

    if(self->ptraiz == NULL){
        self->ptraiz = aloca();
        insere_chave(self, self->ptraiz, x);
        return TRUE;
    }

    return ___ArvoreB_insercao___(self, self->ptraiz,x ,NULL);

}

info *ArvoreB_remocao(Arvore_B *self,unsigned chave){
    if(self->ptraiz == NULL){
            
            return NULL;
    }
    return ___ArvoreB_remocao___(self,self->ptraiz,chave,NULL);
}

void ArvoreB_implecao(Arvore_B *self){

}

//recurciva
Bool ArvoreB_desaloca(Arvore_B *self,unsigned chave){

}

SO_NO *ArvoreB_get(Arvore_B *self, unsigned chave){}

void ArvoreB_set(Arvore_B *self,SO_NO *no){}