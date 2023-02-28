#include <REGX52.H>
#include <intrins.h>
sbit DIO = P3 ^ 4;//一位的串行数据，满足非0即1
sbit SR_CLK = P3 ^ 5;//串行下移
sbit R_CLK = P3 ^ 6;//输出


unsigned char ch[][8] = {
        //电
        //0xEF, 0x83, 0xAB, 0x83, 0xAB, 0x83, 0xED, 0xE1
        0xF7,0xC1,0xD5,0xC1,0xD5,0xC1,0xB7,0x87,

        //子  {0x81,0xFB,0xF7,0x00,0xF7,0xFB,0xDB,0xE7}、
        //反向
        0x81,0xDF,0xEF,0x00,0xEF,0xDF,0xDB,0xE7
};
//
//unsigned char ch[3] = {
//        0xfe, 0x40
//};

void writeByte(unsigned char c) {
    unsigned int i;
    SR_CLK = 0;
    R_CLK = 0;
    for (i = 0; i < 8; i++) {
        //DIO 接收先是一列八位数，再是一行八位数，这样就能保持正确
        DIO = 0x01 & (c >> i);//对c从低到高位依次取值，并将结果串行输入到DIO，取得的结果只能是0或1，所以对DIO直接赋值
        SR_CLK = 1;        //串行输入
        SR_CLK = 0;
    }
    //八位输出
//    R_CLK = 1;
//    R_CLK = 0;
}


void main() {
    unsigned char k,row = 0x80;
    unsigned int j,z=300;
    while(1){
        for(k=0;k<2;k++){
            while(z--){
                for (j = 0; j < 8; j++) {
                    writeByte(ch[k][j]);
                    writeByte(row);//因为共阳极
                    row=_cror_(row,1);//因为k有2次循环，不能用>>
                    R_CLK = 1;//并行输出
                    R_CLK = 0;
                }
            }
        }
    }
}
