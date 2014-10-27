#ifndef DEALWITHFILE_H
#define DEALWITHFILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

typedef struct Word //Stores info about a word in a vector
{
    char current_word[101];
    int current_source_line;
    int current_word_location_in_line;
    int size_current_word;
    int i;
} word;
typedef struct _label_plus //Stores info about a label in a vector where it points to and if right or left
{
    char *points_to;
    int points_to_dir;
} label_plus;
int is_hexa(char *hex); //checks if is hexa
label_plus get_label_by_name(char *name, node *labels); //get label by name
node *get_label(char *file_contents, int *size_file_contents); //puts labels there
void removeComents(char *file_contents,int *size); //remove comments
char *fileToVector(FILE *source, int **size_contents); //turn file into a string
int getNextWord(word *currentword,char *file_contents, int size_file_contents); //get next word of file
void initialize_hex(char *hex_file); //initializes hex
void write_to_hex(char *hex_file, char *memory_address_to_write, char *what_to_write, int write_to_dir); //writes in hex
void expand_dot_set(char **file_contents, int *size_file_contents); //expands .set directives
char *convert_word_to_instruction(char *file_contents, int size_file_contents); //converts words into instructions and writes them to hex
long long hexchar_to_longlong(char *hex);//converts hex as char to long long
char *longlong_to_hexchar_with0x(long long number, char *destiny); //turns long long to hex with 0x
char *longlong_to_hexchar_without0x(long long number, char *destiny); //turns long long to hex without 0x
char *remove_0x(char *hex); //removes 0x
int create_instruction(char *codigo, char *complemento, int tam_complemento, char* instruction, int type, node *labels); //creates instructions
void turn_text_word_in_100_chars(char **file_contents, int *size_file_contents);//make words in text to 100 chars
int is_hexa(char *hex); //checks if hex
int is_valid_hexa(char *hex);//checks if valid hex
int is_valid_small_hex(char *number); //checks if valid small hex (3 chars)
#endif // DEALWITHFILE_H
