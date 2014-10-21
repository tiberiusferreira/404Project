#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Rotulo //Estrutura que armazena informa�oes relevantes do rotulo = ITEM
{
    char *nome; //nome do rotulo
    int comentario; //se aponta para a esquerda ou para a direita
    int endereco; //endereco para onde ele aponta
} rotulo;
typedef struct lista //lista de rotulos
{
    rotulo info; //armazena dados
    struct lista *prox;
} node;
node *cria();
void visualiza(node *LISTA);
void libera(node *LISTA);
int vazia(node *LISTA);
void insere(node *LISTA, rotulo e, int pos);
void insere_final(node *LISTA, rotulo e);
void retira(node *LISTA, char *wanted);
rotulo newItem(char *name, int adress, int comment);
rotulo busca(node *LISTA, char *wanted);


#endif // LINKEDLIST_H
