#include <stdio.h>
#include "Receiver_DataLink_Layer.h"
#include "Tools.h"

int main()
{
    //读入传输过来的MAC帧
    unsigned char MAC[70000];
    FILE* pf;
    pf = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\MACS.txt", "r");
    int i=0;
    unsigned char c;
    while (!feof(pf))
    {
        c=getc(pf);
        MAC[i]=c;
        i++;
    }
    fclose(pf);

    //读入本机IP地址
    FILE* Local_Ip;
    Local_Ip = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\Computer_Network_Receiver\\Receiver_IP.txt", "r");
    fscanf(Local_Ip,"%u.%u.%u.%u",&Local_IP[0],&Local_IP[1],&Local_IP[2],&Local_IP[3]);
    fclose(Local_Ip);

    //读入本机MAC地址
    FILE* Local_MAC;
    Local_MAC = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\Computer_Network_Receiver\\Receiver_MAC.txt", "r");
    fscanf(Local_MAC,"%u.%u.%u.%u.%u.%u",&Local_Address[0],&Local_Address[1],&Local_Address[2],&Local_Address[3],&Local_Address[4],&Local_Address[5]);
    fclose(Local_MAC);

    Receiver_DataLink_Layer(MAC,i-1);
    return 0;
}
