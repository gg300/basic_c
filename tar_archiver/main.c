#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
// #include <stdint.h>
#define BLOCK 512

struct files{
    struct header{
        char path_name[101];
        int file_mode; // octal
        int owner_id; // octal
        int group_id; // octal
        int file_size; // octal
        time_t last_modification; // octal
        int checksum;
        int link; 
        char linked_name[101];
    }header;
    struct file_header{
        char path_name[101];
        char file_mode[9]; // octal
        char owner_id[9]; // octal
        char group_id[9]; // octal
        char file_size[13]; // octal
        char last_modification[13]; // octal
        char checksum[9];
        char link[1];
        char linked_name[101];
    }file_header;
    struct content{
        char block[BLOCK];
    }*content;
};
void print_header(struct files* file); // test prototypes
void print_content(struct files* file,int content_size);


void write_header();// to do 
void write_content(struct files* file,int *content_size){// to do
    FILE *output = fopen(file->header.path_name,"wb");
    int written_content=0;
    if(!output)
        {
            printf("output file error");
            return ;
        }
    for(int i=0;i<*content_size;++i){
        written_content=fwrite(file->content[i].block,sizeof(char),BLOCK,output);
        if(written_content!=BLOCK)
            {
                printf("write error");
                return;
            }
        // printf("\nwritten content %d\n",written_content);
    }
    fclose(output);
    return;
}
 

int octal_string_to_decimal(char buffer[]){
    int size=strlen(buffer)-1;
    int p=1;
    int conversion=0;
    for(int i=size;i>=0;i--){
        if(isdigit(buffer[i])){
            // printf("%d ",buffer[i]-'0');
            conversion+=(buffer[i]-'0')*p;
            p*=8;
        }
    }
    return conversion;
}

