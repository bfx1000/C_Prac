/**
 * 需求：双计数器并用，每秒计数，呼吸灯同步亮灭
 */
#include <reg52.h>
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char

sbit DU = P2 ^ 6;//数码管段选
sbit WE = P2 ^ 7;//数码管段选
sbit LED1 = P1 ^ 0;//LED1
//共阴数码管段选表0-9
uchar code
tabel[]= {
0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F,};

/*====================================
描述	：12T/Fosc11.0592M毫秒级延时
====================================*/
void delay(uint z) {
    uint x, y;
    for (x = z; x > 0; x--)
        for (y = 114; y > 0; y--);
}

/*====================================
描述	：三位共阴数码管动态显示
====================================*/
void display(uchar i) {
    uchar bai, shi, ge;
    bai = i / 100; //236 / 100  = 2
    shi = i % 100 / 10;    //236 % 100 / 10 = 3
    ge = i % 10;//236 % 10 =6

    //第一位数码管
    P0 = 0XFF;//清除断码
    WE = 1;//打开位选锁存器
    P0 = 0XFE; //1111 1110
    WE = 0;//锁存位选数据

    DU = 1;//打开段选锁存器
    P0 = tabel[bai];//
    DU = 0;//锁存段选数据
    delay(5);

    //第二位数码管
    P0 = 0XFF;//清除断码
    WE = 1;//打开位选锁存器
    P0 = 0XFD; //1111 1101
    WE = 0;//锁存位选数据

    DU = 1;//打开段选锁存器
    P0 = tabel[shi];//
    DU = 0;//锁存段选数据
    delay(5);

    //第三位数码管
    P0 = 0XFF;//清除断码
    WE = 1;//打开位选锁存器
    P0 = 0XFB; //1111 1011
    WE = 0;//锁存位选数据

    DU = 1;//打开段选锁存器
    P0 = tabel[ge];//
    DU = 0;//锁存段选数据
    delay(5);
}

//选择哪个模式？定时器：计数器
void timer0Init() {//计数器
    TR0 = 1;//允许计数
    TMOD |= 0x05;//如果同时使用计数器零和计数器一，因只有一个TMOD分别使用低位和高位，需要`或`运算
    TH0 = 0;
    TL0 = 0;
}
//选择哪个模式？定时器：计数器
void timer1Init() {//定时器
    TR1 = 1;//允许计数
    TMOD |= 0x10;//如果同时使用计数器零和计数器一，因只有一个TMOD分别使用低位和高位，需要`或`运算
    TH1 = 0x4b;
    TL1 = 0xff;//定时50ms
}




void main() {//main函数自身会循环
    uchar Xmulti;//多少个50ms
    timer0Init();
    timer1Init();
    while (1){

        //计数器1溢出
        if(TF1==1){
            TF1=0;
            TH1 = 0x4b;
            TL1 = 0xff;//定时50ms
            Xmulti++;
            if(Xmulti >= 10){//每隔1s，呼吸灯亮灭1轮
                Xmulti = 0;
                LED1 = ~LED1;//呼吸灯
            }
        }

        display(TL0);

        //TL0溢出问题
        if(TL0>=250){
            TL0=0;
        }
    }
}