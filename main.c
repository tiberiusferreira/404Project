#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "removecomments.h"
#include "linkedlist.h"



int main(int argc, char* argv[]){
    FILE *in=fopen(argv[1],"r");
    if(in==NULL) {
        printf("Entrada nao encontrada\n");
        return 1;
    }
    FILE *out=fopen(argv[2],"w+");
    retiraComentario(in,out);
    fclose(in);
    fclose(out);
    node *my_node;
    my_node=cria();
    rotulo my_rot,my_rot2,my_rot3;
    my_rot=newItem("My_name",3,1);
    my_rot2=newItem("My_name2",4,1);
    my_rot3=newItem("My_name3",5,1);

    insere_final(my_node,my_rot);
    insere_final(my_node,my_rot2);
    insere_final(my_node,my_rot3);
    retira(my_node,"cC");

    printf(">>%d\n",busca(my_node,"My_name2").endereco);
    printf(">>%d\n",busca(my_node,"My_name").endereco);

    printf(">>%d\n",busca(my_node,"My_name3").endereco);
        printf(">>%d\n",busca(my_node,"My_nad").endereco);

    visualiza(my_node); //not working!!
    return 0;
}
