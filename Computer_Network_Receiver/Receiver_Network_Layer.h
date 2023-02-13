#ifndef RECEIVER_NETWORK_LAYER_H
#define RECEIVER_NETWORK_LAYER_H

#include <stdio.h>
#include <string.h>
#include "Receiver_Transport_Layer.h"
#include "Tools.h"

//��������IPv4Э��

//IPv4���ݱ��ײ��������ǿ�ѡ�ֶΣ���ռ20���ֽ�
unsigned char Version;//�汾��ռ4��bit������IPv4Э�飬�汾��Ϊ4
unsigned char Head_Length;//�ײ����ȣ�ռ4��bit����λ��32λ�ֳ��������ǿ�ѡ�ֶΣ����ײ�����Ϊ�̶���20�ֽڣ���5*32bit
unsigned char DS;//���ַ���û��ʹ�ã���ȫΪ0
unsigned short IP_Length;//IP�ܳ��ȣ�ռ16bit����λΪ�ֽڣ���MTU���ƣ����ֵΪ1500
unsigned short Identification;//��ʶ��ռ16bit��ͬһ�����ݱ���Ƭ���ʶ��ͬ
unsigned char MF;//��־λ�����λ��MF=1��ʾ���桰���з�Ƭ����MF=0��ʾ�Ƿ�Ƭ�����һ��
unsigned char DF;//��־λ���м�λ��DF=1��ʾ���ܷ�Ƭ��DF=0��ʾ���Է�Ƭ
unsigned short Fragment_Offset;//Ƭƫ�ƣ�ռ13bit����ʾ�ڷ�Ƭ��ĳƬ��ԭ�����е����λ�ã���8���ֽ�Ϊ��λ
unsigned char TTL;//����ʱ�䣬ռ8bit����������
unsigned char Protocol;//Э�飬ռ8bit��17��ʾ�����ݲ��ֽ���UDPЭ�鴦��
unsigned short Head_CheckSum=0;//�ײ�����ͣ�ռ16bit��ֻ�������ݱ����ײ������������ݲ���
//Դ��ַ��ռ32bit����4bytes
//Ŀ�ĵ�ַ��ռ32bit����4bytes
//�����ǿ�ѡ�ֶ�
unsigned char IP_Data[65515];//���ݲ���

unsigned int Total_Length=0;//����Ƿ�Ƭ������ݱ�����Ҫ��¼�������ܳ���

