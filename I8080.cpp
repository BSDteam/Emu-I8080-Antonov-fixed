#include "I8080.h"
#include "in08.h"
#include "mainwindow.h"

//---------------------------------------------------------------------------
MICROPROC::MICROPROC(QTableView *mainTable,QTextBrowser * out10,MainWindow *mw ):MainTable(mainTable),Out10(out10),Mw(mw)
{

 memory.SetTable(mainTable);

 for(unsigned int i=0;i<MEMSIZE;i++) memory[i]=0;

 IP=Default_IP;
 SP=Default_SP;
}

MICROPROC::~MICROPROC()
{
   //delete (memory);
}

QString MICROPROC::put_hex4(unsigned int dig)
{
 QString result,buf;
 if (dig>=0x0000 && dig<=0x000f)
  result+=buf.sprintf("000%x",dig);
 if (dig>=0x0010 && dig<=0x00ff)
  result+=buf.sprintf("00%x",dig);
 if (dig>=0x0100 && dig<=0x0fff)
  result+=buf.sprintf("0%x",dig);
 if (dig>=0x1000 && dig<=0xffff)
  result+=buf.sprintf("%x",dig);
 return result;
}

QString MICROPROC::put_hex2(unsigned int dig)
{
 QString result,buf;
 if (dig>=0x0000 && dig<=0x000f)  result+=buf.sprintf("0%x",dig);
 if (dig>=0x0010 && dig<=0x00ff)  result+=buf.sprintf("%x",dig);
 return result;
}

//---------------------------------------------------------------------------
void MICROPROC::set_flags(int number)
{
  if( (number&0x100)==0x100 )
  {
      flag.C=1;//check xxxxxxx1xxxxxxxx
  }
  else
  {
      flag.C=0;
  }

  number&=0xFF;

  if(number<0x80)
  {
      flag.S=0;
  }
  else
  {
      flag.S=1;
  }

  if(number==0)
  {
      flag.Z=1;

  }
  else
  {
      flag.Z=0;
  }

  flag.P=0;
  for(int i=0;i<8;i++)
  { flag.P^=number&0x01;
    number>>=1;
   }
  flag.P^=0x01;
}

