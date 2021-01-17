#include <stdio.h>
#include <stdlib.h>

#define MOVE 0
#define ADD 1
#define SUB 2
#define BRA 3
#define CMP 4
#define BEQ 5
#define BNE 6
#define EXG 7
#define OR  8      /* additional work */
#define XOR  9     /* additional work */
#define AND  10    /* additional work */
#define STOP 15
void main(void)
{
    unsigned short int PC =0;
    unsigned short int D0 =0;
    unsigned short int A0 =0;
    unsigned short int CCR =0;
    unsigned short int MAR;
    unsigned short int MBR;
    unsigned short int IR;
    unsigned short int operand;
    unsigned short int source;
    unsigned short int destination;
    unsigned short int opcode;
    unsigned short int amode;
    unsigned short int direction;
    unsigned short int memory[256];
    unsigned short int run = 1;

    /*Filling up memory array with zeros to clear it...*/
    int n= sizeof(memory)/sizeof(memory[0]);
    for(int i=10;i<n;i++){
       memory[i]=0;
    }
    /*Filling up memory...*/
    memory[0]= 0b00000101;//  MOVE THE NEXT OPERAND TO D0
    memory[1]= 0b00001010;//  OPERAND = 10
    memory[2]= 0b00010101;//  ADD THE NEXT OPERAND TO D0
    memory[3]= 0b00000001;//  OPERAND = 1
    memory[4]= 0b00000000;//  MOVE content of D0 to the below memory location
    memory[5]= 0b00011001;//  memory[25]

    memory[6]= 0b10100100;//  AND memory[15],D0
    memory[7]= 0b00001111;//  memory[15]= 7
    memory[8]= 0b00100101;//  SUB #2,D0
    memory[9]= 0b00000010;//  2
    memory[10]= 0b10000100;//  OR memory[16],D0
    memory[11]= 0b00010000;//  memory[16]= 6
    memory[12]= 0b10010100;//  XOR memory[17],D0
    memory[13]= 0b00010001;//  memory[17]=10

    memory[14]= 0b11110000;//  STOP
    memory[15]= 0b00000111;// memory[15]=7
    memory[16]= 0b00000110;// memory[16]=6
    memory[17]= 0b00001010;// memory[17]=10

    while(run){
        MAR = PC;
        PC = PC+1;
        MBR =memory[MAR];
        IR = MBR;
        opcode = IR;
        MAR = PC;
        PC = PC+1;
        MBR =memory[MAR];
        IR=MBR;
        operand=IR;
        amode = opcode & 0x03;
        direction = (opcode & 0x04)>>2;
        opcode = opcode>>4;

        switch(amode){
          case 0: {source=memory[operand];    break;}
          case 1: {source=operand;            break;}
          case 2: {source=memory[A0+operand]; break;}
          case 3: {source=memory[PC+operand]; break;}
        }

        switch(opcode){
          case MOVE: { if(direction==0) destination=D0;
                       else D0=source;
                       if (D0==0) CCR=1; else CCR=0;
                       break;
                       }
          case ADD: { if(direction==0){
                        destination=D0+source;
                        if(destination==0) CCR=1;  else CCR=0;
                       }
                       else {
                          D0=D0+source;
                          if(D0==0)  CCR=1; else CCR=0;
                       }
                       break;
                       }
          case SUB: { if(direction==0){
                        destination=D0-source;
                        if(destination==0) CCR=1;  else CCR=0;
                       }
                       else {
                          D0=D0-source;
                          if(D0==0)  CCR=1; else CCR=0;
                       }
                       break;
                       }
          case BRA: { if(amode==0) PC=operand;
                      if(amode==1) PC=PC+operand;   break;}
          case CMP: { MBR=D0-source;
                      if(MBR==0) CCR=1;
                      else CCR=0;  break;}
          case BEQ:{ if(CCR==1)
                      {if(amode==0) PC=operand;
                      if(amode==1) PC=PC+operand;
                      }break;
                   }
          case BNE:{ if(CCR!=1)
                      {if(amode==0) PC=operand;
                      if(amode==1) PC=PC+operand;
                      }break;
                   }
          case EXG:{ MBR =D0;  D0=A0; A0=MBR; break;
                   }
          case AND: { if(direction==0){                         /* AND Implementation*/
                        destination=D0&source;                   // perform D0 AND SOURCE IF DIRECTION =0 AND ASSIGN RESULT TO DESTINATION
                        if(destination==0) CCR=1;  else CCR=0;   // SET CCR TO 1 IF DESTINATION =0 ELSE SET CCR TO 0
                       }
                       else {
                          D0=D0&source;                          // ELSE perform D0 AND SOURCE AND ASSIGN RESULT TO D0
                          if(D0==0)  CCR=1; else CCR=0;          // SET CCR TO 1 IF D0 =0 ELSE SET CCR TO 0
                       }
                       break;
                   }
          case OR: { if(direction==0){                          /* OR Implementation*/
                        destination=D0|source;                    // perform D0 OR SOURCE IF DIRECTION =0 AND ASSIGN RESULT TO DESTINATION
                        if(destination==0) CCR=1;  else CCR=0;    // SET CCR TO 1 IF DESTINATION =0 ELSE SET CCR TO 0
                       }
                       else {
                          D0=D0|source;                           // ELSE perform D0 OR SOURCE AND ASSIGN RESULT TO D0
                          if(D0==0)  CCR=1; else CCR=0;           // SET CCR TO 1 IF D0 =0 ELSE SET CCR TO 0
                       }
                       break;
                   }
           case XOR: { if(direction==0){                        /* XOR Implementation*/
                        destination=D0^source;                  // perform D0 XOR SOURCE IF DIRECTION =0 AND ASSIGN RESULT TO DESTINATION
                        if(destination==0) CCR=1;  else CCR=0;  // SET CCR TO 1 IF DESTINATION =0 ELSE SET CCR TO 0
                       }
                       else {
                          D0=D0^source;                         // ELSE perform D0 XOR SOURCE AND ASSIGN RESULT TO D0
                          if(D0==0)  CCR=1; else CCR=0;         // SET CCR TO 1 IF D0 =0 ELSE SET CCR TO 0
                       }
                       break;
                   }

          case STOP:{run=0; break;
                   }
        }

        if(direction==0)
        switch(amode){
          case 0: {memory[operand]=destination;  break;}
          case 1: { break;}
          case 2: {memory[A0+operand]=destination;  break;}
          case 3: {memory[PC+operand]=destination;  break;}
        }
        printf("The value of D0 = %i\n\n\n",D0);
    }
     printf("The value of memory[25] = %i\n",memory[25]);
     printf("Final result of D0 = %i \n",D0);
}
