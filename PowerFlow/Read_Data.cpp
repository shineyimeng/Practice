#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
/*----------------------------------------------------------------------------
牛顿——拉夫逊法潮流计算的程序

----------------------------------------------------------------------------*/

#define NULL 0
const Const_Value=10;
float Calculation_Accuracy=0.001;/*定义精确度*/
int Number_Buses; /*节点总数*/
int Number_Line;  /*线路总数*/
int Number_PQ=0;
int Number_PV=0;
int Number_SB=0;
float Transmission_Matrix[Const_Value][6];  /*线路参数矩阵*/
float Load_Buses[Const_Value][6];        /*节点参数矩阵*/
float Y[Const_Value][Const_Value];  /*节点导纳矩阵*/
float G[Const_Value][Const_Value]={0};
float B[Const_Value][Const_Value]={0};
float Delta_P[Const_Value]={0};  /*计算不平衡量*/
float Delta_Q[Const_Value]={0};


/*-----------------------输入数据格式说明--------------------------------------
潮流上机输入数据（请不要修改汉字部分）

节点数 (Number_Buses)  支路数  (Number_Line)

支路参数
输入格式：
节点1 节点2 电阻    电抗     电导    电纳

节点参数
节点类型注释
1--PQ节点； 2--PV节点； 3--平衡节点
输入格式：
节点序号 有功功率 无功功率 节点电压 电压相角 节点类型
----------------------------------------------------------------------------*/
void Read_Data()
{
    int index_1,index_2;
    FILE *Input_File;
    if((Input_File=fopen("input.txt","r"))==NULL)
    {
        printf("不能打开此文件\n");
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