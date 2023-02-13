#ifndef SENDER_DATALINK_LAYER_H
#define SENDER_DATALINK_LAYER_H

#include<stdio.h>
#include<stdbool.h>
#include<Tools.h>

//������·�����IEEE 802.3֡��ʽ

//MAC֡����󳤶�Ϊ1518�ֽ�
unsigned char MAC[1518];

//MAC֡�ײ���14���ֽ�
unsigned char Destination_Address[6];//Ŀ�ĵ�ַ
unsigned char Source_Address[6];//Դ��ַ
unsigned char Type[2]={0x08,0x00};//���ͣ�0x0800��ʾ�ϲ�ʹ�õ���IP���ݱ�

//���ݲ��֣��Ϊ1500�ֽڣ����Ϊ46�ֽ�
unsigned char MAC_Data[1500];

//β������
unsigned int FCS=0;//֡У�����У�����CRC����

//��Ƭ���
bool flag=false;

void Sender_DataLink_Layer(unsigned char* Data,unsigned short Data_Length)
{
    printf("��ǰ���ڷ��ͷ�������·��...\n\n");

    //����Ŀ��MAC��ַ
    FILE* Destination_MAC;
    Destination_MAC = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\Computer_Network_Receiver\\Receiver_MAC.txt", "r");
    fscanf(Destination_MAC,"%u.%u.%u.%u.%u.%u",&Destination_Address[0],&Destination_Address[1],&Destination_Address[2],&Destination_Address[3],&Destination_Address[4],&Destination_Address[5]);
    fclose(Destination_MAC);

    //���뱾��MAC��ַ
    FILE* Local_MAC;
    Local_MAC = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\Computer_Network_Sender\\Sender_MAC.txt", "r");
    fscanf(Local_MAC,"%u.%u.%u.%u.%u.%u",&Source_Address[0],&Source_Address[1],&Source_Address[2],&Source_Address[3],&Source_Address[4],&Source_Address[5]);
    fclose(Local_MAC);

   //�����ڴ渴�Ƶķ�ʽ��MAC֡�ĸ���������д
    memcpy(&MAC[0],Destination_Address,6);//Ŀ�ĵ�ַ
    memcpy(&MAC[6],Source_Address,6);//Դ��ַ
    memcpy(&MAC[12],Type,2);//����
    memcpy(&MAC[14],Data,Data_Length);

    //������ݳ���С��46�ֽڣ���Ҫ�������ֶεĺ������һ�������ֽڵ�����ֶΣ��Ա�֤��̫����MAC֡����С��64�ֽ�
    if(Data_Length<46)
    {
        for(int i=14+Data_Length;i<14+46;i++)
            MAC[i]=0xff;
        Data_Length=46;
    }

    //ʹ��CRC����
    FCS=crc32(MAC,14+Data_Length);
    memcpy(&MAC[14+Data_Length],&FCS,4);
    Turn(&MAC[14+Data_Length],4);

    //��ӡ��MAC֡�����Ϣ
    printf("������������������������������������������·�㱨���ײ���Ϣ������������������������������������\n");
    printf("Ŀ�ĵ�ַ�� %u.%u.%u.%u.%u.%u\n",Destination_Address[0],Destination_Address[1],Destination_Address[2],Destination_Address[3],Destination_Address[4],Destination_Address[5]);
    printf("Դ��ַ��   %u.%u.%u.%u.%u.%u\n",Source_Address[0],Source_Address[1],Source_Address[2],Source_Address[3],Source_Address[4],Source_Address[5]);
    printf("���ͣ�     IP\n");
    printf("β��FCS��  %u\n",FCS);
    printf("********************************[���ĸ��ֶ�ʮ�����Ƹ�ʽ�����]********************************\n");
    printf("Ŀ�ĵ�ַ��6�ֽڣ��� ");
    Print_Oct_2Bytes(&MAC[0],6);
    printf("Դ��ַ��6�ֽڣ���   ");
    Print_Oct_2Bytes(&MAC[6],6);
    printf("���ͣ�2�ֽڣ���     ");
    Print_Oct_2Bytes(&MAC[12],2);
    printf("FCS��4�ֽڣ���      ");
    Print_Oct_2Bytes(&MAC[14+Data_Length],4);
    printf("���ݣ�%d�ֽڣ���\n",Data_Length);
    Print_Oct_2Bytes(&MAC[12],Data_Length);
    printf("����������������������������������������������������������������������������������������������\n");

    printf("\n�ѳɹ����·��͵������...\n\n");


    //�����ļ���д�ķ�ʽ��MAC֡����
    FILE* pf;
    if(flag==false)
        pf = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\MACS.txt", "w");
    else
        pf = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\MACS.txt", "a");
    fwrite(MAC,1,18+Data_Length,pf);
    fclose(pf);

    //�����δʵ��
}

#endif // SENDER_DATALINK_LAYER_H
