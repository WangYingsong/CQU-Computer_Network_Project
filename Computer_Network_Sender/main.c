#include <stdio.h>
#include <string.h>
#include"Sender_Application_Layer.h"

int main()
{
    //读入本机IP地址
    FILE* Local_IP;
    Local_IP = fopen("C:\\Users\\86182\\Desktop\\Computer_Network_Project\\Computer_Network_Sender\\Sender_IP.txt", "r");
    fscanf(Local_IP,"%u.%u.%u.%u",&Source_IP[0],&Source_IP[1],&Source_IP[2],&Source_IP[3]);
    fclose(Local_IP);

    Sender_Application_Layer();
    return 0;
}
