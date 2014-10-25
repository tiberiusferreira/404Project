#include "dealwithfile.h"
void removeComents(char *file_contents,int *size){
    char c;
    int local_size = *size;
    int i;
    for(i=0;i<local_size;i++){ //go through all vector
        c=file_contents[i];
        if(c=='#'){ //if finds # put space in it until line break
            for(;i<local_size && file_contents[i]!='\n';i++){
                file_contents[i]=' ';
            }
        }
    }
}
int getNextWord(word *currentword,char *file_contents, int size_file_contents){ //receives file contents and size of it
    char *current_word=currentword->current_word;
    int *current_source_line=&(currentword->current_source_line);
    int *current_source_line_word=&(currentword->current_word_location_in_line);
    int *size_current_word=&(currentword->size_current_word),*i=&(currentword->i);
    //returns 0 if got new word or -1 if did not
    char c;
    int i_cur_word;
    for(;(*i)<size_file_contents;){ //go through all file
        c=file_contents[*i]; //get each char from file
        for(; (file_contents[*i]==' ' || file_contents[*i]=='\n') && (*i)<size_file_contents; (*i)++){ //jump white spaces and line breaks
            if(file_contents[*i]=='\n'){ //if finds a line break increase source code line counter
                (*current_source_line_word)=0;
                (*current_source_line)++;
            }
            continue;
        }
        //at this point we expect file_contents[i] to point to the beginning of a word
        for(i_cur_word=0; ;i_cur_word++,(*i)++){
            c=file_contents[*i]; //get each char from file
            if(file_contents[*i]==' ' || file_contents[*i]=='\n' || *i==size_file_contents){//word ended if found whitespace or line break or end of file
                if(*i==size_file_contents){
                    return -1;
                }
                current_word[i_cur_word]='\0';
                (*current_source_line_word)++;
                break;
            }
            current_word[i_cur_word]=file_contents[*i];
        }
        *size_current_word=i_cur_word;
        return 0;
    }
    return -1;
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
    hex_file[18414]='\0';
}

long long hexchar_to_longlong(char *hex){
    long long number = (long long) strtoll(hex,NULL,0);
    return number;
}

char *longlong_to_hexchar_with0x(long long number, char *destiny){
    destiny[0]='0';
    destiny[1]='x';
    sprintf(destiny+2,"%llx",number);
    return destiny;
}

char *longlong_to_hexchar_without0x(long long number, char *destiny){
    sprintf(destiny,"%llx",number);
    return destiny;
}


char *create_instruction(char *codigo, char *complemento, char* instruction){
    printf("%s %s",codigo, complemento);
    instruction[0]=codigo[0];
    instruction[1]=codigo[1];
    instruction[2]=complemento[0];
    instruction[3]=complemento[1];
    instruction[4]=complemento[2];
    instruction[5]='\0';
    printf("==== %s<<",instruction);
    return instruction;
}

int is_hexa(char *hex){
    if(hex[0]=='0' && hex[1]=='x'){
        return 1;
    }
    return 0;
}

char *remove_0x(char *hex){
    int i;
    for(i=2;;i++){
        if(hex[i]=='\0'){
            hex[i]=hex[i-2];
            return hex;
        }
        hex[i]=hex[i-2];
    }
}



