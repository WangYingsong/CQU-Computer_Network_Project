#ifndef SENDER_NETWORK_LAYER_H
#define SENDER_NETWORK_LAYER_H

#include<stdio.h>
#include<string.h>
#include<Tools.h>
#include"Sender_DataLink_Layer.h"

//网络层采用IPv4协议

//IP数据报受MTU限制，最大长度为1500字节
unsigned char IP[1500];

//IPv4数据报首部，不考虑可选字段，共占20个字节
unsigned char Version = 0x04;//版本，占4个bit，采用IPv4协议，版本号为4
unsigned char Head_Length=0x05;//首部长度，占4个bit，单位是32位字长，不考虑可选字段，则首部长度为固定的20字节，即5*32bit
unsigned char DS=0x00;//区分服务，没有使用，故全为0
unsigned short IP_Length=0;//IP总长度，占16bit，单位为字节，受MTU限制，最大值为1500
unsigned short Identification=0;//标识，占16bit，同一个数据报分片后标识相同
unsigned char MF=0x00;//标志位的最低位，MF=1表示后面“还有分片”，MF=0表示是分片的最后一个
unsigned char DF=0x01;//标志位的中间位，DF=1表示不能分片，DF=0表示可以分片
unsigned short Fragment_Offset=0;//片偏移，占13bit，表示在分片后，某片在原分组中的相对位置，以8个字节为单位
unsigned char TTL=0xFF;//生存时间，占8bit，跳数限制
unsigned char Protocol=0x11;//协议，占8bit，17表示将数据部分交给UDP协议处理
unsigned short Head_CheckSum=0;//首部检验和，占16bit，只检验数据报的首部，不包括数据部分
unsigned char Source_IP[4];//源地址，占32bit，即4bytes
unsigned char Destination_IP[4];//目的地址，占32bit，即4bytes
//不考虑可选字段
unsigned char IP_Data[1500-20];//数据部分,受MTU限制，最长为1500个字节,减去首部的20字节，数据部分最长为1480字节


