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
char *get_label_by_name(char *name, node *labels);
node *get_label(char *file_contents, int *size_file_contents);
void removeComents(char *file_contents,int *size);
char *fileToVector(FILE *source, int **size_contents);
int getNextWord(word *currentword,char *file_contents, int size_file_contents);
void initialize_hex(char *hex_file);
void write_to_hex(char *hex_file, char *memory_address_to_write, char *what_to_write, int write_to_dir);
void expand_dot_set(char **file_contents, int *size_file_contents);
void convert_word_to_instruction(char *file_contents, int size_file_contents);
long long hexchar_to_longlong(char *hex);
char *longlong_to_hexchar_with0x(long long number, char *destiny);
char *longlong_to_hexchar_without0x(long long number, char *destiny);
char *remove_0x(char *hex);
int create_instruction(char *codigo, char *complemento, int tam_complemento, char* instruction, int type, node *labels);
void turn_text_word_in_100_chars(char **file_contents, int *size_file_contents);

#endif // DEALWITHFILE_H
