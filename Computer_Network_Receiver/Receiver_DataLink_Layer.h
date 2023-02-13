#ifndef RECEIVER_DATALINK_LAYER_H
#define RECEIVER_DATALINK_LAYER_H

#include<stdio.h>
#include<Tools.h>
#include <string.h>
#include "Receiver_Network_Layer.h"

//������·�����IEEE 802.3֡��ʽ

//MAC֡�ײ���14���ֽ�
unsigned char Local_Address[6];//������ַ
unsigned char Destination_Address[6];//Ŀ�ĵ�ַ
unsigned char Source_Address[6];//Դ��ַ
unsigned char Type[2];//���ͣ�0x0800��ʾ�ϲ�ʹ�õ���IP���ݱ�

//���ݲ��֣��Ϊ1500�ֽڣ����Ϊ46�ֽ�
unsigned char MAC_Data[1500];

//β������
unsigned int FCS=0;//֡У�����У�����CRC����

void Receiver_DataLink_Layer(unsigned char* MAC,unsigned short MAC_Length)
{
    int n_MAC=(MAC_Length)<(1500+18)?1:(MAC_Length%1518==0)?(MAC_Length/1518):(MAC_Length/1518+1);
    for(int index=0;index<n_MAC;index++)
    {
        printf("��ǰ���ڽ��շ�������·��...\n\n");
        int head=index*1518;

        //�����ڴ渴�Ƶķ�ʽ��MAC֡�ĸ���������ȡ����
        memcpy(Destination_Address,&MAC[head],6);//Ŀ�ĵ�ַ

        //��֤�Ƿ��뱾����ַ��ͬ
        for(int i=0;i<6;i++)
        {
            if(Destination_Address[i]!=Local_Address[i])
            {
                printf("�����ַ����...\n");
                exit(1);
            }
        }
        printf("�����ַ����ͨ��\n");
        memcpy(Source_Address,&MAC[head+6],6);//Դ��ַ
        printf("�յ��õ�ַ�����������ݣ� %d.%d.%d.%d.%d.%d\n",Source_Address[0],Source_Address[1],Source_Address[2],Source_Address[3],Source_Address[4],Source_Address[5]);
        unsigned short MAC_length=(n_MAC==1)?(MAC_Length):(index<n_MAC-1)?1518:(MAC_Length-1518*(n_MAC-1));

        //ʹ��CRC����
        FCS=UChartoUInt(&MAC[head+MAC_length-4],4);
        if(FCS!=crc32(&MAC[head],MAC_length-4))
        {
            printf("�������ݴ���...\n");
            exit(1);
        }
        printf("CRC����ͨ��\n");        
        memcpy(Type,&MAC[head+12],2);//����
        memcpy(MAC_Data,&MAC[head+14],MAC_length-18);//����

        //��ӡ��MAC֡�����Ϣ
        printf("������������������������������������������·�㱨���ײ���Ϣ������������������������������������\n");
        printf("Ŀ�ĵ�ַ�� %u.%u.%u.%u.%u.%u\n",Destination_Address[0],Destination_Address[1],Destination_Address[2],Destination_Address[3],Destination_Address[4],Destination_Address[5]);
        printf("Դ��ַ��   %u.%u.%u.%u.%u.%u\n",Source_Address[0],Source_Address[1],Source_Address[2],Source_Address[3],Source_Address[4],Source_Address[5]);
        printf("���ͣ�     IP\n");
        printf("β��FCS��  %u\n",FCS);
        printf("********************************[���ĸ��ֶ�ʮ�����Ƹ�ʽ�����]********************************\n");
        printf("Ŀ�ĵ�ַ��6�ֽڣ��� ");
        Print_Oct_2Bytes(&MAC[head],6);
        printf("Դ��ַ��6�ֽڣ���   ");
        Print_Oct_2Bytes(&MAC[head+6],6);
        printf("���ͣ�2�ֽڣ���     ");
        Print_Oct_2Bytes(&MAC[head+12],2);
        printf("FCS��4�ֽڣ���      ");
        Print_Oct_2Bytes(&MAC[head+MAC_length-4],4);
        printf("���ݣ�%d�ֽڣ���\n",MAC_length-18);
        Print_Oct_2Bytes(&MAC[12],MAC_length-18);
        printf("����������������������������������������������������������������������������������������������\n");

        printf("\n�ѳɹ����Ϸ��͵������...\n\n");

        Receiver_Network_Layer(MAC_Data);

        FCS=0;
    }
}
#endif // RECEIVER_DATALINK_LAYER_H
