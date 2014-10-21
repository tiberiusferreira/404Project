#include <stdio.h>
#include <stdlib.h>
char *Get_next_word(FILE *arq,int *pos){
    char* str= malloc(500*sizeof(char));
    char c;
    int flag=0, i=0;
    for(i=0;i<*pos;i++)    fscanf(arq,"%c",&c);
    if(arq==NULL) return "ERRO: END_OF_FILE";
    fscanf(arq,"%c",&c);
    *pos++;
    while(c==" " ||c=="\n") {
            fscanf(arq,"%c",&c);
            *pos++;
    }
    while(c!=" "&&c!="\n"&&c!=10&&c!=32){
           // printf("%c %d\n",c,c);
            str[i]=c;
            i++;
            fscanf(arq,"%c",&c);
            *pos++;
    }
    str[i]='\0';
    return str;

}

int main(){
    FILE *in=fopen("entrada.txt","r");
    int n=0;
    while(!feof(in)){
        printf("%s\n",Get_next_word(in,&n));
       // system("pause");
    }
    return 1;
}
