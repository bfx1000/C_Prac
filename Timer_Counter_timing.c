/*********************************************************************************
* 【程序功能】：定时器0工作模式1 16位定时模式，数码管动态显示0-10，秒表。
* 【原理说明】：详见timer0Init()注释。
**********************************************************************************/
#include <reg52.h>
#include <intrins.h>

sbit DU = P2 ^ 6;//数码管段选
sbit WE = P2 ^ 7;//数码管位选

//共阴数码管段选表0-9
//加code是存放在ROM
unsigned char code table[]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,};

void delay(unsigned int z) {
    unsigned int x, y;
    for (x = z; x > 0; x--)
        for (y = 114; y > 0; y--);
}

/*====================================
描述	：三位共阴数码管动态显示，显示输入数值，取值范围0-255
====================================*/
void display(unsigned char i) {
    unsigned char bai, shi, ge;
    bai = i / 100; //236 / 100  = 2
    shi = i % 100 / 10;    //236 % 100 / 10 = 3
    ge = i % 10;//236 % 10 =6

    //第一位数码管
    P0 = 0XFF;//清除断码
    WE = 1;//打开位选锁存器
    P0 = 0XFE; //1111 1110
    WE = 0;//锁存位选数据

    DU = 1;//打开段选锁存器
    P0 = table[bai];
    DU = 0;//关闭段选锁存器
    delay(5);

    //第二位数码管
    P0 = 0XFF;
    WE = 1;
    P0 = 0XFD; //1111 1101
    WE = 0;

    DU = 1;
    P0 = table[shi];
    DU = 0;
    delay(5);

    //第三位数码管
    P0 = 0XFF;
    WE = 1;
    P0 = 0XFB; //1111 1011
    WE = 0;

    DU = 1;
    P0 = table[ge];
    DU = 0;
    delay(5);
}

/*
 * 功能：定时器0初始化
 *
 * TR0：Timer Ready，启动定时器0
 *
 * TMOD：工作模式控制位，赋值0x01表示定时器最常用工作模式1，16位定时器计数模式、TL和TH全启用（还有13位工作模式、8位工作模式、禁用模式）
 *
 * TH、TL：计数累加器，16进制，最大累加71.1ms
 * 一个机器周期，等于12个晶振周期，等于 12/11.0592=1.085 ms，例如要计时50ms，就需要赋初值
 *      TH0 = (65535 - (50000 / (12 / 11.0592))) / 256;
 *      TL0 = (65535 - (50000 / (12 / 11.0592))) % 256;
 *
 * TF0 ==1 表示溢出
 */
void timer0Init() {
    TR0 = 1;
    TMOD = 0X01;
    TH0 = 0x4b;//(65535 - (50000 / (12 / 11.0592))) / 256; 高位最多65535，超出部分溢出
    TL0 = 0xfd;//(65535 - (50000 / (12 / 11.0592))) % 256; 低位最多255，超出的部分计入TH
}

void main()//main函数自身会循环
{
    unsigned char mSec, Sec;//毫秒和秒储存变量
    timer0Init();//定时器0初始化
    while (1) {
        if (TF0 == 1) {//判断是否溢出
            TF0 = 0;//溢出清零
            TH0 = 0x4b;
            TL0 = 0xfd;
            mSec++;
            if (mSec == 20) {
                mSec = 0;
                Sec++;//已经累计1秒
            }
        }
        display(Sec); //数码管显示函数，运行程序时间就会累加。所以持续循环display(Sec)，就会造成时间累加
        if (Sec > 10)
            Sec = 0;//秒清零，因为unsigned char最大限制255
    }
}