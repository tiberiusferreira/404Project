#include "dealwithfile.h"
void removeComents(char *file_contents,int *size){
    char c;
    int local_size = *size,i;
    for(i=0;i<local_size;i++){
        c=file_contents[i];
        if(c=='#'){
            for(;i<local_size && file_contents[i]!='\n';i++){
                file_contents[i]=' ';
            }
        }
    }
}
void compileProgram(char *file_contents,int *size){
    char c;
    char current_word[101];
    int current_source_line=1, current_source_line_word=0;
    int current_hex_line=0,size_current_word=0;
    int local_size = *size,i,i_cur_word;
    for(i=0;i<local_size;i++){ //go through all file
        c=file_contents[i]; //get each char
        for(;file_contents[i]==' ' && i<local_size;i++){ //jump white spaces
            continue;
        }
        if(file_contents[i]=='\n'){
            current_source_line++;
            continue;
        }
        //at this point we expect to be file_contents[i] to point to the beginning of a word
        for(i_cur_word=0;;i_cur_word++,i++){
            if(file_contents[i]==' ' || file_contents[i]=='\n' || i==local_size){
                current_word[i_cur_word]='\0';
                break;
            }
            current_word[i_cur_word]=file_contents[i];
            current_source_line_word++;
        }
        size_current_word=i_cur_word;
        printf("Tratando : %s\n",current_word);
        if(!strcmp(current_word,".word")){
            printf("E word!\n");
        }
    }
}

char *fileToVector(FILE *source,int **size_contents){
    char *file_contents;
    long source_size;
    int i;
    *size_contents = (int*) malloc (sizeof(int));
    fseek(source, 0, SEEK_END); //go to end
    source_size = ftell(source); // since it is at the end, the ftell in reality stores in source_size the size of the file given
    rewind(source); //go to beginning of file again
    **size_contents = source_size;
    file_contents = malloc(source_size * (sizeof(char))); //file contents stores the whole file
    for(i=0;i<source_size;i++){
        fscanf(source,"%c", file_contents+i);
    }
    return file_contents;
}





