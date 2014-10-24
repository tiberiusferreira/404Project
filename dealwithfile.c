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
    int i,line=0,num_chars=0,i2,i3;
    char temp[6];
    for(i=0;i<18414;){
        num_chars=sprintf(temp,"%x",line);
        if(num_chars<3){

            for(i3=num_chars;i3!=3;){
                hex_file[i]='0';
                i++;
                i3++;
            }

        }
        for(i2=0;i2<num_chars;){
            hex_file[i]=temp[i2];
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
int hexchar_to_int(char *hex){
    int number= (int) strtol(hex,NULL,0);
    return number;
}

char *int_to_hexchar(int number, char *destiny){
    destiny[0]='0';
    destiny[1]='x';
    sprintf(destiny+2,"%x",number);
    return destiny;
}

void write_to_hex(char *hex_file, char *memory_address_to_write, char *what_to_write,int write_to_dir){
    char c;
    int i_cur_word=0,i,current_source_line_word=0,current_source_line=0;
    int target_line=hexchar_to_int(memory_address_to_write);
    //printf("%s",hex_file);
        if(target_line!=current_source_line){ //search for desired line
        for(i=0;i<18414;(i)++){ //go until end of file
            if(hex_file[i]=='\n'){ //if finds a line break increase source code line counter
                (current_source_line)++;
                if(target_line==current_source_line){
                    i++; //end for pointing to first char of desired line
                    break;
                }
            }
            continue;
        }
        }
        //at this point we expect to point to the first char of desired line
        //if we are to write to left write_to_dir=0, if we are to write to right he is 1, if -1 we write to whole line
        if(write_to_dir==-1){//TODO implement rest!
            i=i+4; //AAA DD DDD DD DDD is the format there, we were pointing to the first A, now to first D
            hex_file[i]=what_to_write[0];
            hex_file[i+1]=what_to_write[1];

            hex_file[i+2]=' ';

            hex_file[i+3]=what_to_write[2];
            hex_file[i+4]=what_to_write[3];
            hex_file[i+5]=what_to_write[4];

            hex_file[i+6]=' ';

            hex_file[i+7]=what_to_write[5];
            hex_file[i+8]=what_to_write[6];

            hex_file[i+9]=' ';


            hex_file[i+10]=what_to_write[7];
            hex_file[i+11]=what_to_write[8];
            hex_file[i+12]=what_to_write[9];

        }
    }

void convert_word_to_instruction(char *file_contents, int size_file_contents){
    char current_word[101], hex_file[18420];
    int current_source_line, current_source_line_word;
    current_source_line_word=0;
    current_source_line=1;
    int current_hex_line=0,current_hex_pos=0,size_current_word; //current_hex_pos -1 = esq, current_hex_dir = 1
    size_current_word=0;
    int i,i_cur_word;
    initialize_hex(hex_file);
    getNextWord(current_word,&current_source_line,&current_source_line_word,&size_current_word,&i,file_contents,size_file_contents);
    printf("Tratando : %s , linha source = %d , %d palavra da linhas\n",current_word, current_source_line,current_source_line_word);
    if(!strcasecmp(current_word,".word")){
        printf("Got word!\n");
        //write_to_hex(hex_file,);
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





