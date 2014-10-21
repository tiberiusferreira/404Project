#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "removecomments.h"
#include "linkedlist.h"


/*-------------------
FUNÇÃO AUXILIAR ÚTIL
----------------------*/

rotulo newItem(char *name, int adress, int comment){
    rotulo novo;
    novo.nome=name;
    novo.endereco=adress;
    novo.comentario=comment;
    return novo;
}
/*---------------------*/
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
    my_rot=newItem("My_name",1,3);
    my_rot=newItem("My_name2",1,4);
    my_rot=newItem("My_name3",1,5);
    insere_final(my_node,my_rot);
    insere_final(my_node,my_rot2);
    insere_final(my_node,my_rot3);
    retira(my_node,"cC");


    visualiza(my_node); //not working!!
    return 0;
}
