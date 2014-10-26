#include "linkedlist.h"

node *cria()//cria a lista
{
    node *novo= (node *) malloc(sizeof(node));
    novo->prox=NULL;
    return novo;
}

void visualiza(node *LISTA) //imprime a lista para visualização
{
    node *aux;
    for(aux=LISTA->prox; aux->prox!=NULL; aux=aux->prox)
    {
        printf("%s, address %d ",aux->info.nome,aux->info.endereco);
    }
    printf("%s, address %d\n",aux->info.nome,aux->info.endereco);
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

void insere(node *LISTA, rotulo e, int pos)//insere novo nó na lista na posicao desejada
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
void insere_final(node *LISTA, rotulo e)// Insere novo no no final da lista
{
    node *novo=cria();
    novo->info=e;
    for(;LISTA!=NULL;LISTA=LISTA->prox){
        if(LISTA->prox==NULL)
        {
            LISTA->prox = novo;
            return;
        }
    }
}
void retira(node *LISTA, char *wanted)
{
    node *atual = LISTA->prox,
          *anterior=LISTA;
    if(atual==NULL) return;
    while(atual->info.nome!=wanted)
    {
        anterior=atual;
        atual=atual->prox;
        if(atual==NULL) return;
    }

    anterior->prox=atual->prox;
}
rotulo busca(node *LISTA, char *wanted)
{
    rotulo vazio=newItem("vazio",-1,-1);
    if(LISTA==NULL){
        return vazio;
    }
    node *atual = LISTA->prox,*anterior=LISTA;
    if(atual==NULL) return vazio;
    while(strcasecmp(wanted,atual->info.nome))
    {
        anterior=atual;
        atual=atual->prox;
        if(atual==NULL) return vazio;
    }

    return atual->info;
}
rotulo newItem(char *name, int adress, int direction){
    rotulo novo;
    novo.nome=name;
    novo.endereco=adress;
    novo.aponta_dir=direction;
    return novo;
}
