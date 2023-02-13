#ifndef SENDER_NETWORK_LAYER_H
#define SENDER_NETWORK_LAYER_H

#include<stdio.h>
#include<string.h>
#include<Tools.h>
#include"Sender_DataLink_Layer.h"

//��������IPv4Э��

//IP���ݱ���MTU���ƣ���󳤶�Ϊ1500�ֽ�
unsigned char IP[1500];

//IPv4���ݱ��ײ��������ǿ�ѡ�ֶΣ���ռ20���ֽ�
unsigned char Version = 0x04;//�汾��ռ4��bit������IPv4Э�飬�汾��Ϊ4
unsigned char Head_Length=0x05;//�ײ����ȣ�ռ4��bit����λ��32λ�ֳ��������ǿ�ѡ�ֶΣ����ײ�����Ϊ�̶���20�ֽڣ���5*32bit
unsigned char DS=0x00;//���ַ���û��ʹ�ã���ȫΪ0
unsigned short IP_Length=0;//IP�ܳ��ȣ�ռ16bit����λΪ�ֽڣ���MTU���ƣ����ֵΪ1500
unsigned short Identification=0;//��ʶ��ռ16bit��ͬһ�����ݱ���Ƭ���ʶ��ͬ
unsigned char MF=0x00;//��־λ�����λ��MF=1��ʾ���桰���з�Ƭ����MF=0��ʾ�Ƿ�Ƭ�����һ��
unsigned char DF=0x01;//��־λ���м�λ��DF=1��ʾ���ܷ�Ƭ��DF=0��ʾ���Է�Ƭ
unsigned short Fragment_Offset=0;//Ƭƫ�ƣ�ռ13bit����ʾ�ڷ�Ƭ��ĳƬ��ԭ�����е����λ�ã���8���ֽ�Ϊ��λ
unsigned char TTL=0xFF;//����ʱ�䣬ռ8bit����������
unsigned char Protocol=0x11;//Э�飬ռ8bit��17��ʾ�����ݲ��ֽ���UDPЭ�鴦��
unsigned short Head_CheckSum=0;//�ײ�����ͣ�ռ16bit��ֻ�������ݱ����ײ������������ݲ���
unsigned char Source_IP[4];//Դ��ַ��ռ32bit����4bytes
unsigned char Destination_IP[4];//Ŀ�ĵ�ַ��ռ32bit����4bytes
//�����ǿ�ѡ�ֶ�
unsigned char IP_Data[1500-20];//���ݲ���,��MTU���ƣ��Ϊ1500���ֽ�,��ȥ�ײ���20�ֽڣ����ݲ����Ϊ1480�ֽ�


