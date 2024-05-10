#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define size 100

struct Iline {
    char label[20], opcode[20], operand[20];
    int locctr;
};

struct symT {
    char label[20];
    int locctr;
};

struct optT {
    char opcode[20];
    int val;
};

// Global arrays to store opcode and symbol information
struct optT OPTAB[size];
int ozi = 0;
struct symT SYMTAB[size];
int szi = 0;
int pgmlen = 0;
int finc = 0;

// Function to get the opcode value from OPTAB
int getopcode(char *c){
    for(int i = 0; i< ozi; i++){
        if(strcmp(OPTAB[i].opcode,c) == 0){
            return OPTAB[i].val;
        }
    }
    return 0;
}

// Function to get the register value
int getRegsterVal(char c){
    switch(c){
        case 'A':
            return 0;
            break;
        case 'X':
            return 1;
            break;
        case 'L':
            return 2;
            break;
        case 'B':
            return 3;
            break;
        case 'S':
            return 4;
            break;
        case 'T':
            return 5;
            break;
        case 'F':
            return 6;
            break;
    }
    return 0;
}

// Main function for pass 2 of the relocation loader
int pass2(){
    FILE *ptr, *imf, *symf, *opf;
    char line[size];
    char t1[size];
    char t2[size];
    char t3[size];
    char t4[size];
    struct Iline l1, l2;

    // File handling: Opening necessary files
    imf = fopen("imf.txt","r");
    if(imf == NULL)
    {
        printf("\nERROR IMF file");
        return 1;
    }

    ptr = fopen("object.txt","wb+");
    if(ptr == NULL)
    {
        printf("\nERROR IMF file");
        return 1;
    }

    opf= fopen("OPTAB.txt","r");
    if(opf == NULL)
    {
        printf("\nERROR");
        return 1;
    }

    symf= fopen("SYMTAB.txt","r");
    if(symf == NULL)
    {
        printf("\nERROR");
        return 1;
    }
    
    // Reads lines from SYMTAB.txt and populates the SYMTAB array.
    while(fgets(line, size, symf) != NULL){
        int temp = sscanf(line, "%s %s", t1, t2);
        SYMTAB[szi].locctr = (int)strtol(t2, NULL, 16);
        strcpy(SYMTAB[szi].label, t1);
        szi++;
    }

    // Reads lines from OPTAB.txt and populates the OPTAB array.
    while(fgets(line, size, opf) != NULL){
        int temp = sscanf(line, "%s %s", t1, t2);
        OPTAB[ozi].val = (int)strtol(t2, NULL, 16);
        strcpy(OPTAB[ozi].opcode, t1);
        ozi++;
    }

    if(fgets(line, size, imf) != NULL){
        int temp = sscanf(line, "%s %s %s %s", t4, t1, t2, t3);
        if(temp == 4){
            l1.locctr = (int)strtol(t4, NULL, 16);
            strcpy(l1.label, t1);
            strcpy(l1.opcode, t2);
            strcpy(l1.operand, t3);
        } else {
            // Handling invalid line
            if(temp == 3){
                l1.locctr = (int)strtol(t4, NULL, 16);
                strcpy(l1.opcode, t1);
                strcpy(l1.operand, t2);
            } else {
                if(temp == 2){
                    l1.locctr = (int)strtol(t4, NULL, 16);
                    strcpy(l1.opcode, t1);
                } else {
                    printf("INVALID Line: %s\n", line);
                    return 0;
                }
            }
        }
        if(strcmp(l1.opcode, "START") == 0){
            // Handling START instruction
            fprintf(ptr, "H^%s^%s^%x\n", l1.label, l1.operand, pgmlen);
            finc = (int)strtol(l1.operand, NULL, 16);

            l1.locctr = 0;
            strcpy(l1.label, "");
            strcpy(l1.opcode, "");
            strcpy(l1.operand, "");

            fgets(line, size, imf);
            int temp = sscanf(line, "%s %s %s %s", t4, t1, t2, t3);
            if(temp == 4){
                l1.locctr = (int)strtol(t4, NULL, 16);
                strcpy(l1.label, t1);
                strcpy(l1.opcode, t2);
                strcpy(l1.operand, t3);
            } else {
                // Handling invalid line
                if(temp == 3){
                    l1.locctr = (int)strtol(t4, NULL, 16);
                    strcpy(l1.opcode, t1);
                    strcpy(l1.operand, t2);
                } else {
                    if(temp == 2){
                        l1.locctr = (int)strtol(t4, NULL, 16);
                        strcpy(l1.opcode, t1);
                    } else {
                        printf("INVALID Line: %s\n", line);
                        return 0;
                    }
                }
            }
        }
    }
    fclose(imf);
    fclose(ptr);
    fclose(opf);
    fclose(symf);
    return 0;
}

// Main function for the relocation loader
int main(){
    int s = pass2();
    if(s == 0){
        printf("Successful Execution\n");
    } else {
        printf("Error Encountered\n");
    }
    return 0;
}
