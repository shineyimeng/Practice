#include <stdio.h>
void main()
{
	float A[4][4]={15,5,-7.5,-2.5,-5,15,2.5,-7.5,-7.5,-2.5,7.5,2.5,2.5,-7.5,-2.5,7.5};
	float B[4]={-0.04,-0.15,-0.04,-0.15};
	float X[4];
	int Number_Matrix;
    int index_1,index_2,index_3,index_4;
    float Temp_Variable=0;
    Number_Matrix=sizeof(A[0])/4;
	printf("%d\n",Number_Matrix);
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
	for(index_1=0;index_1<Number_Matrix;index_1++)
	{
		for(index_2=0;index_2<Number_Matrix;index_2++)
		{
			printf("%f     ",A[index_1][index_2]);
		}
		printf("\n");
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
	printf("%d\n",Number_Matrix);
	for(index_1=0;index_1<Number_Matrix;index_1++)
	{
		printf("%f   \n",X[index_1]);
	}
}