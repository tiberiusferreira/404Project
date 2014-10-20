#include "linkedlist.h"

typedef struct rotulo//Estrutura que armazena informa��es relevantes do rotulo = ITEM
{
    char *nome;
    int comentario;
    int endereco;
} Rotulo;

typedef struct lista//N� da lista
{
    Rotulo info;
    struct lista *prox;

} node;

node *cria()//cria a lista
{
    node *novo= (node *) malloc(sizeof(node));
    novo->prox=NULL;
    return novo;
}

void visualiza(node *LISTA)//imprime a lista para visualiza��o
{
    node *aux;
    for(aux=LISTA->prox; aux->prox!=NULL; aux=aux->prox)
    {
        printf("%s, ",aux->info.nome);
    }
     printf("%s.\n",aux->info.nome);

}
void inicia(node *LISTA)//inicializa a lista
{
    LISTA->prox = NULL;
}
void libera(node *LISTA)//libera a lista e todos nos associados a ela
{
    if(!vazia(LISTA))
    {
        node *proxNode,
             *atual;

        atual = LISTA->prox;
        while(atual != NULL)
        {
            proxNode = atual->prox;
            if(atual!=NULL) free(atual);
            atual = proxNode;
        }
    }
}
int vazia(node *LISTA)//verifica se a lista est� vazia
{
    if(LISTA->prox == NULL)
        return 1;
    else
        return 0;
}

void insere(node *LISTA, Rotulo e, int pos)//insere novo n� na lista na posicao desejada
{
    int count;
    node *novo=(node *) malloc(sizeof(node));
    novo->info=e;
    if(pos==1)
    {
        node *oldHead = LISTA->prox;
        LISTA->prox = novo;
        novo->prox = oldHead;
    }
    else
    {
        node *atual = LISTA->prox,
              *anterior=LISTA;

        for(count=1 ; count < pos ; count++)
        {
            anterior=atual;
            atual=atual->prox;
        }
        anterior->prox=novo;
        novo->prox = atual;
    }
}
void insere_final(node *LISTA, Rotulo e)// Insere novo n� no final da lista
{
    node *novo=(node *) malloc(sizeof(node));
    novo->info=e;
    if(LISTA->prox==NULL)
    {
        node *oldHead = LISTA->prox;
        LISTA->prox = novo;
        novo->prox = oldHead;
    }
    else if(LISTA->prox->prox==NULL)
    {
        LISTA->prox->prox=novo;
        novo->prox=NULL;
    }
}
/*N�O SEI O QUE FAZER COM ISSO...
linkedlist::linkedlist()
{
}
*/