//-----------------ALU-------------------------------------------------------
int MICROPROC::alu(unsigned char code)
{
  switch(code)
  {
    //nop
    case 0x00: { IP++;

           } break;
    //lxi b,&
    case 0x01: {

         C=memory[++IP];
         B=memory[++IP];
         IP++;

           }break;
    //stax b
    case 0x02: {

         memory[B*256+C]=A;
         buf=QString::number(A,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(B*256+C,2),buf,Qt::EditRole);

         IP++;
           }break;
    //inx b
    case 0x03: {
        if (C==0xFF)
        {
            B++;
        }
         C++;
         IP++;
           }break;
    //inr b (do not influence flag c)
    case 0x04: {
         unsigned char temp=flag.C;
         set_flags(B+1);
         flag.C=temp;
         B++;
         IP++;
           }break;
    //dcr b
    case 0x05: { unsigned char temp=flag.C;
         set_flags(B-1);
         flag.C=temp;

         B--;
         IP++;
           }break;
    //mvi b
    case 0x06: { B=memory[++IP];
         IP++;
           }break;
    //rlc
    case 0x07: {
            flag.C=A&0x80;
            flag.C>>=7;
            A<<=1;
            if(flag.C==0x01)  A|=0x01;
            IP++;


           }break;
    //*****no such command!!!*****
    case 0x08: {   /*write dealer!!!*/
           }break;
    //dad b
    case 0x09: { unsigned int temp = L+C;
         L+=C;
         if((temp&0x0100)==0x0100) { temp=H+B+1; H=H+B+1; }
          else { temp=H+B; H+=B;}
         if((temp&0x0100)==0x0100) flag.C=1;
          else flag.C=0;
         IP++;


           }break;
    //stax b
    case 0x0A: { unsigned int addr=B;
         addr<<=8;
         addr|=C;
         A=memory[addr];
         IP++;
           }break;
    //dcx B
    case 0x0B: { if (C==0x00) B--;
         C--;


         IP++;
           }break;
    //Inr c
    case 0x0C: { unsigned char temp=flag.C;
         set_flags(C+1);
         flag.C=temp;
         C++;
         IP++;
           }break;
    //dcr c
    case 0x0D: { unsigned char temp=flag.C;
         set_flags(C-1);
         flag.C=temp;
         C--;
         IP++;
           }break;
    //mvi c
    case 0x0E: { C=memory[++IP];
         IP++;
           }break;
    //rrc
    case 0x0F: { flag.C=A&0x01;
         A>>=1;
         if(flag.C==0x01)  A|=0x80;
         IP++;
           }break;
    //*****no such command!!!*****
    case 0x10: {
           }break;
    //lxi d
    case 0x11: { E=memory[++IP];
         D=memory[++IP];
         IP++;

           }break;
    //stax d
    case 0x12: {

      memory[D*256+E]=A;
      buf=QString::number(A,16).toUpper();
      MainTable->model()->setData(MainTable->model()->index(D*256+E,2),buf,Qt::EditRole);

         IP++;

           }break;
    //inx d
    case 0x13: { if (E==0xFF) D++;
         E++;
         IP++;

           }break;
    //inr d
    case 0x14: { unsigned char temp=flag.C;
         set_flags(D+1);
         flag.C=temp;

         D++;
         IP++;
           }break;
    //dcr d
    case 0x15: { unsigned char temp=flag.C;
         set_flags(D-1);
         flag.C=temp;
         D--;
         IP++;
           }break;
    //mvi d
    case 0x16: { D=memory[++IP];
         IP++;
           }break;
    //ral
    case 0x17: { unsigned char Ctemp=flag.C;
         flag.C=A&0x80; flag.C>>=7;
         A<<=1;
         if(Ctemp==0x01)  A|=0x01;
         IP++;

           }break;
    //*****no such command!!!*****
    case 0x18: {
           }break;
    //dad d
    case 0x19: { unsigned int temp = L+E;
         L+=E;
         if((temp&0x0100)==0x0100) { temp=H+D+1; H=H+D+1; }
          else { temp=H+D; H+=D;}
         if((temp&0x0100)==0x0100) flag.C=1;
          else flag.C=0;
         IP++;

           }break;
    //ldax d
    case 0x1A: { unsigned int addr=D;
         addr<<=8;
         addr|=E;
         A=memory[addr];
         IP++;

           }break;
    //dcx d
    case 0x1B: { if (E==0x00) D--;
         E--;
         IP++;

           }break;
    //inr e
    case 0x1C: { unsigned char temp=flag.C;
         set_flags(E+1);
         flag.C=temp;
         E++;
         IP++;
    }break;
    //dcr e
    case 0x1D: { unsigned char temp=flag.C;
         set_flags(E-1);
         flag.C=temp;
         E--;
         IP++;
               }break;
    //mvi e
    case 0x1E: { E=memory[++IP];
         IP++;
;

           }break;
    //rar
    case 0x1F: { unsigned char Ctemp=flag.C;
         flag.C=A&0x01;
         A>>=1;
         if(Ctemp==0x01)  A|=0x80;
             flag.C;
         IP++;

           }break;
    //*****no such command!!!*****
    case 0x20: {
          }break;
    //lxi h
    case 0x21: {

        L=memory[++IP];
        H=memory[++IP];
        IP++;

          }break;

    case 0x22: {
      unsigned int addr=memory[IP+2];
        addr<<=8;
        addr|=memory[IP+1];


        memory[addr]=L;
        buf=QString::number(L,16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(addr,2),buf,Qt::EditRole);

        memory[++addr]=H;
        buf=QString::number(H,16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(addr,2),buf,Qt::EditRole);

        IP+=3;

          }break;
    //inx h
    case 0x23: { if (L==0xFF) H++;
        L++;
        IP++;

          }break;
    //inr h
    case 0x24: { unsigned char temp=flag.C;
         set_flags(H+1);
         flag.C=temp;
         H++;
         IP++;


           }break;
    //dcr h
    case 0x25: { unsigned char temp=flag.C;
         set_flags(H-1);
         flag.C=temp;
         H--;
         IP++;


           }break;
    //mvi h
    case 0x26: {


        H=memory[++IP];
        IP++;

          }break;
    // command DAA is ignored!!!
    case 0x27: { IP++;
          }break;
    //*****no such command!!!*****
    case 0x28: {
          }break;
    //dad H
    case 0x29: {
      unsigned int temp = L+L;
         L+=L;
         if((temp&0x0100)==0x0100) { temp=H+H+1; H=H+H+1; }
          else { temp=H+H; H+=H;}
         if((temp&0x0100)==0x0100) flag.C=1;
          else flag.C=0;
         IP++;

           }break;
    //lhld
    case 0x2A: { unsigned int addr=memory[IP+2];
        addr<<=8;
        addr|=memory[IP+1];
        L=memory[addr];
        H=memory[++addr];
        IP+=3;


        }break;
    //dcx H
    case 0x2B: { if (L==0x00) H--;
        L--;
        IP++;


          }break;
    //inr l
    case 0x2C: { unsigned char temp=flag.C;
         set_flags(L+1);
         flag.C=temp;
         L++;
         IP++;

          }break;
    //dcr l
    case 0x2D: { unsigned char temp=flag.C;
         set_flags(L-1);
         flag.C=temp;
         L--;
         IP++;

           }break;
    //mvi l
    case 0x2E: { L=memory[++IP];
        IP++;

          }break;
    //cma
    case 0x2F: { A^=0xFF;
        IP++;

          }break;
    //*****no such command!!!*****
    case 0x30: {
          }break;
    //lxi sp
    case 0x31: {
        SP=memory[IP+2];
        SP<<=8;
        SP|=memory[IP+1];
        IP+=3;

          }break;
    case 0x32: { unsigned int addr=memory[IP+2];
        addr<<=8;
        addr|=memory[IP+1];
        memory[addr]=A;

        buf=QString::number(A,16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(addr,2),buf,Qt::EditRole);

        IP+=3;
          }break;
    //inx sp
    case 0x33: { SP++;
        IP++;

          }break;
    //inr m
    case 0x34: { unsigned int addr=H;
         addr<<=8;
         addr|=L;
         unsigned char temp=flag.C;
         set_flags(memory[addr]+1);
         flag.C=temp;
         memory[addr]++;
         buf=QString::number(memory[addr],16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(addr,2),buf,Qt::EditRole);

         IP++;

           }break;
    //dcr m
    case 0x35: { unsigned int addr=H;
         addr<<=8;
         addr|=L;
         unsigned char temp=flag.C;
         set_flags(memory[addr]-1);
         flag.C=temp;
         memory[addr]--;

         buf=QString::number(memory[addr],16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(addr,2),buf,Qt::EditRole);

         IP++;

           }break;
    //mvi m
    case 0x36: {

        unsigned int addr=H;///!!!!!!!!!!!!!!!!!!!?????????
        addr<<=8;
        addr|=L;


        memory[addr]=memory[++IP];

        buf=QString::number(memory[addr],16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(addr,2),buf,Qt::EditRole);


        IP++;

          }break;
    //stc
    case 0x37: { flag.C=0x01;
        IP++;

          }break;
    //*****no such command!!!*****
    case 0x38: {
          }break;
    //DAD SP
    case 0x39: {
        unsigned char rgP=(unsigned char)SP;
         unsigned char rgS=(unsigned char)(SP>>8);
         unsigned int temp = L+rgP;
         L+=rgP;
         if((temp&0x0100)==0x0100) { temp=H+rgS+1; H=H+rgS+1; }
          else { temp=H+rgS; H+=rgS;}
         if((temp&0x0100)==0x0100) flag.C=1;
          else flag.C=0;
         IP++;



           }break;
    //lda
    case 0x3A: { unsigned int addr=memory[IP+2];
        addr<<=8;
        addr|=memory[IP+1];
        A=memory[addr];
        IP+=3;
          }break;
    //dcx sp
    case 0x3B: {
      SP--;
        IP++;
          }break;
    //inr a
    case 0x3C: { unsigned char temp=flag.C;
         set_flags(A+1);
         flag.C=temp;
         A++;
         IP++;

           }break;
    //dcr a
    case 0x3D: { unsigned char temp=flag.C;
         set_flags(A-1);
         flag.C=temp;
         A--;
         IP++;
           }break;
    //mvi A
    case 0x3E: { A=memory[++IP];
         IP++;
           }break;
    //cmc
    case 0x3F: { flag.C^=0x01;
         IP++;
           }break;
    //mov
    case 0x40: { B=B;
         IP++;
           }break;
    //mov
    case 0x41: { B=C;
         IP++;
           }break;
    //mov
    case 0x42: { B=D;
         IP++;
           }break;
    //mov
    case 0x43: { B=E;
         IP++;
           }break;
    //mov
    case 0x44: { B=H;
         IP++;
           }break;
    //mov
    case 0x45: { B=L;
         IP++;
           }break;
    //mov
    case 0x46: { unsigned int addr=H;
        addr<<=8;
        addr|=L;
        B=memory[addr];
        IP++;
          }break;
    //mov
    case 0x47: { B=A;
         IP++;
           }break;
    //mov
    case 0x48: { C=B;
         IP++;
           }break;
    //mov
    case 0x49: { C=C;
         IP++;
           }break;
    //mov
    case 0x4A: { C=D;
         IP++;
           }break;
    //mov
    case 0x4B: { C=E;
         IP++;
           }break;
    //mov
    case 0x4C: { C=H;
         IP++;
           }break;
    //mov
    case 0x4D: { C=L;
         IP++;
           }break;
    //mov
    case 0x4E: { unsigned int addr=H;
        addr<<=8;
        addr|=L;
        C=memory[addr];
        IP++;
          }break;
    //mov
    case 0x4F: { C=A;
         IP++;

           }break;
    //mov
    case 0x50: { D=B;
         IP++;
           }break;
    //mov
    case 0x51: { D=C;
         IP++;
           }break;
    //mov
    case 0x52: { D=D;
         IP++;
           }break;
    //mov
    case 0x53: { D=E;
         IP++;
           }break;
    //mov
    case 0x54: { D=H;
         IP++;
           }break;
    //mov
    case 0x55: { D=L;
         IP++;
           }break;
    //mov
    case 0x56: { unsigned int addr=H;
        addr<<=8;
        addr|=L;
        D=memory[addr];
        IP++;
          }break;
    //mov
    case 0x57: { D=A;
         IP++;
           }break;
    //mov
    case 0x58: { E=B;
         IP++;
           }break;
    //mov
    case 0x59: { E=C;
         IP++;
           }break;
    //mov
    case 0x5A: { E=D;
         IP++;
           }break;
    //mov
    case 0x5B: { E=E;
         IP++;
           }break;
    //mov
    case 0x5C: { E=H;
         IP++;
           }break;
    //mov
    case 0x5D: { E=L;
         IP++;
           }break;
    //mov
    case 0x5E: { unsigned int addr=H;
        addr<<=8;
        addr|=L;
        E=memory[addr];
        IP++;
          }break;
    //mov
    case 0x5F: { E=A;
         IP++;
           }break;
    //mov
    case 0x60: { H=B;
         IP++;
           }break;
    //mov
    case 0x61: { H=C;
         IP++;
           }break;
    //mov
    case 0x62: { H=D;
         IP++;
           }break;
    //mov
    case 0x63: { H=E;
         IP++;
           }break;
    //mov
    case 0x64: { H=H;
         IP++;
           }break;
    //mov
    case 0x65: { H=L;
         IP++;
           }break;
    //mov
    case 0x66: { unsigned int addr=H;
        addr<<=8;
        addr|=L;
        H=memory[addr];
        IP++;
          }break;
    //mov
    case 0x67: { H=A;
         IP++;
           }break;
    //mov
    case 0x68: { L=B;
         IP++;
           }break;
    //mov
    case 0x69: { L=C;
         IP++;
           }break;
    //mov
    case 0x6A: { L=D;
         IP++;
           }break;
    //mov
    case 0x6B: { L=E;
         IP++;
           }break;
    //mov
    case 0x6C: { L=H;
         IP++;
           }break;
    //mov
    case 0x6D: { L=L;
         IP++;
           }break;
    //mov
    case 0x6E: { unsigned int addr=H;
        addr<<=8;
        addr|=L;
        L=memory[addr];
        IP++;
          }break;
    //mov
    case 0x6F: { L=A;
         IP++;
           }break;
    //mov
    case 0x70: { unsigned int addr=H;
        addr<<=8;
        addr|=L;
        memory[addr]=B;
        buf=QString::number(B,16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(addr,2),buf,Qt::EditRole);

        IP++;
          }break;
    //mov
    case 0x71: { unsigned int addr=H;
        addr<<=8;
        addr|=L;

        memory[addr]=C;
        buf=QString::number(C,16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(addr,2),buf,Qt::EditRole);

        IP++;
          }break;
    //mov
    case 0x72: { unsigned int addr=H;
        addr<<=8;
        addr|=L;
        memory[addr]=D;
        buf=QString::number(D,16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(addr,2),buf,Qt::EditRole);


        IP++;
          }break;
    //mov
    case 0x73: { unsigned int addr=H;
        addr<<=8;
        addr|=L;
        memory[addr]=E;
        buf=QString::number(E,16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(addr,2),buf,Qt::EditRole);

        IP++;
          }break;
    //mov
    case 0x74: { unsigned int addr=H;
        addr<<=8;
        addr|=L;
        memory[addr]=H;
        buf=QString::number(H,16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(addr,2),buf,Qt::EditRole);

        IP++;
          }break;
    //mov
    case 0x75: { unsigned int addr=H;
         addr<<=8;
         addr|=L;
         memory[addr]=L;
         buf=QString::number(L,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(addr,2),buf,Qt::EditRole);

         IP++;
           }break;
    //HLT
    case 0x76: return 1;

    //mov
    case 0x77: { unsigned int addr=H;
        addr<<=8;
        addr|=L;
        memory[addr]=A;
        buf=QString::number(A,16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(addr,2),buf,Qt::EditRole);

        IP++;
          }break;
    //mov
    case 0x78: {

      A=B;
        // set_flags(A);
         IP++;
           }break;
    //mov
    case 0x79: { A=C;
      // set_flags(A);
         IP++;
           }break;
    //mov
    case 0x7A: { A=D;
      // set_flags(A);
         IP++;
           }break;
    //mov
    case 0x7B: { A=E;
      // set_flags(A);
         IP++;
           }break;
    //mov
    case 0x7C: { A=H;
      // set_flags(A);
         IP++;
           }break;
    //mov
    case 0x7D: { A=L;
      // set_flags(A);
         IP++;
           }break;
    //mov
    case 0x7E: { unsigned int addr=H;
        addr<<=8;
        addr|=L;
        A=memory[addr];
        // set_flags(A);
        IP++;
          }break;
    //mov
    case 0x7F: { A=A;
      // set_flags(A);
        IP++;
          }break;

    //ADD B
    case 0x80: { set_flags(A+B);
         A+=B;
         IP++;

           }break;

    //ADD C
    case 0x81: { set_flags(A+C);
         A+=C;
         IP++;
           }break;

    //ADD D
    case 0x82: { set_flags(A+D);
         A+=D;
         IP++;
           }break;
    //ADD E
    case 0x83: { set_flags(A+E);
         A+=E;
         IP++;
           }break;
    //ADD H
    case 0x84: { set_flags(A+H);
         A+=H;
         IP++;
           }break;
    //ADD L
    case 0x85: { set_flags(A+L);
         A+=L;
         IP++;
           }break;
    //ADD M
    case 0x86: { unsigned int addr=H;
         addr<<=8;
         addr|=L;
         set_flags(A+memory[addr]);
         A+=memory[addr];
         IP++;
           }break;
    //ADD A
    case 0x87: { set_flags(A+A);
         A+=A;
         IP++;
           }break;
    //ADC B
    case 0x88: { unsigned char temp_fl_c=flag.C;
         set_flags(A+B+flag.C);
         A=A+B+temp_fl_c;
         IP++;
           }break;
    //ADC C
    case 0x89: { unsigned char temp_fl_c=flag.C;
         set_flags(A+C+flag.C);
         A=A+C+temp_fl_c;
         IP++;
           }break;
    //ADC D
    case 0x8A: { unsigned char temp_fl_c=flag.C;
         set_flags(A+D+flag.C);
         A=A+D+temp_fl_c;
         IP++;
           }break;
    //ADC E
    case 0x8B: { unsigned char temp_fl_c=flag.C;
         set_flags(A+E+flag.C);
         A=A+E+temp_fl_c;
         IP++;
           }break;
    //ADC H
    case 0x8C: { unsigned char temp_fl_c=flag.C;
         set_flags(A+H+flag.C);
         A=A+H+temp_fl_c;
         IP++;
           }break;
    //ADC L
    case 0x8D: { unsigned char temp_fl_c=flag.C;
         set_flags(A+L+flag.C);
         A=A+L+temp_fl_c;
         IP++;
           }break;
    //ADC M
    case 0x8E: { unsigned char temp_fl_c=flag.C;
         unsigned int addr=H;
         addr<<=8;
         addr|=L;
         set_flags(A+memory[addr]+flag.C);
         A=A+memory[addr]+temp_fl_c;
         IP++;
           }break;
    //ADC A
    case 0x8F: { unsigned char temp_fl_c=flag.C;
         set_flags(A+A+flag.C);
         A=A+A+temp_fl_c;
         IP++;
           }break;
    //sub b
    case 0x90: { set_flags(A-B);
         A-=B;
         IP++;
           }break;
    //sub c
    case 0x91: { set_flags(A-C);
         A-=C;
         IP++;
           }break;
    //sub d
    case 0x92: { set_flags(A-D);
         A-=D;
         IP++;
           }break;
    //sub e
    case 0x93: { set_flags(A-E);
         A-=E;
         IP++;
           }break;
    //sub h
    case 0x94: { set_flags(A-H);
         A-=H;
         IP++;
           }break;
    //sub l
    case 0x95: { set_flags(A-L);
         A-=L;
         IP++;
           }break;
    //sub m
    case 0x96: { unsigned int addr=H;
         addr<<=8;
         addr|=L;
         set_flags(A-memory[addr]);
         A-=memory[addr];
         IP++;
           }break;
    //sub a
    case 0x97: { set_flags(A-A);
         A-=A;
         IP++;
           }break;
    //SBB B
    case 0x98: { unsigned char temp_fl_c=flag.C;
         set_flags(A-B-flag.C);
         A=A-B-temp_fl_c;
         IP++;
           }break;
    //SBB C
    case 0x99: { unsigned char temp_fl_c=flag.C;
         set_flags(A-C-flag.C);
         A=A-C-temp_fl_c;
         IP++;
           }break;
    //SBB D
    case 0x9A: { unsigned char temp_fl_c=flag.C;
         set_flags(A-D-flag.C);
         A=A-D-temp_fl_c;
         IP++;
           }break;
    //SBB E
    case 0x9B: { unsigned char temp_fl_c=flag.C;
         set_flags(A-E-flag.C);
         A=A-E-temp_fl_c;
         IP++;
           }break;
    //SBB H
    case 0x9C: { unsigned char temp_fl_c=flag.C;
         set_flags(A-H-flag.C);
         A=A-H-temp_fl_c;
         IP++;
           }break;
    //SBB L
    case 0x9D: { unsigned char temp_fl_c=flag.C;
         set_flags(A-L-flag.C);
         A=A-L-temp_fl_c;
         IP++;
           }break;
    //SBB M
    case 0x9E: { unsigned char temp_fl_c=flag.C;
         unsigned int addr=H;
         addr<<=8;
         addr|=L;
         set_flags(A-memory[addr]-flag.C);
         A=A-memory[addr]-temp_fl_c;
         IP++;
           }break;
    //SBB A
    case 0x9F: { unsigned char temp_fl_c=flag.C;
         set_flags(A-A-flag.C);
         A=A-A-temp_fl_c;
         IP++;
           }break;
    //ANA B
    case 0xA0: { A&=B;
         set_flags(A);
         IP++;
           }break;
    //ANA C
    case 0xA1: { A&=C;
         set_flags(A);
         IP++;
           }break;
    //ANA D
    case 0xA2: { A&=D;
         set_flags(A);
         IP++;
           }break;
    //ANA E
    case 0xA3: { A&=E;
         set_flags(A);
         IP++;
           }break;
    //ANA H
    case 0xA4: { A&=H;
         set_flags(A);
         IP++;
           }break;
    //ANA L
    case 0xA5: { A&=L;
         set_flags(A);
         IP++;
           }break;
    //ANA M
    case 0xA6: { unsigned int addr=H;
         addr<<=8;
         addr|=L;
         A&=memory[addr];
         set_flags(A);
         IP++;
          }break;
    //ANA A
    case 0xA7: { A&=A;
         set_flags(A);
         IP++;
           }break;
    //XRA B
    case 0xA8: { A^=B;
         set_flags(A);
         IP++;
           }break;
    //XRA C
    case 0xA9: { A^=C;
         set_flags(A);
         IP++;
           }break;
    //XRA D
    case 0xAA: { A^=D;
         set_flags(A);
         IP++;
           }break;
    //XRA E
    case 0xAB: { A^=E;
         set_flags(A);
         IP++;
           }break;
    //XRA H
    case 0xAC: { A^=H;
         set_flags(A);
         IP++;
           }break;
    //XRA L
    case 0xAD: { A^=L;
         set_flags(A);
         IP++;
           }break;
    //XRA M
    case 0xAE: { unsigned int addr=H;
         addr<<=8;
         addr|=L;
         A^=memory[addr];
         set_flags(A);
         IP++;
           }break;
    //XRA A
    case 0xAF: { A^=A;
         set_flags(A);
         IP++;
           }break;
    //ORA B
    case 0xB0: { A|=B;
         set_flags(A);
         IP++;
           }break;
    //ORA C
    case 0xB1: { A|=C;
         set_flags(A);
         IP++;
           }break;
    //ORA D
    case 0xB2: { A|=D;
         set_flags(A);
         IP++;
           }break;
    //ORA E
    case 0xB3: { A|=E;
         set_flags(A);
         IP++;
           }break;
    //ORA H
    case 0xB4: { A|=H;
         set_flags(A);
         IP++;
           }break;
    //ORA L
    case 0xB5: { A|=L;
         set_flags(A);
         IP++;
           }break;
    //ORA M
    case 0xB6: { unsigned int addr=H;
         addr<<=8;
         addr|=L;
         A|=memory[addr];
         set_flags(A);
         IP++;
           }break;
    //ORA A
    case 0xB7: { A|=A;
         set_flags(A);
         IP++;
           }break;
    //CMP B
           case 0xB8: {
               set_flags(A-B);
         IP++;
           }break;
    //CMP C
    case 0xB9: {
            set_flags(A-C);
         IP++;
           }break;
    //CMP D
    case 0xBA: {
         set_flags(A-D);
         IP++;
           }break;
    //CMP E
    case 0xBB: {
         set_flags(A-E);
         IP++;
           }break;
    //CMP H
    case 0xBC: {
         set_flags(A-H);
         IP++;
           }break;
    //CMP L
    case 0xBD: {
         set_flags(A-L);
         IP++;
           }break;
    //CMP M
    case 0xBE: { unsigned int addr=H;
         addr<<=8;
         addr|=L;
         set_flags(A-memory[addr]);
         IP++;
           }break;
    //CMP A
    case 0xBF: {
         set_flags(0);
         IP++;
           }break;
    //RNZ
    case 0xC0: { if(flag.Z==0x00) alu(0xC9);
         else IP++;
           }break;
    //POP b
    case 0xC1: { C=memory[SP++];
         B=memory[SP++];
         IP++;
           }break;
    //jnz
    case 0xC2: { if (flag.Z==0)
         { unsigned int addr=memory[IP+2];
           addr<<=8;
           addr|=memory[IP+1];
           IP=addr;


         }
         else IP+=3;;

           }break;
    //jmp
    case 0xC3: { unsigned int addr=memory[IP+2];
         addr<<=8;
         addr|=memory[IP+1];
         IP=addr;
           }break;
    //cnz
    case 0xC4: { if(flag.Z==0x00) alu(0xCD);
         else IP+=3;
           }break;
    //push b
    case 0xC5: {
      memory[--SP]=B;
      buf=QString::number(B,16).toUpper();
      MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         memory[--SP]=C;
         buf=QString::number(C,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         IP++;
           }break;
    //adi
    case 0xC6: { set_flags(A+memory[++IP]);
        A+=memory[IP];
        IP++;
          }break;
    //rst 0
    case 0xC7: { IP++;
         unsigned char rgP=(unsigned char)IP;
         unsigned char rgI=(unsigned char)(IP>>8);
         memory[--SP]=rgI;
         buf=QString::number(rgI,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         memory[--SP]=rgP;
         buf=QString::number(rgP,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         IP=0x00;
           }break;
    //rz
    case 0xC8: { if(flag.Z==0x01) alu(0xC9);
         else IP++;
           }break;
    //ret
    case 0xC9: { unsigned int addr=memory[SP++];
         addr|=(memory[SP]<<8);
         SP++;
         IP=addr;
           }break;
    //jz
    case 0xCA: { if (flag.Z==1)
         { unsigned int addr=memory[IP+2];
           addr<<=8;
           addr|=memory[IP+1];
           IP=addr;
         }
         else IP+=3;
           }break;
    //*****no such command!!!*****
    case 0xCB: {
          }break;
    //cz
    case 0xCC: { if(flag.Z==0x01) alu(0xCD);
         else IP+=3;
           }break;
    //call
    case 0xCD: {
         IP+=3;
         unsigned char rgP=(unsigned char)IP;
         unsigned char rgI=(unsigned char)(IP>>8);
          memory[--SP]=rgI;
          buf=QString::number(rgI,16).toUpper();
          MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

          memory[--SP]=rgP;
          buf=QString::number(rgP,16).toUpper();
          MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);


         unsigned int addr=memory[IP+2-3];
         addr<<=8;
         addr|=memory[IP+1-3];

         IP=addr;

         //Mw->SetCurrenRow(addr);
           }break;
    //aci
    case 0xCE: { unsigned char temp_fl_c=flag.C;
         set_flags(A+memory[++IP]+flag.C);
         A=A+memory[IP]+temp_fl_c;
         IP++;
           }break;
    //rst 8
    case 0xCF: { IP++;
         unsigned char rgP=(unsigned char)IP;
         unsigned char rgI=(unsigned char)(IP>>8);
         memory[--SP]=rgI;
         buf=QString::number(rgI,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         memory[--SP]=rgP;
         buf=QString::number(rgP,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         IP=(0x08<<4);
           }break;
    //rnc
    case 0xD0: { if(flag.C==0x00) alu(0xC9);
         else IP++;
           }break;
    //pop d
    case 0xD1: { E=memory[SP++];
        D=memory[SP++];
        IP++;
          }break;
    //jnc
    case 0xD2: { if (flag.C==0)
         { unsigned int addr=memory[IP+2];
           addr<<=8;
           addr|=memory[IP+1];
           IP=addr;
         }
         else IP+=3;
           }break;
    //out
    case 0xD3: {
         switch(memory[++IP])
         {
         case 0x10: {
             char st[3];
             Out10->insertHtml("<font color=#9400D3>"+QString::number(A,16).toUpper()+"</font><br>");

             }break;

           case 0x20: {
                RgTimer=A;
                t=time(NULL);
                  }break;
         }
         IP++;
           }break;
    //cnc
    case 0xD4: { if(flag.C==0x00) alu(0xCD);
         else IP+=3;
           }break;
    //push d
    case 0xD5: {
      memory[--SP]=D;
      buf=QString::number(D,16).toUpper();
      MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

        memory[--SP]=E;
        buf=QString::number(E,16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

        IP++;
          }break;
    //sui
    case 0xD6: { set_flags(A-memory[++IP]);
         A-=memory[IP];
         IP++;
           }break;
    //rst 16
    case 0xD7: { IP++;
         unsigned char rgP=(unsigned char)IP;
         unsigned char rgI=(unsigned char)(IP>>8);
         memory[--SP]=rgI;
         buf=QString::number(rgI,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         memory[--SP]=rgP;
         buf=QString::number(rgP,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);


         IP=(0x16<<4);
           }break;
    //rc
    case 0xD8: { if(flag.C==0x01) alu(0xC9);
         else IP++;
           }break;
    //*****no such command!!!*****
    case 0xD9: {
          }break;
    //jc
    case 0xDA: { if (flag.C==1)
         { unsigned int addr=memory[IP+2];
           addr<<=8;
           addr|=memory[IP+1];
           IP=addr;
         }
         else IP+=3;
           }break;
    //in
    case 0xDB: {
         switch(memory[++IP])
         { case 0x08:{
            in08 w;
            w.exec();
            A=(unsigned char) w.getValue();
         } break;

           case 0x16:{ A=RgTimer;}break;
         }
         IP++;
           }break;
    //cc
    case 0xDC: { if(flag.C==0x01) alu(0xCD);
         else IP+=3;
           }break;
    //*****no such command!!!*****
    case 0xDD: {
          }break;
    //sbi
    case 0xDE: { unsigned char temp_fl_c=flag.C;
         set_flags(A-memory[++IP]-flag.C);
         A=A-memory[IP]-temp_fl_c;
         IP++;
          }break;
    //rst 24
    case 0xDF: { IP++;
         unsigned char rgP=(unsigned char)IP;
         unsigned char rgI=(unsigned char)(IP>>8);
         memory[--SP]=rgI;
         buf=QString::number(rgI,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         memory[--SP]=rgP;
         buf=QString::number(rgP,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         IP=(0x24<<4);
           }break;
    //rpo
    case 0xE0: { if(flag.P==0x00) alu(0xC9);
         else IP++;
           }break;
    //pop h
    case 0xE1: { L=memory[SP++];
        H=memory[SP++];
        IP++;
          }break;
    //jpo
    case 0xE2: { if (flag.P==0)
         { unsigned int addr=memory[IP+2];
           addr<<=8;
           addr|=memory[IP+1];
           IP=addr;
         }
         else IP+=3;
           }break;
    //xthl
    case 0xE3: { unsigned char temp=H;
        H=memory[SP+1];
        memory[SP+1]=temp;

        buf=QString::number(temp,16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(SP+1,2),buf,Qt::EditRole);

        temp=L;
        L=memory[SP];
        memory[SP]=temp;
        buf=QString::number(temp,16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

        IP++;
          }break;
    //cpo
    case 0xE4: { if(flag.P==0x00) alu(0xCD);
         else IP+=3;
           }break;
    //push h
    case 0xE5: { memory[--SP]=H;
      buf=QString::number(H,16).toUpper();
      MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

        memory[--SP]=L;
        buf=QString::number(L,16).toUpper();
        MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

        IP++;
          }break;
    //ani
    case 0xE6: { set_flags(A&memory[++IP]);
         A&=memory[IP];
         IP++;
           }break;
    //rst 32
    case 0xE7: { IP++;
         unsigned char rgP=(unsigned char)IP;
         unsigned char rgI=(unsigned char)(IP>>8);
         memory[--SP]=rgI;
         buf=QString::number(rgI,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         memory[--SP]=rgP;
         buf=QString::number(rgP,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         IP=(0x32<<4);
           }break;
    //rpe
    case 0xE8: { if(flag.P==0x01) alu(0xC9);
         else IP++;
           }break;
    //pchl
    case 0xE9: { unsigned int addr=H;
         addr<<=8;
         addr|=L;
         IP=addr;
           }break;
    //jpe
    case 0xEA: { if (flag.P==1)
         { unsigned int addr=memory[IP+2];
           addr<<=8;
           addr|=memory[IP+1];
           IP=addr;
         }
         else IP+=3;
           }break;
    //xchg
    case 0xEB: { unsigned char temp=H;
        H=D;
        D=temp;
        temp=L;
        L=E;
        E=temp;
        IP++;
          }break;
    //cpe
    case 0xEC: { if(flag.P==0x01) alu(0xCD);
         else IP+=3;
           }break;
    //*****no such command!!!*****
    case 0xED: {
          }break;
    //xri
    case 0xEE: { set_flags(A^memory[++IP]);
         A^=memory[IP];
         IP++;
           }break;
    //rst 40
    case 0xEF: { IP++;
         unsigned char rgP=(unsigned char)IP;
         unsigned char rgI=(unsigned char)(IP>>8);
         memory[--SP]=rgI;
         buf=QString::number(rgI,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         memory[--SP]=rgP;
         buf=QString::number(rgP,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         IP=(0x40<<4);
           }break;
    //rp
    case 0xF0: { if(flag.S==0x00) alu(0xC9);
         else IP++;
           }break;
    //pop psw
    case 0xF1: { if((memory[SP]&0x01)==0x01) flag.C=1;
         else flag.C=0;
        if((memory[SP]&0x02)==0x04) flag.P=1;
         else flag.P=0;
        if((memory[SP]&0x01)==0x40) flag.Z=1;
         else flag.Z=0;
        if((memory[SP]&0x01)==0x80) flag.S=1;
         else flag.S=0;
        SP++;
        A=memory[SP++];
        IP++;
          }break;
    //jp
    case 0xF2: { if (flag.S==0)
         { unsigned int addr=memory[IP+2];
           addr<<=8;
           addr|=memory[IP+1];
           IP=addr;
         }
         else IP+=3;
           }break;
    //COM DI IGNORED
    case 0xF3: { IP++;
           }break;
    //cp
    case 0xF4: { if(flag.S==0x00) alu(0xCD);
         else IP+=3;
           }break;
    //push psw
    case 0xF5: {

      memory[--SP]=A;
      buf=QString::number(A,16).toUpper();
      MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         memory[--SP]=0x02;
         buf=QString::number(0x02,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         memory[SP]|=flag.C;
         memory[SP]|=(flag.P<<2);
         memory[SP]|=(flag.Z<<6);
         memory[SP]|=(flag.S<<7);

         buf=QString::number(memory[SP],16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         IP++;
           }break;
    //ori
    case 0xF6: { set_flags(A|memory[++IP]);
         A|=memory[IP];
         IP++;
           }break;
    //rst 48
    case 0xF7: { IP++;
         unsigned char rgP=(unsigned char)IP;
         unsigned char rgI=(unsigned char)(IP>>8);
         memory[--SP]=rgI;
         buf=QString::number(rgI,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         memory[--SP]=rgP;
         buf=QString::number(rgP,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         IP=(0x48<<4);
           }break;
    //rm
    case 0xF8: { if(flag.S==0x01) alu(0xC9);
         else IP++;
           }break;
    //sphl
    case 0xF9: { unsigned int addr=H;
        addr<<=8;
        addr|=L;
        SP=addr;
        IP++;
          }break;
    //jm
    case 0xFA: { if (flag.S==1)
         { unsigned int addr=memory[IP+2];
           addr<<=8;
           addr|=memory[IP+1];
           IP=addr;
         }
         else IP+=3;
           }break;
    //COM EI IGNORED
    case 0xFB: { IP++;
          }break;
    //cm
    case 0xFC: { if(flag.S==0x01) alu(0xCD);
         else IP+=3;
           }break;
    //*****no such command!!!*****
    case 0xFD: {
          }break;
    //cpi
    case 0xFE: {
      if (A==memory[++IP])
          flag.Z=1;
         else
          if(A<memory[IP])
              flag.C=1;
         IP++;
           }break;
    //rst 56
    case 0xFF: { IP++;
         unsigned char rgP=(unsigned char)IP;
         unsigned char rgI=(unsigned char)(IP>>8);
         memory[--SP]=rgI;
         buf=QString::number(rgI,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         memory[--SP]=rgP;
         buf=QString::number(rgP,16).toUpper();
         MainTable->model()->setData(MainTable->model()->index(SP,2),buf,Qt::EditRole);

         IP=(0x56<<4);
           }break;
   }

   if((RgTimer!=0)&&(t-time(NULL)<0))
   {
     time_t t1=time(NULL);
     RgTimer=RgTimer-(unsigned char)(t1-t);
     t=t1;
   }

  return 0;
};




