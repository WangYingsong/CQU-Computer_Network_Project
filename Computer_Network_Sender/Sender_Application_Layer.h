#ifndef SENDER_APPLICATION_LAYER_H
#define SENDER_APPLICATION_LAYER_H

#include <stdio.h>
#include "Sender_Transport_Layer.h"

//发送方应用层

//应用层协议定义如下：
unsigned char APP[65527];//应用层报文最大长度为65527，因为UDP报文最大长度为65535，需要减去UDP首部的8个字节
//应用层报文由5个字段组成,前四个字段为首部，第五个字段为数据部分
//首部
//第一个字段是协议，由2个字节组成
unsigned char APP_Protocol[2]={0xAC,0x44};//协议为AC 44表示采用WYS_HZC协议
//第二个字段是发送的消息类型,由1个字节组成
unsigned char Message_Type[1]={0x01};//消息类型为01表示为文本消息，02为图片消息，03为语音消息，04为戳一戳消息
//第三个字段是发送者姓名，由12个字节组成
unsigned char Sender_Name[12]={0};
//第四个字段是接收者姓名，由12个字节组成
unsigned char Receiver_Name[12]={0};

//第五个字段是发送的消息，最多可以有65527-2-1-12-12=65500个字节
int MessageLength=50;//初始化一条消息最初能发送的长度为50个字符


void Sender_Application_Layer()
{
    printf("当前正在发送方应用层...\n\n");

    //读入本应用进程的端口号
    FILE* Local_Port;
    Local_Port = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\Computer_Network_Sender\\Sender_Port.txt", "r");
    fscanf(Local_Port,"%u",&Source_Port);
    fclose(Local_Port);
    Turn(Source_Port,2);

    printf("请输入你的姓名（最长6个中文或12个英文）：");
    scanf("%s",Sender_Name);
    printf("请输入你发送消息的接收者的姓名（最长6个中文或12个英文）：");
    scanf("%s",Receiver_Name);
    printf("请输入消息接收者的IP地址和端口号：");
    scanf("%u.%u.%u.%u:%u",&Destination_IP[0],&Destination_IP[1],&Destination_IP[2],&Destination_IP[3],&Destination_Port);
    getc(stdin);
    Turn(Destination_Port,2);
    int AlreadyInputChars=0;//记录已经输入的字符个数
    char theInputChar =NULL;//用于接受当前输入的字符
    unsigned char *Messeage = (unsigned char *)malloc(sizeof(char)*50);//用于存储输入的消息，大小初始化为50个字符的大小
    printf("请输入你想发送的消息（最大消息长度为65500字节）：\n");
    while((theInputChar=getc(stdin))!='\n') {
        Messeage[AlreadyInputChars++] = theInputChar;
        if(AlreadyInputChars%50==0) { //如果字符串已用长度等于分配的总长度-1时
            MessageLength+=50;    //更新数组能存储的最大长度
            Messeage =(unsigned char*)realloc(Messeage,MessageLength);//扩大字符串的最大容量
        }
        if(AlreadyInputChars==65500)
        {
            printf("输入消息已达长度上限...\n");
            break;
        }
    }

    //采用内存复制方式将各字段填入APP报文
    memcpy(&APP[0],APP_Protocol,2);//协议
    memcpy(&APP[2],Message_Type,1);//消息类型
    memcpy(&APP[3],Sender_Name,12);//发送者姓名
    memcpy(&APP[15],Receiver_Name,12);//接收者姓名
    memcpy(&APP[27],Messeage,AlreadyInputChars);//消息

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
    Print_Oct_2Bytes(Message_Type,1);
    printf("发送者姓名（12字节）： ");
    Print_Oct_2Bytes(Sender_Name,12);
    printf("接收者姓名（12字节）： ");
    Print_Oct_2Bytes(Receiver_Name,12);
    printf("消息（%d个字节）：\n",AlreadyInputChars);
    Print_Oct_2Bytes(Messeage,AlreadyInputChars);
    printf("———————————————————————————————————————————————\n");
    printf("\n已成功向下发送到传输层...\n\n");

    Sender_Transport_Layer(APP,AlreadyInputChars+27);
}

#endif // SENDER_APPLICATION_LAYER_H
