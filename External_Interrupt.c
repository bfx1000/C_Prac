#include <reg52.h>

//0、参数准备
sbit button_S2 = P3 ^ 0;
sbit flag = P3 ^ 7;//中断信号产生引脚

//1、中断初始化
void Int0Init() {
    EA = 1;
    EX0 = 1;
    IT0 = 0;//P32低电平触发
}

void Delay(unsigned char t) {
    unsigned char i, j;
    for (j = 0; j < t; j++) {
        for (i = 0; i < 114; i++);
    }
}

//2、main函数
void main() {
    Int0Init();
    while (1) {
        if (button_S2 == 0) {
            Delay(20);//防误触
            if (button_S2 == 0) {
                flag = 0;
                flag = 0;
                flag = 1;
                while (!button_S2);//松手检测
            }
        }
    }
}

//3、中断
void Int0() interrupt 0{
    P1 = ~P1;//P1是LED
}