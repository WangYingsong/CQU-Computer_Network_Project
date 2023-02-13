#ifndef RECEIVER_TRANSPORT_LAYER_H
#define RECEIVER_TRANSPORT_LAYER_H

#include<stdio.h>
#include<string.h>
#include"Receiver_Application_Layer.h"
#include<Tools.h>

//网络层采用UDP协议

//UDP有两个字段，首部和数据与部分
//UDP首部共八个字节，由四个字段组成，每个字段占2个字节
unsigned char Source_Port[2];//源端口号占UDP报文的2个字节，在需要对方回信时使用，不需要时可全用0。用十六进制表示
//目的端口号占UDP报文的2个字节，在终点交付报文时必须使用。用十六进制表示
unsigned short UDP_Length;//UDP用户数据报的长度，最小值是8（仅有首部时），单位是字节
unsigned short CheckSum=0;//检验和，检测UDP用户数据报在传输中是否有错，有错就丢弃。
//UDP数据部分
unsigned char UDP_Data[65527];//数据部分的最大长度为65527字节，因为IP数据报的最大长度为2^16，需要减去8个字节的UDP首部长

//UDP伪首部
unsigned char UDP_Pseudohead[12];//用于校验和，包括4字节源IP地址，4字节目的IP地址，1字节0，1字节17，2字节UDP长度

void Receiver_Transport_Layer(unsigned char* UDP)
{
    printf("当前正在接收方传输层...\n\n");

    //采用内存复制的方式将UDP报文的各个部分提取出来
    UDP_Length=UChartoUShort(&UDP[4],2);//UDP长度

    //伪首部填写
    memcpy(&UDP_Pseudohead[0],Source_IP,4);//源IP地址
    memcpy(&UDP_Pseudohead[4],Destination_IP,4);//目的IP地址
    unsigned char Zero[1]={0x00};
    memcpy(&UDP_Pseudohead[8],Zero,1);//第9个字节全为0
    unsigned char UDP_Protocol[1]={0x11};
    memcpy(&UDP_Pseudohead[9],UDP_Protocol,1);//第10个字节为17
    memcpy(&UDP_Pseudohead[10],&UDP[4],2);//UDP长度

    //计算检验和，按照二进制反码运算求和
    unsigned Sum=0;
    for(int i=0;i<12;i=i+2)
        Sum+=(UDP_Pseudohead[i]<<8)+UDP_Pseudohead[i+1];//计算伪首部的检验和
    if(UDP_Length%2==1)//如果UDP的长度为基数，需要在尾部补上一个字节的全0再求和
    {
        for(int i=0;i<UDP_Length-2;i=i+2)
            Sum+=(UDP[i]<<8)+UDP[i+1];
        Sum+=(UDP[UDP_Length-1]<<8)+Zero[0];
    }
    else//UDP的长度为偶数，直接求和
    {
        for(int i=0;i<UDP_Length-1;i=i+2)
            Sum+=(UDP[i]<<8)+UDP[i+1];
    }
    CheckSum = (Sum >> 16) + (Sum & 0xffff);
    if(CheckSum!=0xffff)
    {
        printf("UDP检验和错误\n");
        exit(1);
    }
    printf("UDP检验和正确\n");
    memcpy(Source_Port,&UDP[0],2);//源端口
    memcpy(Destination_Port,&UDP[2],2);//目的端口
    memcpy(&UDP_Data,&UDP[8],UDP_Length-8);//数据部分

    //打印出UDP相关信息
    printf("———————————————————传输层报文首部信息———————————————————\n");
    printf("源端口：   %d\n",UChartoUShort(&UDP[0],2));
    printf("目的端口： %d\n",UChartoUShort(&UDP[2],2));
    printf("UDP长度：  %d\n",UChartoUShort(&UDP[4],2));
    printf("检验和：   %d\n",UChartoUShort(&UDP[6],2));
    printf("********************************[报文各字段十六进制格式化输出]********************************\n");
    printf("源端口（2字节）：         ");
    Print_Oct_2Bytes(&UDP[0],2);
    printf("目的端口（2字节）:        ");
    Print_Oct_2Bytes(&UDP[2],2);
    printf("UDP长度（2字节）          ");
    Print_Oct_2Bytes(&UDP[4],2);
    printf("检验和（2字节）           ");
    Print_Oct_2Bytes(&UDP[6],2);
    printf("UDP的数据部分（%d字节）：\n",UDP_Length-8);
    Print_Oct_2Bytes(&UDP[8],UDP_Length-8);
    printf("———————————————————————————————————————————————\n");
    printf("已成功向上发送到应用层...\n\n");

    Receiver_Application_Layer(UDP_Data,UDP_Length-8);
}
#endif // RECEIVER_TRANSPORT_LAYER_H
