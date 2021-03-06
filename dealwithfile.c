#include "dealwithfile.h"
void removeComents(char *file_contents,int *size)
{
    char c;
    int local_size = *size;
    int i;
    for(i=0; i<local_size; i++) //go through all vector
    {
        c=file_contents[i];
        if(c=='#')  //if finds # put space in it until line break
        {
            for(; i<local_size && file_contents[i]!='\n'; i++)
            {
                file_contents[i]=' ';
            } // ?? profit ! ??
        }
    }
}


int create_instruction(char *codigo, char *complemento, int tam_complemento, char* instruction, int type, node *labels)
{
    int i=0,j=0;
    char info[102],endereco[6];//info receive all parameters, endereco will receive the instruction's adress
    if(type==1)//instructions with this format: mnemonico M(0x000 || label)
    {
        if(complemento[i++]!='M') return 0;
        if(complemento[i]!='(') return 0;
        for(i=i+1; i<tam_complemento; i++)//set adress parameter
        {
            if(complemento[i]==')')
            {
                info[j]='\0';
                break;
            }
            info[j]=complemento[i];
            j++;
        }
    }
    else if(type==2)//instructions with this format: mnemonico M(0x000,20:39 || label)
    {
        char pos[6];
        strcpy(pos,"0");//set a defaut value
        int flag=0;//control when we are wornking with the adress or the position
        if(complemento[i++]!='M') return 0;
        if(complemento[i]!='(') return 0;
        for(i=i+1; i<tam_complemento; i++)//set the adress and the position
        {
            //  printf("%c ",complemento[i]);
            if(complemento[i]==',')
            {
                i++;
                info[j]='\0';
                j=0;
                flag=1;
            }
            else if(complemento[i]==')')
            {
                if(flag)pos[j]='\0';
                else info[j]='\0';
                break;
            }
            if(!flag) info[j]=complemento[i];
            else pos[j]=complemento[i];
            j++;
        }
        j=strlen(info);
        info[j]=':';
        info[j+1]='\0';
        i=get_label_by_name(info,labels).points_to_dir;//this actions are necessary to verify if the label gives left or right position or none information
        if(strcasecmp("JMP",codigo)==0)
        {
            if(strcasecmp("0:19",pos)==0||i==0)
            {
                instruction[0]='0';
                instruction[1]='D';
            }
            else if(strcasecmp("20:39",pos)==0||i==1)
            {
                instruction[0]='0';
                instruction[1]='E';
            }
            else return 0;
        }
        else if(strcasecmp("JGEZ",codigo)==0)
        {
            if(strcasecmp("0:19",pos)==0||i==0)
            {
                instruction[0]='0';
                instruction[1]='F';
            }
            else if(strcasecmp("20:39",pos)==0||i==1)
            {
                instruction[0]='1';
                instruction[1]='0';
            }
            else return 0;
        }
        else if(strcasecmp("STOR",codigo)==0)
        {
            if(strcasecmp("8:19",pos)==0||i==0)
            {
                instruction[0]='1';
                instruction[1]='2';
            }
            else if(strcasecmp("28:39",pos)==0||i==1)
            {
                instruction[0]='1';
                instruction[1]='3';
            }
            else return 0;
        }
    }
    else if(type==3)//instructions with this format: mnemonico NULL
    {
        strcpy(info,"0x000");
    }
    printf("label? %s\n",info);
    if(!is_hexa(info))//verify if its a valid hexadecimal adress
    {
        if(type!=2){
            j=strlen(info);
            info[j]=':';
            info[j+1]='\0';
        }
        strcpy(info,get_label_by_name(info,labels).points_to);
        if(!strcasecmp("ERR",info))return 0;//verify if parameter is a valid label
    }
    strcpy(endereco,info);
    //printf("endereco: %s",endereco);
    i=j=2;
    if(strlen(endereco)>4)
    {
        instruction[i]=endereco[j];
        j++;
    }
    i++;
    if(strlen(endereco)>3)
    {
        instruction[i]=endereco[j];
        j++;
    }
    i++;
    if(strlen(endereco)>2)
    {
        instruction[i]=endereco[j];
        j++;
    }
    instruction[5]='\0';
    return 1;
}

