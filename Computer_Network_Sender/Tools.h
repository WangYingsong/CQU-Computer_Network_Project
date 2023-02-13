#ifndef TOOLS_H
#define TOOLS_H

#include<stdio.h>

//将数据按照16进制输出，每一个字节跟一个空格
void Print_Oct_2Bytes(unsigned char* x,unsigned int len)
{
    for(int i=0;i<len;i++)
    {
        printf("%02x",x[i]);
        if(i!=len-1)
            printf(" ");
    }
    printf("\n");
}

//将小端存储的数据交换顺序成大端存储
void Turn(unsigned char* x,unsigned int len)
{
    unsigned char temp[1];
    for(int i=0;i<len/2;i++)
    {
        memcpy(&temp,&x[i],1);
        memcpy(&x[i],&x[len-i-1],1);
        memcpy(&x[len-i-1],&temp,1);
    }
}

//将unsigned char数组转换为unsigned short
unsigned short UChartoUShort(unsigned char* Chars,int Chars_Length)
{
    unsigned short sum=0;
    for(int i=0;i<Chars_Length;i++)
        sum+=(Chars[i]<<(8*(Chars_Length-i-1)));
    return sum;
}

//将unsigned char数组转换为unsigned int
unsigned int UChartoUInt(unsigned char* Chars,int Chars_Length)
{
    unsigned int sum=0;
    for(int i=0;i<Chars_Length;i++)
        sum+=(Chars[i]<<(8*(Chars_Length-i-1)));
    return sum;
}

//采用32位循环检验码
/******************************************************************************
 * Name:    CRC-32  x32+x26+x23+x22+x16+x12+x11+x10+x8+x7+x5+x4+x2+x+1
 * Poly:    0x4C11DB7
 * Init:    0xFFFFFFF
 * Refin:   True
 * Refout:  True
 * Xorout:  0xFFFFFFF
 * Alias:   CRC_32/ADCCP
 * Use:     WinRAR,ect.
 *****************************************************************************/
unsigned int crc32(unsigned char *data, unsigned short length)
{
    unsigned short i;
    unsigned int crc = 0xffffffff;        // Initial value
    while(length--)
    {
        crc ^= *data++;                // crc ^= *data; data++;
        for (i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;// 0xEDB88320= reverse 0x04C11DB7
            else
                crc = (crc >> 1);
        }
    }
    return ~crc;
}

#endif // TOOLS_H
