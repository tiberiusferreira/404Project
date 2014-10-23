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
void compileProgram(char *file_contents,int *size){ //receives file contents and size of it
    char c;
    char current_word[101], hex_file[100000]; //TODO get right hex file max size
    int current_source_line=1, current_source_line_word=0;
    int current_hex_pos=0,size_current_word=0;
    int local_size = *size,i,i_cur_word;
    initialize_hex(hex_file);
    for(i=0;i<local_size;){ //go through all file
        c=file_contents[i]; //get each char from file
        for(;(file_contents[i]==' ' || file_contents[i]=='\n' )&& i<local_size;i++){ //jump white spaces and line breaks
            if(file_contents[i]=='\n'){ //if finds a line break increase source code line counter
                current_source_line_word=0;
                current_source_line++;
            }
            continue;
        }
        //at this point we expect file_contents[i] to point to the beginning of a word
        for(i_cur_word=0;;i_cur_word++,i++){
            c=file_contents[i]; //get each char from file
            if(file_contents[i]==' ' || file_contents[i]=='\n' || i==local_size){//word ended if found whitespace or line break or end of file
                current_word[i_cur_word]='\0';
                current_source_line_word++;
                break;
            }
            current_word[i_cur_word]=file_contents[i];
        }
        size_current_word=i_cur_word;
        //printf("Tratando : %s , linha source = %d , %d palavra da linhas\n",current_word, current_source_line,current_source_line_word);
        //convert_word_to_instruction(current_word,size_current_word,current_source_line_word,hex_file,current_hex_pos);
//        if(!strcmp(current_word,".word")){
//            printf("E word!\n");
//        }
    }
}

void initialize_hex(char *hex_file){
    //17 chars
    int i,line=0, line_as_hex=0,num_chars=0,i2;
    //printf("%ld\n",strtol(,NULL,16));
    //hex_file[i]=strtol(line,NULL,16);
    char temp[100];
    for(i=0;i<17407;){
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
    printf("%s",hex_file);
}

void convert_word_to_instruction(char *word, int word_size,int word_pos_in_line, char *hex_file, int hex_pos){
    printf("Received word : %s, size %d, word_pos_in_line %d\n",word,word_size,word_pos_in_line);

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