int getNextWord(word *currentword,char *file_contents, int size_file_contents)  //receives file contents and size of it
{ //changes currentword struct to have new values corresponding to next word in vector
    char *current_word=currentword->current_word;
    int *current_source_line=&(currentword->current_source_line);
    int *current_source_line_word=&(currentword->current_word_location_in_line);
    int *size_current_word=&(currentword->size_current_word),*i=&(currentword->i); //making sure any changes reflect outside
    //returns 0 if got new word or -1 if did not and 1 if got but is end of file (in this case, vector)
    char c;
    int i_cur_word;
    for(;(*i)<size_file_contents;) //go through all file
    {
        c=file_contents[*i]; //get each char from file
        for(; (file_contents[*i]==' ' || file_contents[*i]=='\n' || file_contents[*i]=='\0' || file_contents[*i]=='\t') && (*i)<size_file_contents; (*i)++)  //jump white spaces and line breaks
        {
            if(file_contents[*i]=='\n')  //if finds a line break increase source code line counter
            {
                (*current_source_line_word)=0;
                (*current_source_line)++;
            }
            continue;
        }
        //at this point we expect file_contents[i] to point to the beginning of a word
        for(i_cur_word=0; ; i_cur_word++,(*i)++)
        {
            c=file_contents[*i]; //get each char from file
            if(file_contents[*i]==' ' || file_contents[*i]=='\n' || *i==size_file_contents) //word ended if found whitespace or line break or end of file or tab
            {
                if((*i==size_file_contents) && (file_contents[*i]==' ' || file_contents[*i]=='\n' || file_contents[*i]=='\t' || ( (file_contents[*i]=='\0') && (i_cur_word==0) )))
                {
                    return -1;//end of file and last char was not a valid word char, was a space or tab o \0 etc

                }
                if(*i==size_file_contents)
                {
                    current_word[i_cur_word]='\0';
                    (*current_source_line_word)++;
                    return 0; //end of file, but got word since did not stop at if above
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

void initialize_hex(char *hex_file)
{
    //18 chars per line = 17 + \n
    int i,line=0,num_chars=0,i2,i3;
    char temp[6];
    for(i=0; i<18414;) //calculted this = 18*1023
    {
        num_chars=sprintf(temp,"%x",line); //temp = line number as hex, num_chars has how name chars the line as hex takes
        if(num_chars<3) //write line number as hex, take care of amount of digits and write accordingly
        {
            for(i3=num_chars; i3!=3;)
            {
                hex_file[i]='0';
                i++;
                i3++;
            }

        }
        for(i2=0; i2<num_chars;)
        {
            hex_file[i]=temp[i2];
            i++;
            i2++;
        }
        hex_file[i]=' '; //now write L as place holder but already in right format
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

long long hexchar_to_longlong(char *hex) //converts hex as char to long long
{
    long long number = (long long) strtoll(hex,NULL,0);
    return number;
}

char *longlong_to_hexchar_with0x(long long number, char *destiny)
{//converts long long to hex as char with 0x at beginning
    destiny[0]='0';
    destiny[1]='x';
    sprintf(destiny+2,"%llx",number);
    return destiny;
}

char *longlong_to_hexchar_without0x(long long number, char *destiny)
{//converts long long to hex as char without 0x at beginning
    sprintf(destiny,"%llx",number);
    return destiny;
}



int is_hexa(char *hex)
{//sees if string starts with 0x
    if(hex[0]=='0' && hex[1]=='x')
    {
        return 1;
    }
    return 0;
}
int is_valid_hexa(char *hex)
{
    int i;
    if(hex[0]=='0' && hex[1]=='x')
    {
        for(i=2;i<strlen(hex);i++){
                /*   IS 0-9?         */      /*  IS A-F    */             /* IS a-f      */
            if((hex[i]>=48&&hex[i]<=57)||(hex[i]>=65&&hex[i]<=70)||(hex[i]>=97&&hex[i]<=102)) continue;
            else return 0;
        }
        if(i>5) return 0;
        else return 1;
    }
    return 0;
}

int is_valid_small_hexa(char *number)
{
    int i;
    if(number[0]=='0' && number[1]=='x')
    {
        for(i=2;i<strlen(number);i++){
                /*   IS 0-9?         */      /*  IS A-F    */             /* IS a-f      */
            if((number[i]>=48&&number[i]<=57)||(number[i]>=65&&number[i]<=70)||(number[i]>=97&&number[i]<=102)) continue;
            else return 0;
        }
        if(i>12) return 0;
        else return 1;
    }
    else{
        for(i=0;i<strlen(number);i++){
                /*   IS 0-9?         */
            if(number[i]>=48&&number[i]<=57) continue;
            else return 0;
        }
        if(i>10) return 0;
        else return 1;
    }
    return 0;
}

char *remove_0x(char *hex)
{//removes 0x from string and moves it so it starts with previous first digit after 0x
    int i;
    for(i=2;; i++)
    {
        if(hex[i]=='\0')
        {
            hex[i]=hex[i-2];
            return hex;
        }
        hex[i]=hex[i-2];
    }
}



void write_to_hex(char *hex_file, char *memory_address_to_write, char *what_to_write,int write_to_dir)
{ //this function is supposed (lets take a minute to pray here) to write to hex file the what_to_write
    //write_to_dir says if to left(0) or dir(1) or whole(-1) memory_address_to_write tells which line to write to.
    int i=0,current_source_line=0,aux=0,amount_to_move=0,tam_what_to_write;
    int target_line=hexchar_to_longlong(memory_address_to_write);
    char what_to_write_full[11];
    strcpy(what_to_write_full,what_to_write);
    if(target_line!=current_source_line)  //search for desired line
    {
        for(i=0; i<18414; (i)++) //go until end of file
        {
            if(hex_file[i]=='\n')  //if finds a line break increase source code line counter
            {
                (current_source_line)++;
                if(target_line==current_source_line)
                {
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
    for(aux=0; 1;)
    {
        if(what_to_write_full[aux]=='\0')
        {
            tam_what_to_write=aux+1;//+1 cause it starts at 0
            amount_to_move=10-aux;
            break;
        }
        aux++;
    }
    for(aux=(tam_what_to_write-1); aux>=0; aux--)
    {
        what_to_write_full[aux+amount_to_move]=what_to_write_full[aux];//moves so all content is aligned to right
    }
    for(aux=0; aux<amount_to_move; aux++)
    {
        what_to_write_full[aux]='0';
    }
    if(write_to_dir==-1)
    {
        if(i+12>18414)  // wanna write beyond hex file size, do not do it
        {
            return;
        }
        i=i+4; //AAA DD DDD DD DDD is the format there, we were pointing to the first A, now to first D
        hex_file[i]=what_to_write_full[0];
        hex_file[i+1]=what_to_write_full[1];

        hex_file[i+2]=' ';

        hex_file[i+3]=what_to_write_full[2];
        hex_file[i+4]=what_to_write_full[3];
        hex_file[i+5]=what_to_write_full[4];

        hex_file[i+6]=' ';

        hex_file[i+7]=what_to_write_full[5];
        hex_file[i+8]=what_to_write_full[6];

        hex_file[i+9]=' ';


        hex_file[i+10]=what_to_write_full[7];
        hex_file[i+11]=what_to_write_full[8];
        hex_file[i+12]=what_to_write_full[9];

    }
    if(write_to_dir==0)
    {
        if(i+12>18414)
        {
            return;
        }
        i=i+4; //AAA DD DDD DD DDD is the format there, we were pointing to the first A, now to first D
        hex_file[i]=what_to_write_full[5];
        hex_file[i+1]=what_to_write_full[6];

        hex_file[i+2]=' ';

        hex_file[i+3]=what_to_write_full[7];
        hex_file[i+4]=what_to_write_full[8];
        hex_file[i+5]=what_to_write_full[9];
    }
    if(write_to_dir==1)
    {
        if(i+12>18414)
        {
            return;
        }
        i=i+4; //AAA DD DDD DD DDD is the format there, we were pointing to the first A, now to first D
        hex_file[i+7]=what_to_write_full[5];
        hex_file[i+8]=what_to_write_full[6];

        hex_file[i+9]=' ';


        hex_file[i+10]=what_to_write_full[7];
        hex_file[i+11]=what_to_write_full[8];
        hex_file[i+12]=what_to_write_full[9];
    }
}

void turn_text_word_in_100_chars(char **file_contents, int *size_file_contents)
{ //turns all words into 100 chars words and writes to new array
    //necessary because the .set can cause A to become AAAAAA and the array can not be expanded
    word word_in_original,word_pointing_to_origin;
    word_pointing_to_origin.current_source_line=1;
    word_pointing_to_origin.current_word_location_in_line=0;
    word_pointing_to_origin.i=0;
    word_pointing_to_origin.size_current_word=0;
    word_in_original=word_pointing_to_origin; //declares word which points to beginning of array of file contents
    int number_words_in_file=0,i,new_file_size=0,aux;
    int previous_source_line=1;
    char *new_file_contents,c;
    while(-1!=getNextWord(&word_in_original,(*file_contents),(*size_file_contents)))
    {
        number_words_in_file++;
    }
    word_in_original=word_pointing_to_origin; //go back to beginning, new = beginning at this moment
    new_file_contents = malloc( (100*number_words_in_file+1) * (sizeof(char))); //each word can be up to 100 chars (+1 to store \0)
    for(i=0; -1!=getNextWord(&word_in_original,(*file_contents),(*size_file_contents));)
    {
        //printf("Putting %s word in file with 100 chars.\n",word_in_original.current_word);
        if(previous_source_line<word_in_original.current_source_line)  //if the original file jumped a line, jump too
        {
            new_file_contents[i]='\n';
            i++;
        }
        previous_source_line=word_in_original.current_source_line;
        for(aux=0;; aux++,i++)
        {
            if(aux==101)  //at 99 it has 100 chars, at 100 it has 100 chars + \0 so if it gets in here with aux = 101 give error
            {
                printf(".set has a name argument which is bigger than 100 chars\n");
                return;
            }
            if(word_in_original.current_word[aux]=='\0')
            {
                for(; aux<100; aux++,i++)
                {
                    new_file_contents[i]=' ';
                }
                break;
            }
            c=new_file_contents[i];
            new_file_contents[i]=word_in_original.current_word[aux];
        }
    }
    new_file_size=i+1;
    new_file_contents[i]='\0';
    free(*file_contents);
    *file_contents=new_file_contents;
    *size_file_contents=new_file_size;
}
void change_word_A_to_word_B(char *text_to_be_changed, char *what_to_change_to,int what_to_change_to_size, char **file_contents, int *size_file_contents)
{
    //used to implement the .set, changes word A to B and searches for the .set who called it and erases it
    word word_in_original,word_pointing_to_origin,next_word,word2;
    word_pointing_to_origin.current_source_line=1;
    word_pointing_to_origin.current_word_location_in_line=0;
    word_pointing_to_origin.i=0;
    word_pointing_to_origin.size_current_word=0;
    word_in_original=word_pointing_to_origin;
    int i=0,i2,i3;
    for(; -1!=getNextWord(&word_in_original,*file_contents,(*size_file_contents));)
    {
        if(!strcasecmp(word_in_original.current_word,".set"))  //if found a .set check if it is not the .set we are executing
        {
            //check arguments
            next_word=word_in_original;
            if(getNextWord(&next_word,*file_contents,(*size_file_contents))==-1)
            {
                //if arg does not exist break
                break;
            }
            if(!strcasecmp(next_word.current_word,text_to_be_changed))
            {
                //if here first arg is equal, check other one
                if(getNextWord(&next_word,*file_contents,(*size_file_contents))==-1)
                {
                    break;
                }
                if(!strcasecmp(next_word.current_word,what_to_change_to))
                {
                    //same set erase it, erase the .set arg1 and arg2
                    for(i=0; i<3; i++)
                    {
                        for(i2=0; i2<word_in_original.size_current_word; i2++)
                        {
                            (*file_contents)[word_in_original.i-word_in_original.size_current_word+i2]=' ';
                        }
                        if(getNextWord(&word_in_original,*file_contents,(*size_file_contents))==-1)
                        {
                            break;
                        }
                    }
                }
            }
        }
        //if found a word, overwrite it with new word
        //printf("Comparing word %s to %s.\n",word_in_original.current_word,text_to_be_changed);
        for(word2=word_pointing_to_origin; -1!=getNextWord(&word2,*file_contents,(*size_file_contents));)
        {
            if(!strcasecmp(word2.current_word,text_to_be_changed))
            {
                printf("Changing following word in original text: %s For %s\n",word2.current_word,what_to_change_to);
                for(i2=0; i2<what_to_change_to_size; i2++)
                {
                    (*file_contents)[word2.i-word2.size_current_word+i2]=what_to_change_to[i2];
                }
                for(i3=0; i3<(word2.size_current_word-what_to_change_to_size); i2++,i3++)
                {
                    (*file_contents)[word2.i-word2.size_current_word+i2]=' ';
                }
            }
        }
    }
}

void shrink_file(char **file_contents, int *size_file_contents)
{ //eliminates excess of white spaces generated by turn_text_word_in_100_chars
    word word_in_original,word_pointing_to_origin;
    word_pointing_to_origin.current_source_line=1;
    word_pointing_to_origin.current_word_location_in_line=0;
    word_pointing_to_origin.i=0;
    word_pointing_to_origin.size_current_word=0;
    word_in_original=word_pointing_to_origin;
    char *new_file_contents;
    int i,i2,previous_source_line;
    new_file_contents = (char*) malloc(sizeof(char)*((*size_file_contents)+1));
    for(i=0; -1!=getNextWord(&word_in_original,(*file_contents),(*size_file_contents));)
    {
        if(previous_source_line<word_in_original.current_source_line)  //if the original file jumped a line, jump too
        {
            new_file_contents[i]='\n';
            i++;
        }
        previous_source_line=word_in_original.current_source_line;
        for(i2=0; i2<word_in_original.size_current_word; i2++,i++)
        {
            new_file_contents[i]=word_in_original.current_word[i2]; //copy contents for all word size
            if(i2+1==word_in_original.size_current_word)
            {
                i++;
                new_file_contents[i]=' ';
            }
        }
    }
    new_file_contents[i]='\0';
    free(*file_contents);
    (*file_contents)=new_file_contents;
    (*size_file_contents)=i;
}

void expand_dot_set(char **file_contents, int *size_file_contents)
{ //expand .set directive
    printf("Expanding .set directives...\nGot following file:\n%s\n",*file_contents);
    word word_in_original,word_pointing_to_origin;
    word_pointing_to_origin.current_source_line=1;
    word_pointing_to_origin.current_word_location_in_line=0;
    word_pointing_to_origin.i=0;
    word_pointing_to_origin.size_current_word=0;
    word_in_original=word_pointing_to_origin;
    char text_to_be_changed[101],what_to_change_to[101]; //max word is a label of 100 chars, so 101 is max to get \0 in
    char c; //gonna be the new file where all words take 101 chars
    int i=0,i2,i3,i4,what_to_change_to_size,current_line=0;
    //copy file contents to new file contents with each word having 100 chars//
    turn_text_word_in_100_chars(file_contents,size_file_contents);
    printf("After turning each word to 100 chars:\n%s\nEND\n",*file_contents);
    /*Now the ideia is to go in *file_contents and search for .set,
    once found, start searching while *file_contents executing the .set
    when it gets to the .set line which it is executing, delete it
    after it has gone all file, start looking from beginning for more .set
    because the file has changed and we cant know how it is now
    */
    while(-1!=getNextWord(&word_in_original,*file_contents,(*size_file_contents)))
    {
        //--.set//
        if(!strcasecmp(word_in_original.current_word,".set"))
        {
            current_line=word_in_original.current_source_line;
            printf("Got .set!\n"); //get what appears in the code and should be replaced
            if(getNextWord(&word_in_original,*file_contents,(*size_file_contents))==-1 || current_line!=word_in_original.current_source_line )
            {
                printf("Set directive incomplete! Missing what to replace!\n");
                exit (-1);
                break;

            }
            //now current_word has what needs to be replaced in the text, lets store it
            strcpy(text_to_be_changed,word_in_original.current_word);
            //now lets store what to change to
            if(getNextWord(&word_in_original,*file_contents,(*size_file_contents))==-1 || current_line!=word_in_original.current_source_line )
            {
                printf("Set directive incomplete! Missing what to replace for !\n");
                exit (-1);
                break;

            }
            strcpy(what_to_change_to,word_in_original.current_word);
            what_to_change_to_size=word_in_original.size_current_word;
            //now go all text replacing it
            //go to beginning of text
            word_in_original=word_pointing_to_origin;
            change_word_A_to_word_B(text_to_be_changed,what_to_change_to,what_to_change_to_size,file_contents,size_file_contents);
            //--.set//
        }
    }
    printf("Expanding done, returning following file:\n%s\n",*file_contents);
    printf("Shrinking it..."); //getting rid of white spaces
    shrink_file(file_contents,size_file_contents);
    printf("Result:\n%s\n",*file_contents);
}

node *get_label(char *file_contents,int *size_file_contents)
{
    char hex_file[18420],temp[12],current_line_as_hex[5],c; //max word is a label of 100 chars, so 101 is max
    int current_hex_line=0,hex_pos=0; //hex_pos -1 = esq, hex_pos = 1
    int i=0;
    node *label_list = cria();
    word word_in_file;
    word_in_file.current_source_line=1;
    word_in_file.current_word_location_in_line=0;
    word_in_file.i=0;
    word_in_file.size_current_word=0;
    long long temp_longlong, temp_longlongaux;
    initialize_hex(hex_file);
    /* Idea: goes through all file as if it was to write instructions already, get where each instruction
    would be placed and if finds a label, make it point to memory location in which a new instruction would
    be placed
    */
    while(-1!=getNextWord(&word_in_file,file_contents,*size_file_contents))
    {
        //printf("Tratando : %s , linha source = %d , %d palavra da linhas\n",word_in_file.current_word, word_in_file.current_source_line,word_in_file.current_word_location_in_line);
        //--.align//
        if(!strcasecmp(word_in_file.current_word,".align"))  //goes to line which is multiple of given number
        {
            //printf("Got .align!\n");
            getNextWord(&word_in_file,file_contents,(*size_file_contents));
            if(is_hexa(word_in_file.current_word))  //if word was given in hexa, turn to long long
            {
                if(!is_valid_hexa(word_in_file.current_word)){
                    printf("Not a valid hexa passed to align!");
                    exit (1);
                }
                temp_longlong=hexchar_to_longlong(word_in_file.current_word);
            }
            else   //if given in decimal base, convert string to number
            {
                temp_longlong=strtoll(word_in_file.current_word,NULL,10); //if int, convert string to long long
                if(temp_longlong<1){
                    printf("Not a valid number passed to align!");
                    exit (1);
                }
            }
            while(current_hex_line%temp_longlong!=0)  //go to line which is multiple of given number
            {
                current_hex_line++;
                hex_pos=0;
            }
        }
        //--.align//

        //--.word//
        if(!strcasecmp(word_in_file.current_word,".word"))  //puts a data in the next memory location which supports it
        {
            //printf("Got .word!\n");
            getNextWord(&word_in_file,file_contents,(*size_file_contents)); //get the argument and do nothing
            if(is_hexa(word_in_file.current_word)){
            if(!is_valid_hexa(word_in_file.current_word)){
                printf("Not a valid hexa passed to .word!");
                exit (1);
            }
            }
            current_hex_line++;
            hex_pos=0;

        }
        //--.word//

        //--.org//
        if(!strcasecmp(word_in_file.current_word,".org"))
        {
            //printf("Got .org!\n");
            getNextWord(&word_in_file,file_contents,(*size_file_contents));
            if(is_hexa(word_in_file.current_word)) // turn to long long
            {
                if(!is_valid_hexa(word_in_file.current_word)){
                    printf("Not a valid hexa passed to .org!");
                    exit (1);
                }
                temp_longlong=hexchar_to_longlong(word_in_file.current_word);
            }
            else
            {
                temp_longlong=strtoll(word_in_file.current_word,NULL,10); //turn to long long
            }
            current_hex_line=temp_longlong;
            hex_pos=0;

        }
        //--.org//

        //--.wfill//
        if(!strcasecmp(word_in_file.current_word,".wfill"))
        {
            //printf("Got .wfill!\n");
            //next word should store how many lines are gonna be filled by the data
            getNextWord(&word_in_file,file_contents,(*size_file_contents));
            if(is_hexa(word_in_file.current_word)) //if is hexa, turn to long long
            {

                temp_longlong=hexchar_to_longlong(word_in_file.current_word);
            }
            else
            {
                temp_longlong=strtoll(word_in_file.current_word,NULL,10); // if decimal, convert to long long
            }
            //temp_longlong now stores how many lines are gonna be filled by data
            //now lets store it in a safe variable and call the .word code the amount of time with the data given
            temp_longlongaux=temp_longlong;
            getNextWord(&word_in_file,file_contents,(*size_file_contents));// current_word now has the data to be inserted ALL data in the IAS has to be in HEXA
            for(; temp_longlongaux>0; temp_longlongaux--)
            {
                //.word code
                longlong_to_hexchar_with0x(current_hex_line,current_line_as_hex);
                current_hex_line++;
                hex_pos=0;
                //.word code
            }
        }
        //--.wfill//


        if(!strcasecmp(word_in_file.current_word,"ADD"))
        {
            getNextWord(&word_in_file,file_contents,(*size_file_contents)); //get arg
            if(hex_pos==0)
            {
                hex_pos=1;
            }
            else if(hex_pos==1)
            {
                hex_pos=0;
                current_hex_line++;
            }
        }
        if( (!strcasecmp(word_in_file.current_word,"LDMQM")) || (!strcasecmp(word_in_file.current_word,"LOAD"))
                || (!strcasecmp(word_in_file.current_word,"STR")) || (!strcasecmp(word_in_file.current_word,"DIV"))
                || (!strcasecmp(word_in_file.current_word,"LDN")) || (!strcasecmp(word_in_file.current_word,"LDABS"))
                || (!strcasecmp(word_in_file.current_word,"JMP")) || (!strcasecmp(word_in_file.current_word,"STM"))
                || (!strcasecmp(word_in_file.current_word,"JGEZ")) || (!strcasecmp(word_in_file.current_word,"LDABS"))
                || (!strcasecmp(word_in_file.current_word,"ADD")) || (!strcasecmp(word_in_file.current_word,"ADDABS"))
                || (!strcasecmp(word_in_file.current_word,"SUB")) || (!strcasecmp(word_in_file.current_word,"SUBABS"))
                || (!strcasecmp(word_in_file.current_word,"MUL")))
        {

            getNextWord(&word_in_file,file_contents,(*size_file_contents)); //get arg
            if(hex_pos==0)
            {
                hex_pos=1;
            }
            else if(hex_pos==1)
            {
                hex_pos=0;
                current_hex_line++;
            }
        }
        if( (!strcasecmp(word_in_file.current_word,"LDMQ")) || (!strcasecmp(word_in_file.current_word,"LSH"))
                || (!strcasecmp(word_in_file.current_word,"RSH")))
        {
            if(hex_pos==0)
            {
                hex_pos=1;
            }
            else if(hex_pos==1)
            {
                hex_pos=0;
                current_hex_line++;
            }

        }
        for(i=0; i<word_in_file.size_current_word; i++) //see if label and if so get it in linked list
        {
            c=word_in_file.current_word[i];
            if( (word_in_file.current_word[i]==':' && i==(word_in_file.size_current_word-1)) || (word_in_file.current_word[i]==':' && word_in_file.current_word[i+1]==' '))
            {
                rotulo label;
                char *name= (char*) malloc(sizeof(char)*101);
                printf("%s is LABEL!",word_in_file.current_word );
                label_plus my_label_plus = get_label_by_name(word_in_file.current_word,label_list);
                rotulo *pointer_to_rot;
                if(strcasecmp(my_label_plus.points_to,"ERR")){//if already exists just update already existing one
                    pointer_to_rot=busca_retu_point(label_list,word_in_file.current_word);//ERR = when not found
                    if(hex_pos==0){
                        pointer_to_rot->aponta_dir=0;
                    }else{
                        pointer_to_rot->aponta_dir=1;
                    }
                    strcpy(name,word_in_file.current_word);
                    pointer_to_rot->nome=name; //updating current existing label
                    pointer_to_rot->endereco=current_hex_line;
                    continue;
                }
                if(hex_pos==0){
                    label.aponta_dir=0;
                }else{
                    label.aponta_dir=1;
                }
                strcpy(name,word_in_file.current_word);
                label.nome=name;
                label.endereco=current_hex_line;
                insere_final(label_list,label);
            }
        }
    }
    return label_list;

}
label_plus get_label_by_name(char *name,node *labels) //receives a label name and returns struct with memory address it points to
{ //and if it points to right or left
    rotulo wanted = busca(labels,name);
    char *address;
    address = (char*) malloc(sizeof(char)*5);
    label_plus my_label_plus;
    if(wanted.endereco==-1)
    {
        printf("Rotulo %s inexistente\n",name);
        address="ERR";
        my_label_plus.points_to=address;
        return my_label_plus;
    }
    longlong_to_hexchar_with0x(wanted.endereco,address);
    my_label_plus.points_to=address;
    my_label_plus.points_to_dir=wanted.aponta_dir;
    return my_label_plus;
}

char *convert_word_to_instruction(char *file_contents, int size_file_contents)
{
    char *hex_file,current_line_as_hex[5], instruction[6],code[7]; //max word is a label of 100 chars, so 101 is max
    hex_file = (char*) malloc(sizeof(char)*18420);
    int current_hex_line=0; //current_hex_pos -1 = esq, current_hex_dir = 1
    int ins=0,hex_pos=0;
    node *label_list=get_label(file_contents,&size_file_contents);
    word word_in_file;
    word_in_file.current_source_line=1;
    word_in_file.current_word_location_in_line=0;
    word_in_file.i=0;
    word_in_file.size_current_word=0;
    long long temp_longlong, temp_longlongaux,check_number;
    initialize_hex(hex_file);
    //visualiza(label_list);
    while(-1!=getNextWord(&word_in_file,file_contents,size_file_contents))
    {
        printf("Tratando : %s , linha source = %d , %d palavra da linhas\n",word_in_file.current_word, word_in_file.current_source_line,word_in_file.current_word_location_in_line);
        //--.align//
        if(!strcasecmp(word_in_file.current_word,".align"))  //goes to line which is multiple of given number
        {
            printf("Got .align!\n");
            getNextWord(&word_in_file,file_contents,size_file_contents);
            if(is_hexa(word_in_file.current_word))  //if word was given in hexa, turn to long long
            {
                if(!is_valid_hexa(word_in_file.current_word)){
                    printf("Not a valid hexa passed to align!");
                    exit (1);
                }
                temp_longlong=hexchar_to_longlong(word_in_file.current_word);

            }
            else   //if given in decimal base, convert string to number
            {

                temp_longlong=strtoll(word_in_file.current_word,NULL,10); //if int, convert string to long long
                if(temp_longlong<1){
                    printf("Not a valid number passed to align!");
                    exit (1);
                }

            }
            current_hex_line++;
            while(current_hex_line%temp_longlong!=0)  //go to line which is multiple of given number
            {
                current_hex_line++;
            }
        }
        //--.align//

        //--.word//
        if(!strcasecmp(word_in_file.current_word,".word"))  //puts a data in the next memory location which supports it
        {
            printf("Got .word!\n");
            getNextWord(&word_in_file,file_contents,size_file_contents);
            if(is_hexa(word_in_file.current_word)) // current_word now has the data to be inserted ALL data in the IAS has to be in HEXA
            {
                temp_longlong=hexchar_to_longlong(word_in_file.current_word); //so we need to convert to HEX if necessary
            }
            else
            {
                temp_longlong=strtoll(word_in_file.current_word,NULL,10);
            }
            longlong_to_hexchar_with0x(current_hex_line,current_line_as_hex);
            write_to_hex(hex_file,current_line_as_hex,word_in_file.current_word,-1);
            current_hex_line++;
        }
        //--.word//

        //--.org//
        if(!strcasecmp(word_in_file.current_word,".org"))
        {
            printf("Got .org!\n");
            getNextWord(&word_in_file,file_contents,size_file_contents);
            if(is_hexa(word_in_file.current_word)) // turn to long long
            {
                temp_longlong=hexchar_to_longlong(word_in_file.current_word);
            }
            else
            {
                temp_longlong=strtoll(word_in_file.current_word,NULL,10); //turn to long long
            }
            current_hex_line=temp_longlong;
        }
        //--.org//

        //--.wfill//
        if(!strcasecmp(word_in_file.current_word,".wfill"))
        {
            printf("Got .wfill!\n");
            //next word should store how many lines are gonna be filled by the data
            getNextWord(&word_in_file,file_contents,size_file_contents);
            if(is_hexa(word_in_file.current_word)) //if is hexa, turn to long long
            {
                temp_longlong=hexchar_to_longlong(word_in_file.current_word);
            }
            else
            {
                temp_longlong=strtoll(word_in_file.current_word,NULL,10); // if decimal, convert to long long
            }
            //temp_longlong now stores how many lines are gonna be filled by data
            //now lets store it in a safe variable and call the .word code the amount of time with the data given
            temp_longlongaux=temp_longlong;
            getNextWord(&word_in_file,file_contents,size_file_contents);
            if(is_hexa(word_in_file.current_word)) // current_word now has the data to be inserted ALL data in the IAS has to be in HEXA
            {
                temp_longlong=hexchar_to_longlong(word_in_file.current_word); //so we need to convert to HEX if necessary
            }
            else
            {
                temp_longlong=strtoll(word_in_file.current_word,NULL,10);
            }
            for(; temp_longlongaux>0; temp_longlongaux--)
            {
                //.word code
                longlong_to_hexchar_with0x(current_hex_line,current_line_as_hex);
                write_to_hex(hex_file,current_line_as_hex,word_in_file.current_word,-1);
                current_hex_line++;
                //.word code
            }
        }
        //--.wfill//


        if(!strcasecmp(word_in_file.current_word,"LDMQ"))
        {
            strcpy(instruction,"0A000");
            ins=3;
        }
        else if(!strcasecmp(word_in_file.current_word,"LDMQM"))
        {
            strcpy(instruction,"09000");
            ins=1;
        }
        else if(!strcasecmp(word_in_file.current_word,"STR"))
        {
            strcpy(instruction,"21000");
            ins=1;
        }
        else if(!strcasecmp(word_in_file.current_word,"LOAD"))
        {
            strcpy(instruction,"01000");
            ins=1;
        }
        else if(!strcasecmp(word_in_file.current_word,"LDN"))
        {
            strcpy(instruction,"02000");
            ins=1;
        }
        else if(!strcasecmp(word_in_file.current_word,"LDABS"))
        {
            strcpy(instruction,"03000");
            ins=1;
        }
        else if(!strcasecmp(word_in_file.current_word,"MUL"))
        {
            strcpy(instruction,"0B000");
            ins=1;
        }
        else if(!strcasecmp(word_in_file.current_word,"DIV"))
        {
            strcpy(instruction,"0C000");
            ins=1;
        }
        else if(!strcasecmp(word_in_file.current_word,"JMP"))
        {
            strcpy(instruction,"0D000");//0E
            ins=2;
        }
        else if(!strcasecmp(word_in_file.current_word,"JGEZ"))
        {
            strcpy(instruction,"0F000");//10
            ins=2;
        }
        else if(!strcasecmp(word_in_file.current_word,"ADDABS"))
        {
            strcpy(instruction,"07000");
            ins=1;
        }
        else if(!strcasecmp(word_in_file.current_word,"ADD"))
        {
            strcpy(instruction,"05000");
            ins=1;
        }
        else if(!strcasecmp(word_in_file.current_word,"SUBABS"))
        {
            strcpy(instruction,"08000");
            ins=1;
        }
        else if(!strcasecmp(word_in_file.current_word,"SUB"))
        {
            strcpy(instruction,"06000");
            ins=1;
        }
        else if(!strcasecmp(word_in_file.current_word,"LSH"))
        {
            strcpy(instruction,"14000");
            ins=3;
        }
        else if(!strcasecmp(word_in_file.current_word,"RSH"))
        {
            strcpy(instruction,"15000");
            ins=3;
        }
        else if(!strcasecmp(word_in_file.current_word,"STM"))
        {
            strcpy(instruction,"12000");//13
            ins=2;
        }
        if(ins>0)
        {
            strcpy(code,word_in_file.current_word);
            if(ins!=3)  getNextWord(&word_in_file,file_contents,size_file_contents);
            if(!create_instruction(code,word_in_file.current_word,word_in_file.size_current_word,instruction,ins,label_list)) printf("ERROR!!!\n");
            else
            {
                //printf("instruction: (%s) current_hex_pos:%d\n",instruction,hex_pos);
                longlong_to_hexchar_with0x(current_hex_line,current_line_as_hex);
                write_to_hex(hex_file,current_line_as_hex,instruction,hex_pos);
                if(hex_pos==0)
                {
                    hex_pos=1;
                }
                else if(hex_pos==1)
                {
                    current_hex_line++;
                    hex_pos=0;
                }
            }
        }
        ins=0;

    }

    printf("%s",hex_file);
    return hex_file;

}

char *fileToVector(FILE *source,int **size_contents)
{
    char *file_contents;
    long source_size;
    int i;
    *size_contents = (int*) malloc (sizeof(int)); //alocating the pointer received one int data space
    fseek(source, 0, SEEK_END); //go to end
    source_size = ftell(source); // since it is at the end, the ftell in reality stores in source_size the size of the file given
    rewind(source); //go to beginning of file again
    **size_contents = source_size; //setting the value
    file_contents = malloc( (source_size+1) * (sizeof(char))); //file contents stores the whole file (+1 to store \0)
    for(i=0; i<source_size; i++)
    {
        fscanf(source,"%c", file_contents+i);
    }
    *(file_contents+source_size)='\0';
    return file_contents;
}





