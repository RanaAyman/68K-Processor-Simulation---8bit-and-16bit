#include <stdio.h>
#include <stdlib.h>

#define LDA 0
#define STA 1
#define ADD 2
#define BRA 3
#define BEQ 4
#define XOR 5   // additional work <<<<<<<
#define AND 6   // additional work <<<<<<<
#define STOP 7
void main(void)
{

    unsigned short int PC =0;       /*program counter*/
    unsigned short int D0 =0;       /*Data register*/
    unsigned short int MAR;         /*memory address register*/
    unsigned short int MBR;         /*memory buffer register*/
    unsigned short int IR;          /*instruction register*/
    unsigned short int operand;     /*the 8-bit operand from the IR*/
    unsigned short int source;      /*source operand*/
    unsigned short int opcode;      /*the 3-bit op-code from the IR*/
    unsigned short int amode;       /*the 1-bit addressing mode*/
    unsigned short int memory[16];  /*the memory*/
    unsigned short int run = 1;     /*execute program while run is 1*/

    //fill memory array will zeros to clear it...
    int n= sizeof(memory)/sizeof(memory[0]);
    for(int i=0;i<n;i++){
       memory[i]=0;
    }

    //Filling up the memory
    memory[0] = 0b00011111;//   LDA #15,D0  >>>> D0 = 15
    memory[1] = 0b01000111;//   ADD  7,D0   >>>> D0 = 21
    memory[2] = 0b10101100;//   XOR  12,D0  >>>> D0 = 27
    memory[3] = 0b11011101;//   AND  #13,D0  >>>> D0 = 9
    memory[4] = 0b00100111;//   STR D0,7    >>>> memory[7] = 9
    memory[5] = 0b11110000;//   STOP

    memory[7]= 0b00000110;// memory[7]= 6
    memory[12]= 0b00001110;// memory[12]= 14
    while(run){
        MAR = PC;
        PC = PC+1;
        MBR =memory[MAR];
        IR = MBR;

        opcode = IR>>5;
        amode = (IR & 0x10)>>4;
        operand = IR & 0x0F;

        if(amode == 0){
            source = memory[operand];
        }else{
            source = operand;
        }

        switch(opcode){
          case LDA: { D0 =source;               break;}
          case STA: { memory[operand]=D0;       break;}
          case ADD: { D0 = D0 + source ;        break;}
          case BRA: { PC =operand;              break;}
          case BEQ: { if(D0 ==0) PC = operand;  break;}
          case XOR: { D0 = D0 ^ source ;        break;} // XOR Implementation
          case AND: { D0 = D0 & source ;        break;} // AND Implementation
          case STOP:{ run=0;                    break;}
        }
       // break;
       printf("The value of D0 = %i \n\n",D0);
    }
    printf("The value of memory[7] = %i \n",memory[7]);
    printf("Final result of D0 = %i \n",D0);
}
