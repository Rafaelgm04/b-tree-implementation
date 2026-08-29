#include <stdio.h>
#include <stdlib.h>
#include "arvore_B.h"


static void limpa_buffer(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}


int main(void)
{
    unsigned D;
    int opcao = 0;
    int chave;

    printf("Digite a ordem D da Arvore B: ");
    while(opcao <= 0)
        if (scanf("%d", &opcao) != 1 || opcao <= 0)
            printf("Valor de D invalido. Digite novamente: \n");
        

    D = (unsigned)opcao;
    Arvore_B arvore = init(D);

    do
    {
        printf("\n");
        printf("==============================\n");
        printf("          ARVORE B            \n");
        printf("==============================\n");
        printf("Ordem D = %u\n\n", D);

        printf("1 - Inserir chave\n");
        printf("2 - Remover chave\n");
        printf("3 - Buscar chave\n");
        printf("4 - Imprimir arvore\n");
        printf("5 - Desalocar arvore\n");
        printf("0 - Sair\n");

        printf("\nOpcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            printf("Entrada invalida.\n");
            limpa_buffer();
            continue;
        }

        switch (opcao)
        {
            case 1:

                printf("Digite a chave para inserir: ");

                if (scanf("%d", &chave) != 1)
                {
                    printf("Chave invalida.\n");
                    limpa_buffer();
                    break;
                }

                if (arvore.Insercao(&arvore, chave))
                    printf("Chave %d inserida.\n", chave);
                else
                    printf("Nao foi possivel inserir a chave %d.\n", chave);


                arvore.Implecao(&arvore);

                break;


            case 2:

                printf("Digite a chave para remover: ");

                if (scanf("%d", &chave) != 1)
                {
                    printf("Chave invalida.\n");
                    limpa_buffer();
                    break;
                }

                if (arvore.Remocao(&arvore, chave))
                    printf("Chave %d removida.\n", chave);
                else
                    printf("Chave %d nao encontrada.\n", chave);

                arvore.Implecao(&arvore);

                break;


            case 3:
            {
                int *resultado;

                printf("Digite a chave para buscar: ");

                if (scanf("%d", &chave) != 1)
                {
                    printf("Chave invalida.\n");
                    limpa_buffer();
                    break;
                }

                resultado = arvore.Busca(&arvore, chave);

                if (resultado != NULL)
                    printf("Chave %d encontrada.\n", *resultado);
                else
                    printf("Chave %d nao encontrada.\n", chave);

                arvore.Implecao(&arvore);

                break;
            }


            case 4:

                arvore.Implecao(&arvore);

                break;


            case 5:

                if (arvore.Desaloca(&arvore))
                    printf("Arvore desalocada.\n");
                else
                    printf("Erro ao desalocar a arvore.\n");

                arvore.Implecao(&arvore);

                break;


            case 0:

                arvore.Desaloca(&arvore);

                printf("Programa encerrado.\n");

                break;


            default:

                printf("Opcao invalida.\n");
                break;
        }

    } while (opcao != 0);

    return EXIT_SUCCESS;
}