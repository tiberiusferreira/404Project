
void retiraComentario(FILE *arq, FILE *out){
    char c;
    while(!feof(arq)){
        fscanf(arq, "%c", &c );
        if(c=='#'){
            while(c!='\n'){
                fscanf(arq, "%c", &c );
            }
            //is out of if, it means it found a \n put it in the file
            fprintf(out,"%c",c);
        }
        else{
            fprintf(out,"%c",c);
        }
    }
}





