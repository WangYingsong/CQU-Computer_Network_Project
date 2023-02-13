#ifndef RECEIVER_APPLICATION_LAYER_H
#define RECEIVER_APPLICATION_LAYER_H

#include <stdio.h>
#include "Tools.h"
//���շ�Ӧ�ò�

unsigned char Local_Port[2];//��Ӧ�ó���Ķ˿ں�

//Ӧ�ò�Э�鶨�����£�
unsigned char APP[65527];//Ӧ�ò㱨����󳤶�Ϊ65527����ΪUDP������󳤶�Ϊ65535����Ҫ��ȥUDP�ײ���8���ֽ�
//Ӧ�ò㱨����5���ֶ����,ǰ�ĸ��ֶ�Ϊ�ײ���������ֶ�Ϊ���ݲ���
//�ײ�
//��һ���ֶ���Э�飬��2���ֽ����
unsigned char APP_Protocol[2];//Э��ΪAC 44��ʾ����WYS_HZCЭ��
//�ڶ����ֶ��Ƿ��͵���Ϣ����,��1���ֽ����
unsigned char MessageType[1];//��Ϣ����Ϊ01��ʾΪ�ı���Ϣ��02ΪͼƬ��Ϣ��03Ϊ������Ϣ��04Ϊ��һ����Ϣ
//�������ֶ��Ƿ�������������12���ֽ����
unsigned char Sender_Name[12]={0};
//���ĸ��ֶ��ǽ�������������12���ֽ����
unsigned char Receiver_Name[12]={0};

//������ֶ��Ƿ��͵���Ϣ����������65527-2-1-12-12=65500���ֽ�
int MessageLength=50;//��ʼ��һ����Ϣ����ܷ��͵ĳ���Ϊ50���ַ�
unsigned char Message[65507];

void Receiver_Application_Layer(unsigned char* APP,unsigned short APP_Length)
{
    printf("��ǰ���ڽ��շ�Ӧ�ò�...\n\n");

    //���뱾Ӧ�ý��̵Ķ˿ں�
    FILE* Local_port;
    Local_port = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\Computer_Network_Receiver\\Receiver_Port.txt", "r");
    fscanf(Local_port,"%u",&Local_Port);
    fclose(Local_port);
    Turn(Local_Port,2);

    //��֤Ŀ�Ķ˿ں��Ƿ��뱾Ӧ�ó���˿ں���ͬ
    for(int i=0;i<2;i++)
    {
        if(Destination_Port[i]!=Local_Port[i])
        {
            printf("�˿ںŴ���...\n");
            exit(1);
        }
    }
    printf("�˿ںż���ͨ��\n");

    //�����ڴ渴�Ƶķ�ʽ��ȡAPP���ĵĸ��ֶ���Ϣ
    memcpy(APP_Protocol,&APP[0],2);//Э��
    memcpy(MessageType,&APP[2],1);//��Ϣ����
    memcpy(Sender_Name,&APP[3],12);//����������
    memcpy(Receiver_Name,&APP[15],12);//����������
    memcpy(Message,&APP[27],APP_Length-27);//��Ϣ
    printf("�յ����� %s ����Ϣ\n",Sender_Name);
    printf("���յ�����ϢΪ��\n%s\n",Message);
    //��ӡAPP���������Ϣ
    printf("��������������������������������������Ӧ�ò㱨���ײ���Ϣ��������������������������������������\n");
    printf("Э�飺       WYS_HZCЭ��\n");
    printf("��Ϣ���ͣ�   �ı���Ϣ\n");
    printf("������������ %s\n",Sender_Name);
    printf("������������ %s\n",Receiver_Name);
    printf("********************************[���ĸ��ֶ�ʮ�����Ƹ�ʽ�����]********************************\n");
    printf("Э�飨2�ֽڣ���        ");
    Print_Oct_2Bytes(APP_Protocol,2);
    printf("��Ϣ���ͣ�1�ֽڣ���    ");
    Print_Oct_2Bytes(MessageType,1);
    printf("������������12�ֽڣ��� ");
    Print_Oct_2Bytes(Sender_Name,12);
    printf("������������12�ֽڣ��� ");
    Print_Oct_2Bytes(Receiver_Name,12);
    printf("��Ϣ��%d���ֽڣ���\n",APP_Length-27);
    Print_Oct_2Bytes(Message,APP_Length-27);
    printf("����������������������������������������������������������������������������������������������\n");
}
#endif // RECEIVER_APPLICATION_LAYER_H
