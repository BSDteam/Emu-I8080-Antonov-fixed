#include "INPUT.h"
#include "GLOBAL.h"

//---------------------------------------------------------------------------
int strfind(char *str1,char *str2)
{
int len1,len2,p1,p2,fl;
len1=strlen(str1);
len2=strlen(str2);
 for(p2=0;p2<len2;p2++)
 { fl=0;
   for(p1=0;p1<len1;p1++)
    if (str1[p1]!=str2[p1+p2]) fl=1;
   if (fl==0) return(0);
 }
return(1);
}

//---------------------------------------------------------------------------
char check( char *number)
{
  if(strlen(number)>4) return 1;//wrong length
  for(int i=0;i<strlen(number);i++)
   if( (number[i]>='0' && number[i]<='9') || (number[i]>='A' && number[i]<='F') );
   else return 1; //wrong char
  return 0; //all is ok
}

//---------------------------------------------------------------------------
void put_num( char *number,int index,QAbstractItemModel *model)
{
    QStandardItem *    BlockItem;


    bool find2 = false;
    for (int i=0; i<len_3_count; i++)
    {
        if (len_3[i]==processor->memory[index-1])
        {
            find2=true;
            break;
        }
    }

  QString str=QString::number(processor->memory[index-1],16);
  model->setData(model->index(index-1,2),str);
    GL_STR=QString(comm[processor->memory[index-1]]);
    GL_STR.remove("%");
  char *endptr;

  bool find = false;
  for (int i=0; i<len_3_count; i++)
  {
      if (len_3[i]==processor->memory[index-1])
      {
          find=true;
          break;
      }
  }

  if(!find)//strlen(number)<=2
  {

   processor->memory[index++]=strtoul(number,&endptr,16);
   QString str=QString::number(processor->memory[index-1],16);
   model->setData(model->index(index-1,2),str);
   if (str.length()==1) str="0"+str;
   GL_STR+=str;


   BlockItem= new QStandardItem ( QString("^^^") );
   BlockItem->setBackground(QBrush(QColor(255,0,0,30)));
   BlockItem->setEditable(false);
   GL_MODEL->setItem(index-1,1,BlockItem);
   if (!find2)
   {
       GL_MW->DisplayCommand(index,1);

   }
  }
   else
  {
    unsigned int temp=strtoul(number,&endptr,16);
    processor->memory[index++]=(unsigned char)temp;
    QString str1=QString::number(processor->memory[index-1],16);
    model->setData(model->index(index-1,2),str1);
    BlockItem= new QStandardItem ( QString("^^^") );
    BlockItem->setBackground(QBrush(QColor(255,0,0,30)));
    BlockItem->setEditable(false);
    GL_MODEL->setItem(index-1,1,BlockItem);


    processor->memory[index++]=(unsigned char)(temp>>8);
    QString str2=QString::number(processor->memory[index-1],16);
    if (str2.length()==1) str2="0"+str2;
    model->setData(model->index(index-1,2),str2);
    BlockItem= new QStandardItem ( QString("^^^") );
    BlockItem->setBackground(QBrush(QColor(255,0,0,30)));
    BlockItem->setEditable(false);
    GL_MODEL->setItem(index-1,1,BlockItem);
    if (str1.length()==1) str1="0"+str1;
    GL_STR+=str2+str1;

    GL_MW->DisplayCommand(index,1);

  }
   global_flag=false;
}

