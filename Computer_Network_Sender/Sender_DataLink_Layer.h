#ifndef SENDER_DATALINK_LAYER_H
#define SENDER_DATALINK_LAYER_H

#include<stdio.h>
#include<stdbool.h>
#include<Tools.h>

//数据链路层采用IEEE 802.3帧格式

//MAC帧的最大长度为1518字节
unsigned char MAC[1518];

//MAC帧首部共14个字节
unsigned char Destination_Address[6];//目的地址
unsigned char Source_Address[6];//源地址
unsigned char Type[2]={0x08,0x00};//类型，0x0800表示上层使用的是IP数据报

//数据部分，最长为1500字节，最短为46字节
unsigned char MAC_Data[1500];

//尾部部分
unsigned int FCS=0;//帧校验序列，采用CRC检验

//切片标记
bool flag=false;

void Sender_DataLink_Layer(unsigned char* Data,unsigned short Data_Length)
{
    printf("当前正在发送方数据链路层...\n\n");

    //查找目的MAC地址
    FILE* Destination_MAC;
    Destination_MAC = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\Computer_Network_Receiver\\Receiver_MAC.txt", "r");
    fscanf(Destination_MAC,"%u.%u.%u.%u.%u.%u",&Destination_Address[0],&Destination_Address[1],&Destination_Address[2],&Destination_Address[3],&Destination_Address[4],&Destination_Address[5]);
    fclose(Destination_MAC);

    //读入本机MAC地址
    FILE* Local_MAC;
    Local_MAC = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\Computer_Network_Sender\\Sender_MAC.txt", "r");
    fscanf(Local_MAC,"%u.%u.%u.%u.%u.%u",&Source_Address[0],&Source_Address[1],&Source_Address[2],&Source_Address[3],&Source_Address[4],&Source_Address[5]);
    fclose(Local_MAC);

   //采用内存复制的方式将MAC帧的各个部分填写
    memcpy(&MAC[0],Destination_Address,6);//目的地址
    memcpy(&MAC[6],Source_Address,6);//源地址
    memcpy(&MAC[12],Type,2);//类型
    memcpy(&MAC[14],Data,Data_Length);

    //如果数据长度小于46字节，需要在数据字段的后面加入一个整数字节的填充字段，以保证以太网的MAC帧长不小于64字节
    if(Data_Length<46)
    {
        for(int i=14+Data_Length;i<14+46;i++)
            MAC[i]=0xff;
        Data_Length=46;
    }

    //使用CRC检验
    FCS=crc32(MAC,14+Data_Length);
    memcpy(&MAC[14+Data_Length],&FCS,4);
    Turn(&MAC[14+Data_Length],4);

    //打印出MAC帧相关信息
    printf("——————————————————数据链路层报文首部信息——————————————————\n");
    printf("目的地址： %u.%u.%u.%u.%u.%u\n",Destination_Address[0],Destination_Address[1],Destination_Address[2],Destination_Address[3],Destination_Address[4],Destination_Address[5]);
    printf("源地址：   %u.%u.%u.%u.%u.%u\n",Source_Address[0],Source_Address[1],Source_Address[2],Source_Address[3],Source_Address[4],Source_Address[5]);
    printf("类型：     IP\n");
    printf("尾部FCS：  %u\n",FCS);
    printf("********************************[报文各字段十六进制格式化输出]********************************\n");
    printf("目的地址（6字节）： ");
    Print_Oct_2Bytes(&MAC[0],6);
    printf("源地址（6字节）：   ");
    Print_Oct_2Bytes(&MAC[6],6);
    printf("类型（2字节）：     ");
    Print_Oct_2Bytes(&MAC[12],2);
    printf("FCS（4字节）：      ");
    Print_Oct_2Bytes(&MAC[14+Data_Length],4);
    printf("数据（%d字节）：\n",Data_Length);
    Print_Oct_2Bytes(&MAC[12],Data_Length);
    printf("———————————————————————————————————————————————\n");

    printf("\n已成功向下发送到物理层...\n\n");


    //采用文件读写的方式将MAC帧传输
    FILE* pf;
    if(flag==false)
        pf = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\MACS.txt", "w");
    else
        pf = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\MACS.txt", "a");
    fwrite(MAC,1,18+Data_Length,pf);
    fclose(pf);

    //物理层未实现
}

#endif // SENDER_DATALINK_LAYER_H
