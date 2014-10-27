#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dealwithfile.h"
#include "linkedlist.h"



int main(int argc, char* argv[]){
    FILE *in=fopen(argv[1],"r"); //opens input file passed as first arg
    FILE *out=fopen(argv[2],"w+"); //open output file passed as arg
    char *file_contents=NULL;
    int *size_contents=NULL; //num_lines stores the number of lines, num_chars is a vector and stores the number
    if(in==NULL) { //of chars in the line which is the index of the vector
        printf("Entrada nao encontrada\n");
        return 1;
    }
    if(out==NULL) {
        printf("Arquivo para saida nao encontrado");
        return 1;
    }
    file_contents=fileToVector(in,&size_contents); //turn read file into a vector of chars

    removeComents(file_contents,size_contents); //remove comments of code
    expand_dot_set(&file_contents,size_contents);
    //printf("%s \n %d",file_contents,*size_contents);
    //printf("%s",convert_word_to_instruction(file_contents,*size_contents));
    fprintf(out,"%s",convert_word_to_instruction(file_contents,*size_contents));
    fclose(in); //closes input file
    fclose(out); //closes output file

//    char temp[10];
//    int_to_hexchar(10,temp);
//    printf("%s",temp);
//      printf("%d",hexchar_to_int("0xAa"));

    return 0;
}
