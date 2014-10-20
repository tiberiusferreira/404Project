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
    my_rot.comentario=1;
    my_rot.endereco=3;
    my_rot.nome="My_name";

    my_rot2.comentario=1;
    my_rot2.endereco=4;
    my_rot2.nome="My_name2";

    my_rot3.comentario=1;
    my_rot3.endereco=5;
    my_rot3.nome="My_name3";
    insere_final(my_node,my_rot);
    insere_final(my_node,my_rot2);
    insere_final(my_node,my_rot3);
    visualiza(my_node); //not working!!
    return 0;
}