void Receiver_Network_Layer(unsigned char* IP)
{
    printf("��ǰ���ڽ��շ������...\n\n");

    //�����ڴ渴�Ƶķ�ʽ��IP���ݱ��ĸ���������ȡ����
    memcpy(Source_IP,&IP[12],4);//Դ��ַ
    printf("�յ��õ�ַ�����������ݣ� %d.%d.%d.%d\n",Source_IP[0],Source_IP[1],Source_IP[2],Source_IP[3]);
    memcpy(Destination_IP,&IP[16],4);//Ŀ�ĵ�ַ

    //    ��֤IP��ַ�Ƿ��뱾��IP��ַ��ͬ
    for(int i=0;i<4;i++)
    {
        if(Destination_IP[i]!=Local_IP[i])
        {
            printf("����IP��ַ����...\n");
            exit(1);
        }
    }
    printf("����IP��ַ����ͨ��\n");

    //�����ײ�����ͣ����ն����Ʒ����������
    unsigned int Sum=0;
    for(int i=0;i<19;i=i+2)
        Sum+=(IP[i]<<8)+IP[i+1];
    Head_CheckSum = (Sum >> 16) + (Sum & 0xffff);
    Head_CheckSum = ~Head_CheckSum;//ȡ����
    if(Head_CheckSum!=0x00)//�ײ��������
    {
        printf("�ײ��������\n");
        exit(1);
    }
    printf("�ײ�������ȷ\n");

    unsigned char Version_HeadLength;//�汾�ź��ײ����ȸ�ռ4bit�����԰�����ƴ����ռIP�ĵ�һ��byte
    memcpy(&Version_HeadLength,&IP[0],1);
    Version =  Version_HeadLength>>4;//�汾
    Head_Length = Version_HeadLength - (Version<<4);//�ײ�����
    memcpy(&DS,&IP[1],1);//���ַ���
    IP_Length=UChartoUShort(&IP[2],2);//�ܳ���
    Identification=UChartoUShort(&IP[4],2);//��ʶ
    memcpy(&TTL,&IP[8],1);//����ʱ��
    memcpy(&Protocol,&IP[9],1);//Э��
    unsigned char Flag_FragmentOffset[2];//��־ռ3bit��Ƭƫ��ռ13bit�����԰�����ƴ����ռ2��byte
    memcpy(Flag_FragmentOffset,&IP[6],2);
    DF=Flag_FragmentOffset[0]>>6;
    MF=(Flag_FragmentOffset[0]-(DF<<6))>>5;
    Fragment_Offset=((Flag_FragmentOffset[0]&0x1f)<<8)+Flag_FragmentOffset[1];
    if(DF==0x01)//DF==1��ʾ��IPδ����Ƭ������ֱ�Ӱ����ݲ��������ύ
    {
        memcpy(IP_Data,&IP[20],IP_Length-20);

        //��ӡ��IP���ݱ������Ϣ
        printf("������������������������������������������㱨���ײ���Ϣ��������������������������������������\n");
        printf("�汾��       %d\n",Version);
        printf("�ײ����ȣ�   %d\n",Head_Length);
        printf("���ַ���   δʹ��\n");
        printf("�ܳ��ȣ�     %u\n",IP_Length);
        printf("��־��       0%d%d\n",DF,MF);
        printf("Ƭƫ�ƣ�     %u\n",Fragment_Offset);
        printf("����ʱ�䣺   %u\n",TTL);
        printf("Э�飺       UDP\n");
        printf("�ײ�����ͣ� %u\n",UChartoUInt(&IP[10],2));
        printf("Դ��ַ��     %u.%u.%u.%u\n",Source_IP[0],Source_IP[1],Source_IP[2],Source_IP[3]);
        printf("Ŀ�ĵ�ַ��   %u.%u.%u.%u\n",Destination_IP[0],Destination_IP[1],Destination_IP[2],Destination_IP[3]);
        printf("��ѡ�ֶΣ�   ��\n");
        printf("********************************[���ĸ��ֶ�ʮ�����Ƹ�ʽ�����]********************************\n");
        printf("�汾 �ײ����ȣ�1�ֽڣ��� ");
        Print_Oct_2Bytes(&IP[0],1);
        printf("���ַ���1�ֽڣ���      ");
        Print_Oct_2Bytes(&IP[1],1);
        printf("�ܳ��ȣ�2�ֽڣ���        ");
        Print_Oct_2Bytes(&IP[2],2);
        printf("��ʶ��2�ֽڣ���          ");
        Print_Oct_2Bytes(&IP[4],2);
        printf("��־ Ƭƫ�ƣ�2�ֽڣ���   ");
        Print_Oct_2Bytes(&IP[6],2);
        printf("����ʱ�䣨1�ֽڣ���      ");
        Print_Oct_2Bytes(&IP[8],1);
        printf("Э�飨1�ֽڣ���          ");
        Print_Oct_2Bytes(&IP[9],1);
        printf("�ײ�����ͣ�2�ֽڣ���    ");
        Print_Oct_2Bytes(&IP[10],2);
        printf("Դ��ַ��4�ֽڣ���        ");
        Print_Oct_2Bytes(&IP[12],4);
        printf("Ŀ�ĵ�ַ��4�ֽڣ���      ");
        Print_Oct_2Bytes(&IP[16],4);
        printf("���ݲ��֣�%d�ֽڣ���\n",IP_Length-20);
        Print_Oct_2Bytes(&IP[20],IP_Length-20);
        printf("����������������������������������������������������������������������������������������������\n");
        printf("\n�ѳɹ����Ϸ��͵������...\n\n");

        Receiver_Transport_Layer(IP_Data);
    }
    else//��IP������Ƭ����Ҫ��ȫ����Ƭ��������������ύ
    {
        memcpy(&IP_Data[Fragment_Offset*8],&IP[20],IP_Length-20);
        Total_Length+=(IP_Length-20);
        if(MF==0x00&&DF==00&&Fragment_Offset!=0)//˵����Ƭ�Ƿ�Ƭ������һƬ�����һƬ������������ύ
        {
            //��ӡ��IP���ݱ������Ϣ
            printf("���з�Ƭ�ѵ���...\n");
            printf("������������������������������������������㱨���ײ���Ϣ��������������������������������������\n");
            printf("�汾��       %d\n",Version);
            printf("�ײ����ȣ�   %d\n",Head_Length);
            printf("���ַ���   δʹ��\n");
            printf("�ܳ��ȣ�     %d\n",IP_Length);
            printf("��־��       0%d%d\n",DF,MF);
            printf("Ƭƫ�ƣ�     %d\n",Fragment_Offset);
            printf("����ʱ�䣺   %d\n",TTL);
            printf("Э�飺       UDP\n");
            printf("�ײ�����ͣ� %d\n",Head_CheckSum);
            printf("Դ��ַ��     %d.%d.%d.%d\n",Source_IP[0],Source_IP[1],Source_IP[2],Source_IP[3]);
            printf("Ŀ�ĵ�ַ��   %d.%d.%d.%d\n",Destination_IP[0],Destination_IP[1],Destination_IP[2],Destination_IP[3]);
            printf("��ѡ�ֶΣ�   ��\n");
            printf("********************************[���ĸ��ֶ�ʮ�����Ƹ�ʽ�����]********************************\n");
            printf("�汾 �ײ����ȣ�1�ֽڣ��� ");
            Print_Oct_2Bytes(&IP[0],1);
            printf("���ַ���1�ֽڣ���      ");
            Print_Oct_2Bytes(&IP[1],1);
            printf("�ܳ��ȣ�2�ֽڣ���        ");
            Print_Oct_2Bytes(&IP[2],2);
            printf("��ʶ��2�ֽڣ���          ");
            Print_Oct_2Bytes(&IP[4],2);
            printf("��־ Ƭƫ�ƣ�2�ֽڣ���   ");
            Print_Oct_2Bytes(&IP[6],2);
            printf("����ʱ�䣨1�ֽڣ���      ");
            Print_Oct_2Bytes(&IP[8],1);
            printf("Э�飨1�ֽڣ���          ");
            Print_Oct_2Bytes(&IP[9],1);
            printf("�ײ�����ͣ�2�ֽڣ���    ");
            Print_Oct_2Bytes(&IP[10],2);
            printf("Դ��ַ��4�ֽڣ���        ");
            Print_Oct_2Bytes(&IP[12],4);
            printf("Ŀ�ĵ�ַ��4�ֽڣ���      ");
            Print_Oct_2Bytes(&IP[16],4);
            printf("���ݲ��֣�%d�ֽڣ���\n",Total_Length);
            Print_Oct_2Bytes(IP_Data,Total_Length);
            printf("����������������������������������������������������������������������������������������������\n");
            printf("\n�ѳɹ����Ϸ��͵������...\n\n");

            Receiver_Transport_Layer(IP_Data);
        }
        else
            printf("��IP���ݱ�Ϊ����Ƭ������ݱ�����Ҫ�ȵ�ȫ����Ƭ������ϴ�...\n\n");
    }

}
#endif // RECEIVER_NETWORK_LAYER_H
