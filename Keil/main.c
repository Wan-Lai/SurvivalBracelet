#include <reg52.h>
#include <intrins.h>

#define GPIO_LCD P0

sbit GPIO_RS = P1^0;
sbit GPIO_RW = P1^1;
sbit GPIO_E = P1^2;
sbit GPIO_BTN_LAST = P1^3;
sbit GPIO_BTN_NEXT = P1^4;
sbit GPIO_BTN_OK = P1^5;
sbit GPIO_LED_RED = P1^6;
sbit GPIO_LED_GREEN = P1^7;

unsigned long i = 0;
unsigned long sec = 0;
unsigned int index = 3;
unsigned int isboot = 0;

void KeyEvent();
void Show_Main();
void Show_Boot();
void Show_Page();
void Page_1();
void Page_2();
void Page_3();
void Page_4();
void Page_5();
void Display_String(unsigned char row, unsigned char col, unsigned char *dat);
void Lcd_Init();
void Write_CMD(unsigned char cmd);
void Write_DAT(unsigned char dat);
void Delay_ns(unsigned long ns);

void main()
{
  TMOD &= 0xF0;
  TMOD |= 0x01;
  EA = 1;
  TH0 = 0xFC;
  TL0 = 0x67;
  ET0 = 1;
  TR0 = 1;
  GPIO_LED_RED = 0;
  GPIO_LED_GREEN = 0;
  while(1)
  {
    KeyEvent();
  }
}

void Show_Boot();
void KeyEvent()
{
  if(GPIO_BTN_LAST == 0)
  {
    Delay_ns(1000);
    if(GPIO_BTN_LAST == 0)
    {
      index--;
      if(index < 1)
        index = 5;
      Show_Page();
    }
    while(!GPIO_BTN_LAST);
  }

  if(GPIO_BTN_NEXT == 0)
  {
    Delay_ns(1000);
    if(GPIO_BTN_NEXT == 0)
    {
      index++;
      if(index > 5)
        index = 1;
      Show_Page();
    }
    while(!GPIO_BTN_NEXT);
  }

  if(GPIO_BTN_OK == 0)
  {
    Delay_ns(1000);
    if(GPIO_BTN_OK == 0)
    {
      if(isboot == 0)
      {
        Show_Boot();
        isboot = 1;
      }
    }
    while(!GPIO_BTN_OK);
  }
}

void Show_Main()
{
  GPIO_LED_RED = 0;
  GPIO_LED_GREEN = 1;
  Display_String(1,2,"DATA:2022/05/17");
  Display_String(2,2,"TIME:AM 12:30");
  Display_String(3,2,"STEP:12000");
  Display_String(4,2,"FREQ:98bpm safe");
}

void Show_Boot()
{
  Lcd_Init();
  Delay_ns(10000);
  GPIO_LED_RED = 1;
  Display_String(2,2,"STARTING UP.");
  Display_String(2,2,"STARTING UP..");
  Display_String(2,2,"STARTING UP...");
  Delay_ns(10000);
  Display_String(2,2,"READY.           ");
  Display_String(2,2,"READY..");
  Display_String(2,2,"READY...");
  Delay_ns(10000);
  Show_Main();
}

void Show_Page()
{
  Write_CMD(0x01);
  switch(index)
  {
    case 1:
      Page_1();
      break;
    case 2:
      Page_2();
      break;
    case 3:
      Page_3();
      break;
    case 4:
      Page_4();
      break;
    case 5:
      Page_5();
      break;
  }
}

unsigned int num1 = 0;
void Page_1()
{
  Display_String(1,2,"Place input num1:" + num1);
  
}

void Page_2()
{

  Display_String(1,2,"2");
}

void Page_3()
{
  Show_Main();
}

void Page_4()
{
  Display_String(1,2,"4");
}

void Page_5()
{
  Display_String(1,2,"5");
}

void Display_String(unsigned char row, unsigned char col, unsigned char *dat)
{
  switch(row)
  {
    case 1:
      Write_CMD(0x80 + col);
      break;
    case 2:
      Write_CMD(0xC0 + col);
      break;
    case 3:
      Write_CMD(0x94 + col);
      break;
    case 4:
      Write_CMD(0xD4 + col);
      break;
  }
  while(1)
  {
    if(*dat == '\0') break;
    Write_DAT(*dat);
    dat++;
  }
}

void Lcd_Init()
{
  Write_CMD(0x38);
  Write_CMD(0x08);
  Write_CMD(0x01);
  Write_CMD(0x06);
  Write_CMD(0x0C);
}


void Write_CMD(unsigned char cmd)
{
  GPIO_RS = 0;
  GPIO_RW = 0;
  GPIO_LCD = cmd;
  GPIO_E = 0;
  Delay_ns(1);
  GPIO_E = 1;
  Delay_ns(500);
}

void Write_DAT(unsigned char dat)
{
  GPIO_RS = 1;
  GPIO_RW = 0;
  GPIO_LCD = dat;
  GPIO_E = 0;
  Delay_ns(1);
  GPIO_E = 1;
  Delay_ns(500);
}

void Delay_ns(unsigned long ns)
{
  while(ns--)
    _nop_();
}

void InterruptTimer0() interrupt 1
{
  TH0 = 0xFC;
  TL0 = 0x67; 
  i++;
}