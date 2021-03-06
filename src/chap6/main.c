#include <mtc.h>
#include <dos.h>
#include <libepc.h>

#include "serial.h"

/* Function prototypes for the threads */
void OutBound(void) ;
void InBound(void) ;
void DisplayElapsedTime(void) ;

void PrintData(const unsigned long Input);

PRIVATE void SerialInit(void) ;

QUEUE *inbound_queue ;

 WINDOW *w ;

int main()
{
  inbound_queue = QueueCreate(sizeof(char), 20) ;
  
  ClearScreen(0x07) ;
  SetCursorVisible(TRUE) ;
  SerialInit() ;
  
  MtCCoroutine(OutBound()) ;
  MtCCoroutine(InBound()) ;

  MtCCoroutine(DisplayElapsedTime());

  
  return 0 ;
}

PRIVATE void SerialInit(void)
{
  /* Disable interrupts during initialization */
  disable() ;
  
  /* 9600 baud */
  outportb(SER_LCR, SER_LCR_DLAB) ;
  outportb(SER_DLO, 12) ;
  outportb(SER_DHI, 0) ;
  
  /* 8 data bits, even parity, 1 stop bit */
  outportb(SER_LCR, 0x1B) ;
  
  /* Enable only receiver data ready interrupts */
  outportb(SER_IER, 0x01) ;
  
  /* Request to send, data terminal ready, enable interrupts */
  outportb(SER_MCR, SER_MCR_RTS|SER_MCR_DTR|SER_MCR_OUT2) ;
  
  /* Store address of ISR in IDT */
  SetISR(IRQ2INT(SER_IRQ), SerialISR) ;
  
  /* Unmask the UART's IRQ line */
  outportb(0x21, inportb(0x21) & ~SER_MSK) ;
  
  /* Re-enable interrupts */
  enable() ;
}


void DisplayElapsedTime(void)
{

  w = WindowCreate("Time", 22, 24, 0, 79);

  unsigned long int time;
  unsigned int tmp = 0;

  WindowSelect(w);
  SetCursorVisible(0);
  PutString("Time: ");



  while(1)
  {
    
    time = Milliseconds()/1000;

    if(time > tmp){
      tmp = time;
      WindowSelect(w);
      SetCursorPosition(23, 7);
      PutUnsigned(time, 10, 2);
      
    }
    
    else{
      MtCYield();
    }

  }

}

