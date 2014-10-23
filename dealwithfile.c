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
void getNextWord(char *current_word, int *current_source_line,int *current_source_line_word, int *size_current_word, int *i, char *file_contents, int size_file_contents){ //receives file contents and size of it
    char c;
    int i_cur_word;
    //printf("%s",hex_file);
    for(;*i<size_file_contents;){ //go through all file
        c=file_contents[*i]; //get each char from file
        for(;(file_contents[*i]==' ' || file_contents[*i]=='\n' )&& *i<size_file_contents;(*i)++){ //jump white spaces and line breaks
            if(file_contents[*i]=='\n'){ //if finds a line break increase source code line counter
                (*current_source_line_word)=0;
                (*current_source_line)++;
            }
            continue;
        }
        //at this point we expect file_contents[i] to point to the beginning of a word
        for(i_cur_word=0;;i_cur_word++,(*i)++){
            c=file_contents[*i]; //get each char from file
            if(file_contents[*i]==' ' || file_contents[*i]=='\n' || *i==size_file_contents){//word ended if found whitespace or line break or end of file
                current_word[i_cur_word]='\0';
                (*current_source_line_word)++;
                break;
            }
            current_word[i_cur_word]=file_contents[*i];
        }
        *size_current_word=i_cur_word;
        return;
    }
}

void initialize_hex(char *hex_file){
    //18 chars per line = 17 + \n
    int i,line=0,num_chars=0,i2;
    char temp[6];
    for(i=0;i<18414;){
        num_chars=sprintf(temp,"%x",line);
        for(i2=0;i2<num_chars;i2++){
            hex_file[i+i2]=temp[i2];
        }
        i=i+i2;
        while(i2<3){
            hex_file[i]='0';
            i++;
            i2++;
        }
        hex_file[i]=' ';
        i++;
        hex_file[i]='L';
        i++;
        hex_file[i]='L';
        i++;
        hex_file[i]=' ';
        i++;
        hex_file[i]='L';
        i++;
        hex_file[i]='L';
        i++;
        hex_file[i]='L';
        i++;
        hex_file[i]=' ';
        i++;
        hex_file[i]='L';
        i++;
        hex_file[i]='L';
        i++;
        hex_file[i]=' ';
        i++;
        hex_file[i]='L';
        i++;
        hex_file[i]='L';
        i++;
        hex_file[i]='L';
        i++;
        hex_file[i]='\n';
        line++;
        i++;
    }
}


void convert_word_to_instruction(char *file_contents, int size_file_contents){
    char current_word[101], hex_file[18420];
    int current_source_line, current_source_line_word;
    current_source_line_word=0;
    current_source_line=1;
    int current_hex_pos=0,size_current_word;
    size_current_word=0;
    int i,i_cur_word;
    initialize_hex(hex_file);
    getNextWord(current_word,&current_source_line,&current_source_line_word,&size_current_word,&i,file_contents,size_file_contents);
    printf("Tratando : %s , linha source = %d , %d palavra da linhas\n",current_word, current_source_line,current_source_line_word);
    getNextWord(current_word,&current_source_line,&current_source_line_word,&size_current_word,&i,file_contents,size_file_contents);
    printf("Tratando : %s , linha source = %d , %d palavra da linhas\n",current_word, current_source_line,current_source_line_word);
    getNextWord(current_word,&current_source_line,&current_source_line_word,&size_current_word,&i,file_contents,size_file_contents);
    printf("Tratando : %s , linha source = %d , %d palavra da linhas\n",current_word, current_source_line,current_source_line_word);
    getNextWord(current_word,&current_source_line,&current_source_line_word,&size_current_word,&i,file_contents,size_file_contents);
    printf("Tratando : %s , linha source = %d , %d palavra da linhas\n",current_word, current_source_line,current_source_line_word);



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





