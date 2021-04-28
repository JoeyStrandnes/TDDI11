#include <libepc.h>



void llmultiply(unsigned long long int l1, unsigned long long int l2, unsigned char *result);




void C_llmultiply(unsigned long long int l1, unsigned long long int l2, unsigned char *result)
{

  unsigned long long int TEST_L1 = 100;
  unsigned long long int TEST_L2 = 200;

  unsigned int Carry_Flag = 0;
  unsigned long int Carry_Checker; // OwO

  unsigned long long int Temp_Mul;
  unsigned long int* Result_ptr = ((unsigned long int*) result);
  Result_ptr[0] = 0;
  Result_ptr[1] = 0;
  Result_ptr[2] = 0;
  Result_ptr[3] = 0;

  unsigned long int *Mul_ptr = (unsigned long int*)&Temp_Mul;

/*
  unsigned long int AL = (TEST_L1);
  unsigned long int AH = (TEST_L1 >> 32); 

  unsigned long int BL = (TEST_L2);
  unsigned long int BH = (TEST_L2 >> 32);
*/
  
  unsigned long int AL = (l1);
  unsigned long int AH = (l1 >> 32); 

  unsigned long int BL = (l2);
  unsigned long int BH = (l2 >> 32);
  
  unsigned long int Mul_L;
  unsigned long int Mul_H;

  //Step 1
  Temp_Mul = (AL * BL);
  Mul_L = Temp_Mul;
  Mul_H = (Temp_Mul >> 32);

  Result_ptr[0] += Mul_L;       // First time addressing memory location. ^_^
  Result_ptr[1] += Mul_H; // First time addressing memory location. ^_^

  //Step 2
  Temp_Mul = (AH * BL);
  Mul_L = Temp_Mul;
  Mul_H = (Temp_Mul >> 32);
  /*
  PutString("\r\n");
  PutUnsigned(Mul_L, 18, 8);
  PutString("\r\n");
  PutUnsigned(Mul_H, 18, 8);  
  PutString("Res: \r\n");
  PutString(result);
  */
  //Carry_Checker = *(Result_ptr+1);
  Result_ptr[1] += Mul_L;      
  //Carry_Flag = (Carry_Checker > *(Result_ptr+1));
  
  //Carry_Checker = *(Result_ptr+2);
  //*(Result_ptr + 2) += Carry_Flag;
  Result_ptr[2] += (((Mul_H)));// + Carry_Flag);    
  //Carry_Flag = (Carry_Checker > *(Result_ptr+2));
  //Result_ptr[3] += Carry_Flag; // First time addressing memory location. ^_^

  Temp_Mul = (AL * BH);
  Mul_L = Temp_Mul;
  Mul_H = (Temp_Mul >> 32);

  //Carry_Checker = *(Result_ptr+1);
  *(Result_ptr +1) += Mul_L;      
  //Carry_Flag = (Carry_Checker > *(Result_ptr+1));
  //Carry_Checker = *(Result_ptr+2);
  //*(Result_ptr + 2) += Carry_Flag;
  
  Result_ptr[2] += ((Mul_H));// + Carry_Flag);
  //Carry_Flag = (Carry_Checker > *(Result_ptr+2));
  //Result_ptr[3] += Carry_Flag;


  //Step 3
  Temp_Mul = (AH * BH);

  //Carry_Checker = *(Result_ptr+2);
  Result_ptr[2] += Mul_L; 
  //Carry_Flag = (Carry_Checker > *(Result_ptr+2));
  //*(Result_ptr + 2) += Carry_Flag;
  Result_ptr[3] += (Mul_H);// + Carry_Flag); 
/*
  PutString("\r\n");
  PutUnsigned(Carry_Flag, 16, 8);
  PutString("\r\n");


  */
}


struct test_case {
  unsigned long long int a;
  unsigned long long int b;
  unsigned long long int rh;
  unsigned long long int rl;
};

struct test_case cases[6] = {

  { 0x0000111122223333ULL, 0x0000555566667777ULL,
    0x0000000005B061D9ULL, 0x58BF0ECA7C0481B5ULL },

  { 0x3456FEDCAAAA1000ULL, 0xEDBA00112233FF01ULL,
    0x309A912AF7188C57ULL, 0xE62072DD409A1000ULL },

  { 0xFFFFEEEEDDDDCCCCULL, 0xBBBBAAAA99998888ULL,
    0xBBBB9E2692C5DDDCULL, 0xC28F7531048D2C60ULL },

  { 0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL,
    0xFFFFFFFFFFFFFFFEULL, 0x0000000000000001ULL },
  
  { 0x00000001FFFFFFFFULL, 0x00000001FFFFFFFFULL,
    0x0000000000000003ULL, 0xFFFFFFFC00000001ULL },

  { 0xFFFEFFFFFFFFFFFFULL, 0xFFFF0001FFFFFFFFULL,
    0xFFFE0002FFFDFFFEULL, 0x0001FFFE00000001ULL }
};

void PutUnsignedLongLong(unsigned long long int* ulli)
{
  unsigned long int* uli = (unsigned long int*)ulli; 
  PutUnsigned(uli[1], 16, 8);
  PutUnsigned(uli[0], 16, 8); 
}

int main(int argc, char *argv[])
{

  unsigned char result[16];
  int i;
  
  ClearScreen(0x07);
  SetCursorPosition(0, 0);

  for (i = 0; i < 6; ++i)
  {
    PutString("Test : ");
    PutUnsignedLongLong(&cases[i].a);
    PutString(" * ");
    PutUnsignedLongLong(&cases[i].b); 
    PutString("\r\n");
   
    PutString("    == ");
    PutUnsignedLongLong(&cases[i].rh);
    PutUnsignedLongLong(&cases[i].rl);
    PutString("\r\n");
    
    //llmultiply(cases[i].a, cases[i].b, result);
    C_llmultiply(cases[i].a, cases[i].b, result);


    PutString("Result ");
    PutUnsignedLongLong(&result[8]);
    PutUnsignedLongLong(&result[0]); 
    PutString("\r\n");
    
    PutString("\r\n");
  }
  
  return 0;
}
