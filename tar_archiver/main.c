#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
// #include <stdint.h>
#define BLOCK 512

struct files{
    struct header{
        char path_name[100];
        int file_mode; // octal
        int owner_id; // octal
        int group_id; // octal
        int file_size; // octal
        time_t last_modification; // octal
        int checksum;
        int link; 
        char linked_name[100];
    }header;
    struct content{

    };
};
void print_header(struct files* file); // test prototypes

void read_content(); // to do 
void write_content(); // to do

int octal_string_to_decimal(char string[],int size){
    int p=1;
    int conversion=0;
    for(int i=size-1;i>=0;i--){
        if(isdigit(string[i])){
            // printf("%d ",string[i]-'0');
            conversion+=(string[i]-'0')*p;
            p*=8;
        }
    }
    return conversion;
}

void process_header(FILE* input,struct files* file){
    //header filed sizes
    int path_and_name_size=100; 
    int file_mode_size=8; // octal
    int owner_id_size=8; // octal
    int group_id_size=8; // octal
    int file_size=12; // octal
    int last_modification_size=12; // octal
    int checksum_size=8;
    int link_size=1;
    int linked_name_size=100; 
    //end of header filed sizes
    int total_sizes=0;

    char buffer[100];

    if(fread(buffer,sizeof(*buffer),path_and_name_size,input) != path_and_name_size){
        printf("Error reading file\n");
      return;
    }
    strcpy(file->header.path_name,buffer);
    total_sizes+=path_and_name_size;

    if(fread(buffer,sizeof(*buffer),file_mode_size,input) != file_mode_size){
        printf("Error reading file\n");
        return;
    }
    file->header.file_mode=octal_string_to_decimal(buffer,file_mode_size);
    total_sizes+=file_mode_size;

    if(fread(buffer,sizeof(*buffer),owner_id_size,input) != owner_id_size){
        printf("Error reading file\n");
        return;
    }
    file->header.owner_id=octal_string_to_decimal(buffer,owner_id_size);
    total_sizes+=owner_id_size;


    if(fread(buffer,sizeof(*buffer),group_id_size,input) != group_id_size){
        printf("Error reading file\n");
        return;
    }
    file->header.group_id=octal_string_to_decimal(buffer,group_id_size);
    total_sizes+=group_id_size;

    if(fread(buffer,sizeof(*buffer),file_size,input) != file_size){
        printf("Error reading file\n");
        return;
    }
    file->header.file_size=octal_string_to_decimal(buffer,file_size);
    total_sizes+=file_size;
    
    if(fread(buffer,sizeof(*buffer),last_modification_size,input) != last_modification_size){
        printf("Error reading file\n");
        return;
    }
    file->header.last_modification=octal_string_to_decimal(buffer,last_modification_size);
    total_sizes+=last_modification_size;
    
    if(fread(buffer,sizeof(*buffer),checksum_size,input) != checksum_size){
        printf("Error reading file\n");
        return;
    }
    file->header.checksum=octal_string_to_decimal(buffer,checksum_size);
    total_sizes+=checksum_size;

    if(fread(buffer,sizeof(*buffer),link_size,input) != link_size){
        printf("Error reading file\n");
        return;
    }

    file->header.link=buffer[0]-'0'; //has only one byte
    total_sizes+=link_size;
    


    if(fread(buffer,sizeof(*buffer),linked_name_size,input) != linked_name_size){
        printf("Error reading file\n");
        return;
    }
    strcpy(file->header.linked_name,buffer);
    total_sizes+=linked_name_size;

    while(total_sizes<BLOCK){ // ignore the rest of the header
        total_sizes++;
    }

}

void unpack(FILE* input){ // to do
    // read_header();
    struct files *file=(struct files*)malloc(sizeof(struct files));
    if(file == NULL){
        free(file);
        printf("error occured when allocating memory to the files");
        return ;
    }
    process_header(input,file);
    print_header(file);
    // printf(file->header.path_name);
    // read_content();
    // write_content();
    free(file);
} 

int main(int arg, char* args[]) {
    const char* mode = args[1];
    int count=2;
    if(*mode=='x')
    while(count<arg){
        char* path=args[count]; // only first file yet
        FILE* input= fopen(path,"rb");
        unpack(input);
        count++;
    }
    return 0;
}

//integration tests
void print_header(struct files* file){
    printf("path_name: %s\n",file->header.path_name);
    printf("file_mode: %d\n",file->header.file_mode);
    printf("owner_id: %d\n",file->header.owner_id);
    printf("file_size: %d\n",file->header.file_size);
    printf("last_modification: %lld\n",file->header.last_modification);
    printf("checksum: %d\n",file->header.checksum);
    printf("link: %d\n",file->header.link);
    printf("linked_name: %s",file->header.linked_name);
}

//  ./a x plm.tar
//