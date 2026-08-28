#ifndef FILA_H
#define FILA_H

typedef struct NO NO;
typedef struct FILA FILA;

struct NO
{
    void *info;
    NO *prox;
};

struct FILA
{
    NO *ptinicio;
    NO *ptfim;

    FILA *(*fila_init)();
    void  (*insere)(FILA *, void *);
    void *(*retira)(FILA *);
    void  (*desaloca)(FILA *);
};

FILA *fila_init(void);
void insere(FILA *, void *);
void *retira(FILA *);
void desaloca(FILA *);

#endif