#include <stdio.h>
#include <stdlib.h>

#define MEMSIZE 65536

#define INC 1
#define DEC 2
#define JMP 3
#define JNZ 4
#define MOV 5
#define CPY 6
#define SET 7
#define OUT 8
#define INP 9
#define ADD 10
#define SUB 11

#define HLT 255

unsigned char program[65536];

/*= {
    SET, 0X00, 0X41, 0X0A, 0X00,
    INP, 0X00, 0X40, 0X00,
    DEC, 0X00, 0X40, 0X00,
    OUT, 0X00, 0X40, 0X01, 0X00,
    OUT, 0X00, 0X41, 0X00, 0X00,
    JNZ, 0X00, 0X09, 0X00, 0X40, 0X00,
    HLT
};*/

unsigned char mem[MEMSIZE];
unsigned long long count = 0;

void initTape() {
    for (int i=0; i!=65536; i++)
        mem[i] = 0;
}

/*void loadProgram(unsigned char p[], long size) {
    unsigned short i = 0;
    while (i != size) {
        mem[i] = p[i];
        i++;
        //printf("%d", i);
    }
    mem[i] = p[i];
}*/

void executeProgram() {
    unsigned short pc = 0;
    unsigned short addrT1 = 0;
    unsigned short addrT2 = 0;
    unsigned short addr1 = 0;
    unsigned short addr2 = 0;
    unsigned char mode1 = 0;
    unsigned char mode2 = 0;
    unsigned char mode3 = 0;

    unsigned char hi = 0;
    unsigned char lo = 0;
    unsigned char in = 0;

    char run = 1;
    count = 0;

    while (/*mem[pc] != HLT*/ run) {
        count++;
        //printf("PC=x%x | x%x\n", pc, mem[pc]);
        //scanf("");

        addr1 = (mem[pc+1]<<8)|mem[pc+2];
        addrT1 = addr1;
        addr2 = (mem[pc+3]<<8)|mem[pc+4];
        addrT2 = addr2;
        mode1 = mem[pc+3];
        mode2 = mem[pc+4];
        mode3 = mem[pc+5];

        switch (mem[pc]) {
        case INC:
            //printf("INC DETECTED %d\n", addr1);
            switch (mode1) {
            case 0x1:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
            }
            mem[addrT1]++;
            pc+=4;
            break;
        case DEC:
            switch (mode1) {
            case 0x1:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
            }
            mem[addrT1]--;
            pc+=4;
            break;
        case JMP:
            switch (mode1) {
            case 0x1:
                //addrT1 = (mem[pc+1]<<8)|mem[pc+2];
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
                break;
            }
            pc = addrT1;
            //pc+=3;
            break;
        case JNZ:
            switch (mode3) {
            case 0x1:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
                break;
            case 0x2:
                hi = mem[addrT2];
                lo = mem[addrT2+1];
                addrT2 = (hi<<8)|lo;
                break;
            case 0x3:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
                hi = mem[addrT2];
                lo = mem[addrT2+1];
                addrT2 = (hi<<8)|lo;
                break;
            }

            if (mem[addrT2] != 0) {
                pc = addrT1;
            } else {
                pc+=6;
            }
            break;
        case MOV:
            switch (mode3) {
            case 0x1:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
                break;
            case 0x2:
                hi = mem[addrT2];
                lo = mem[addrT2+1];
                addrT2 = (hi<<8)|lo;
                break;
            case 0x3:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
                hi = mem[addrT2];
                lo = mem[addrT2+1];
                addrT2 = (hi<<8)|lo;
                break;
            }
            mem[addrT2] = mem[addrT1];
            mem[addrT1] = 0;
            pc+=5;
            break;
        case CPY:
            switch (mode3) {
            case 0x1:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
                break;
            case 0x2:
                hi = mem[addrT2];
                lo = mem[addrT2+1];
                addrT2 = (hi<<8)|lo;
                break;
            case 0x3:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
                hi = mem[addrT2];
                lo = mem[addrT2+1];
                addrT2 = (hi<<8)|lo;
                break;
            }
            mem[addrT2] = mem[addrT1];
            pc+=6;
            break;
        case SET:
            switch (mode2) {
            case 0x1:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
            }
            mem[addrT1] = mode1;
            pc+=5;
            break;
        case OUT:
            switch (mode2) {
            case 0x1:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
            }
            switch (mode1) {
            case 0x0:
                printf("%c", mem[addrT1]);
                break;
            case 0x1:
                printf("%u", mem[addrT1]);
                break;
            case 0x2:
                printf("%x", mem[addrT1]);
                break;
            }
            pc+=5;
            break;
        case INP:
            switch (mode1) {
            case 0x1:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
            }
            scanf("%d", &in);
            mem[addrT1] = in;
            pc+=4;
            break;
        case ADD:
            switch (mode3) {
            case 0x1:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
                break;
            case 0x2:
                hi = mem[addrT2];
                lo = mem[addrT2+1];
                addrT2 = (hi<<8)|lo;
                break;
            case 0x3:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
                hi = mem[addrT2];
                lo = mem[addrT2+1];
                addrT2 = (hi<<8)|lo;
                break;
            }
            mem[addrT1] += mem[addrT2];
            pc+=6;
            break;
        case SUB:
            switch (mode3) {
            case 0x1:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
                break;
            case 0x2:
                hi = mem[addrT2];
                lo = mem[addrT2+1];
                addrT2 = (hi<<8)|lo;
                break;
            case 0x3:
                hi = mem[addrT1];
                lo = mem[addrT1+1];
                addrT1 = (hi<<8)|lo;
                hi = mem[addrT2];
                lo = mem[addrT2+1];
                addrT2 = (hi<<8)|lo;
                break;
            }
            mem[addrT1] -= mem[addrT2];
            pc+=6;
            break;
        case HLT:
            run = 0;
            break;
        default:
            printf("Unrecognised opcode x%x (@x%x)\n", mem[pc], pc);
            count--;
            pc++;
        }
    }
}

int main(int argc, char *argv[])
{
    printf("Initialising %d bytes...\n", MEMSIZE);
    initTape();
    printf("Initialised!\n");

    /*for (int i=0; i!=65536; i++) {
        //printf("Byte %d = %d\n", i, mem[i]);
        program[i] = 0;
    }*/

    char *filepath = argv[1];
    FILE *file = fopen(filepath, "rb");

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    /*for (int i=0; i<=argc; i++) {
        printf("Argument %d | %s\n",i,  argv[i]);
    }*/

    /*unsigned char byte;
    short i = 0;
    while (byte=fgetc(file) != EOF) {
        program[i] = byte;
        i++;
    }*/

    fread(program, 1, size, file);
    fclose(file);

    /*for (int i=0; i!=128; i++) {
        printf("PByte %d = %d\n", i, program[i]);
    }*/

    /*printf("%s", program);

    for (int i=0; i!=128; i++) {
        printf("MByte %d = %d\n", i, mem[i]);
    }*/

    printf("Loading program...\n");
    //loadProgram(program);
    unsigned short i = 0;
    while (i != size) {
        mem[i] = program[i];
        i++;
        //printf("%d", i);
    }
    mem[i] = program[i];

    printf("Program loaded!\n");

    /*for (int i=0; i!=128; i++) {
        printf("MByte %d = %d\n", i, mem[i]);
    }*/

    printf("Executing...\n\n");
    executeProgram();
    printf("\n\nExecution finished in %llu instruction(s)!\n", count);

    /*for (int i=0; i!=128; i++) {
        printf("Byte %d = %d\n", i, mem[i]);
    }*/
    while (1) {
        printf("");
    }
}
