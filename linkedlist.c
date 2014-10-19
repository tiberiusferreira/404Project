#include "linkedlist.h"

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

node *cria()//cria a lista
{
    node *novo= (node *) malloc(sizeof(node));
    novo->prox=NULL;
    return novo;
}
void retira(node *LISTA, int posicao)//remove nó da lista na posição desejada, começando da posição 1
{
    node *atual = LISTA->prox,
          *anterior=LISTA;

    while(atual->info.prioridade < posicao && atual->info.concluido==0)
    {
        anterior=atual;
        atual=atual->prox;
    }

    anterior->prox=atual->prox;
}
void visualiza(node *LISTA)//imprime a lista para visualização
{
    node *aux;
    for(aux=LISTA->prox; aux->prox!=NULL; aux=aux->prox)
    {
        printf("%s, ",aux->info.id);
    }
     printf("%s.\n",aux->info.id);
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
int vazia(node *LISTA)//verifica se a lista está vazia
{
    if(LISTA->prox == NULL)
        return 1;
    else
        return 0;
}

void insere(node *LISTA, Rotulo e, int pos)//insere novo nó na lista na posicao desejada
{
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
void insere_final(node *LISTA, Rotulo e)// Insere novo nó no final da lista
{
    node *novo=(node *) malloc(sizeof(node));
    novo->info=e;
    if(LISTA->prox==NULL||LISTA->prox->info.prioridade < novo->info.prioridade)
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
    else
    {
        node *atual = LISTA->prox,
              *anterior=LISTA;
        while(atual->info.prioridade >= novo->info.prioridade&&atual!=NULL)
        {
            anterior=atual;
            if(atual->prox!=NULL) atual=atual->prox;
            else break;
        }
        if(atual->prox==NULL&&atual->info.prioridade >= novo->info.prioridade)
        {
            atual->prox=novo;
            novo->prox=NULL;
        }
        else
        {
            anterior->prox=novo;
            novo->prox = atual;
        }
    }
}
/*NÃO SEI O QUE FAZER COM ISSO...
linkedlist::linkedlist()
{
}
*/
