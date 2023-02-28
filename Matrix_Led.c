/**
 * 需求：8*8 点阵屏，循环显示 “电子”。
 * 原理：共阳极点阵屏每次只点亮一列，8列依次快速轮动，看起来所有LED同时点亮。
 *      所有的数据都是由P3^4接收，8位数每位数据串行输入、并行输出。每列控制亮灭需要两个8位数，
 *          第一个数控制该列哪个给低电平，给低电平点亮。
 *          第二个数控制该列哪个给高电平，给高电平点亮。这样P3^4每接收两个8位数完成一列亮灭控制。循环8次完成一个屏幕控制。
 * 接口：P3^4 数据传输接口，只接收1或0
 *      P3^5 串行开关
 *      p3^6 并行开关
 */
#include <REGX52.H>
#include <intrins.h>

sbit DIO = P3 ^ 4;   //一位的串行数据输入端
sbit SR_CLK = P3 ^ 5;//串行输入开关
sbit R_CLK = P3 ^ 6; //并行输出开关

unsigned char ch[][8] = {
        //电
        0xF7, 0xC1, 0xD5, 0xC1, 0xD5, 0xC1, 0xB7, 0x87,
        //子
        0x81, 0xDF, 0xEF, 0x00, 0xEF, 0xDF, 0xDB, 0xE7
};

void writeByte(unsigned char c) {
    unsigned int i;
    SR_CLK = 0;
    R_CLK = 0;
    for (i = 0; i < 8; i++) {
        DIO = 0x01 & (c >> i);//二进制数单个位结果就是零或一，直接赋值给DIO即可
        SR_CLK = 1;//串行输入
        SR_CLK = 0;
    }
    //八位输出
//    R_CLK = 1;
//    R_CLK = 0;
}

void main() {
    unsigned char k, row;
    unsigned int j, z;
    while (1) {
        for (k = 0; k < 2; k++) {//两个字体，用二维数组表示
            for (z = 300; z > 0; z--) {//每个字体刷新300次
                row = 0x80;//300次"电"循环，每次循环重新赋值，避免出现问题
                for (j = 0; j < 8; j++) {//一个屏幕八列，循环8次
                    writeByte(ch[k][j]);//控制该列中哪个灯亮，就给低电平
                    writeByte(row);//哪列灯亮给高电平
                    R_CLK = 1;//并行输出
                    R_CLK = 0;
                    row = _cror_(row, 1);//让下一列灯亮
                }
            }
        }
    }
}