void process_header(FILE* input,struct files* file, int *header_counter,bool *done){  /// forgot to free memory for every fread error;
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

    if(fread(buffer,sizeof(char),path_and_name_size,input) != path_and_name_size){
        if(*header_counter==0)
            printf("Error reading header file\n");
        *done=true;
      return;
    }
    if(buffer[0]==0){
        *done=true;
        return;
    }
    strcpy(file->header.path_name,buffer);
    strcpy(file->file_header.path_name,buffer);
    // total_sizes+=path_and_name_size;
    // printf("\n buffer %s\n",buffer);

    if(fread(buffer,sizeof(char),file_mode_size,input) != file_mode_size){
        
        printf("Error reading file mode\n");
        *done=true;
        return;
    }
    file->header.file_mode=octal_string_to_decimal(buffer);
    strcpy(file->file_header.file_mode,buffer);
    // total_sizes+=file_mode_size;
    // printf("\n buffer %s\n",buffer);

    if(fread(buffer,sizeof(char),owner_id_size,input) != owner_id_size){
        printf("Error reading file owner_id\n");
        *done=true;
        return;
    }
    file->header.owner_id=octal_string_to_decimal(buffer);
    strcpy(file->file_header.owner_id,buffer);
    // total_sizes+=owner_id_size;
    // printf("\n buffer %s\n",buffer);


    if(fread(buffer,sizeof(char),group_id_size,input) != group_id_size){
        printf("Error reading file group_id\n");
        *done=true;
        return;
    }
    file->header.group_id=octal_string_to_decimal(buffer);
    strcpy(file->file_header.group_id,buffer);
    // total_sizes+=group_id_size;
    // printf("\n buffer %s\n",buffer);

    if(fread(buffer,sizeof(char),file_size,input) != file_size){
        printf("Error reading file size\n");
        *done=true;
        return;
    }
    file->header.file_size=octal_string_to_decimal(buffer);
    strcpy(file->file_header.file_size,buffer);
    // total_sizes+=file_size;
    // printf("\n buffer %s\n",buffer);
    
    if(fread(buffer,sizeof(char),last_modification_size,input) != last_modification_size){
        printf("Error reading file modification\n");
        *done=true;
        return;
    }
    file->header.last_modification=octal_string_to_decimal(buffer);
    strcpy(file->file_header.last_modification,buffer);
    // total_sizes+=last_modification_size;
    // printf("\n buffer %s\n",buffer);
    
    if(fread(buffer,sizeof(char),checksum_size,input) != checksum_size){
        printf("Error reading file checksum\n");
        *done=true;
        return;
    }
    file->header.checksum=octal_string_to_decimal(buffer);
    strcpy(file->file_header.checksum,buffer);
    // total_sizes+=checksum_size;
    // printf("\n buffer %s\n",buffer);

    if(fread(buffer,sizeof(char),link_size,input) != link_size){
        printf("Error reading file link\n");
        *done=true;
        return;
    }

    file->header.link=buffer[0]-'0'; //has only one byte
    strcpy(file->file_header.link,buffer);
    // total_sizes+=link_size;
    // printf("\n buffer %s\n",buffer);
    
    if(fread(buffer,sizeof(char),linked_name_size,input) != linked_name_size){
        printf("Error reading file linked_name\n");
        *done=true;
        return;
    }
    strcpy(file->header.linked_name,buffer);
    strcpy(file->file_header.linked_name,buffer);
    // total_sizes+=linked_name_size;
    total_sizes=ftell(input)%512;
    int remaining=BLOCK-total_sizes;
    while(total_sizes!=BLOCK){
        int garbage_number=0;
        if(remaining<100 && remaining>0){
            garbage_number=remaining%100;
            if(fread(buffer,1,garbage_number,input)!=garbage_number){
                printf(":P   , remaining %d",remaining);
                *done=true;
                return;
            }
        }
        else{
            garbage_number=100;
            if(fread(buffer,1,garbage_number,input)!=garbage_number){
                printf(":0   , remaining %d",remaining);
                *done=true;
                return;
            }
        }
        remaining-=garbage_number;
        total_sizes+=garbage_number;
        // printf("\nheader cursor %ld\n",ftell(input));
    }
    // while(total_sizes!=BLOCK){
    //     int garbage_number;
    //     if((remaining%100)>0){garbage_number=remaining%100;
    //        if(fread(buffer,sizeof(char),garbage_number,input)!=garbage_number)
    //             printf(":P");
    //     }
    //     else{garbage_number=100;
    //         if(fread(buffer,sizeof(char),garbage_number,input)!=100)
    //         printf(":0");
    // }
    //     remaining-=garbage_number;
    //     total_sizes+=garbage_number;
    //     printf("\nheader cursor %ld\n",ftell(input));
    // }
    // printf("\nafter while %ld\n",ftell(input));
    (*header_counter)++;
}
void process_content(FILE* input,struct files* file,int *content_size,bool *done){  //// UTF-8 problems with this kind of processing but content is intact
    *content_size = file->header.file_size/BLOCK + ((file->header.file_size%BLOCK)&&1);
    file->content = (struct content*)malloc(sizeof(struct content) * (*content_size));
    if(file->content==NULL){
        free(file->content);
        *done=true;
        free(file);
        printf("content memory allocation error");
        return ;
    }

    for(int i=0;i<*content_size;++i){ // read the memory blocks for content
        char buffer[512];
        // printf("\n for2 content %d\n",i);
        if(fread(buffer,sizeof(*(buffer)),BLOCK,input) != BLOCK){
            printf("Error reading file content\n");
            free(file->content);
            *done=true;
            free(file);
          return;
        }
        strcpy(file->content[i].block,buffer);
        // printf("\ncontent\n%s\n",buffer);
        // printf("\ncursor %ld \n ",ftell(input));
    }
    // printf("content size: %d",*content_size);
}

void unpack(FILE* input){ // to verify for more than one file
    int content_size=0,header_counter=0,file_counter=1;
    bool done=false;
    struct files *file=(struct files*)malloc(sizeof(struct files));
    if(file == NULL){
        done=true;
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
            done=true;
            free(file);
            printf("error occured when allocating memory to the files");
            return ;
        }
        file=aux;
        process_header(input,&file[file_counter],&header_counter,&done);
        if(!done){
            process_content(input,&file[file_counter],&content_size,&done);
            // print_header(&file[file_counter]);
            // print_content(&file[file_counter],content_size);
            write_content(&file[file_counter],&content_size);
        }
    }while(!done);
    // printf("\n%d\n",file_counter);
    for(int i=0;i<=file_counter;i++)
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
        fclose(input);
        count++;
    }
    return 0;
}

//integration tests
void print_header(struct files* file){
    printf("\nheader:\n");
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