#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
/*----------------------------------------------------------------------------
ţ�١�������ѷ����������ĳ���

----------------------------------------------------------------------------*/

#define NULL 0
const Const_Value=10;
float Calculation_Accuracy=0.001;/*���徫ȷ��*/
int Number_Buses; /*�ڵ�����*/
int Number_Line;  /*��·����*/
int Number_PQ=0;
int Number_PV=0;
int Number_SB=0;
float Transmission_Matrix[Const_Value][6];  /*��·��������*/
float Load_Buses[Const_Value][6];        /*�ڵ��������*/
float Y[Const_Value][Const_Value];  /*�ڵ㵼�ɾ���*/
float G[Const_Value][Const_Value]={0};
float B[Const_Value][Const_Value]={0};
float Delta_P[Const_Value]={0};  /*���㲻ƽ����*/
float Delta_Q[Const_Value]={0};


/*-----------------------�������ݸ�ʽ˵��--------------------------------------
�����ϻ��������ݣ��벻Ҫ�޸ĺ��ֲ��֣�

�ڵ��� (Number_Buses)  ֧·��  (Number_Line)

֧·����
�����ʽ��
�ڵ�1 �ڵ�2 ����    �翹     �絼    ����

�ڵ����
�ڵ�����ע��
1--PQ�ڵ㣻 2--PV�ڵ㣻 3--ƽ��ڵ�
�����ʽ��
�ڵ���� �й����� �޹����� �ڵ��ѹ ��ѹ��� �ڵ�����
----------------------------------------------------------------------------*/
void Read_Data()
{
    int index_1,index_2;
    FILE *Input_File;
    if((Input_File=fopen("input.txt","r"))==NULL)
    {
        printf("���ܴ򿪴��ļ�\n");
        exit(0);
    }
    fseek(Input_File,50L,1);
    fscanf(Input_File,"%d",&Number_Buses);
    fseek(Input_File,12L,1);
    fscanf(Input_File,"%d",&Number_Line);
    fseek(Input_File,70L,1);
    for(index_1=0;index_1<Number_Line;index_1++)
    {
        for(index_2=0;index_2<6;index_2++)
        {
            fscanf(Input_File,"%f",&Transmission_Matrix[index_1][index_2]);
			printf("%f    ",Transmission_Matrix[index_1][index_2]);
        }
		printf("\n");
    }
	printf("\n");
    fseek(Input_File,132L,1);  
    for(index_1=0;index_1<Number_Buses;index_1++)
    {
        for(index_2=0;index_2<6;index_2++)
        {
	        fscanf(Input_File,"%f",&Load_Buses[index_1][index_2]);
			printf("%f    ",Load_Buses[index_1][index_2]);
        }
		printf("\n");
    }
	printf("\n");
    fclose(Input_File);
    for(index_1=0;index_1<Number_Buses;index_1++)
    {
        switch((int)Load_Buses[index_1][5])
        {
            case 1:
            {
                Number_PQ++;
				break;
            }
            case 2:
            {
                Number_PV++;
				break;
            }
            case 3:
            {
                Number_SB++;
				break;
            }
        }
    }
	printf("%d\n",Number_PQ);
	printf("%d\n",Number_PV);
	printf("%d\n",Number_SB);
}


void main()
{
    Read_Data();
}