void write_to_hex(char *hex_file, char *memory_address_to_write, char *what_to_write,int write_to_dir){
    int i=0,current_source_line=0,aux=0,amount_to_move=0;
    int target_line=hexchar_to_longlong(memory_address_to_write);
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

    //make sure what_to_write is valid
    for(aux=0;1;){
        if(what_to_write[aux]=='\0' && aux!=10){
            amount_to_move++;
        }
        if(aux==10){
            break;
        }
        aux++;
    }
    for(aux=0;aux<amount_to_move;aux++){
        what_to_write[aux+amount_to_move]=what_to_write[aux];
    }
    for(aux=0;aux<amount_to_move;aux++){
        what_to_write[aux]='0';
    }
    if(write_to_dir==-1){
        if(i+12>18414){ // wanna write beyond hex file size, do not do it
            return;
        }
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
    if(write_to_dir==0){
        if(i+12>18414){
            return;
        }
        i=i+4; //AAA DD DDD DD DDD is the format there, we were pointing to the first A, now to first D
        hex_file[i]=what_to_write[0];
        hex_file[i+1]=what_to_write[1];

        hex_file[i+2]=' ';

        hex_file[i+3]=what_to_write[2];
        hex_file[i+4]=what_to_write[3];
        hex_file[i+5]=what_to_write[4];
    }
    if(write_to_dir==1){
        if(i+12>18414){
            return;
        }
        i=i+4; //AAA DD DDD DD DDD is the format there, we were pointing to the first A, now to first D
        hex_file[i+7]=what_to_write[0];
        hex_file[i+8]=what_to_write[1];

        hex_file[i+9]=' ';


        hex_file[i+10]=what_to_write[2];
        hex_file[i+11]=what_to_write[3];
        hex_file[i+12]=what_to_write[4];
    }
}

void expand_dot_set(char *file_contents, int size_file_contents){
    word word_in_original,word_in_new,word_see_next,word_see_next_next,word_temp;
    word_in_original.current_source_line=1;
    word_in_original.current_word_location_in_line=0;
    word_in_original.i=0;
    word_in_original.size_current_word=0;
    word_in_new.current_source_line=1;
    word_in_new.current_word_location_in_line=0;
    word_in_new.i=0;
    word_in_new.size_current_word=0;
    char text_to_be_changed[101],what_to_change_to[101]; //max word is a label of 100 chars, so 101 is max
    char *new_file_contents;
    int previous_source_line=1; //current_hex_pos -1 = esq, current_hex_dir = 1
    int i=0,i2=0,i3=0,aux,number_words_in_file=0;
    while(-1!=getNextWord(&word_in_original,file_contents,size_file_contents)){
           number_words_in_file++;
    }
    word_in_original=word_in_new; //go back to beginning
    new_file_contents = malloc( (100*number_words_in_file+1) * (sizeof(char))); //each word can be up to 100 chars (+1 to store \0)
    for(i3=0;-1!=getNextWord(&word_in_original,file_contents,size_file_contents);){
        if(previous_source_line<word_in_original.current_source_line){ //if the original file jumped a line, jump too
            new_file_contents[i3]='\n';
            i3++;
        }
        previous_source_line=word_in_original.current_source_line;

        for(aux=0;;aux++,i3++){
            if(word_in_original.current_word[aux]=='\0'){
                for(;aux<100;aux++,i3++){
                    new_file_contents[i3]=' ';
                }
                break;
            }
            new_file_contents[i3]=word_in_original.current_word[aux];
        }

    }







//    new_file_contents = malloc( (3*size_file_contents+1) * (sizeof(char))); //file contents stores the whole file (+1 to store \0)
//    while(-1!=getNextWord(&word_in_original,file_contents,size_file_contents)){
//        printf("Tratando : %s , linha source = %d , %d palavra da linhas\n",word_in_original.current_word, word_in_original.current_source_line,word_in_original.current_word_location_in_line);

//        //--.set//
//        if(!strcasecmp(word_in_original.current_word,".set")){ //acts as #define
//            printf("Got .set!\n"); //get what appears in the code and should be replaced
//            word_temp=word_in_original;
//            getNextWord(&word_in_original,file_contents,size_file_contents);
//            //now current_word has what needs to be replaced in the text, lets store it
//            strcpy(text_to_be_changed,word_in_original.current_word);
//            //now lets store what to change to
//            getNextWord(&word_in_original,file_contents,size_file_contents);
//            strcpy(what_to_change_to,word_in_original.current_word);
//            //now go all text replacing it
//            //go to beginning of text
//            i2=0;
//            //get each word from the text and store in a new array
//            while(-1!=getNextWord(&word_in_new,file_contents,size_file_contents)){
//                if(previous_source_line<word_in_new.current_source_line){ //if the original file jumped a line, jump too
//                    new_file_contents[i3]='\n';
//                    i3++;
//                }
//                previous_source_line=word_in_new.current_source_line;
//                if(!strcasecmp(word_in_new.current_word,".set")){ //if found a .set check if it is not the .set we are executing
//                    //check arguments
//                    word_see_next=word_in_new;
//                    getNextWord(&word_see_next,file_contents,size_file_contents);
//                    if(!strcasecmp(word_see_next.current_word,text_to_be_changed)){
//                        //if here first arg is equal, check other one
//                        getNextWord(&word_see_next,file_contents,size_file_contents);
//                        if(!strcasecmp(word_see_next.current_word,what_to_change_to)){
//                            //same set, skip it
//                            getNextWord(&word_in_new,file_contents,size_file_contents);
//                            getNextWord(&word_in_new,file_contents,size_file_contents);
//                            continue;
//                        }
//                    }

//                }

//                if(!strcasecmp(word_in_new.current_word,text_to_be_changed)){
//                    //if found the word it needs to change, copy the new word to the file
//                    for(aux=0;;aux++,i3++){
//                        if(what_to_change_to[aux]=='\0'){
//                            new_file_contents[i3]=' ';
//                            i3++;
//                            break;
//                        }
//                        new_file_contents[i3]=what_to_change_to[aux];
//                    }
//                    continue; //continue so it doesnt copy the original content too
//                }
//                //if here, not found text to be changed, simply copy it to new file contents
//                for(aux=0;;aux++,i3++){
//                    if(word_in_new.current_word[aux]=='\0'){
//                        new_file_contents[i3]=' ';
//                        i3++;
//                        break;
//                    }
//                    new_file_contents[i3]=word_in_new.current_word[aux];
//                }
//            }
//            new_file_contents[i3]='\0';
//        }
//        //--.set//
//    }
    printf("%s",new_file_contents);
}

void convert_word_to_instruction(char *file_contents, int size_file_contents){
    //    char current_word[101], hex_file[18420],temp[12],current_line_as_hex[5], instruction[6]; //max word is a label of 100 chars, so 101 is max
    //    int current_source_line=1, current_word_location_in_line=0; //temp stores something to be written to IAS so 12 is enough
    //    int current_hex_line=0,current_hex_pos=0,size_current_word=0; //current_hex_pos -1 = esq, current_hex_dir = 1
    //    int i=0;
    //    long long temp_longlong, temp_longlongaux;
    //    initialize_hex(hex_file);
    //    while(-1!=getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents)){
    //        printf("Tratando : %s , linha source = %d , %d palavra da linhas\n",current_word, current_source_line,current_word_location_in_line);

    //        //--.align//
    //        if(!strcasecmp(current_word,".align")){ //goes to line which is multiple of given number
    //            printf("Got .align!\n");
    //            getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents);
    //            if(is_hexa(current_word)){ //if word was given in hexa, turn to long long
    //                temp_longlong=hexchar_to_longlong(current_word);
    //            }else{ //if given in decimal base, convert string to number
    //                temp_longlong=strtoll(current_word,NULL,10); //if int, convert string to long long
    //            }
    //            while(current_hex_line%temp_longlong!=0){ //go to line which is multiple of given number
    //                current_hex_line++;
    //            }
    //        }
    //        //--.align//

    //        //--.word//
    //        if(!strcasecmp(current_word,".word")){ //puts a data in the next memory location which supports it
    //            printf("Got .word!\n");
    //            getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents);
    //            if(is_hexa(current_word)){// current_word now has the data to be inserted ALL data in the IAS has to be in HEXA
    //                temp_longlong=hexchar_to_longlong(current_word); //so we need to convert to HEX if necessary
    //            }else{
    //                temp_longlong=strtoll(current_word,NULL,10);
    //            }
    //            longlong_to_hexchar_with0x(current_hex_line,current_line_as_hex);
    //            write_to_hex(hex_file,current_line_as_hex,current_word,-1);
    //            current_hex_line++;
    //        }
    //        //--.word//

    //        //--.org//
    //        if(!strcasecmp(current_word,".org")){
    //            printf("Got .org!\n");
    //            getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents);
    //            if(is_hexa(current_word)){// turn to long long
    //                temp_longlong=hexchar_to_longlong(current_word);
    //            }else{
    //                temp_longlong=strtoll(current_word,NULL,10); //turn to long long
    //            }
    //            current_hex_line=temp_longlong;
    //        }
    //        //--.org//

    //        //--.wfill//
    //        if(!strcasecmp(current_word,".wfill")){
    //            printf("Got .wfill!\n");
    //            //next word should store how many lines are gonna be filled by the data
    //            getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents);
    //            if(is_hexa(current_word)){//if is hexa, turn to long long
    //                temp_longlong=hexchar_to_longlong(current_word);
    //            }else{
    //                temp_longlong=strtoll(current_word,NULL,10); // if decimal, convert to long long
    //            }
    //            //temp_longlong now stores how many lines are gonna be filled by data
    //            //now lets store it in a safe variable and call the .word code the amount of time with the data given
    //            temp_longlongaux=temp_longlong;
    //            getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents);
    //            if(is_hexa(current_word)){// current_word now has the data to be inserted ALL data in the IAS has to be in HEXA
    //                temp_longlong=hexchar_to_longlong(current_word); //so we need to convert to HEX if necessary
    //            }else{
    //                temp_longlong=strtoll(current_word,NULL,10);
    //            }
    //            for(;temp_longlongaux>0;temp_longlongaux--){
    //                //.word code
    //                longlong_to_hexchar_with0x(current_hex_line,current_line_as_hex);
    //                write_to_hex(hex_file,current_line_as_hex,current_word,-1);
    //                current_hex_line++;
    //                //.word code
    //            }
    //        }
    //        //--.wfill//


    //        if(!strcasecmp(current_word,"ADD")){
    //            printf("Got ADD!\n");
    //            getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents);
    //            create_instruction("05",current_word,instruction);
    //            printf(">>%s<<\n",instruction);
    //            longlong_to_hexchar_with0x(current_hex_line,temp);
    //            write_to_hex(hex_file,current_line_as_hex,instruction,0);
    //            current_hex_line++;
    //        }



    //    }
    //    printf("%s",hex_file);
}

char *fileToVector(FILE *source,int **size_contents){
    char *file_contents;
    long source_size;
    int i;
    *size_contents = (int*) malloc (sizeof(int)); //alocating the pointer received one int data space
    fseek(source, 0, SEEK_END); //go to end
    source_size = ftell(source); // since it is at the end, the ftell in reality stores in source_size the size of the file given
    rewind(source); //go to beginning of file again
    **size_contents = source_size; //setting the value
    file_contents = malloc( (source_size+1) * (sizeof(char))); //file contents stores the whole file (+1 to store \0)
    for(i=0;i<source_size;i++){
        fscanf(source,"%c", file_contents+i);
    }
    *(file_contents+source_size)='\0';
    return file_contents;
}





