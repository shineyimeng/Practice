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
float Jacobi_Matric[2*Const_Value][2*Const_Value]={0};
float Delta_PQ[2*Const_Value]={0};
float Delta_fe[2*Const_Value]={0};



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
节点序号 有功功率 无功功率 电压实部 电压虚部 节点类型
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
    int index_1,index_2;
    //临时注释：1只进行PQ节点计算2默认节点中存储为顺序存储3语法调试无错误计算未验证
    float P[Const_Value]={0};
    float Q[Const_Value]={0};
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
    
    for(index_1=0;index_1<Number_PQ;index_1++)
    {
        Delta_PQ[2*index_1]=Delta_P[index_1];
        Delta_PQ[2*index_1+1]=Delta_Q[index_1];
    }
}


void Form_Jacobi_Matric()
{
    int index_1,index_2;
    float H[Const_Value][Const_Value]={0};
    float N[Const_Value][Const_Value]={0};
    float J[Const_Value][Const_Value]={0};
    float L[Const_Value][Const_Value]={0};
    float a[Const_Value]={0};
    float b[Const_Value]={0};
    //暂时不考虑PV节点
    //疑问
    for(index_1=0;index_1<Number_PQ;index_1++)
    {
        for(index_2=0;index_2<Number_PQ;index_2++)
        {
            a[index_1]=a[index_1]+G[index_1][index_2]*Load_Buses[index_2][3]-B[index_1][index_2]*Load_Buses[index_2][4];
            b[index_1]=b[index_1]+G[index_1][index_2]*Load_Buses[index_2][4]+B[index_1][index_2]*Load_Buses[index_2][3];            
        }
    }
    
    for(index_1=0;index_1<Number_PQ;index_1++)
    {
        for(index_2=0;index_2<Number_PQ;index_2++)
        {
            if(index_1!=index_2)
            {
                H[index_1][index_2]=-B[index_1][index_2]*Load_Buses[index_1][3]+G[index_1][index_2]*Load_Buses[index_1][4];
                N[index_1][index_2]=G[index_1][index_2]*Load_Buses[index_1][3]+B[index_1][index_2]*Load_Buses[index_1][4];
                J[index_1][index_2]=-N[index_1][index_2];
                L[index_1][index_2]=H[index_1][index_2];
            }
            else
            {
                H[index_1][index_1]=-B[index_1][index_1]*Load_Buses[index_1][3]+G[index_1][index_1]*Load_Buses[index_1][4]+b[index_1];
                N[index_1][index_1]=G[index_1][index_1]*Load_Buses[index_1][3]+B[index_1][index_1]*Load_Buses[index_1][4]+a[index_1];
                J[index_1][index_1]=-G[index_1][index_1]*Load_Buses[index_1][3]-B[index_1][index_1]*Load_Buses[index_1][4]+a[index_1];
                L[index_1][index_2]=-B[index_1][index_1]*Load_Buses[index_1][3]+G[index_1][index_1]*Load_Buses[index_1][4]-b[index_1];
            }
        }
    }
    for(index_1=0;index_1<Number_PQ;index_1++)
    {
        for(index_2=0;index_2<Number_PQ;index_2++)
        {
            Jacobi_Matric[2*index_1][2*index_2]=H[index_1][index_2];
            Jacobi_Matric[2*index_1+1][2*index_2]=N[index_1][index_2];
            Jacobi_Matric[2*index_1][2*index_2+1]=J[index_1][index_2];
            Jacobi_Matric[2*index_1+1][2*index_2+1]=L[index_1][index_2];
        }
    }
}


void Solve_Equations(float A[][2*Const_Value],float B[],float X[])
{
    int index_1,index_2,index_3,index_4;
    int Number_Matrix;
    float Temp_Variable;
    Number_Matrix=sizeof(A[0])/4;
    Number_Matrix=2*Number_PQ;
    for(index_1=1;index_1<Number_Matrix;index_1++)
    {
        A[index_1][0]=A[index_1][0]/A[0][0];
    }
    for(index_3=1;index_3<Number_Matrix;index_3++)
    {
        for(index_2=index_3;index_2<Number_Matrix;index_2++)
        {
            for(index_4=0,Temp_Variable=0;index_4<index_3;index_4++)
            {
                Temp_Variable=Temp_Variable+A[index_3][index_4]*A[index_4][index_2];
            }
            A[index_3][index_2]=A[index_3][index_2]-Temp_Variable;
        }
        for(index_1=index_3+1;index_1<Number_Matrix;index_1++)
        {
            for(index_4=0,Temp_Variable=0;index_4<index_3;index_4++)
            {
                Temp_Variable=Temp_Variable+A[index_1][index_4]*A[index_4][index_3];
            }
            A[index_1][index_3]=(A[index_1][index_3]-Temp_Variable)/A[index_3][index_3];
        }
        
    }
    X[0]=B[0];
    for(index_1=1;index_1<Number_Matrix;index_1++)
    {
        for(index_4=0,Temp_Variable=0;index_4<index_1;index_4++)
        {
            Temp_Variable=Temp_Variable+A[index_1][index_4]*X[index_4];
        }
        X[index_1]=B[index_1]-Temp_Variable;
    }
    X[Number_Matrix-1]=X[Number_Matrix-1]/A[Number_Matrix-1][Number_Matrix-1];
    for(index_1=Number_Matrix-2;index_1>=0;index_1--)
    {
        for(index_4=index_1+1,Temp_Variable=0;index_4<Number_Matrix;index_4++)
        {
            Temp_Variable=Temp_Variable+A[index_1][index_4]*X[index_4];
        }
        X[index_1]=(X[index_1]-Temp_Variable)/A[index_1][index_1];
    }
}


/*----------------------------------------------------------------------------
输出格式介绍:

----------------------------------------------------------------------------*/
void Powerflow_Results()
{
    FILE *Output_File;
    Output_File=fopen("output.txt","w"); 
}


void main()
{
    int index;
    Read_Data();
    Form_Y();
    Calculate_Unblanced_Value();
    //修改判断条件精确度
    for(index=0;index<2;index++)
    {
        Form_Jacobi_Matric();
        Solve_Equations(Jacobi_Matric,Delta_PQ,Delta_fe);
        Powerflow_Results();
    }
}