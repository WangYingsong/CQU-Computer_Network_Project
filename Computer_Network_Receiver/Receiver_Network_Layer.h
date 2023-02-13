#ifndef RECEIVER_NETWORK_LAYER_H
#define RECEIVER_NETWORK_LAYER_H

#include <stdio.h>
#include <string.h>
#include "Receiver_Transport_Layer.h"
#include "Tools.h"

//网络层采用IPv4协议

//IPv4数据报首部，不考虑可选字段，共占20个字节
unsigned char Version;//版本，占4个bit，采用IPv4协议，版本号为4
unsigned char Head_Length;//首部长度，占4个bit，单位是32位字长，不考虑可选字段，则首部长度为固定的20字节，即5*32bit
unsigned char DS;//区分服务，没有使用，故全为0
unsigned short IP_Length;//IP总长度，占16bit，单位为字节，受MTU限制，最大值为1500
unsigned short Identification;//标识，占16bit，同一个数据报分片后标识相同
unsigned char MF;//标志位的最低位，MF=1表示后面“还有分片”，MF=0表示是分片的最后一个
unsigned char DF;//标志位的中间位，DF=1表示不能分片，DF=0表示可以分片
unsigned short Fragment_Offset;//片偏移，占13bit，表示在分片后，某片在原分组中的相对位置，以8个字节为单位
unsigned char TTL;//生存时间，占8bit，跳数限制
unsigned char Protocol;//协议，占8bit，17表示将数据部分交给UDP协议处理
unsigned short Head_CheckSum=0;//首部检验和，占16bit，只检验数据报的首部，不包括数据部分
//源地址，占32bit，即4bytes
//目的地址，占32bit，即4bytes
//不考虑可选字段
unsigned char IP_Data[65515];//数据部分

unsigned int Total_Length=0;//如果是分片后的数据报，需要记录重组后的总长度

void Receiver_Network_Layer(unsigned char* IP)
{
    printf("当前正在接收方网络层...\n\n");

    //采用内存复制的方式将IP数据报的各个部分提取出来
    memcpy(Source_IP,&IP[12],4);//源地址
    printf("收到该地址传输来的数据： %d.%d.%d.%d\n",Source_IP[0],Source_IP[1],Source_IP[2],Source_IP[3]);
    memcpy(Destination_IP,&IP[16],4);//目的地址

    //    验证IP地址是否与本机IP地址相同
    for(int i=0;i<4;i++)
    {
        if(Destination_IP[i]!=Local_IP[i])
        {
            printf("传输IP地址错误...\n");
            exit(1);
        }
    }
    printf("传输IP地址检验通过\n");

    //计算首部检验和，按照二进制反码运算求和
    unsigned int Sum=0;
    for(int i=0;i<19;i=i+2)
        Sum+=(IP[i]<<8)+IP[i+1];
    Head_CheckSum = (Sum >> 16) + (Sum & 0xffff);
    Head_CheckSum = ~Head_CheckSum;//取反码
    if(Head_CheckSum!=0x00)//首部检验错误
    {
        printf("首部检验错误\n");
        exit(1);
    }
    printf("首部检验正确\n");

    unsigned char Version_HeadLength;//版本号和首部长度各占4bit，所以把他们拼起来占IP的第一个byte
    memcpy(&Version_HeadLength,&IP[0],1);
    Version =  Version_HeadLength>>4;//版本
    Head_Length = Version_HeadLength - (Version<<4);//首部长度
    memcpy(&DS,&IP[1],1);//区分服务
    IP_Length=UChartoUShort(&IP[2],2);//总长度
    Identification=UChartoUShort(&IP[4],2);//标识
    memcpy(&TTL,&IP[8],1);//生存时间
    memcpy(&Protocol,&IP[9],1);//协议
    unsigned char Flag_FragmentOffset[2];//标志占3bit，片偏移占13bit，所以把他们拼起来占2个byte
    memcpy(Flag_FragmentOffset,&IP[6],2);
    DF=Flag_FragmentOffset[0]>>6;
    MF=(Flag_FragmentOffset[0]-(DF<<6))>>5;
    Fragment_Offset=((Flag_FragmentOffset[0]&0x1f)<<8)+Flag_FragmentOffset[1];
    if(DF==0x01)//DF==1表示该IP未经分片，可以直接把数据部分向上提交
    {
        memcpy(IP_Data,&IP[20],IP_Length-20);

        //打印出IP数据报相关信息
        printf("———————————————————网络层报文首部信息———————————————————\n");
        printf("版本：       %d\n",Version);
        printf("首部长度：   %d\n",Head_Length);
        printf("区分服务：   未使用\n");
        printf("总长度：     %u\n",IP_Length);
        printf("标志：       0%d%d\n",DF,MF);
        printf("片偏移：     %u\n",Fragment_Offset);
        printf("生存时间：   %u\n",TTL);
        printf("协议：       UDP\n");
        printf("首部检验和： %u\n",UChartoUInt(&IP[10],2));
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
        printf("\n已成功向上发送到传输层...\n\n");

        Receiver_Transport_Layer(IP_Data);
    }
    else//该IP经过分片，需要等全部分片都到达后再向上提交
    {
        memcpy(&IP_Data[Fragment_Offset*8],&IP[20],IP_Length-20);
        Total_Length+=(IP_Length-20);
        if(MF==0x00&&DF==00&&Fragment_Offset!=0)//说明该片是分片后的最后一片，最后一片到达，可以向上提交
        {
            //打印出IP数据报相关信息
            printf("所有分片已到齐...\n");
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
            printf("数据部分（%d字节）：\n",Total_Length);
            Print_Oct_2Bytes(IP_Data,Total_Length);
            printf("———————————————————————————————————————————————\n");
            printf("\n已成功向上发送到传输层...\n\n");

            Receiver_Transport_Layer(IP_Data);
        }
        else
            printf("本IP数据报为经分片后的数据报，需要等到全部分片到达后上传...\n\n");
    }

}
#endif // RECEIVER_NETWORK_LAYER_H
