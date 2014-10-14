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

#include "ias.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
----------------------------------------------------------
                DEFINIÇÕES DE VARIÁVEIS
----------------------------------------------------------
*/


/*
Definem o tipo do comando
*/
#define WORD 1
#define INSTRU 2

/*
Definem o lado da instrução
*/
#define LH 0
#define RH 1

/*
Definição de alguns tamanhos básicos
*/
#define INST_MAX 2024
#define STRI_MAX 128

/*
----------------------------------------------------------
                VARIÁVEIS GLOBAIS
----------------------------------------------------------
*/

int RotuloT = 0; /* tamanho do rótulo */
int DiretivaT = 0; /*Tamanho da diretiva */
int comandoT = 0; /*Tamanho do comando*/
Rotulo rotulos[INST_MAX]; /*vetor para os rótulos */
DirSet dir[INST_MAX]; /* vetor das diretivas */
CComando comandos[INST_MAX]; /* vetor de comandos*/
char instrucoes[INST_MAX][STRI_MAX]; /* vetor de instruções*/



/*
----------------------------------------------------------
            DECLARAÇÃO DE FUNÇÕES
----------------------------------------------------------
*/

/*
O que faz cada função será descrita melhor dentro do código
*/


/*
-------FUNÇÕES GENELARIZADAS---------
*/
void montagem ();
void escrita (char *nome);
void leitura (char *nome);
/*
-------FUNÇÕES DIRETIVAS---------
*/
DirOrg orgDiretiva (char comando [STRI_MAX]);
DirAlign alignDiretiva (char comando [STRI_MAX]);
DirWord wordDiretiva (char comando [STRI_MAX]);
DirWfill wfillDiretiva (char comando [STRI_MAX]);
DirSet setDiretiva (char comando [STRI_MAX]);
/*
-------FUNÇÕES REMOVEDORAS---------
*/
void removeRotulo (char comando [STRI_MAX]);
void removeComentario (char comando [STRI_MAX]);
void removeEspaco (char comando [STRI_MAX]);
/*
-------FUNÇÕES CONVERSORAS & AUXILIARES---------
*/
Cursor cursor (char *nome);
long long int conversor(char *nome);
Rotulo rotulo (char comando [STRI_MAX]);
/*
-------FUNÇÕES DE COMANDO---------
*/
void comando (char *instrucao, Cursor cursor, int word);
int endereco (CComando comando);
int opcode (CComando comando);


/*
----------------------------------------------------------
                FUNÇÃO GENERALIZADAS
----------------------------------------------------------

Escopo: Função mais básicas usadas na .main. Uma delas lê o arquivo, outra monta o mapa de memoria e a última escreve no arquivo texto o mapa de memória
*/

