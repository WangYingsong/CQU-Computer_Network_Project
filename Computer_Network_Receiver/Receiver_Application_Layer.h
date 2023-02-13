#ifndef RECEIVER_APPLICATION_LAYER_H
#define RECEIVER_APPLICATION_LAYER_H

#include <stdio.h>
#include "Tools.h"
//接收方应用层

unsigned char Local_Port[2];//本应用程序的端口号

//应用层协议定义如下：
unsigned char APP[65527];//应用层报文最大长度为65527，因为UDP报文最大长度为65535，需要减去UDP首部的8个字节
//应用层报文由5个字段组成,前四个字段为首部，第五个字段为数据部分
//首部
//第一个字段是协议，由2个字节组成
unsigned char APP_Protocol[2];//协议为AC 44表示采用WYS_HZC协议
//第二个字段是发送的消息类型,由1个字节组成
unsigned char MessageType[1];//消息类型为01表示为文本消息，02为图片消息，03为语音消息，04为戳一戳消息
//第三个字段是发送者姓名，由12个字节组成
unsigned char Sender_Name[12]={0};
//第四个字段是接收者姓名，由12个字节组成
unsigned char Receiver_Name[12]={0};

//第五个字段是发送的消息，最多可以有65527-2-1-12-12=65500个字节
int MessageLength=50;//初始化一条消息最初能发送的长度为50个字符
unsigned char Message[65507];

void Receiver_Application_Layer(unsigned char* APP,unsigned short APP_Length)
{
    printf("当前正在接收方应用层...\n\n");

    //读入本应用进程的端口号
    FILE* Local_port;
    Local_port = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\Computer_Network_Receiver\\Receiver_Port.txt", "r");
    fscanf(Local_port,"%u",&Local_Port);
    fclose(Local_port);
    Turn(Local_Port,2);

    //验证目的端口号是否与本应用程序端口号相同
    for(int i=0;i<2;i++)
    {
        if(Destination_Port[i]!=Local_Port[i])
        {
            printf("端口号错误...\n");
            exit(1);
        }
    }
    printf("端口号检验通过\n");

    //采用内存复制的方式获取APP报文的各字段信息
    memcpy(APP_Protocol,&APP[0],2);//协议
    memcpy(MessageType,&APP[2],1);//消息类型
    memcpy(Sender_Name,&APP[3],12);//发送者姓名
    memcpy(Receiver_Name,&APP[15],12);//接收者姓名
    memcpy(Message,&APP[27],APP_Length-27);//消息
    printf("收到来自 %s 的消息\n",Sender_Name);
    printf("接收到的消息为：\n%s\n",Message);
    //打印APP报文相关信息
    printf("———————————————————应用层报文首部信息———————————————————\n");
    printf("协议：       WYS_HZC协议\n");
    printf("消息类型：   文本消息\n");
    printf("发送者姓名： %s\n",Sender_Name);
    printf("接收者姓名： %s\n",Receiver_Name);
    printf("********************************[报文各字段十六进制格式化输出]********************************\n");
    printf("协议（2字节）：        ");
    Print_Oct_2Bytes(APP_Protocol,2);
    printf("消息类型（1字节）：    ");
    Print_Oct_2Bytes(MessageType,1);
    printf("发送者姓名（12字节）： ");
    Print_Oct_2Bytes(Sender_Name,12);
    printf("接收者姓名（12字节）： ");
    Print_Oct_2Bytes(Receiver_Name,12);
    printf("消息（%d个字节）：\n",APP_Length-27);
    Print_Oct_2Bytes(Message,APP_Length-27);
    printf("———————————————————————————————————————————————\n");
}
#endif // RECEIVER_APPLICATION_LAYER_H
