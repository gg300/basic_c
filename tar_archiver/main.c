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
        char block[BLOCK];
    }*content;
};
void print_header(struct files* file); // test prototypes
void print_content(struct files* file,int content_size);


void write_header();// to do 
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

void process_header(FILE* input,struct files* file, int *header_counter){
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
        if(*header_counter==0)
            printf("Error reading header file\n");
        else
            *header_counter=-1;
      return;
    }
    strcpy(file->header.path_name,buffer);
    total_sizes+=path_and_name_size;

    if(fread(buffer,sizeof(*buffer),file_mode_size,input) != file_mode_size){
        printf("Error reading file mode\n");
        return;
    }
    file->header.file_mode=octal_string_to_decimal(buffer,file_mode_size);
    total_sizes+=file_mode_size;

    if(fread(buffer,sizeof(*buffer),owner_id_size,input) != owner_id_size){
        printf("Error reading file owner_id\n");
        return;
    }
    file->header.owner_id=octal_string_to_decimal(buffer,owner_id_size);
    total_sizes+=owner_id_size;


    if(fread(buffer,sizeof(*buffer),group_id_size,input) != group_id_size){
        printf("Error reading file group_id\n");
        return;
    }
    file->header.group_id=octal_string_to_decimal(buffer,group_id_size);
    total_sizes+=group_id_size;

    if(fread(buffer,sizeof(*buffer),file_size,input) != file_size){
        printf("Error reading file size\n");
        return;
    }
    file->header.file_size=octal_string_to_decimal(buffer,file_size);
    total_sizes+=file_size;
    
    if(fread(buffer,sizeof(*buffer),last_modification_size,input) != last_modification_size){
        printf("Error reading file modification\n");
        return;
    }
    file->header.last_modification=octal_string_to_decimal(buffer,last_modification_size);
    total_sizes+=last_modification_size;
    
    if(fread(buffer,sizeof(*buffer),checksum_size,input) != checksum_size){
        printf("Error reading file checksum\n");
        return;
    }
    file->header.checksum=octal_string_to_decimal(buffer,checksum_size);
    total_sizes+=checksum_size;

    if(fread(buffer,sizeof(*buffer),link_size,input) != link_size){
        printf("Error reading file link\n");
        return;
    }

    file->header.link=buffer[0]-'0'; //has only one byte
    total_sizes+=link_size;
    
    if(fread(buffer,sizeof(*buffer),linked_name_size,input) != linked_name_size){
        printf("Error reading file linked_name\n");
        return;
    }
    strcpy(file->header.linked_name,buffer);
    total_sizes+=linked_name_size;
    int remaining=BLOCK-total_sizes;
    while(total_sizes<BLOCK){
        int garbage_number;
        if((remaining%100)>0){garbage_number=remaining%100;
           if(fread(buffer,sizeof(*buffer),garbage_number,input)!=garbage_number)
                printf(":P");
        remaining-=remaining%100;
        }
        else{garbage_number=100;
            if(fread(buffer,sizeof(*buffer),garbage_number,input)!=100)
                printf(":0");
        }
        total_sizes+=garbage_number;
        printf("\nheader cursor %ld\n",ftell(input));
    }

    (*header_counter)++;
}
void process_content(FILE* input,struct files* file,int *content_size){  //// UTF-8 problems with this kind of processing but content is intact
    *content_size = file->header.file_size/BLOCK + ((file->header.file_size%BLOCK)&&1);
    
    file->content = (struct content*)malloc(sizeof(struct content) * (*content_size));

    if(file->content==NULL){
        free(file->content);
        free(file);
        printf("content memory allocation error");
        return ;
    }
    for(int i=0;i<*content_size;++i){ // read the memory blocks for content
        printf("\n for2 content %d\n",i);
        if(fread(file->content[i].block,sizeof(*(file->content[i].block)),BLOCK,input) != BLOCK){
            printf("Error reading file content\n");
            free(file->content);
            free(file);
          return;
        }
        printf("\ncursor %ld \n ",ftell(input));
    }
    printf("%d",*content_size);
}

void unpack(FILE* input){ // to verify for more than one file
    int content_size=0,header_counter=0,file_counter=0;
    struct files *file=(struct files*)malloc(sizeof(struct files));
    if(file == NULL){
        free(file);
        printf("error occured when allocating memory to the files");
        return ;
    }
        // file_counter=header_counter;
        // process_header(input,file+file_counter,&header_counter);
        // process_content(input,file+file_counter,&content_size);
        // print_header(file);
        // print_content(file+file_counter,content_size);
    
    do{
        file_counter=header_counter;
        struct files *aux = (struct files*)realloc(file,sizeof(struct files)*file_counter+1);
        if(aux == NULL){
            for(int i=0;i<file_counter;i++)
                free(file[i].content);
            free(file);
            printf("error occured when allocating memory to the files");
            return ;
        }
        file=aux;
        process_header(input,&file[file_counter],&header_counter);
        process_content(input,&file[file_counter],&content_size);
        print_header(&file[file_counter]);
        print_content(&file[file_counter],content_size);
    }while(header_counter!=-1);
    printf("\n%d\n",file_counter-1);
    for(int i=0;i<file_counter;i++)
        free(file[i].content);
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
    printf("\n");
}
void print_content(struct files* file,int content_size){
    printf("\n File Content: \n");
    for(int i=0;i<content_size;++i)
        printf("%s",file->content[i].block);
    printf("\n");
}

//  ./a x plm.tar
//