/*
-------------FUNÇÃO MONTAGEM----------------

Escopo: Esta função basicamente monta o vetor de comandos
*/
void montagem () {
	int i=0,j=0;
	Cursor cursor;
	cursor.linha = 0; /* Iicio o cursor */
	cursor.lado = LH;
	for (i = 0; i < INST_MAX; i++) { /*Nesta parte do código, ocorre apenas uma triagem, calculando as linhas e lados das instruçõese calculamos os Rótulos e constantes */
        removeComentario(instrucoes[i]);
        if (instrucoes[i][0]!='\0') {
    		if (orgDiretiva(instrucoes[i]).temp) { /* Verifica se o comando é uma diretiva org */
    			cursor.linha = orgDiretiva(instrucoes[i]).linha; /* Ajusto a posição do cursor */
    			cursor.lado = LH;
    		}
    		else
                if (alignDiretiva(instrucoes[i]).temp){ /* Verifica se o comando é uma diretiva align */
    			    cursor.linha = cursor.linha - (cursor.linha % alignDiretiva(instrucoes[i]).mult) + alignDiretiva(instrucoes[i]).mult; /* Ajusto a posição do cursor */
    			    cursor.lado = LH;
    		    }
    		else
                if (wordDiretiva(instrucoes[i]).temp){ /* Ajusto a posição do cursor */
    			   if (cursor.lado == RH) {
    				   cursor.linha ++;
    				   cursor.lado = LH;
    			   } /* Inserindo um comando do tipo word */
    			comando(instrucoes[i], cursor, -1);
    			cursor.linha ++;
    		   }
            else
                if (wfillDiretiva(instrucoes[i]).temp) { /* Verifica se o comando é uma diretiva wfill */
                    if (cursor.lado == RH) { /* Ajusto a posição do cursor */
    			    	cursor.linha ++;
    			    	cursor.lado = LH;
    			    } /* Para cada posição do wfill insiro um comando do tipo word */
    		     	for (j = 0; j < wfillDiretiva(instrucoes[i]).linha; j++) {
    		    		comando(instrucoes[i], cursor, -1);
    		    		cursor.linha++;
    		     	}
    		    }
            else
                if (setDiretiva(instrucoes[i]).temp) { /* Verifica se o comando é uma diretiva set */
    			   dir[DiretivaT].rotulo = setDiretiva(instrucoes[i]).rotulo;
    			   dir[DiretivaT].valor = setDiretiva(instrucoes[i]).valor;
                   DiretivaT++;
    		   }
            else
                if (instrucoes[i][0] != '\0') { /* Verifica se o comando é uma instrução válida */
    			comando(instrucoes[i], cursor, -1);
                if (comandos[comandoT - 1].instrucao[0] != '\0'){/* Ajusto a posição do cursor */
                    if (cursor.lado == LH)
                        cursor.lado = RH;
                    else {
                        cursor.lado = LH;
                        cursor.linha++;
                    }
                }
                else {
                    comandoT--;
                }
    		}
        }
}
	for (i = 0; i < comandoT; i++) { /* Após calculadas as linhas das intruções, os rótulos e constantes, podemos calcular os endereços ou palavras das instruções e o opcode das instruções */
		if (wordDiretiva(comandos[i].instrucao).temp) {
			comandos[i].word = wordDiretiva(comandos[i].instrucao).valor;
			comandos[i].tipo = WORD;
		}
		else
		   if (wfillDiretiva(comandos[i].instrucao).temp) {
			  comandos[i].word = wfillDiretiva(comandos[i].instrucao).valor;
			  comandos[i].tipo = WORD;
		}
		else
            if (comandos[i].instrucao[0] != '\0') {
              comandos[i].tipo = INSTRU;
			  comandos[i].endereco = endereco(comandos[i]);
			  comandos[i].opcode  = opcode(comandos[i]);
		}
	}
}

/*
-------------FUNÇÃO ESCRITA----------------

Escopo: Imprime no arquivo já o código em linguagem de montagem
*/
void escrita (char *nome) {
    FILE *file;
    char word [STRI_MAX];
    int i;
    if(!(file = fopen(nome, "w")))   /* le o arquivo */
        printf("ERRO! Arquivo nao encontrado!\n");
    else {
        for (i = 0; i < comandoT; i++) {
            if (comandos[i].cursor.lado == LH) /* Se for um comando à esquerda, imprime o número da linha */
                fprintf(file, "%03X ", comandos[i].cursor.linha);
            if (comandos[i].tipo == INSTRU) { /* Verifica se é um comando normal */
                sprintf(word, "%016X", comandos[i].endereco);
                fprintf(file, "%02X%s", comandos[i].opcode, word + 13);
            }
            if (comandos[i].tipo == WORD){/* Verifica se é um comando do tipo word*/
                sprintf(word, "%016llX", comandos[i].word);
                fprintf(file, "%10s", word + 6);
            }
            if (comandos[i+1].cursor.lado == LH && comandos[i].cursor.lado == LH && comandos[i].tipo == INSTRU) {
                fprintf(file, "00000");
            } /* Se na linha do comando não aparecer mais nenhum comando, imprimir 00000 */
            if (comandos[i+1].cursor.lado == LH){/* Se o comando for o ultimo da linha, imprimir quebra de linha */
                fprintf(file, "\n");
            }
        }
        fprintf(file, "\n");
        fclose(file);
    }
}

/*
-------------FUNÇÃO LEITURA----------------

Escopo: Função de leitura de um arquivo, ele lê o arquivo "cru"
*/
void leitura (char *nome) {
    FILE *file;
    char temp;
    int status = 0;
    int i,j;
    for (i = 0; i < INST_MAX; i++) {
        for (j = 0; j < STRI_MAX; j++) {
            instrucoes[i][j] = '\0';
        }
    }
    if(!(file = fopen(nome, "r+")))
        printf("ERRO! Arquivo nao encontrado!\n");
    else{
        i = 0;
        j = 0;
        while(status > -1) {
            status = fscanf(file, "%c", &temp);
            if (temp == '\n' || status == -1) {
                i++;
                j = 0;
            } else {
                if (temp >= 'a' && temp <= 'z') {
                    temp = temp - 'a' + 'A';
                }
                instrucoes[i][j] = temp;
                j++;
            }
        }
    }
}


