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

static Bool func_realoc(unsigned D, SO_NO *pt){
    unsigned nova = novaCapacidade(pt->tam_vector, D);

    if(nova <= pt->tam_vector)
        return FALSE;


    int *novo_s = malloc((nova + 1) * sizeof(novo_s[0]));

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
static void busca_aux(Arvore_B *self, int x, SO_NO **pt, unsigned *f, unsigned *g)
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


static Bool resize_chaves(unsigned D, SO_NO *NO){
    return func_realoc(D, NO);
}

static Bool insere_chave_aux(unsigned D, SO_NO *NO, int x){
    unsigned i = 1;
    unsigned m = (unsigned)NO->s[0];

    //Procura a posicao correta de x
    while (i <= m && NO->s[i] < x)
        i++;

    //caso chaves repetidas
    if (i <= m && NO->s[i] == x)
        return FALSE;
    
    //Garante espaco para mais uma chave 
    if(m >= NO->tam_vector && m <= 2 * D){

        if(!resize_chaves(D, NO))
            return FALSE;
        }
    //abre espaco para x 
    if (i <= m) {
        memmove(&NO->s[i + 1], &NO->s[i], (m - i + 1) * sizeof(NO->s[0]));
    }

    //Insere a chave
    NO->s[i] = x;

    //Atualiza m
    NO->s[0]++;
    return TRUE;
}

//*(p->s) == M
//acho que nao trata o caso quando nao tem pai no caso e o pt raiz// pai == NULL
static Bool cisao(Arvore_B *self, SO_NO *pt, SO_NO *pai){

    unsigned M = (unsigned)pt->s[0];

    //o tamanho qunado for nessesario fazer uma cisao sempre vais er impar 
    unsigned meio       = self->D + 1;
    unsigned qtdDireita = M - meio;
    int chaveSobe  = pt->s[meio];


    SO_NO *p2 = malloc(sizeof(*p2));

    if (p2 == NULL)
        return FALSE;

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
        return FALSE;
    }

    p2->p = NULL;

    if(pt->p != NULL){

        p2->p = malloc(
            (p2->tam_vector + 1) * sizeof(p2->p[0])
        );

        if(p2->p == NULL){
            free(p2->s);
            free(p2);
            return FALSE;
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
            free(p2->p);
            free(p2->s);
            free(p2);
            return FALSE;
        }

        novaRaiz->tam_vector = 2 * self->D + 1;

        novaRaiz->s = calloc(novaRaiz->tam_vector + 1, sizeof(novaRaiz->s[0]));

        novaRaiz->p = calloc(novaRaiz->tam_vector + 1, sizeof(novaRaiz->p[0]));

        if(novaRaiz->s == NULL || novaRaiz->p == NULL){

            free(novaRaiz->s);
            free(novaRaiz->p);
            free(novaRaiz);

            free(p2->p);
            free(p2->s);
            free(p2);

            return FALSE;
        }

        novaRaiz->s[0] = 1;
        novaRaiz->s[1] = chaveSobe;

        novaRaiz->p[0] = pt;
        novaRaiz->p[1] = p2;

        pt->s[0] = meio - 1;

        self->ptraiz = novaRaiz;

        return TRUE;
    }


    //procura o lugar que vai colocar a chave que vai subir no pai 
    while(i <= (unsigned)*(pai->s) && pai->s[i] < chaveSobe)
        i++;
    
    //se o vetor de pai estiver com m = tamanho alocado dar um realoc nele
    if ((unsigned)*pai->s >= pai->tam_vector) {

        //se der errado desaloca p2 para nao dar vazamento de memoria 
        if (!func_realoc(self->D, pai)) {

            free(p2->s);
            free(p2->p);
            free(p2);

            return FALSE;
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

    return TRUE;
}

//ver o caso que a arvore esta vasia
static Bool ArvoreB_insercao_aux(Arvore_B *self, SO_NO *NO_atu, int x, SO_NO *pai){


    unsigned i = 0;
    while(i < (unsigned)*NO_atu->s && x > NO_atu->s[i + 1])
        i++;

    if(i < (unsigned)*NO_atu->s && x == NO_atu->s[i + 1])
        return FALSE;

        
    //se for folha 
    if(NO_atu->p == NULL){
        if(!insere_chave_aux(self, NO_atu, x)){
            return FALSE;
        }
        if((unsigned)*NO_atu->s > 2 * self->D)
            if(!cisao(self,NO_atu,pai)){
                return FALSE;
            }
        return TRUE;
    }
    

    Bool temp =  ArvoreB_insercao_aux(self, NO_atu->p[i], x, NO_atu);

    if((unsigned)*NO_atu->s > 2 * self->D)
        if(!cisao(self,NO_atu,pai)){
                return FALSE;
        }

    return temp;
}

static inline Bool inser_chave_ponteiro_aux(unsigned D, SO_NO *NO, int x, SO_NO *ponteiro){
    unsigned i = 1;
    unsigned m = (unsigned)NO->s[0];

    //essa função e para pagina interna
    if(NO->p == NULL)
        return FALSE;

    //Procura posição correta da chave
    while(i <= m && NO->s[i] < x)
        i++;

    //nao permite chave repetida
    if(i <= m && NO->s[i] == x)
        return FALSE;

    //garante espaco para mais uma chave e um ponteiro 
    if(m >= NO->tam_vector && m <= 2 * D){

        if(!resize_chaves(D, NO))
            return FALSE;
    }


    if(i <= m){
        memmove(&NO->s[i + 1], &NO->s[i], (m - i + 1) * sizeof(NO->s[0]));
    }

    //Abre espaco nos ponteiros
    memmove(&NO->p[i + 1], &NO->p[i], (m - i + 1) * sizeof(NO->p[0]));

    //Insere chave */
    NO->s[i] = x;

    //Insere o novo ponteiro a direita da chave
    NO->p[i] = ponteiro;

    //Incrementa m
    (*NO->s)++;

    return TRUE;
}

//i = a posicao do pai que vai receber a comcatenacao
static void comcatenacao_aux(unsigned D,unsigned i, SO_NO *NO_atu, SO_NO *pai, SO_NO *recebedor){


    unsigned n = 1;
    //passa todos as chaves e os ponteiro para o recebedor
    while(n <= (unsigned)*NO_atu->s){
        inser_chave_ponteiro_aux(D,recebedor,NO_atu->s[n],NO_atu->p[n]);
        (*recebedor->s)++;
        n++;
    }
    //passa a chave do pai para o recebedor
    inser_chave_ponteiro_aux(D,recebedor,pai->s[i],NO_atu->p[0]);



    //remove a chave do pai 
    memmove(&pai->s[i],&pai->s[i+1],(*pai->s - i -1)*sizeof(*NO_atu->s));
    memmove(&pai->p[i],&pai->p[i+1],(*pai->s - i)*sizeof(SO_NO));
    pai->s[0]--;
    free(NO_atu->s);
    free(NO_atu->p);
    free(NO_atu);
}


//dir_esq se for 0 e esq// i = a posicao da chave que esta sendo modificado no pai
static inline void redistribuicao_aux(unsigned D,unsigned i, SO_NO *NO_atu, SO_NO *pai, SO_NO *doador, unsigned dir_esq){

    //caso doador a esquerda
    if(dir_esq == 0)
        while(*doador->s > *NO_atu->s){
            
            //insere a chave do pai e o ponteiro do doador
            inser_chave_ponteiro_aux(D, NO_atu, pai->s[i], doador->p[*doador->s]);
            //coloca a chave do doador no pai
            pai->s[i] = doador->s[*doador->s];
            //tira a ultima chave e o ultimo ponteiro do adoador
            (*doador->s)--;

        }
    else
        while(*doador->s > *NO_atu->s){
            
            //insere a chave do pai e o ponteiro do doador
            inser_chave_ponteiro_aux(D, NO_atu, pai->s[i], doador->p[0]);
            //coloca a chave do doador no pai
            pai->s[i] = doador->s[1];
            //tira a primeira chave e o p0 ponteiro do adoador
            memmove(doador->s + sizeof(int), doador->s + 2*sizeof(int), ((*doador->s) - 1)*sizeof(*NO_atu->s));
            memmove(doador->p, doador->p + sizeof(SO_NO), (*doador->s)*sizeof(SO_NO));
            (*doador->s)--;

        }
        
        
}

static inline void casos_aux(unsigned D,unsigned i, SO_NO *NO_atu, SO_NO *pai){

    //se ta no comeco
    if(i == 0){
        if((unsigned)*NO_atu->s + (unsigned)*pai->p[1]->s >= 2 * D)
            redistribuicao_aux(D, 1, NO_atu, pai, /*doador*/pai->p[i + 1],1);
        else
            comcatenacao_aux(D, 1, NO_atu, pai, pai->p[i+1]);

    //se ta no final
    }else if(i == (unsigned)pai->s[0]){
        if((unsigned)*pai->p[i - 1]->s >= 2*D)
            redistribuicao_aux(D, i, NO_atu, pai, /*doador*/pai->p[i - 1],0);
        else
            comcatenacao_aux(D, i, NO_atu, pai, pai->p[i-1]);

    //se ta no meio
    }else{
        if((unsigned)*pai->p[i + 1]->s >= 2*D)
            redistribuicao_aux(D, i, NO_atu, pai, /*doador*/pai->p[i + 1],1);

        else if((unsigned)*pai->p[i - 1]->s >= 2*D)
            redistribuicao_aux(D, i, NO_atu, pai, /*doador*/pai->p[i - 1],0);

        else{

            if((unsigned)*pai->p[i + 1]->s < (unsigned)*pai->p[i - 1]->s)
                comcatenacao_aux(D, i , NO_atu, pai, pai->p[i-1]);

            else
                comcatenacao_aux(D, i, NO_atu, pai, pai->p[i+1]);

        }

    }
    


}

//retornar 1 se for comcatenacao ou 0 se for redistribuicao
inline static void comcatenacao_ou_redistribuicao(unsigned D, SO_NO *NO_atu, SO_NO *pai){
    //se nao precisar de alguma operacao retorna
    if((unsigned)*NO_atu->s >= D || pai == NULL)
        return;

    unsigned i = 0;
    //procura a posicao do ponteiro do No_atu no pai
    while(pai->p[i] != NO_atu)
        i++;

    casos_aux(D ,i, NO_atu , pai);

}

//acha o sucessor da sub arvore e remove_sucessor_aux e faz as operavao que precisa ate o NO_atua 
static int remove_sucessor_aux(unsigned D, SO_NO *NO_atu, SO_NO *pai){

    if(NO_atu->p == NULL){
        //remove_sucessor_aux a chave que foi substituida
        int temp = NO_atu->s[1];
        memmove(&NO_atu->s[1],&NO_atu->s[2],(*NO_atu->s - 1) * sizeof(*NO_atu->s));
        (*NO_atu->s)--;
        comcatenacao_ou_redistribuicao(D, NO_atu, pai);
        return temp;
    }   
    
    int temp;
    //para retornar o sucessor
    temp = remove_sucessor_aux(D, *NO_atu->p,NO_atu);
    
    //verifica se precisa de alguma operacao se precisar faz
    comcatenacao_ou_redistribuicao(D, NO_atu,pai);

    return temp;
}

static Bool ArvoreB_remocao_aux(unsigned D, SO_NO *NO_atu, int x, SO_NO *pai){

    unsigned i = 0;
    Bool temp_ret = FALSE;
    while(i < (unsigned)*NO_atu->s && x > NO_atu->s[i + 1])
        i++;

    
    if(i < (unsigned)*NO_atu->s && x == NO_atu->s[i + 1]){

        if(NO_atu->p == NULL){
            //remove_sucessor_aux a chave
            memmove(&NO_atu->s[i+1],&NO_atu->s[i+2],(*NO_atu->s - i - 1) * sizeof(*NO_atu->s));
            //decrementa o m
            (*NO_atu->s)--;
        }else{
            //chama a funcao que vai remove_sucessor_auxr o sucessor e rotar ele e vai verificar se precica fazer alguma opercao de comcatenacao_ou_substitui ate o ponto do NO_atual
            NO_atu->s[i + 1] = remove_sucessor_aux(D, NO_atu->p[i + 1],NO_atu);
        }

        comcatenacao_ou_redistribuicao(D, NO_atu,pai);

        //retorna a chave retirada
        return TRUE;
    }        

    if(NO_atu->p != NULL)
        temp_ret = ArvoreB_remocao_aux(D, NO_atu->p[i],x,NO_atu);

    //verifica se essa pagina precisa de alguma operacao se precisar faz ela inline talves fazer dpi aqui mesmo para otmizar
    if(temp_ret)
        comcatenacao_ou_redistribuicao(D, NO_atu,pai);

    //retorna a chave mas caso fosse uma struuct de dasdos retornaria ela 
    return temp_ret;
}

static void ArvoreB_desaloca_aux(SO_NO *NO){

    if(NO == NULL)
        return;

    if(NO->p != NULL){

        unsigned m = (unsigned)NO->s[0];

        for(unsigned i = 0; i <= m; i++){
            ArvoreB_desaloca_aux(NO->p[i]);
        }
    }

    free(NO->p);
    free(NO->s);
    free(NO);
}


int *ArvoreB_busca(Arvore_B   *self, int x){
    unsigned f = 0;
    SO_NO *pt  = NULL;
    unsigned g = 0;
    busca_aux(self, x, &pt, &f, &g);

    if (f != 1)
        return NULL;
    return &pt->s[g];

}

Bool ArvoreB_insercao(Arvore_B *self,int x){

    if(self->ptraiz == NULL){
        self->ptraiz = aloca();
        return insere_chave_aux(self->D, self->ptraiz, x);
    }

    return ArvoreB_insercao_aux(self, self->ptraiz,x ,NULL);

}

Bool ArvoreB_remocao(Arvore_B *self,int chave){
    if(self->ptraiz == NULL){
            
            return FALSE;
    }
    return ArvoreB_remocao_aux(self->D,self->ptraiz,chave,NULL);
}

void ArvoreB_implecao(Arvore_B *self){

}

//recurciva
Bool ArvoreB_desaloca(Arvore_B *self){
    ArvoreB_desaloca_aux(self->ptraiz);
    free(self->ptraiz);
}

SO_NO *ArvoreB_get(Arvore_B *self, int chave){}

void ArvoreB_set(Arvore_B *self,SO_NO *no){}