#include <stdio.h>

#define BLOCK 512

void read_header(); // to do
void read_content(); // to do 
void write_content(); // to do

void process_header(FILE* file){
    int path_and_name=100; //header filed sizes
    int file_mode=8;
    int owner_id=8;
    int group_id=8;
    int file_size=12;
    int last_modification=8;    
    int link=1;
    int linked_name=100; //end of header filed sizes
    int counter=0,i;

    for(i=0;i<path_and_name;++i); // to do
    counter+=i;

    for(i=0;i<file_mode;++i); // to do
    counter+=i;

    for(i=0;i<owner_id;++i); // to do
    counter+=i;
    
    for(i=0;i<group_id;++i); // to do
    counter+=i;

    for(i=0;i<file_size;++i); // to do
    counter+=i;
    
    for(i=0;i<last_modification;++i); // to do
    counter+=i;
    
    for(i=0;i<link;++i); // to do
    counter+=i;
    
    for(i=0;i<linked_name;++i); // to do
    counter+=i;
    
    printf("%d",counter);
    while(counter<BLOCK){ // to pad with 0es
        counter++;
    }

} // to do

void unpack(FILE* file){ // to do
    // read_header();
    process_header(file);
    // read_content();
    // write_content();
} 

int main(int arg, char* args[]) {
    const char* mode = args[1];
    int count=1;
    while(count<arg){
        char* path=args[count]; // only first file yet
        FILE* input= fopen(path,"rb");
        unpack(input);
        count++;
    }
    return 0;
}