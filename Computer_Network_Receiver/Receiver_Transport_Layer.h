#ifndef RECEIVER_TRANSPORT_LAYER_H
#define RECEIVER_TRANSPORT_LAYER_H

#include<stdio.h>
#include<string.h>
#include"Receiver_Application_Layer.h"
#include<Tools.h>

//��������UDPЭ��

//UDP�������ֶΣ��ײ��������벿��
//UDP�ײ����˸��ֽڣ����ĸ��ֶ���ɣ�ÿ���ֶ�ռ2���ֽ�
unsigned char Source_Port[2];//Դ�˿ں�ռUDP���ĵ�2���ֽڣ�����Ҫ�Է�����ʱʹ�ã�����Ҫʱ��ȫ��0����ʮ�����Ʊ�ʾ
//Ŀ�Ķ˿ں�ռUDP���ĵ�2���ֽڣ����յ㽻������ʱ����ʹ�á���ʮ�����Ʊ�ʾ
unsigned short UDP_Length;//UDP�û����ݱ��ĳ��ȣ���Сֵ��8�������ײ�ʱ������λ���ֽ�
unsigned short CheckSum=0;//����ͣ����UDP�û����ݱ��ڴ������Ƿ��д��д�Ͷ�����
//UDP���ݲ���
unsigned char UDP_Data[65527];//���ݲ��ֵ���󳤶�Ϊ65527�ֽڣ���ΪIP���ݱ�����󳤶�Ϊ2^16����Ҫ��ȥ8���ֽڵ�UDP�ײ���

//UDPα�ײ�
unsigned char UDP_Pseudohead[12];//����У��ͣ�����4�ֽ�ԴIP��ַ��4�ֽ�Ŀ��IP��ַ��1�ֽ�0��1�ֽ�17��2�ֽ�UDP����

void Receiver_Transport_Layer(unsigned char* UDP)
{
    printf("��ǰ���ڽ��շ������...\n\n");

    //�����ڴ渴�Ƶķ�ʽ��UDP���ĵĸ���������ȡ����
    UDP_Length=UChartoUShort(&UDP[4],2);//UDP����

    //α�ײ���д
    memcpy(&UDP_Pseudohead[0],Source_IP,4);//ԴIP��ַ
    memcpy(&UDP_Pseudohead[4],Destination_IP,4);//Ŀ��IP��ַ
    unsigned char Zero[1]={0x00};
    memcpy(&UDP_Pseudohead[8],Zero,1);//��9���ֽ�ȫΪ0
    unsigned char UDP_Protocol[1]={0x11};
    memcpy(&UDP_Pseudohead[9],UDP_Protocol,1);//��10���ֽ�Ϊ17
    memcpy(&UDP_Pseudohead[10],&UDP[4],2);//UDP����

    //�������ͣ����ն����Ʒ����������
    unsigned Sum=0;
    for(int i=0;i<12;i=i+2)
        Sum+=(UDP_Pseudohead[i]<<8)+UDP_Pseudohead[i+1];//����α�ײ��ļ����
    if(UDP_Length%2==1)//���UDP�ĳ���Ϊ��������Ҫ��β������һ���ֽڵ�ȫ0�����
    {
        for(int i=0;i<UDP_Length-2;i=i+2)
            Sum+=(UDP[i]<<8)+UDP[i+1];
        Sum+=(UDP[UDP_Length-1]<<8)+Zero[0];
    }
    else//UDP�ĳ���Ϊż����ֱ�����
    {
        for(int i=0;i<UDP_Length-1;i=i+2)
            Sum+=(UDP[i]<<8)+UDP[i+1];
    }
    CheckSum = (Sum >> 16) + (Sum & 0xffff);
    if(CheckSum!=0xffff)
    {
        printf("UDP����ʹ���\n");
        exit(1);
    }
    printf("UDP�������ȷ\n");
    memcpy(Source_Port,&UDP[0],2);//Դ�˿�
    memcpy(Destination_Port,&UDP[2],2);//Ŀ�Ķ˿�
    memcpy(&UDP_Data,&UDP[8],UDP_Length-8);//���ݲ���

    //��ӡ��UDP�����Ϣ
    printf("������������������������������������������㱨���ײ���Ϣ��������������������������������������\n");
    printf("Դ�˿ڣ�   %d\n",UChartoUShort(&UDP[0],2));
    printf("Ŀ�Ķ˿ڣ� %d\n",UChartoUShort(&UDP[2],2));
    printf("UDP���ȣ�  %d\n",UChartoUShort(&UDP[4],2));
    printf("����ͣ�   %d\n",UChartoUShort(&UDP[6],2));
    printf("********************************[���ĸ��ֶ�ʮ�����Ƹ�ʽ�����]********************************\n");
    printf("Դ�˿ڣ�2�ֽڣ���         ");
    Print_Oct_2Bytes(&UDP[0],2);
    printf("Ŀ�Ķ˿ڣ�2�ֽڣ�:        ");
    Print_Oct_2Bytes(&UDP[2],2);
    printf("UDP���ȣ�2�ֽڣ�          ");
    Print_Oct_2Bytes(&UDP[4],2);
    printf("����ͣ�2�ֽڣ�           ");
    Print_Oct_2Bytes(&UDP[6],2);
    printf("UDP�����ݲ��֣�%d�ֽڣ���\n",UDP_Length-8);
    Print_Oct_2Bytes(&UDP[8],UDP_Length-8);
    printf("����������������������������������������������������������������������������������������������\n");
    printf("�ѳɹ����Ϸ��͵�Ӧ�ò�...\n\n");

    Receiver_Application_Layer(UDP_Data,UDP_Length-8);
}
#endif // RECEIVER_TRANSPORT_LAYER_H
