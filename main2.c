#include <stdio.h>
#include <stdlib.h>
#include<string.h>

void retiraComentario(FILE *arq, FILE *out){
    char c;
    int flag=0;
    int comentario=0;
    while(!feof(arq)){
        fscanf(arq, "%c", &c );
        if(c=='#'){
           comentario=1;
        }
        //if(c==' '||c==':'||c=='\t'||c==','||c=='.'||c==';'||c=='('||c==')'||c=='['||c==']'||c=='*'||c=='?'||c=='!'||c=='-'||c==34/* caracter " */||c=='/'||c=='"'){
          //  if(flag==0)
            //    fprintf(out,"%c",'\n');
          //  flag=1;
        //}
        else{
            if(comentario==0){
                fprintf(out,"%c",c);
                flag=0;
            }
            if(c=='\n'){
                comentario=0;
                if(flag==1) {
                fprintf(out,"%c",'\n');
                flag=0;
                }
            }
        }
    }
    return;
}

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
    return 0;

}