/*
----------------------------------------------------------
                FUNÇÃO DIRETIVAS
----------------------------------------------------------

Escopo: Estas funções abaixo tem como objetivo ajustas as diretivas. A identificação está nas funções de comando
*/


/*
-------------FUNÇÃO DIRETIVA ORG----------------

Escopo: Esta função basicamente tenta reconhecer se o comando é uma diretiva .org, caso contrário retorna uma mensagem de erro. É válido chamar a atenção que esta é uma funão recursiva
*/
DirOrg orgDiretiva (char comando [STRI_MAX]){
	DirOrg saida;
	char *position = strstr(comando, ".ORG"); /* Localizar a diretiva .org*/
	if (position != NULL){
		saida.linha = conversor(position+5);
		if (saida.linha < 0){ /* Aqui é verificado se realmente é um .org que está sendo colocado */
			printf("ERRO!\nDiretiva .ORG nao recebe parametros negativos!.\n");
			exit(1); /*Caso dê errado, sai da função*/
		}
		saida.temp = 1;
	}
	else
		saida.temp = 0;
	return saida;
}

/*
-------------FUNÇÃO DIRETIVA ALIGN----------------

Escopo: Aqui é verificado se o comando é um .align, função análoga à anterior
*/
DirAlign alignDiretiva (char comando [STRI_MAX]) {
	DirAlign saida;
	char *position = strstr(comando, ".ALIGN"); /* Localizar a diretiva .align*/
	if (position != NULL){
		saida.mult = conversor(position+7);
		if (saida.mult < 1){/* Aqui é verificado se realmente é um .align que está sendo colocado */
			printf("ERRO!\nDiretiva .ALIGN nao recebe parametros negativos ou nulos!\n");
			exit(1); /*Saida da funão */
		}
		saida.temp = 1;
	}
	else
		saida.temp = 0;
	return saida;
}

/*
-------------FUNÇÃO DIRETIVA WORD----------------

Escopo: Aqui é verificado se o comando é um .word, função análoga à anterior
*/
DirWord wordDiretiva (char comando [STRI_MAX]) {
	DirWord saida;
	char *position = strstr(comando, ".WORD");/* Aqui é verificado se realmente é um .word que está sendo colocado */
	if (position != NULL){
		saida.valor = conversor(position+6);
		saida.temp = 1;
	}
	else
		saida.temp = 0;
	return saida;
}

/*
-------------FUNÇÃO DIRETIVA WFILL----------------

Escopo: Aqui é verificado se o comando é um .wfill, função análoga à anterior
*/
DirWfill wfillDiretiva (char comando [STRI_MAX]) {
	DirWfill saida;
	char *position = strstr(comando, ".WFILL"); /* Aqui é verificado se realmente é um .word que está sendo colocado */
	if (position != NULL) {
		saida.linha = conversor(position+7);
		saida.valor = conversor(strstr(position+7," ")+1);
		if (saida.linha < 1){/* Aqui é verificado se realmente é um .wfill que está sendo colocado */
			printf("ERRO!\nDiretiva .WFILL nao recebe parametros negativos ou nulos!\n");
			exit(1);
		}
		saida.temp = 1;
	}
	else
		saida.temp = 0;
	return saida;
}

/*
-------------FUNÇÃO DIRETIVA SET----------------

Escopo: Aqui é verificado se o comando é um .set, função análoga à anterior
*/
DirSet setDiretiva (char comando [STRI_MAX]) {
	DirSet saida;
	char *position = strstr(comando, ".SET"); /* Aqui é verificado se realmente é um .set que está sendo colocado */
	if (position != NULL) {
		saida.rotulo = malloc(sizeof(char) * STRI_MAX);
		memcpy(saida.rotulo, comando + 5, strstr(comando + 5, " ") - comando - 5);
		saida.valor = conversor(strstr(comando + 5, " ") + 1);
		saida.temp = 1;
	}
	else
		saida.temp = 0;
	return saida;
}

/*
-------------FUNÇÃO ROTULO----------------

Escopo: Verifica o rótulo do comando e retorna o mesmo caso exista
*/
Rotulo rotulo (char comando [STRI_MAX]) {
	Rotulo saida;
	char *position;
	position = strstr(comando, ":");
	if (position != NULL) {
		saida.rotulo = malloc(sizeof(char) * STRI_MAX);
		memcpy(saida.rotulo, comando, position - comando);
		saida.temp = 1;
	}
	else
		saida.temp = 0;
	return saida;
}



