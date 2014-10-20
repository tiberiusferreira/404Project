#ifndef LINKEDLIST_H//Nao sei o que fazer com isso
#define LINKEDLIST_H//Nem com isso

#include <stdio.h>
#include<stdlib.h>

typedef struct rotulo//Estrutura que armazena informações relevantes do rotulo = ITEM
{
    char *nome;
    int comentario;
    int endereco;
} Rotulo;
typedef struct lista//Nó da lista
{
    Rotulo info;
    struct lista *prox;

} node;
node *cria();
void visualiza(node *LISTA);
void inicia(node *LISTA);
void libera(node *LISTA);
int vazia(node *LISTA);
void insere(node *LISTA, Rotulo e, int pos);
void insere_final(node *LISTA, Rotulo e);


#endif // LINKEDLIST_H
