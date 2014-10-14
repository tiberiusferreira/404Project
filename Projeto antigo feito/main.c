/*
 Nomes:
        * Alexandre Kei Saito
        * Bruno de Souza Ferreira
 R.A.s:
        * 134763
        * 135164
 MC404 -- Projeto 1 -- Conversor de IAS em linguagem de máquina

*/

/*
----------------------------------------------------------
                    BIBLIOTECAS
----------------------------------------------------------
*/

#include <stdio.h>
#include "ias.h"

/*
----------------------------------------------------------
                  FUNÇÃO PRINCIPAL
----------------------------------------------------------
*/

int main (int argc, char *argv[]){ /* Argumentos de entrada */
    ler(argv[1]); /* Descrição melhor no ias.c */
	montagem( ); /* Descrição melhor no ias.c */
    escrita(argv[2]); /* Descrição melhor no ias.c */
	return 0;
}
