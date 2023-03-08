/**
 * 更改ch[][]中的代表文字的八进制数，在8*8点阵显示
 *
 * param:
 *  DIO: P3^4     串行数据输入端
 *  SR_CLK: P3^5  串行输入开关
 *  R_CLK: P3^6   并行输出开关
 *
 * return: 无
 */

#include <REGX52.H>
#include <intrins.h>

sbit DIO = P3 ^ 4;
sbit SR_CLK = P3 ^ 5;
sbit R_CLK = P3 ^ 6;

unsigned char ch[][8] = {
        //电
        0xF7, 0xC1, 0xD5, 0xC1, 0xD5, 0xC1, 0xB7, 0x87,
        //子
        0x81, 0xDF, 0xEF, 0x00, 0xEF, 0xDF, 0xDB, 0xE7

//        0xDF, 0x9F, 0x68, 0xBD, 0xC8, 0x7D, 0x98, 0xCF,//珍
//        0xFF, 0x99, 0x66, 0x7E, 0xBD, 0xDB, 0xE7, 0xFF,//爱
//        0xFF, 0xEB, 0x83, 0xED, 0xC6, 0xEF, 0x01, 0xFF,//生
//        0xE7, 0xDB, 0x3C, 0xC3, 0x1F, 0x53, 0x53, 0xDF//命
};

void foo(unsigned char k, unsigned char row, unsigned int j, unsigned int z, unsigned int array_r);

/**
 * 每次向串口写入一个8位数
 * param:
 * return: 无
 */
void writeByte(unsigned char c) {
    unsigned int i;
    SR_CLK = 0;
    R_CLK = 0;
    for (i = 0; i < 8; i++) {
        DIO = 0x01 & (c >> i);//c右移i位后，&0x01，就是单独获取第i位的值，
        //二进制数每一位结果就是零或一，直接赋值给DIO即可
        SR_CLK = 1;//P35串行输入
        SR_CLK = 0;
    }
//    八位输出
//    R_CLK = 1;
//    R_CLK = 0;
}

/**
 * 每次向串口写入一个8位数
 * param:
 *  k:
 *  row:
 *  j:
 *  z:
 * return: 无
 */
void Matrix_display() {
    unsigned char j,k,row,z=255;
    unsigned char array_r = sizeof(ch)/sizeof(ch[0]);//计算有几个字
    while (1) {
        for (k = 0; k < array_r; k++) {//有几个字，就循环几次
            while (z--) {//每个字刷新255次
                row = 0x80;//字每次刷新，`行高电平`就要重新赋值，避免出现问题，8*8矩阵左上角是行列高位，所以赋值0x80

                //writeByte调用两次，每次向DIO输入两个八进制数，代表高低电平
                for (j = 0; j < 8; j++) {//一个屏幕八列，遍历显示,循环8次
                    writeByte(ch[k][j]);//列：输入一个8进制数，二极管低电平端工作，每次输入从数组取值
                    writeByte(row);//行：输入一个8进制数，二极管高电平端工作，代表显示点阵屏第几列被点亮
                    R_CLK = 1;//P36并行输出
                    R_CLK = 0;
                    row = _cror_(row, 1);//让下一列灯亮
                }
            }
        }
    }
}

void main() {
    Matrix_display();
}