void Sender_Network_Layer(unsigned char* Data,unsigned short Data_Length)
{
    printf("当前正在发送方网络层...\n\n");

    //检查数据的长度是否超过1480字节，若没超过，可以直接封装
    if(Data_Length<(1500-20))
    {
        //采用内存复制的方式将IP的各个部分填写
        unsigned char Version_HeadLength=(Version<<4)+Head_Length;//版本号和首部长度各占4bit，所以把他们拼起来占IP的第一个byte
        memcpy(&IP[0],&Version_HeadLength,1);
        memcpy(&IP[1],&DS,1);//区分服务
        IP_Length=20+Data_Length;
        memcpy(&IP[2],&IP_Length,2);//总长度
        Turn(&IP[2],2);
        memcpy(&IP[4],&Identification,2);//标识
        Turn(&IP[4],2);
        unsigned char Flag_FragmentOffset[2];//标志占3bit，片偏移占13bit，所以把他们拼起来占2个byte
        Flag_FragmentOffset[0]=(0x00<<7)+(DF<<6)+(MF<<5)+((Fragment_Offset&0x1f00)>>8);
        Flag_FragmentOffset[1]=Fragment_Offset;
        memcpy(&IP[6],Flag_FragmentOffset,2);
        memcpy(&IP[8],&TTL,1);//生存时间
        memcpy(&IP[9],&Protocol,1);//协议
        memcpy(&IP[10],&Head_CheckSum,2);//首部检验和
        memcpy(&IP[12],Source_IP,4);//源地址
        memcpy(&IP[16],Destination_IP,4);//目的地址
        memcpy(&IP[20],Data,Data_Length);//数据部分

        //计算首部检验和，按照二进制反码运算求和
        unsigned int Sum=0;
        for(int i=0;i<19;i=i+2)
            Sum+=(IP[i]<<8)+IP[i+1];
        Head_CheckSum = (Sum >> 16) + (Sum & 0xffff);
        Head_CheckSum = ~Head_CheckSum;//取反码
        memcpy(&IP[10],&Head_CheckSum,2);//将计算出的检验和填入检验和字段
        Turn(&IP[10],2);

        //打印出IP数据报相关信息
        printf("———————————————————网络层报文首部信息———————————————————\n");
        printf("版本：       %d\n",Version);
        printf("首部长度：   %d\n",Head_Length);
        printf("区分服务：   未使用\n");
        printf("总长度：     %d\n",IP_Length);
        printf("标志：       0%d%d\n",DF,MF);
        printf("片偏移：     %d\n",Fragment_Offset);
        printf("生存时间：   %d\n",TTL);
        printf("协议：       UDP\n");
        printf("首部检验和： %d\n",Head_CheckSum);
        printf("源地址：     %u.%u.%u.%u\n",Source_IP[0],Source_IP[1],Source_IP[2],Source_IP[3]);
        printf("目的地址：   %u.%u.%u.%u\n",Destination_IP[0],Destination_IP[1],Destination_IP[2],Destination_IP[3]);
        printf("可选字段：   无\n");
        printf("********************************[报文各字段十六进制格式化输出]********************************\n");
        printf("版本 首部长度（1字节）： ");
        Print_Oct_2Bytes(&IP[0],1);
        printf("区分服务（1字节）：      ");
        Print_Oct_2Bytes(&IP[1],1);
        printf("总长度（2字节）：        ");
        Print_Oct_2Bytes(&IP[2],2);
        printf("标识（2字节）：          ");
        Print_Oct_2Bytes(&IP[4],2);
        printf("标志 片偏移（2字节）：   ");
        Print_Oct_2Bytes(&IP[6],2);
        printf("生存时间（1字节）：      ");
        Print_Oct_2Bytes(&IP[8],1);
        printf("协议（1字节）：          ");
        Print_Oct_2Bytes(&IP[9],1);
        printf("首部检验和（2字节）：    ");
        Print_Oct_2Bytes(&IP[10],2);
        printf("源地址（4字节）：        ");
        Print_Oct_2Bytes(&IP[12],4);
        printf("目的地址（4字节）：      ");
        Print_Oct_2Bytes(&IP[16],4);
        printf("数据部分（%d字节）：\n",IP_Length-20);
        Print_Oct_2Bytes(&IP[20],IP_Length-20);
        printf("———————————————————————————————————————————————\n");
        printf("\n已成功向下发送到数据链路层...\n\n");

        Sender_DataLink_Layer(IP,IP_Length);
    }
    //超过1480字节，需要分片后再封装
    else
    {
        printf("数据长度为%d字节，超过1480字节，需要分片...\n\n",Data_Length);
        int i_IP=0;
        int data_length=Data_Length;
        while(data_length>0)
        {
            if (i_IP>0)
                flag=true;
            //采用内存复制的方式将IP的各个部分填写
            unsigned char Version_HeadLength=(Version<<4)+Head_Length;//版本号和首部长度各占4bit，所以把他们拼起来占IP的第一个byte
            memcpy(&IP[0],&Version_HeadLength,1);
            memcpy(&IP[1],&DS,1);//区分服务
            IP_Length=(data_length<1480)?(20+data_length):(20+1480);
            memcpy(&IP[2],&IP_Length,2);//总长度
            Turn(&IP[2],2);
            memcpy(&IP[4],&Identification,2);//标识
            DF=0;//分片后DF置0
            Turn(&IP[4],2);
            Fragment_Offset=i_IP*1480/8;
            MF=(data_length<1480)?0:1;//MF在未切片的数据长度小于1480时为0，其余时候为1
            unsigned char Flag_FragmentOffset[2];//标志占3bit，片偏移占13bit，所以把他们拼起来占2个byte
            Flag_FragmentOffset[0]=(0x00<<7)+(DF<<6)+(MF<<5)+((Fragment_Offset&0x1f00)>>8);
            Flag_FragmentOffset[1]=Fragment_Offset;
            memcpy(&IP[6],Flag_FragmentOffset,2);
            memcpy(&IP[8],&TTL,1);//生存时间
            memcpy(&IP[9],&Protocol,1);//协议
            memcpy(&IP[10],&Head_CheckSum,2);//首部检验和
            memcpy(&IP[12],Source_IP,4);//源地址
            memcpy(&IP[16],Destination_IP,4);//目的地址
            unsigned short l=(data_length<1480)?data_length:1480;
            memcpy(&IP[20],&Data[i_IP*1480],l);//数据部分

            //计算首部检验和，按照二进制反码运算求和
            unsigned int Sum=0;
            for(int i=0;i<19;i=i+2)
                Sum+=(IP[i]<<8)+IP[i+1];
            Head_CheckSum = (Sum >> 16) + (Sum & 0xffff);
            Head_CheckSum = ~Head_CheckSum;//取反码
            memcpy(&IP[10],&Head_CheckSum,2);//将计算出的检验和填入检验和字段
            Turn(&IP[10],2);

            //打印出IP数据报相关信息
            printf("———————————————————网络层报文首部信息———————————————————\n");
            printf("[分片后的第%d个IP数据报]\n",i_IP+1);
            printf("版本：       %d\n",Version);
            printf("首部长度：   %d\n",Head_Length);
            printf("区分服务：   未使用\n");
            printf("总长度：     %d\n",IP_Length);
            printf("标志：       0%d%d\n",DF,MF);
            printf("片偏移：     %d\n",Fragment_Offset);
            printf("生存时间：   %d\n",TTL);
            printf("协议：       UDP\n");
            printf("首部检验和： %d\n",Head_CheckSum);
            printf("源地址：     %d.%d.%d.%d\n",Source_IP[0],Source_IP[1],Source_IP[2],Source_IP[3]);
            printf("目的地址：   %d.%d.%d.%d\n",Destination_IP[0],Destination_IP[1],Destination_IP[2],Destination_IP[3]);
            printf("可选字段：   无\n");
            printf("********************************[报文各字段十六进制格式化输出]********************************\n");
            printf("版本 首部长度（1字节）： ");
            Print_Oct_2Bytes(&IP[0],1);
            printf("区分服务（1字节）：      ");
            Print_Oct_2Bytes(&IP[1],1);
            printf("总长度（2字节）：        ");
            Print_Oct_2Bytes(&IP[2],2);
            printf("标识（2字节）：          ");
            Print_Oct_2Bytes(&IP[4],2);
            printf("标志 片偏移（2字节）：   ");
            Print_Oct_2Bytes(&IP[6],2);
            printf("生存时间（1字节）：      ");
            Print_Oct_2Bytes(&IP[8],1);
            printf("协议（1字节）：          ");
            Print_Oct_2Bytes(&IP[9],1);
            printf("首部检验和（2字节）：    ");
            Print_Oct_2Bytes(&IP[10],2);
            printf("源地址（4字节）：        ");
            Print_Oct_2Bytes(&IP[12],4);
            printf("目的地址（4字节）：      ");
            Print_Oct_2Bytes(&IP[16],4);
            printf("数据部分（%d字节）：\n",IP_Length-20);
            Print_Oct_2Bytes(&IP[20],IP_Length-20);
            printf("———————————————————————————————————————————————\n");
            printf("\n已成功向下发送到数据链路层...\n\n");

            Sender_DataLink_Layer(IP,IP_Length);

            i_IP++;
            data_length-=1480;
            Head_CheckSum=0;
        }
    }
}

#endif // SENDER_NETWORK_LAYER_H
