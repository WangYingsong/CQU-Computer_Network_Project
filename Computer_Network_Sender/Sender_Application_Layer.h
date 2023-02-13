#ifndef SENDER_APPLICATION_LAYER_H
#define SENDER_APPLICATION_LAYER_H

#include <stdio.h>
#include "Sender_Transport_Layer.h"

//���ͷ�Ӧ�ò�

//Ӧ�ò�Э�鶨�����£�
unsigned char APP[65527];//Ӧ�ò㱨����󳤶�Ϊ65527����ΪUDP������󳤶�Ϊ65535����Ҫ��ȥUDP�ײ���8���ֽ�
//Ӧ�ò㱨����5���ֶ����,ǰ�ĸ��ֶ�Ϊ�ײ���������ֶ�Ϊ���ݲ���
//�ײ�
//��һ���ֶ���Э�飬��2���ֽ����
unsigned char APP_Protocol[2]={0xAC,0x44};//Э��ΪAC 44��ʾ����WYS_HZCЭ��
//�ڶ����ֶ��Ƿ��͵���Ϣ����,��1���ֽ����
unsigned char Message_Type[1]={0x01};//��Ϣ����Ϊ01��ʾΪ�ı���Ϣ��02ΪͼƬ��Ϣ��03Ϊ������Ϣ��04Ϊ��һ����Ϣ
//�������ֶ��Ƿ�������������12���ֽ����
unsigned char Sender_Name[12]={0};
//���ĸ��ֶ��ǽ�������������12���ֽ����
unsigned char Receiver_Name[12]={0};

//������ֶ��Ƿ��͵���Ϣ����������65527-2-1-12-12=65500���ֽ�
int MessageLength=50;//��ʼ��һ����Ϣ����ܷ��͵ĳ���Ϊ50���ַ�


void Sender_Application_Layer()
{
    printf("��ǰ���ڷ��ͷ�Ӧ�ò�...\n\n");

    //���뱾Ӧ�ý��̵Ķ˿ں�
    FILE* Local_Port;
    Local_Port = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\Computer_Network_Sender\\Sender_Port.txt", "r");
    fscanf(Local_Port,"%u",&Source_Port);
    fclose(Local_Port);
    Turn(Source_Port,2);

    printf("����������������6�����Ļ�12��Ӣ�ģ���");
    scanf("%s",Sender_Name);
    printf("�������㷢����Ϣ�Ľ����ߵ��������6�����Ļ�12��Ӣ�ģ���");
    scanf("%s",Receiver_Name);
    printf("��������Ϣ�����ߵ�IP��ַ�Ͷ˿ںţ�");
    scanf("%u.%u.%u.%u:%u",&Destination_IP[0],&Destination_IP[1],&Destination_IP[2],&Destination_IP[3],&Destination_Port);
    getc(stdin);
    Turn(Destination_Port,2);
    int AlreadyInputChars=0;//��¼�Ѿ�������ַ�����
    char theInputChar =NULL;//���ڽ��ܵ�ǰ������ַ�
    unsigned char *Messeage = (unsigned char *)malloc(sizeof(char)*50);//���ڴ洢�������Ϣ����С��ʼ��Ϊ50���ַ��Ĵ�С
    printf("���������뷢�͵���Ϣ�������Ϣ����Ϊ65500�ֽڣ���\n");
    while((theInputChar=getc(stdin))!='\n') {
        Messeage[AlreadyInputChars++] = theInputChar;
        if(AlreadyInputChars%50==0) { //����ַ������ó��ȵ��ڷ�����ܳ���-1ʱ
            MessageLength+=50;    //���������ܴ洢����󳤶�
            Messeage =(unsigned char*)realloc(Messeage,MessageLength);//�����ַ������������
        }
        if(AlreadyInputChars==65500)
        {
            printf("������Ϣ�Ѵﳤ������...\n");
            break;
        }
    }

    //�����ڴ渴�Ʒ�ʽ�����ֶ�����APP����
    memcpy(&APP[0],APP_Protocol,2);//Э��
    memcpy(&APP[2],Message_Type,1);//��Ϣ����
    memcpy(&APP[3],Sender_Name,12);//����������
    memcpy(&APP[15],Receiver_Name,12);//����������
    memcpy(&APP[27],Messeage,AlreadyInputChars);//��Ϣ

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
    Print_Oct_2Bytes(Message_Type,1);
    printf("������������12�ֽڣ��� ");
    Print_Oct_2Bytes(Sender_Name,12);
    printf("������������12�ֽڣ��� ");
    Print_Oct_2Bytes(Receiver_Name,12);
    printf("��Ϣ��%d���ֽڣ���\n",AlreadyInputChars);
    Print_Oct_2Bytes(Messeage,AlreadyInputChars);
    printf("����������������������������������������������������������������������������������������������\n");
    printf("\n�ѳɹ����·��͵������...\n\n");

    Sender_Transport_Layer(APP,AlreadyInputChars+27);
}

#endif // SENDER_APPLICATION_LAYER_H