void Sender_Network_Layer(unsigned char* Data,unsigned short Data_Length)
{
    printf("��ǰ���ڷ��ͷ������...\n\n");

    //������ݵĳ����Ƿ񳬹�1480�ֽڣ���û����������ֱ�ӷ�װ
    if(Data_Length<(1500-20))
    {
        //�����ڴ渴�Ƶķ�ʽ��IP�ĸ���������д
        unsigned char Version_HeadLength=(Version<<4)+Head_Length;//�汾�ź��ײ����ȸ�ռ4bit�����԰�����ƴ����ռIP�ĵ�һ��byte
        memcpy(&IP[0],&Version_HeadLength,1);
        memcpy(&IP[1],&DS,1);//���ַ���
        IP_Length=20+Data_Length;
        memcpy(&IP[2],&IP_Length,2);//�ܳ���
        Turn(&IP[2],2);
        memcpy(&IP[4],&Identification,2);//��ʶ
        Turn(&IP[4],2);
        unsigned char Flag_FragmentOffset[2];//��־ռ3bit��Ƭƫ��ռ13bit�����԰�����ƴ����ռ2��byte
        Flag_FragmentOffset[0]=(0x00<<7)+(DF<<6)+(MF<<5)+((Fragment_Offset&0x1f00)>>8);
        Flag_FragmentOffset[1]=Fragment_Offset;
        memcpy(&IP[6],Flag_FragmentOffset,2);
        memcpy(&IP[8],&TTL,1);//����ʱ��
        memcpy(&IP[9],&Protocol,1);//Э��
        memcpy(&IP[10],&Head_CheckSum,2);//�ײ������
        memcpy(&IP[12],Source_IP,4);//Դ��ַ
        memcpy(&IP[16],Destination_IP,4);//Ŀ�ĵ�ַ
        memcpy(&IP[20],Data,Data_Length);//���ݲ���

        //�����ײ�����ͣ����ն����Ʒ����������
        unsigned int Sum=0;
        for(int i=0;i<19;i=i+2)
            Sum+=(IP[i]<<8)+IP[i+1];
        Head_CheckSum = (Sum >> 16) + (Sum & 0xffff);
        Head_CheckSum = ~Head_CheckSum;//ȡ����
        memcpy(&IP[10],&Head_CheckSum,2);//��������ļ�������������ֶ�
        Turn(&IP[10],2);

        //��ӡ��IP���ݱ������Ϣ
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
        printf("\n�ѳɹ����·��͵�������·��...\n\n");

        Sender_DataLink_Layer(IP,IP_Length);
    }
    //����1480�ֽڣ���Ҫ��Ƭ���ٷ�װ
    else
    {
        printf("���ݳ���Ϊ%d�ֽڣ�����1480�ֽڣ���Ҫ��Ƭ...\n\n",Data_Length);
        int i_IP=0;
        int data_length=Data_Length;
        while(data_length>0)
        {
            if (i_IP>0)
                flag=true;
            //�����ڴ渴�Ƶķ�ʽ��IP�ĸ���������д
            unsigned char Version_HeadLength=(Version<<4)+Head_Length;//�汾�ź��ײ����ȸ�ռ4bit�����԰�����ƴ����ռIP�ĵ�һ��byte
            memcpy(&IP[0],&Version_HeadLength,1);
            memcpy(&IP[1],&DS,1);//���ַ���
            IP_Length=(data_length<1480)?(20+data_length):(20+1480);
            memcpy(&IP[2],&IP_Length,2);//�ܳ���
            Turn(&IP[2],2);
            memcpy(&IP[4],&Identification,2);//��ʶ
            DF=0;//��Ƭ��DF��0
            Turn(&IP[4],2);
            Fragment_Offset=i_IP*1480/8;
            MF=(data_length<1480)?0:1;//MF��δ��Ƭ�����ݳ���С��1480ʱΪ0������ʱ��Ϊ1
            unsigned char Flag_FragmentOffset[2];//��־ռ3bit��Ƭƫ��ռ13bit�����԰�����ƴ����ռ2��byte
            Flag_FragmentOffset[0]=(0x00<<7)+(DF<<6)+(MF<<5)+((Fragment_Offset&0x1f00)>>8);
            Flag_FragmentOffset[1]=Fragment_Offset;
            memcpy(&IP[6],Flag_FragmentOffset,2);
            memcpy(&IP[8],&TTL,1);//����ʱ��
            memcpy(&IP[9],&Protocol,1);//Э��
            memcpy(&IP[10],&Head_CheckSum,2);//�ײ������
            memcpy(&IP[12],Source_IP,4);//Դ��ַ
            memcpy(&IP[16],Destination_IP,4);//Ŀ�ĵ�ַ
            unsigned short l=(data_length<1480)?data_length:1480;
            memcpy(&IP[20],&Data[i_IP*1480],l);//���ݲ���

            //�����ײ�����ͣ����ն����Ʒ����������
            unsigned int Sum=0;
            for(int i=0;i<19;i=i+2)
                Sum+=(IP[i]<<8)+IP[i+1];
            Head_CheckSum = (Sum >> 16) + (Sum & 0xffff);
            Head_CheckSum = ~Head_CheckSum;//ȡ����
            memcpy(&IP[10],&Head_CheckSum,2);//��������ļ�������������ֶ�
            Turn(&IP[10],2);

            //��ӡ��IP���ݱ������Ϣ
            printf("������������������������������������������㱨���ײ���Ϣ��������������������������������������\n");
            printf("[��Ƭ��ĵ�%d��IP���ݱ�]\n",i_IP+1);
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
            printf("���ݲ��֣�%d�ֽڣ���\n",IP_Length-20);
            Print_Oct_2Bytes(&IP[20],IP_Length-20);
            printf("����������������������������������������������������������������������������������������������\n");
            printf("\n�ѳɹ����·��͵�������·��...\n\n");

            Sender_DataLink_Layer(IP,IP_Length);

            i_IP++;
            data_length-=1480;
            Head_CheckSum=0;
        }
    }
}

#endif // SENDER_NETWORK_LAYER_H
