#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define size 100

struct Memory {
    int addr;
    char val[10];
};

int main()
{
    FILE *ptr, *imf, *symf, *opf;
    int startaddr = 0x0000;
    char line[size], temp[size], value_m[50], v1[2],v2[2],v3[2];
    strcpy(value_m, "");
    struct Memory m[500];
    int mi = 0, addr_count= 0;
    char Hex_str[16] = "0123456789ABCDEF";
    int mask[12];
    
    printf("Enter The Starting Address: ");
    scanf("%x", &startaddr);
    
    ptr = fopen("object_code.txt","r");
    if(ptr == NULL)
    {
        printf("\nERROR IMF file");
        return 1;
    }
    
    opf = fopen("memory.txt","wb+");
    if(opf == NULL)
    {
        printf("\nERROR IMF file");
        return 1;
    }
    
    
    
    while(fgets(line,size,ptr)!=NULL){
        if(line[0] != 'E'){
            if(line[0] == 'T'){
                // printf("%c%c%c\n", line[12],line[13],line[14]);
                value_m[0] = line[12];
                value_m[1] = line[13];
                value_m[2] = line[14];
                value_m[3] = '\0';
                
                int tempwhile = (int)strtol(value_m, NULL, 16);
                
                for(int i = 11; i>=0; i--){
                    mask[i] = tempwhile%2;
                    tempwhile = tempwhile/2;
                }
                
                int maski = 0;
                
                for(int i = 16; i<strlen(line); i++){
                    // Add Opcode to Memory
                    if(line[i] == '\n'){
                        break;
                    }
                    m[mi].addr = addr_count + startaddr;
                    addr_count++;
                    m[mi].val[0] = line[i];
                    i++;
                    m[mi].val[1] = line[i];
                    i++;
                    m[mi].val[2] = '\0';
                    // printf("%s %d\n", m[mi].val,line[i]);
                    mi++;
                    
                    if(line[i] == 13){
                        break;
                    }
                    
                    if(line[i] == '\n'){
                        break;
                    }
                    
                    if(line[i]!='^'){
                        value_m[0] = line[i];
                        i++;
                        value_m[1] = line[i];
                        i++;
                        value_m[2] = line[i];
                        i++;
                        value_m[3] = line[i];
                        i++;
                        value_m[4] = '\0';
                        
                        tempwhile = (int)strtol(value_m, NULL, 16);
                        
                        if( mask[maski] == 1){
                            tempwhile += startaddr;   
                        }
                        
                        // printf("val: %x\n", tempwhile);
                        
                        m[mi].addr = addr_count + startaddr;
                        addr_count++;
                        m[mi+1].val[1] = Hex_str[tempwhile%16];
                        tempwhile = tempwhile/16;
                        // i++;
                        m[mi+1].val[0] = Hex_str[tempwhile%16];
                        tempwhile = tempwhile/16;
                        // i++;
                        m[mi+1].val[2] = '\0';
                        mi++;
                        
                        m[mi].addr = addr_count + startaddr;
                        addr_count++;
                        m[mi-1].val[1] = Hex_str[tempwhile%16];
                        tempwhile = tempwhile/16;
                        // i++;
                        m[mi-1].val[0] = Hex_str[tempwhile%16];
                        tempwhile = tempwhile/16;
                        // i++;
                        m[mi].val[2] = '\0';
                        mi++;
                        
                        
                    }
                    if(line[i] == '\n'){
                        break;
                    }
                    maski++;
                }
            }
        }
        printf("%s", line);
    }
    
    for(int i  = 0; i<mi; i++){
        fprintf(opf,"%x %c%c\n", m[i].addr, m[i].val[0],m[i].val[1]);
        // printf("%x %c%c\n", m[i].addr, m[i].val[0],m[i].val[1]);
        
    }

    return 0;
}
