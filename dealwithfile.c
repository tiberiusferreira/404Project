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
            }
        }
    }
}
int getNextWord(char *current_word, int *current_source_line,int *current_source_line_word, int *size_current_word, int *i, char *file_contents, int size_file_contents)  //receives file contents and size of it
{
    //returns 0 if got new word or -1 if did not
    char c;
    int i_cur_word;
    for(; (*i)<size_file_contents;) //go through all file
    {
        c=file_contents[*i]; //get each char from file
        for(; (file_contents[*i]==' ' || file_contents[*i]=='\n') && (*i)<size_file_contents; (*i)++)  //jump white spaces and line breaks
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
            if(file_contents[*i]==' ' || file_contents[*i]=='\n' || *i==size_file_contents) //word ended if found whitespace or line break or end of file
            {
                if(*i==size_file_contents)
                {
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

void initialize_hex(char *hex_file)
{
    //18 chars per line = 17 + \n
    int i,line=0,num_chars=0,i2,i3;
    char temp[6];
    for(i=0; i<18414;)
    {
        num_chars=sprintf(temp,"%x",line);
        if(num_chars<3)
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

long long hexchar_to_longlong(char *hex)
{
    long long number = (long long) strtoll(hex,NULL,0);
    return number;
}

char *longlong_to_hexchar_with0x(long long number, char *destiny)
{
    destiny[0]='0';
    destiny[1]='x';
    sprintf(destiny+2,"%llx",number);
    return destiny;
}
/*
char *longlong_to_hexchar_without0x(long long number, char *destiny){
    sprintf(destiny,"%llx",number);
    return destiny;
}*/


int create_instruction(char *codigo, char *complemento, int tam_complemento, char* instruction)
{
    int i=0,j=0;
    char endereco[6];
   // printf("(%d)complemento: (%s)\n",tam_complemento,complemento);
    if(strcmp("ADD",codigo)==0||strcmp("SUB",codigo)==0||(strcmp("LOAD",codigo)==0&&tam_complemento>2&&tam_complemento<=10)||strcmp("MUL",codigo)==0||strcmp("DIV",codigo)==0||(strcmp("STOR",codigo)==0)&&tam_complemento<9)
    {
        int abs=0, neg=0;
        if(complemento[i]=='-'&&strcmp("LOAD",codigo)==0)
        {
            i++;
            neg=1;
        }
        else if(complemento[i]=='|')
        {
            i++;
            abs=1;
        }
        if(complemento[i++]!='M') return 0;
        if(complemento[i]!='(') return 0;
        for(i=i+1; i<tam_complemento; i++)
        {
            if(complemento[i]==')')
            {
                endereco[j]='\0';
                break;
            }
            endereco[j]=complemento[i];
            j++;
        }
        if(abs==1&&complemento[++i]!='|') return 0;
        if(!is_hexa(endereco)||j>5) return 0;
        if(strcmp("ADD",codigo)==0)
        {
            if(abs||neg)
            {
                instruction[0]='0';
                instruction[1]='7';
            }
            else
            {
                instruction[0]='0';
                instruction[1]='5';
            }
        }
        if(strcmp("SUB",codigo)==0)
        {

            if(abs||neg)
            {
                instruction[0]='0';
                instruction[1]='6';
            }
            else
            {
                instruction[0]='0';
                instruction[1]='8';
            }
        }
        if(strcmp("LOAD",codigo)==0)
        {

            if(abs)
            {
                instruction[0]='0';
                instruction[1]='3';
            }
            if(neg)
            {
                instruction[0]='0';
                instruction[1]='2';
            }
            else
            {
                instruction[0]='0';
                instruction[1]='1';
            }
        }
        if(strcmp("MUL",codigo)==0)
        {

            if(abs||neg)
            {
                return 0;
            }
            else
            {
                instruction[0]='0';
                instruction[1]='B';
            }
        }
        if(strcmp("DIV",codigo)==0)
        {

            if(abs||neg)
            {
                return 0;
            }
            else
            {
                instruction[0]='0';
                instruction[1]='C';
            }
        }
        if(strcmp("STOR",codigo)==0){
            if(abs||neg)
            {
                return 0;
            }
            else
            {
                instruction[0]='2';
                instruction[1]='1';
            }
        }

    }
    else if(!strcasecmp(codigo,"JUMP")||!strcasecmp(codigo,"JUMP+")||strcmp("STOR",codigo)==0)
    {
        char pos[6];
        int flag=0;
        if(complemento[i++]!='M') return 0;
        if(complemento[i]!='(') return 0;
        for(i=i+1; i<tam_complemento; i++)
        {
            if(j>5) return 0;
            else if(complemento[i]==',')
            {
                i++;
                endereco[j]='\0';
                j=0;
                flag=1;
            }
            else if(complemento[i]==')')
            {
                pos[j]='\0';
                break;
            }
            if(!flag) endereco[j]=complemento[i];
            else pos[j]=complemento[i];
            j++;
        }
        if(!is_hexa(endereco)) return 0;
        if(strcmp("JUMP",codigo)==0)
        {
            if(strcmp("0:19",pos)==0)
            {
                instruction[0]='0';
                instruction[1]='D';
            }
            if(strcmp("20:39",pos)==0)
            {
                instruction[0]='0';
                instruction[1]='E';
            }
        }
        else if(strcmp("JUMP+",codigo)==0)
        {
            if(strcmp("0:19",pos)==0)
            {
                instruction[0]='0';
                instruction[1]='F';
            }
            else if(strcmp("20:39",pos)==0)
            {
                instruction[0]='1';
                instruction[1]='0';
            }
            else return 0;
        }
        else if(strcmp("STOR",codigo)==0){
            if(strcmp("8:19",pos)==0)
            {
                instruction[0]='1';
                instruction[1]='3';
            }
            else if(strcmp("28:39",pos)==0)
            {
                instruction[0]='1';
                instruction[1]='4';
            }
            else return 0;
        }
    }
    else if((strcmp("LOAD",codigo)==0&&tam_complemento==2)||strcmp("LSH",codigo)==0||strcmp("RSH",codigo)==0){
        if(strcmp("LOAD",codigo)==0){
            if(complemento[0]=='M'&&complemento[1]=='Q'){
                instruction[0]='0';
                instruction[1]='A';
            }
            else return 0;
        }
        else if(strcmp("LSH",codigo)==0){
            instruction[0]='1';
            instruction[1]='0';
        }
        else if(strcmp("RSH",codigo)==0){
            instruction[0]='1';
            instruction[1]='1';
        }
        else return 0;
        strcpy(endereco,"0x000");
    }
    else if(strcmp("LOAD",codigo)==0&&tam_complemento>10){
        if(complemento[i++]!='M'||complemento[i++]!='Q'||complemento[i++]!=','||complemento[i++]!='M'||complemento[i]!='(') return 0;
        for(i=i+1; i<tam_complemento; i++)
        {
            if(complemento[i]==')')
            {
                endereco[j]='\0';
                break;
            }
            endereco[j]=complemento[i];
            j++;
        }
        if(!is_hexa(endereco)||j>5) return 0;
        instruction[0]='0';
        instruction[1]='9';
    }
    instruction[2]=endereco[2];
    instruction[3]=endereco[3];
    instruction[4]=endereco[4];
    instruction[5]='\0';
    return 1;
}

int is_hexa(char *hex)
{
    if(hex[0]=='0' && hex[1]=='x')
    {
        return 1;
    }
    return 0;
}

char *remove_0x(char *hex)
{
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
{
    int i=0,current_source_line=0;
    int target_line=hexchar_to_longlong(memory_address_to_write);
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
    if(write_to_dir==-1)
    {
        if(i+12>18414)  // wanna write beyond hex file size, do not do it
        {
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
    if(write_to_dir==0)
    {
        if(i+12>18414)
        {
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
    if(write_to_dir==1)
    {
        if(i+12>18414)
        {
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

void convert_word_to_instruction(char *file_contents, int size_file_contents)
{
    char current_word[101], hex_file[18420],temp[12],current_line_as_hex[5], instruction[6]; //max word is a label of 100 chars, so 101 is max
    int current_source_line=1, current_word_location_in_line=0; //temp stores something to be written to IAS so 12 is enough
    int current_hex_line=0,current_hex_pos=-1,size_current_word=0, hex_pos=0; //current_hex_pos -1 = esq, current_hex_dir = 1
    int i,ins=0;
    long long temp_longlong, temp_longlongaux;
    initialize_hex(hex_file);
    while(-1!=getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents))
    {
        printf("Tratando : %s , linha source = %d , %d palavra da linhas\n",current_word, current_source_line,current_word_location_in_line);

        //--.align//
        if(!strcasecmp(current_word,".align"))  //goes to line which is multiple of given number
        {
            printf("Got .align!\n");
            getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents);
            if(is_hexa(current_word))  //if word was given in hexa, turn to long long
            {
                temp_longlong=hexchar_to_longlong(current_word);
            }
            else   //if given in decimal base, convert string to number
            {
                temp_longlong=strtoll(current_word,NULL,10); //if int, convert string to long long
            }
            while(current_hex_line%temp_longlong!=0)  //go to line which is multiple of given number
            {
                current_hex_line++;
            }
        }
        //--.align//

        //--.word//
        if(!strcasecmp(current_word,".word"))  //puts a data in the next memory location which supports it
        {
            printf("Got .word!\n");
            getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents);
            if(is_hexa(current_word)) // current_word now has the data to be inserted ALL data in the IAS has to be in HEXA
            {
                temp_longlong=hexchar_to_longlong(current_word); //so we need to convert to HEX if necessary
            }
            else
            {
                temp_longlong=strtoll(current_word,NULL,10);
            }
            longlong_to_hexchar_with0x(current_hex_line,current_line_as_hex);
            write_to_hex(hex_file,current_line_as_hex,current_word,-1);
            current_hex_line++;
        }
        //--.word//

        //--.org//
        if(!strcasecmp(current_word,".org"))
        {
            printf("Got .org!\n");
            getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents);
            longlong_to_hexchar_with0x(current_hex_line,temp);
            write_to_hex(hex_file,temp,current_word,-1);
            current_hex_line++;
        }
        //--.org//

        //--.wfill//
        if(!strcasecmp(current_word,".wfill"))
        {
            printf("Got .wfill!\n");
            //next word should store how many lines are gonna be filled by the data
            getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents);
            if(is_hexa(current_word)) //if is hexa, turn to long long
            {
                temp_longlong=hexchar_to_longlong(current_word);
            }
            else
            {
                temp_longlong=strtoll(current_word,NULL,10); // if decimal, convert to long long
            }
            //temp_longlong now stores how many lines are gonna be filled by data
            //now lets store it in a safe variable and call the .word code the amount of time with the data given
            temp_longlongaux=temp_longlong;
            getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents);
            if(is_hexa(current_word)) // current_word now has the data to be inserted ALL data in the IAS has to be in HEXA
            {
                temp_longlong=hexchar_to_longlong(current_word); //so we need to convert to HEX if necessary
            }
            else
            {
                temp_longlong=strtoll(current_word,NULL,10);
            }
            for(; temp_longlongaux>0; temp_longlongaux--)
            {
                //.word code
                longlong_to_hexchar_with0x(current_hex_line,current_line_as_hex);
                write_to_hex(hex_file,current_line_as_hex,current_word,-1);
                current_hex_line++;
                //.word code
            }
        }
        //--.wfill//


        if(!strcasecmp(current_word,"ADD"))
        {
            printf("Got ADD! ");
            ins=1;
        }
        else if(!strcasecmp(current_word,"SUB"))
        {
            printf("Got SUB! ");
            ins=1;
        }
        else if(!strcasecmp(current_word,"LOAD"))
        {
            printf("Got LOAD! ");
            ins=1;
        }
        else if(!strcasecmp(current_word,"MUL"))
        {
            printf("Got MUL! ");
            ins=1;
        }
        else if(!strcasecmp(current_word,"DIV"))
        {
            printf("Got DIV! ");
            ins=1;
        }
        else if(!strcasecmp(current_word,"JUMP"))
        {
            printf("Got JUMP! ");
            ins=1;
        }
        else if(!strcasecmp(current_word,"JUMP+"))
        {
            printf("Got JUMP+! ");
            ins=1;
        }
        else if(!strcasecmp(current_word,"STOR"))
        {
            printf("Got STOR! ");
            ins=1;
        }
        else if(!strcasecmp(current_word,"LSH"))
        {
            printf("Got LSH! ");
            ins=1;
        }
        else if(!strcasecmp(current_word,"RSH"))
        {
            printf("Got RSH! ");
            ins=1;
        }
        if(ins)
        {
            ins=0;
            strcpy(instruction,current_word);
            if(strcasecmp(current_word,"RSH")!=0&&strcasecmp(current_word,"LSH")!=0)getNextWord(current_word,&current_source_line,&current_word_location_in_line,&size_current_word,&i,file_contents,size_file_contents);
            if(!create_instruction(instruction,current_word,size_current_word,instruction)) printf("ERROR!!!\n");
            else
            {
                if(current_hex_pos==-1)
                {
                    printf("ESQUERDA\n");
                    current_hex_pos=1;
                    hex_pos=0;
                }
                else if(current_hex_pos==1)
                {
                    printf("DIREITA\n");
                    current_hex_pos==-1;
                    hex_pos=1;
                }
                longlong_to_hexchar_with0x(current_hex_line,temp);
                write_to_hex(hex_file,temp,instruction,hex_pos);
                if(hex_pos==1)
                {
                    current_hex_line++;
                    current_hex_pos=-1;
                }
            }
        }


    }
    printf("%s",hex_file);
}

char *fileToVector(FILE *source,int **size_contents)
{
    char *file_contents;
    long source_size;
    int i;
    *size_contents = (int*) malloc (sizeof(int)); //alocating the pointer received a int data space
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





