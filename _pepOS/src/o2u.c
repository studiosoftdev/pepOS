#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define nb 256

char tb[] = {'h', 'x', 'd'};

char mnum1[] = {0x0A, 0x32, 0x46, 0x55};
char mnum2[] = {0x9E, 0x5D, 0x51, 0x57};
char mnum3[] = {0x0A, 0xB1, 0x6F, 0x30};
char fID[] = {0xE4, 0x8B, 0xFF, 0x56};
char flags[] = {0x00, 0x00, 0x20, 0x00};
char nB[] = {0x00, 0x00, 0x01, 0x00}; //num bytes used in data, usually 256 but theres 476 (0x01DC) available bytes, is padded with 0s if not used all that space.
char sbn[] = {0x00, 0x00, 0x00, 0x00}; //sequential block number

int main(){
    FILE* os;
    if(os = fopen("build/pep.os", "rb")){
        printf("os loaded succesfully.\n");

        fseek(os, 0, SEEK_END);
        unsigned long len = ftell(os);
        rewind(os);
        printf("Filesize: %d\n", (int)len);

        //create buffer to store os file to read from in blocks
        char* buffer = (char*)malloc(sizeof(char) * len);

        if(buffer == NULL){
            fprintf(stderr, "MEM ERROR. File size is apparently non-existent how did that happen");
            return 1;
        }

        size_t result = fread(buffer, 1, len, os);
        if(result != len){
            fputs("Reading error", stderr);
            return 1;
        }

        FILE* uf2;

        if(uf2 = fopen("build/pepOS.uf2", "wb")){
            printf("uf2 loaded succesfully.\n");
            for(unsigned long i = 0; i < len; i += nb){
                //assemble each block as go along and write to uf2
                //printf("1. %d\n", i);
                char block[512];
                //printf("2. %d\n", i);
                for(int m = 0; m < 4; m++){
                    block[m] = mnum1[3-m];
                    block[m+4] = mnum2[3-m];
                    block[m+8] = flags[3-m]; //flags
                    block[m+12] = 0; //address in flash where data should be written - is this static?
                    block[m+16] = nB[3-m]; //num bytes used in data
                    block[m+28] = fID[3-m];
                    block[m+508] = mnum3[3-m];
                }
                block[24] = (len/nb)&0xFF;
                block[25] = (len/nb)&0xFF00;
                for(int x = 0; x < 3; x++){
                    block[24+x] = (((int)(ceil((double)(len/nb))+1)&(0xFF << (8 * x))) >> (8 * x));
                    block[20+x] = ((i/nb)&(0xFF << (8 * x))) >> (8 * x);
                }
                /*block[20] = (i/nb)&0xFF; //isolate the highest bytes of i to push to lowest bytes of sbn, because little endian
                block[21] = ((i/nb)&0xFF00) >> 8;
                block[22] = ((i/nb)&0xFF0000) >> 16;
                block[23] = ((i/nb)&0xFF000000) >> 24;*/
                //printf("3. sbn[%d]: %d\n", 0, (int)block[20]);
                //printf("4. %d, %d\n", i, (i/nb)&0xFF);

                //put the data in
                for(int n = i; n < i + nb; n++){ //copy the data in, in 256 byte chunks
                    block[n-i+28] = buffer[n]; //i follows len so can > 256
                }
                for(int n = nb; n < 476; n++){
                   block[n+28] = 0; //pad with 0s
                }
                for(int n = 0; n < 512; n++){
                    //printf("%d ", block[n]);
                }
                //printf("\n");
                fwrite(block, 512, sizeof(char), uf2);
                //printf("5. %d\n\n", block[20] + (block[21] << 8));
            }
            fclose(uf2);
        }

        else{
            printf("uf2 could not be loaded. Is it in build/pepos.uf2? Does the file have permission to be read?");
        }
        free(buffer);
        fclose(os);
    }

    else{
        printf("File could not be loaded. Is it in build/pep.os? Does the file have permission to be read?");
        return 1;
    }
    
    return 0;
}