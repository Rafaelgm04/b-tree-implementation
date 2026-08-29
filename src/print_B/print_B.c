#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "arvore_B.h"
#include "fila.h"

typedef struct SO_print {
    const SO_NO *NO;
    unsigned nivel;
    int colunai;
    int colunaf;
} SO_print;



static void imprime(const SO_NO *s_no, int salto_linha, int colunai, int colunaf){
    if (s_no == NULL)
        return;

    FILA *fila = fila_init();

    SO_print *raiz = malloc(sizeof(*raiz));

    raiz->NO = s_no;
    raiz->nivel = 1;
    raiz->colunai = colunai;
    raiz->colunaf = colunaf;

    fila->insere(fila, raiz);


    while (fila->ptinicio != NULL)
    {
        SO_print *temp = fila->retira(fila);

        char buffer[1000] = "";

        size_t n = 0;

        unsigned m = (unsigned)*temp->NO->s;

        for (unsigned i = 1; i <= m; i++)
        {
            int escrito = snprintf(buffer + n, sizeof(buffer) - n, "%d ", temp->NO->s[i]);

            if (escrito < 0)
                break;

            if ((size_t)escrito >= sizeof(buffer) - n)
                break;

            n += (size_t)escrito;
        }


        /*
         * Centro da região reservada
         * para esta página.
         */
        int meio = (temp->colunai + temp->colunaf) / 2;


        attron(COLOR_PAIR(2));

        mvprintw( (int)temp->nivel * salto_linha, meio - (int)strlen(buffer) / 2, "%s", buffer);

        attroff(COLOR_PAIR(2));


        /*
         * Página interna:
         * m chaves => m+1 filhos.
         */
        if (temp->NO->p != NULL)
        {
            unsigned quantidade_filhos = m + 1;

            int largura =
                temp->colunaf - temp->colunai;

            for (unsigned i = 0; i < quantidade_filhos;i++){
                if (temp->NO->p[i] != NULL)
                {
                    SO_print *filho =
                        malloc(sizeof(*filho));

                    filho->NO = temp->NO->p[i];

                    filho->nivel = temp->nivel + 1;


                    /*
                     * Divide a região horizontal
                     * do pai entre os filhos.
                     */
                    filho->colunai = temp->colunai + (largura * (int)i) / (int)quantidade_filhos;

                    filho->colunaf =temp->colunai + (largura * (int)(i + 1)) / (int)quantidade_filhos;


                    fila->insere(fila, filho);
                }
            }
        }


        free(temp);
    }


    fila->desaloca(fila);
}



void imprime_arvore(const void *ptraiz){

	const struct SO_NO *ptr = ptraiz;
	int nlinhas, ncolunas, salto_linha;
	
	initscr();
	clear();
	getmaxyx(stdscr, nlinhas, ncolunas);
	salto_linha = (nlinhas-4)/6;

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
 	init_pair(3, COLOR_RED, COLOR_GREEN);
  	attron(COLOR_PAIR(1));	

	if( ptr == NULL )
	{
		mvprintw(nlinhas/2, ncolunas/2-7, "Arvore vazia!");
		attroff(COLOR_PAIR(1));
	}
	else
	{
	 	mvprintw(salto_linha/2, ncolunas/2-2, "Raiz");	
 		attroff(COLOR_PAIR(1));
 		
 		imprime(ptr, salto_linha, 1, ncolunas);
	}
	
	move(getmaxy(stdscr)-1, getmaxx(stdscr)-1);
	refresh();
	getch();
	endwin();
}
