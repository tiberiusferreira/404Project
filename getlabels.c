#include "getlabels.h"

void put_labels_in_data_struct(char *file_contents, int *size_contents){
    char c;
    int beginning_of_label;
    int local_size = *size_contents,i;
    for(i=0;i<local_size;i++){ // go through all data
        c=file_contents[i];
        if(c=='\n'){ //means a new line has just began
            for(;i<local_size && file_contents[i]==' ';i++){
                continue; //clear all empty space
            }
            for(i=beginning_of_label;i<local_size && file_contents[i]!=' ';i++){ //until next white space
                if(file_contents[i]==':' && (file_contents[i+1]==' ' || file_contents[i+1]=='\n') ){
                    //rotulo
                }
            }
        }
    }
}