/*
----------------------------------------------------------
                FUNÇÃO REMOVEDORAS
----------------------------------------------------------

Escopo: O objetivo destas funções é tanto remover os espaos do código quanto remover comentários e rótulos, ou seja, "limpar" o código.
*/


/*
-------------FUNÇÃO REMOVE RÓTULO----------------

Escopo: Remove os rótulos, colocando ' ' em seu lugar
*/
void removeRotulo (char comando [STRI_MAX]) {
	int i;
	char *posicao = strchr(comando, ':');
	if (posicao != NULL) {
		for (i = 0; i < posicao - comando + 1; i++)
			comando[i] = ' ';/* Todas as posições anteriores ao ':'' serão ' ' */
	}
}

/*
-------------FUNÇÃO REMOVE COMENTÁRIO----------------

Escopo: Função que remove os comentários
*/
void removeComentario (char comando [STRI_MAX]) {
	int i;
	char *posicao = strchr(comando, ';');
	if (posicao != NULL) {
		for (i = posicao - comando; i < STRI_MAX; i++)
			comando[i] = '\0'; /* Coloca todas as posições depois ';'' para '\0' */
	}
}

/*
-------------FUNÇÃO REMOVE ESPAÇO----------------

Escopo: Remove todos os espaços em braco da string.

*/
void removeEspaco (char comando [STRI_MAX]) {
    size_t len = 0;
    char *frontp = comando - 1;
    char *endp = NULL;
    if (comando[0] != '\0') {
	    len = strlen(comando);
	    endp = comando + len;
	    while (isspace(*(++frontp)));
	    while (isspace(*(--endp)) && endp != frontp);
	    if (comando + len - 1 != endp)
	        *(endp + 1) = '\0';
	    else
            if (frontp != comando && endp == frontp)
	    	   *comando = '\0';
	    endp = comando;
	    if(frontp != comando) {
	        while (*frontp)
	        	*endp++ = *frontp++;
	        *endp = '\0';
	    }
	}
}

/*
----------------------------------------------------------
            FUNÇÃO DE CONVERSÃO & AUXILIARES
----------------------------------------------------------

Escopo:Converte um número em HEX, BIN ou OCTA. Contém funões auxíliares cujas as funcionalidades estão descritas

*/

/*
-------------FUNÇÃO ENDEREÇO----------------

Escopo: Atualiza o valor do cursor

*/
Cursor cursor (char *nome) {
	Cursor resul;
	int j=0;
	resul.linha = -1;
	while(j < RotuloT){
		if (memcmp(rotulos[j].rotulo, nome, strlen(rotulos[j].rotulo)) == 0) {
			resul.linha = (*rotulos[j].comando).cursor.linha;
			resul.lado = (*rotulos[j].comando).cursor.lado;
		}
		j++;
	}
	return resul;
}


/*
-------------FUNÇÃO CONVERSOR----------------

Escopo: Converte a string para um inteiro válido. Lembrando que esta função utiliza um int do tipo long long, o que pode dar problemas em alguns tipos de computadores. Além disso, foi implementada uma lógica para outros tipos de bases numéricas também.

*/
long long int conversor(char *nome) {
	long long int i = 0;
	int j=0;
	Cursor rotuloCursor;
	if (nome != NULL) { /* Verifica se exite algum valor na string */
		if ((nome[0] > '9' || nome[0] < '0') && nome[0] != '-') { /* Verifica se o valor passado é um rótulo ou uma constante */
			rotuloCursor = cursor(nome);
			if (rotuloCursor.linha > -1)
                i = rotuloCursor.linha;
			else{/* Caso seja uma constante, pega o valor dela */
				for (j = 0; j < DiretivaT; j++) {
					if (memcmp(dir[j].rotulo, nome, strlen(dir[j].rotulo)) == 0) {
						i = dir[j].valor;
					}
				}
			}
		}
		else
            if (nome[0] == '0' && nome[1] == 'X') /* Verifica se o valor passado é hexadecimal */
			      sscanf(nome+2, "%llx", &i);
        else
            if (nome[0] == '0' && nome[1] == 'O') /* Verifica se o valor passado é octal */
			      sscanf(nome+2, "%llo", &i);
		else
		    if (nome[0] == '0' && nome[1] == 'B') { /* Verifica se o valor passado é binário */
			   i = 0;
			   j = 2;
			   while (j < STRI_MAX && nome[j] != '\0' && nome[j] != ')' && nome[j] != ' '){/* Conversor de binário para decimal */
			    	i = i*2;
			    	i += nome[j] - '0';
			    	j++;
			}
		}
		else
			sscanf(nome, "%lld", &i); /* Pega o valor em decimal */
	}
	return i;
}