//---------------------------------------------------------------------------
void input( char in[20],int index,QAbstractItemModel *model)
{
    global_flag=true;
  char *ptr;
  in=_strupr(in);

//check one byte commands
  for(int i=0;i<212;i++)
  {
    if (strcmp(comm[len_1[i]],in)==0)
    {
        bool find=false;
        for (int j=0; j<len_1_count; j++)
            if (len_1[j]==processor->memory[index])
            {
                find=true;
                break;
            }

        GL_STR=QString(comm[len_1[i]]);
      processor->memory[index++]=len_1[i];
      QString str=QString::number(processor->memory[index-1],16);
      model->setData(model->index(index-1,2),str);
      // processor->IP--;


      if (!find)
      {
            GL_MW->DisplayCommand(index,1);

      }

        global_flag=false;
      return;
    }
  }

//check other commands
  ptr=in;
  while(*ptr!=',')
  { if( (*ptr=='\n')||(*ptr=='\0') )
    { ptr=in;
      while(*ptr!=' ') ptr++;
      break;
    }
    else ptr++;
  }


  if(check(++ptr)==1) return;


  if (strfind("MVI A,",in)==0)
  {
    processor->memory[index++]=0x3e;
    put_num(ptr,index,model);

    //processor->IP--;


    return;
  }

  if (strfind("MVI B,",in)==0)
  {
    processor->memory[index++]=0x06;
    put_num(ptr,index,model);
    //processor->IP--;

    return;
  }
  if (strfind("MVI C,",in)==0)
  {
    processor->memory[index++]=0x0e;
    put_num(ptr,index,model);
    //processor->IP--;


    return;
  }
  if (strfind("MVI D,",in)==0)
  {
    processor->memory[index++]=0x16;
    put_num(ptr,index,model);
    //processor->IP--;



    return;
  }
  if (strfind("MVI E,",in)==0)
  {
    processor->memory[index++]=0x1e;
    put_num(ptr,index,model);
    //processor->IP--;


    return;
  }
  if (strfind("MVI H,",in)==0)
  {
    processor->memory[index++]=0x26;
    put_num(ptr,index,model);
    //processor->IP--;


    return;
  }
  if (strfind("MVI L,",in)==0)
  {
    processor->memory[index++]=0x2e;
    put_num(ptr,index,model);
    //processor->IP--;


    return;
  }

  if (strfind("MVI M,",in)==0)
  {
    processor->memory[index++]=0x36;
    put_num(ptr,index,model);
    //processor->IP--;

    return;
  }
  if (strfind("ADI ",in)==0)
  {
    processor->memory[index++]=0xc6;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("ACI ",in)==0)
  {
    processor->memory[index++]=0xce;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }

  if (strfind("SUI ",in)==0)
  {
    processor->memory[index++]=0xd6;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("SBI ",in)==0)
  {
    processor->memory[index++]=0xde;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("ANI ",in)==0)
  {
    processor->memory[index++]=0xe6;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("XRI ",in)==0)
  {
    processor->memory[index++]=0xee;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("ORI ",in)==0)
  {
    processor->memory[index++]=0xf6;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("CPI ",in)==0)
  {
    processor->memory[index++]=0xfe;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }

  if (strfind("OUT ",in)==0)
  {
    processor->memory[index++]=0xd3;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("IN ",in)==0)
  {
    processor->memory[index++]=0xdb;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("LXI B,",in)==0)
  {
    processor->memory[index++]=0x01;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("LXI D,",in)==0)
  {
    processor->memory[index++]=0x11;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("LXI H,",in)==0)
  {
    processor->memory[index++]=0x21;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("LXI SP,",in)==0)
  {
    processor->memory[index++]=0x31;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("LHLD ",in)==0)
  {
    processor->memory[index++]=0x2a;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("SHLD ",in)==0)
  {
    processor->memory[index++]=0x22;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("STA ",in)==0)
  {
    processor->memory[index++]=0x32;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("LDA ",in)==0)
  {
    processor->memory[index++]=0x3a;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("JNZ ",in)==0)
  {
    processor->memory[index++]=0xc2;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("JMP ",in)==0)
  {
    processor->memory[index++]=0xc3;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("CNZ ",in)==0)
  {
    processor->memory[index++]=0xc4;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("JZ ",in)==0)
  {
    processor->memory[index++]=0xca;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("CZ ",in)==0)
  {
    processor->memory[index++]=0xcc;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("CALL ",in)==0)
  {
    processor->memory[index++]=0xcd;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("JNC ",in)==0)
  {
    processor->memory[index++]=0xd2;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("CNC ",in)==0)
  {
    processor->memory[index++]=0xd4;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("JC ",in)==0)
  {
    processor->memory[index++]=0xda;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("CC ",in)==0)
  {
    processor->memory[index++]=0xdc;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("JPO ",in)==0)
  {
    processor->memory[index++]=0xe2;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("CPO ",in)==0)
  {
    processor->memory[index++]=0xe4;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("JPE ",in)==0)
  {
    processor->memory[index++]=0xea;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("CPE ",in)==0)
  {
    processor->memory[index++]=0xec;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("JP ",in)==0)
  {
    processor->memory[index++]=0xf2;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("CP ",in)==0)
  {
    processor->memory[index++]=0xf4;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("JM ",in)==0)
  {
    processor->memory[index++]=0xfa;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
  if (strfind("CM ",in)==0)
  {
    processor->memory[index++]=0xfc;
    put_num(ptr,index,model);
    // processor->IP--;
    return;
  }
}

