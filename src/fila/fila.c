#include "fila.h"

#include <stdio.h>
#include <stdlib.h>

FILA *fila_init()
{
    FILA *fila = malloc(sizeof(*fila));

    if (fila == NULL)
    {
        fprintf(stderr, "Erro ao alocar fila.\n");
        exit(EXIT_FAILURE);
    }

    fila->ptinicio = NULL;
    fila->ptfim = NULL;

    fila->fila_init = fila_init;
    fila->insere = insere;
    fila->retira = retira;
    fila->desaloca = desaloca;

    return fila;
}

void insere(FILA *fila, void *x)
{
    NO *novo = malloc(sizeof(*novo));

    if (novo == NULL)
    {
        fprintf(stderr, "Erro ao alocar no.\n");
        exit(EXIT_FAILURE);
    }

    novo->info = x;
    novo->prox = NULL;

    if (fila->ptinicio == NULL)
    {
        fila->ptinicio = novo;
        fila->ptfim = novo;
    }
    else
    {
        fila->ptfim->prox = novo;
        fila->ptfim = novo;
    }
}

void *retira(FILA *fila)
{
    NO *temp;
    void *valor;

    if (fila == NULL || fila->ptinicio == NULL)
    {
        return NULL;
    }

    temp = fila->ptinicio;
    valor = temp->info;

    fila->ptinicio = temp->prox;

    if (fila->ptinicio == NULL)
        fila->ptfim = NULL;

    free(temp);

    return valor;
}

void desaloca(FILA *fila)
{
    NO *temp;

    if (fila == NULL)
        return;

    while (fila->ptinicio != NULL)
    {
        temp = fila->ptinicio;
        fila->ptinicio = fila->ptinicio->prox;
        free(temp);
    }

    free(fila);
}