/*
----------------------------------------------------------
                FUNÇÃO DE COMANDOS
----------------------------------------------------------

Escopo: Estas funções implementam os comandos de fato. Elas leem o comando, fazem a identificação deles

*/

/*
-------------FUNÇÃO COMANDO----------------

Escopo: Insere o comando no vetor global de comandos

*/
void comando (char *instrucao, Cursor cursor, int word) {
    comandos[comandoT].cursor = cursor;
    comandos[comandoT].word = word;
    comandos[comandoT].instrucao = instrucao;
    if (comandos[comandoT].instrucao != NULL) {
        if (rotulo(instrucao).temp) { /* Se o comando possuir uma rotulo, insere-a no vetor de rotulos */
            rotulos[RotuloT].rotulo = rotulo(instrucao).rotulo;
            rotulos[RotuloT].comando = &comandos[comandoT];
            RotuloT++;
        }
        removeRotulo(comandos[comandoT].instrucao);  /* Limpa a string */
        removeEspaco(comandos[comandoT].instrucao); /* Limpa a string */
    }
    comandoT++; /* Atualiza o valor to tamanho da string */
}

/*
-------------FUNÇÃO ENDEREÇO----------------

Escopo: Esta função retorna o endereço da instrução

*/
int endereco (CComando comando) {
	if (strstr(comando.instrucao,"(") != NULL)
		return conversor(strstr(comando.instrucao,"(") + 1);
	else
		return 0;
}

/*
-------------FUNÇÃO OPCODE----------------

Escopo: Retorna o opcode da instrução

*/
int opcode (CComando comando){
	Cursor rotuloCursor;
	if (strstr(comando.instrucao,"(") != NULL) /* Caso o endereço da instrução seja um Rotulo, pega o cursor do Rotulo */
		rotuloCursor = cursor(strstr(comando.instrucao,"(") + 1);
	if (strstr(comando.instrucao, " ") != NULL) /* Agora irá entrar em uma sequencia de IFs para identificar a instrução */
		*strstr(comando.instrucao, " ") = '\0';
	if (strcmp(comando.instrucao, "LDMQ") == 0)
		return 10;
	else
        if (strcmp(comando.instrucao, "LDMQM") == 0)
		   return 9;
	else
	    if (strcmp(comando.instrucao, "STR") == 0)
		   return 33;
	else
	    if (strcmp(comando.instrucao, "LOAD") == 0)
		   return 1;
	else
	    if (strcmp(comando.instrucao, "LDN") == 0)
           return 2;
	else
	    if (strcmp(comando.instrucao, "LDABS") == 0)
		   return 3;
	else
	    if (strcmp(comando.instrucao, "JMP") == 0){
           if (rotuloCursor.lado == LH)
              return 13;
        else
           if (rotuloCursor.lado == RH)
              return 14;
        else
           return 13;
	    }
        else
           if (strcmp(comando.instrucao, "JGEZ") == 0){
              if (rotuloCursor.lado == LH)
                return 15;
            else
              if (rotuloCursor.lado == RH)
                return 16;
            else
            return 15;
	       }
        else
            if (strcmp(comando.instrucao, "ADD") == 0)
		       return 5;
	    else
	        if (strcmp(comando.instrucao, "ADDABS") == 0)
		       return 7;
	    else
	        if (strcmp(comando.instrucao, "SUB") == 0)
		       return 6;
	    else
	        if (strcmp(comando.instrucao, "SUBABS") == 0)
		       return 8;
	    else
	        if (strcmp(comando.instrucao, "MUL") == 0)
		       return 11;
	    else
	        if (strcmp(comando.instrucao, "DIV") == 0)
		       return 12;
	    else
	        if (strcmp(comando.instrucao, "LSH") == 0)
		       return 20;
	    else
	        if (strcmp(comando.instrucao, "RSH") == 0)
		       return 21;
	    else
	        if (strcmp(comando.instrucao, "STM") == 0){
		       if (rotuloCursor.lado == LH) {
               return 18;
		    }
            else
               if (rotuloCursor.lado == RH)
		        	return 19;
		   else{
             return 18;
	        }
       }
    return 0;
}
