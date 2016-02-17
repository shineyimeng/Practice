#include <stdio.h>
#include <math.h>

#define NULL 0
const Const_Value=10;
float Calculation_Accuracy=0.001;/*���徫ȷ��*/
int Number_Buses=3; /*�ڵ�����*/
int Number_Line=2;  /*��·����*/
float Transmission_Matrix[Const_Value][6];  /*��·��������*/
float Load_Buses[Const_Value][6];        /*�ڵ��������*/
float Y[Const_Value][Const_Value];  /*�ڵ㵼�ɾ���*/
float G[Const_Value][Const_Value]={0};
float B[Const_Value][Const_Value]={0};

void Form_Y()
{
    int index_1,index_2;
	float Temp_Variable;
	/*ֻ���������ڵ�֮��ֻ��һ��ֵ*/
    for(index_1=0;index_1<Number_Line;index_1++)
    {
        if((Transmission_Matrix[index_1][2]!=0)&&(Transmission_Matrix[index_1][3]!=0))
        {
            G[(int)Transmission_Matrix[index_1][0]-1][(int)Transmission_Matrix[index_1][1]-1]=-Transmission_Matrix[index_1][2]/(pow(Transmission_Matrix[index_1][2],2)+pow(Transmission_Matrix[index_1][3],2));
            G[(int)Transmission_Matrix[index_1][1]-1][(int)Transmission_Matrix[index_1][0]-1]=G[(int)Transmission_Matrix[index_1][0]-1][(int)Transmission_Matrix[index_1][1]-1];
            B[(int)Transmission_Matrix[index_1][0]-1][(int)Transmission_Matrix[index_1][1]-1]=Transmission_Matrix[index_1][3]/(pow(Transmission_Matrix[index_1][2],2)+pow(Transmission_Matrix[index_1][3],2));
            B[(int)Transmission_Matrix[index_1][1]-1][(int)Transmission_Matrix[index_1][0]-1]=B[(int)Transmission_Matrix[index_1][0]-1][(int)Transmission_Matrix[index_1][1]-1];
			printf("%d\n\n",(int)Transmission_Matrix[index_1][0]-1);
		}
        else
        {
            G[(int)Transmission_Matrix[index_1][0]-1][(int)Transmission_Matrix[index_1][1]-1]=-Transmission_Matrix[index_1][4];
            G[(int)Transmission_Matrix[index_1][1]-1][(int)Transmission_Matrix[index_1][0]-1]=G[(int)Transmission_Matrix[index_1][0]-1][(int)Transmission_Matrix[index_1][1]-1];
            B[(int)Transmission_Matrix[index_1][0]-1][(int)Transmission_Matrix[index_1][1]-1]=-Transmission_Matrix[index_1][5];
            B[(int)Transmission_Matrix[index_1][1]-1][(int)Transmission_Matrix[index_1][0]-1]=B[(int)Transmission_Matrix[index_1][0]-1][(int)Transmission_Matrix[index_1][1]-1];
        }

    }
	for(index_1=0;index_1<Number_Line;index_1++)
	{
		for(index_2=0,Temp_Variable=0;index_2<Number_Buses;index_2++)
		{
			Temp_Variable=Temp_Variable+G[index_1][index_2];
		}
		G[index_1][index_1]=-Temp_Variable;
		for(index_2=0,Temp_Variable=0;index_2<Number_Buses;index_2++)
		{
			Temp_Variable=Temp_Variable+B[index_1][index_2];
		}
		B[index_1][index_1]=-Temp_Variable;
	}
}
void main()
{
	FILE *Input_File;
	Input_File=fopen("input.txt","r");
	int index_1,index_2;
	for(index_1=0;index_1<2;index_1++)
	{
		for(index_2=0;index_2<6;index_2++)
		{
			fscanf(Input_File,"%f",&Transmission_Matrix[index_1][index_2]);
			printf("%f   ",Transmission_Matrix[index_1][index_2]);
		}
		printf("\n");
	}
	printf("\n\n");
	Form_Y();
	for(index_1=0;index_1<3;index_1++)
	{
		for(index_2=0;index_2<3;index_2++)
		{
			printf("%f  ",G[index_1][index_2]);
		}
		printf("\n");
	}

	printf("\n");

    for(index_1=0;index_1<3;index_1++)
	{
		for(index_2=0;index_2<3;index_2++)
		{
			printf("%f  ",B[index_1][index_2]);
		}
		printf("\n");
	}
}