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
        }
    }
    fseek(Input_File,132L,1);  
    for(index_1=0;index_1<Number_Buses;index_1++)
    {
        for(index_2=0;index_2<6;index_2++)
        {
	        fscanf(Input_File,"%f",&Load_Buses[index_1][index_2]);
        }
    }
	
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
    
}


void Form_Y()
{
    int index_1,index_2;
    float Temp_Variable;
    /*只考虑两个节点之间只有一个值*/
    for(index_1=0;index_1<Number_Line;index_1++)
    {
        if((Transmission_Matrix[index_1][2]==0)&&(Transmission_Matrix[index_1][3])==0)
        {
            G[(int)Transmission_Matrix[index_1][0]-1][(int)Transmission_Matrix[index_1][1]-1]=-Transmission_Matrix[index_1][2]/(pow(Transmission_Matrix[index_1][2],2)+pow(Transmission_Matrix[index_1][3],2));
            G[(int)Transmission_Matrix[index_1][1]-1][(int)Transmission_Matrix[index_1][0]-1]=G[(int)Transmission_Matrix[index_1][0]-1][(int)Transmission_Matrix[index_1][1]-1];
            B[(int)Transmission_Matrix[index_1][0]-1][(int)Transmission_Matrix[index_1][1]-1]=Transmission_Matrix[index_1][3]/(pow(Transmission_Matrix[index_1][2],2)+pow(Transmission_Matrix[index_1][3],2));
            B[(int)Transmission_Matrix[index_1][1]-1][(int)Transmission_Matrix[index_1][0]-1]=B[(int)Transmission_Matrix[index_1][0]-1][(int)Transmission_Matrix[index_1][1]-1];
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


void Calculate_Unblanced_Value()
{
    /*只进行PQ节点计算,默认节点中存储为顺序存储*/
    float P[Const_Value]={0};
    float Q[Const_Value]={0};
    int index_1,index_2;
    for(index_1=0;index_1<Number_PQ;index_1++)
    {
        for(index_2=0;index_2<Number_PQ;index_2++)
        {
            P[index_1]=P[index_1]+Load_Buses[index_1][3]*(G[index_1][index_2]*Load_Buses[index_2][3]-B[index_1][index_2]*Load_Buses[index_2][4])+Load_Buses[index_1][4]*(G[index_1][index_2]*Load_Buses[index_2][4]+B[index_1][index_2]*Load_Buses[index_2][3]);
            Q[index_1]=Q[index_1]+Load_Buses[index_1][4]*(G[index_1][index_2]*Load_Buses[index_2][3]-B[index_1][index_2]*Load_Buses[index_2][4])-Load_Buses[index_1][3]*(G[index_1][index_2]*Load_Buses[index_2][4]+B[index_1][index_2]*Load_Buses[index_2][3]);
        }
        Delta_P[index_1]=Load_Buses[index_1][1]-P[index_1];
        Delta_Q[index_1]=Load_Buses[index_1][2]-Q[index_1];
    }    
}



void main()
{
    Read_Data();
    Form_Y();
    Calculate_Unblanced_Value();
}