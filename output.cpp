#include "output.h"
#include "global.h"

/*-------------------------------------------------------*/
int find_1(void)
{
 int i,j;
 j=0;
 for(i=0;i<S_1;i++)
 {
  if(processor->memory[PP]==len_1[i])
  {
   if (PP==processor->IP)
   {
    ////gotoxy(2,wherey());
    printf(">>");
   }
   if (PP!=processor->IP)
   {
    ////gotoxy(2,wherey());
    printf("  ");
   }
   put_hex4(PP);
   //printf("  ");
   while (comm[len_1[i]][j]!=0)
   {
    //printf("%c",comm[len_1[i]][j]);
    j++;
   }
   ////gotoxy(22,wherey());
   put_hex2(processor->memory[PP]);
   //printf("\n");
   PP++;
   ON++;
   return(1);
  }
 }
 return(NULL);
}
/*-------------------------------------------------------*/
int find_2(void)
{
 int i,j,k;
 j=0;
 for(i=0;i<S_2;i++)
 {
  if(processor->memory[PP]==len_2[i])
  {
   if (PP==processor->IP)
   {
    //gotoxy(2,wherey());
    printf(">>");
   }
   if (PP!=processor->IP)
   {
    //gotoxy(2,wherey());
    printf("  ");
   }
   put_hex4(PP);
   printf("  ");
   while (comm[ len_2[i]][j]!='%')
   {
    printf("%c",comm[len_2[i]][j]);
    j++;
   }
   j++;
   PP++;

   put_hex2(processor->memory[PP]);

   //gotoxy(22,wherey());
   put_hex2(processor->memory[PP-1]);
   printf(" ");
   put_hex2(processor->memory[PP]);
   printf("\n");
   PP++;
   ON=ON+2;
   return(1);
  }
 }
 return(NULL);
}
/*-------------------------------------------------------*/
int find_3(void)
{
 int i,j,k;
 j=0;
 for(i=0;i<S_3;i++)
 {
  if(processor->memory[PP]==len_3[i])
  {
   if (PP==processor->IP)
   {
    //gotoxy(2,wherey());
    printf(">>");
   }
   if (PP!=processor->IP)
   {
    //gotoxy(2,wherey());
    printf("  ");
   }
   put_hex4(PP);
   printf("  ");
   while (comm[len_3[i]][j]!='%')
   {
    printf("%c",comm[len_3[i]][j]);
    j++;
   }
   j++;
   PP++;
   put_hex2(processor->memory[PP+1]);
   PP++;
   put_hex2(processor->memory[PP-1]);
   //gotoxy(22,wherey());
   put_hex2(processor->memory[PP-2]);
   printf(" ");
   put_hex2(processor->memory[PP-1]);
   printf(" ");
   put_hex2(processor->memory[PP]);
   printf("\n");
   ON=ON+3;
   PP++;
   return(1);
  }
 }
 return(NULL);
}
/*-------------------------------------------------------*/
void put_inst(void)
{
 if( (find_3())==NULL)
  if( (find_2())==NULL)
   if( (find_1())==NULL)
     printf(" ");
}
/*-------------------------------------------------------*/
void show_prog(void)
{
 int i;
 //make_wind(4,2,16,28,15,' ',1);

 //gotoxy(2,3);
 printf("  ADDR INSTR       CODES     \n\n");
 for(i=0;i<16;i++)
 {
  PP%=MEMSIZE;
  put_inst();
 }
 show_regis();
 show_virtual_screen();
}
/*------------------------------------------------------*/
void show_regis(void)
{
 int i;
 //make_wind(4,32,11,9,15,' ',1);
 //gotoxy(33,3);
 printf("REGISTERS\n\n");

 //gotoxy(33,5);
 printf("  A=");
 put_hex2(processor->A);

 //gotoxy(33,6);
 printf(" BC=");
 put_hex2(processor->B);
 put_hex2(processor->C);

 //gotoxy(33,7);
 printf(" DE=");
 put_hex2(processor->D);
 put_hex2(processor->E);

 //gotoxy(33,8);
 printf(" HL=");
 put_hex2(processor->H);
 put_hex2(processor->L);

 //gotoxy(33,10);
 printf(" SP=");
 put_hex4(processor->SP);

 //gotoxy(33,11);
 printf(" IP=");
 put_hex4(processor->IP);

 //gotoxy(33,12);
 printf("같같같같같");
 //gotoxy(33,13);
 printf("  FLAGS  ");
 //gotoxy(33,14);
 printf(" C=%x Z=%x ",processor->flag.C,processor->flag.Z);
 //gotoxy(33,15);
 printf(" P=%x S=%x ",processor->flag.P,processor->flag.S);
}
/*------------------------------------------------------*/
void show_virtual_screen(void)
{
 //gotoxy(53,3);
// printf("  VIRTUAL SCREEN  \n\n");
// prik(4 ,44,"                            32*10 $",(3<<4)|5);
// prik(15,44,"   o o o o o                      $",(3<<4)|5);
// prik(5 ,44," $",(3<<4)|15);  prik(5 ,77," $",(3<<4)|15);
// prik(6 ,44," $",(3<<4)|15);  prik(6 ,77," $",(3<<4)|15);
// prik(7 ,44," $",(3<<4)|15);  prik(7 ,77," $",(3<<4)|15);
// prik(8 ,44," $",(3<<4)|15);  prik(8 ,77," $",(3<<4)|15);
// prik(9 ,44," $",(3<<4)|15);  prik(9 ,77," $",(3<<4)|15);
// prik(10 ,44," $",(3<<4)|15);  prik(10 ,77," $",(3<<4)|15);
// prik(11 ,44," $",(3<<4)|15);  prik(11 ,77," $",(3<<4)|15);
// prik(12 ,44," $",(3<<4)|15);  prik(12 ,77," $",(3<<4)|15);
// prik(13 ,44," $",(3<<4)|15);  prik(13 ,77," $",(3<<4)|15);
// prik(14 ,44," $",(3<<4)|15);  prik(14 ,77," $",(3<<4)|15);
// prik(16 ,59,"    $",(3<<4)|15);
// prik(17 ,50,"                      $",(3<<4)|15);

//  make_wind(4,44,12,34,15,'�',5);
//  make_wind(5,45,10,32,0,' ',0);

 unsigned int VMP=0x0800;
 for(int i=0;i<32;i++)
  for(int j=0;j<10;j++,VMP+=2)
   ;//prikol(5+j,45+i,processor->memory[VMP],processor->memory[VMP+1]);
}

/*------------------------------------------------------*/
void put_hex4(unsigned int dig)
{
 if (dig>=0x0000 && dig<=0x000f)  printf("000%x",dig);
 if (dig>=0x0010 && dig<=0x00ff)  printf("00%x",dig);
 if (dig>=0x0100 && dig<=0x0fff)  printf("0%x",dig);
 if (dig>=0x1000 && dig<=0xffff)  printf("%x",dig);
}

/*-----------------------------------------------------*/
void put_hex2(unsigned int dig)
{
 if (dig>=0x0000 && dig<=0x000f)  printf("0%x",dig);
 if (dig>=0x0010 && dig<=0x00ff)  printf("%x",dig);
}

