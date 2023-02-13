#ifndef RECEIVER_DATALINK_LAYER_H
#define RECEIVER_DATALINK_LAYER_H

#include<stdio.h>
#include<Tools.h>
#include <string.h>
#include "Receiver_Network_Layer.h"

//数据链路层采用IEEE 802.3帧格式

//MAC帧首部共14个字节
unsigned char Local_Address[6];//本机地址
unsigned char Destination_Address[6];//目的地址
unsigned char Source_Address[6];//源地址
unsigned char Type[2];//类型，0x0800表示上层使用的是IP数据报

//数据部分，最长为1500字节，最短为46字节
unsigned char MAC_Data[1500];

//尾部部分
unsigned int FCS=0;//帧校验序列，采用CRC检验

void Receiver_DataLink_Layer(unsigned char* MAC,unsigned short MAC_Length)
{
    int n_MAC=(MAC_Length)<(1500+18)?1:(MAC_Length%1518==0)?(MAC_Length/1518):(MAC_Length/1518+1);
    for(int index=0;index<n_MAC;index++)
    {
        printf("当前正在接收方数据链路层...\n\n");
        int head=index*1518;

        //采用内存复制的方式将MAC帧的各个部分提取出来
        memcpy(Destination_Address,&MAC[head],6);//目的地址

        //验证是否与本机地址相同
        for(int i=0;i<6;i++)
        {
            if(Destination_Address[i]!=Local_Address[i])
            {
                printf("传输地址错误...\n");
                exit(1);
            }
        }
        printf("传输地址检验通过\n");
        memcpy(Source_Address,&MAC[head+6],6);//源地址
        printf("收到该地址传输来的数据： %d.%d.%d.%d.%d.%d\n",Source_Address[0],Source_Address[1],Source_Address[2],Source_Address[3],Source_Address[4],Source_Address[5]);
        unsigned short MAC_length=(n_MAC==1)?(MAC_Length):(index<n_MAC-1)?1518:(MAC_Length-1518*(n_MAC-1));

        //使用CRC检验
        FCS=UChartoUInt(&MAC[head+MAC_length-4],4);
        if(FCS!=crc32(&MAC[head],MAC_length-4))
        {
            printf("传输数据错误...\n");
            exit(1);
        }
        printf("CRC检验通过\n");        
        memcpy(Type,&MAC[head+12],2);//类型
        memcpy(MAC_Data,&MAC[head+14],MAC_length-18);//数据

        //打印出MAC帧相关信息
        printf("——————————————————数据链路层报文首部信息——————————————————\n");
        printf("目的地址： %u.%u.%u.%u.%u.%u\n",Destination_Address[0],Destination_Address[1],Destination_Address[2],Destination_Address[3],Destination_Address[4],Destination_Address[5]);
        printf("源地址：   %u.%u.%u.%u.%u.%u\n",Source_Address[0],Source_Address[1],Source_Address[2],Source_Address[3],Source_Address[4],Source_Address[5]);
        printf("类型：     IP\n");
        printf("尾部FCS：  %u\n",FCS);
        printf("********************************[报文各字段十六进制格式化输出]********************************\n");
        printf("目的地址（6字节）： ");
        Print_Oct_2Bytes(&MAC[head],6);
        printf("源地址（6字节）：   ");
        Print_Oct_2Bytes(&MAC[head+6],6);
        printf("类型（2字节）：     ");
        Print_Oct_2Bytes(&MAC[head+12],2);
        printf("FCS（4字节）：      ");
        Print_Oct_2Bytes(&MAC[head+MAC_length-4],4);
        printf("数据（%d字节）：\n",MAC_length-18);
        Print_Oct_2Bytes(&MAC[12],MAC_length-18);
        printf("———————————————————————————————————————————————\n");

        printf("\n已成功向上发送到网络层...\n\n");

        Receiver_Network_Layer(MAC_Data);

        FCS=0;
    }
}
#endif // RECEIVER_DATALINK_LAYER_H
