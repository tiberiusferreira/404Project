#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dealwithfile.h"
#include "linkedlist.h"



int main(int argc, char* argv[]){
    FILE *in=fopen(argv[1],"r"); //abre o arquivo de entrada passado pela como arg
    FILE *out=fopen(argv[2],"w+"); //abre o arquivo de saida passado como segundo arg
    char *file_contents;
    int *size_contents=NULL; //num_lines stores the number of lines, num_chars is a vector and stores the number
    if(in==NULL) { //of chars in the line which is the index of the vector
        printf("Entrada nao encontrada\n");
        return 1;
    }
    if(out==NULL) {
        printf("Arquivo para saida nao encontrado");
        return 1;
    }
    //retiraComentario(in,out);
    //file_matriz=fileToMatriz(in,&num_lines,&num_chars);
    file_contents=fileToVector(in,&size_contents);
    fclose(in);
    fclose(out);
    removeComents(file_contents,size_contents);
    convert_word_to_instruction(file_contents,*size_contents);

//    char temp[10];
//    int_to_hexchar(10,temp);
//    printf("%s",temp);

    return 0;
}
