#include"stdio.h"
#include"math.h"
#include"fstream.h"
//��������Դ����
//ֱ�������µļ���
//�����ļ�������InputData1.txt���ڵ����
//�����ļ�������InputData2.txt��֧·����
//����ļ�������OutputData.txt��
/*�Ӻ�������
  ReadData();����ԭʼ���ݣ�
  Form_Y();  ���γɽڵ㵼�ɾ���
  Gauss();   ����˹���¶��������ֵ��
  Calculate_Unbalanced_Value();�����㲻ƽ�⹦�ʺ͵�ѹ��
  Form_Jacobi_Matric();        ���γ��ſɱȾ���
  Solve_Equations();           ������������ʽ��
  Powerflow_Results();         �����������������*/
  int BN,LM;//BNΪ�ڵ������������LMΪ֧·���������� 
  int nPQ=0,nPV=0,nSW=0;//nPQ,nPV,nSW�ֱ�Ϊ����ڵ����;
  const N=100;
  float bus[N][6];//�ڵ����
  float line[N][7];//֧·����
  float yr[N][N],yi[N][N];//�ڵ㵼�ɾ���ʵ�����鲿
  float Per[N],Qer[N],Uer[N];//�й����ʡ��޹����ʡ���ѹ�Ĳ�ƽ����
  float e[N],f[N],p[N],q[N];//��Žڵ��ѹ,�й����޹�����
  float Eer[N],Fer[N];//��Žڵ��ѹʵ���鲿�Ĳ�ƽ����
  float jac[N][N];//����ſ˱Ⱦ���
  float x[N];//��f,e�Ĳ�ƽ�����ϳ�һ������
  float big;//x[N]�е����ֵ
  FILE *fp;
void main()
{ int i,j,k=0;
  float lim;
  void ReadData();
  void Form_Y();
  void Gauss();
  void Calculate_Unbalanced_Value();
  void Form_Jacobi_Matric();
  void Solve_Equations();
  void Powerflow_Results();
  printf("�����뾫��lim=");
  scanf("%f",&lim);
  fp=fopen("OutputData.txt","w+");
  //�����Ӻ������������ݽ��д�뵽�ļ���
  ReadData();//������
  Form_Y();//�γɽڵ㵼�ɾ���
  //��˹���¶����������μ����ֵ
  Gauss();
  //ţ��������������Ҫ��ľ���
  do
  {k++;
   fprintf(fp,"===============��%d�ε������=============\n",k);
   Calculate_Unbalanced_Value();  //���㲻ƽ����
   Form_Jacobi_Matric();  //�γ��ſ˱Ⱦ���
   Solve_Equations();  //�������Է���
  }while(big>lim);  //limΪ����        
  Powerflow_Results();  //������
}
//******************ReadData()***Start***********************
//��ԭʼ����,�����ڵ㡢֧·������PQ,PV,ƽ��ڵ��˳����������
void ReadData()
{ float PQ[N][6],PV[N][6],SW[N][6];//PQ��PV��ƽ��ڵ�����ֱ���
  int i,j,k;
  FILE *fp1,*fp2;
  fp1=fopen("InputData1.txt","r+");
  fp2=fopen("InputData2.txt","r+");
//��ȡ�ڵ�����BN
  ifstream in1("InputData1.txt");
  char str1[N];//1000�Ƕ���ĳ�����Ŀ����Ϊ�˶�ȡ�㹻������
  BN=0;
  while(!in1.eof())//eof()�ж��Ƿ�����ļ���ֹ
  { 
	  in1.getline(str1, sizeof(str1));
	  BN++;		  
  }//ȷ���ڵ�����BN
//��ȡ֧·����LM
  ifstream in2("InputData2.txt");
  char str2[N];//1000�Ƕ���ĳ�����Ŀ����Ϊ�˶�ȡ�㹻������
  LM=0;
  while(!in2.eof())//eof()�ж��Ƿ�����ļ���ֹ
  { 
	  in2.getline(str2, sizeof(str2));
	  LM++;		  
  }//ȷ��֧·����LM
  for(i=0;i<BN;i++)
	  for(j=0;j<6;j++)
	   fscanf(fp1,"%f",&bus[i][j]);
  for(i=0;i<LM;i++)
	  for(j=0;j<7;j++)
	   fscanf(fp2,"%f",&line[i][j]);//���ļ���ֵ��������
  fclose(fp1);
  fclose(fp2);
  for(i=0;i<BN;i++)
	  if(bus[i][5]==1)
	  {for(j=0;j<6;j++)
		PQ[nPQ][j]=bus[i][j];
	  nPQ++;}
	  else if(bus[i][5]==2)
	  {for(j=0;j<6;j++)
		PV[nPV][j]=bus[i][j];
	  nPV++;}
	  else if(bus[i][5]==3)
	  {for(j=0;j<6;j++)
	    SW[nSW][j]=bus[i][j];
	  nSW++;}//PQ[][]PV[][]SW[][]�ֱ���PQ��PV��SW�ڵ����
  for(i=0;i<nPQ;i++)
	  for(j=0;j<6;j++)
		  bus[i][j]=PQ[i][j];
  for(i=nPQ;i<nPQ+nPV;i++)
	  for(j=0;j<6;j++)
		  bus[i][j]=PV[i-nPQ][j];
  for(i=nPQ+nPV;i<nPQ+nPV+nSW;i++)
	  for(j=0;j<6;j++)
		  bus[i][j]=SW[i-nPQ-nPV][j];//��PQ��PV��SW�ڵ�˳�����������
  for(i=0;i<LM;i++)
      for(j=0;j<2;j++)
          for(k=0;k<BN;k++)
              if(line[i][j]==bus[k][0])
			  {line[i][j]=k+1;
			  break;}//�ı�֧·��������Ӧ�Ľڵ����
  for(i=0;i<BN;i++)
	  bus[i][0]=i+1; //�ڵ��ر��
  fprintf(fp,"**************************************��������**********************************\n");
  fprintf(fp,"���������༶������ʵ10���������������ߣ����󡪡�������ѧ�ţ�201001001316��������\n");
  fprintf(fp,"�������������������������ڣ�2012-01�����������ص㣺�̾���¥D��������������������\n");
  fprintf(fp,"\n");
  fprintf(fp,"                        ****************Ŀ¼***************\n");
  fprintf(fp,"                        Part1.ԭʼ����*********************\n");
  fprintf(fp,"                        Part2.�ڵ㵼�ɾ���******************\n");
  fprintf(fp,"                        Part3.��һ�����θ�˹���¶��������**\n");
  fprintf(fp,"                        Part4.ţ�����������****************\n");
  fprintf(fp,"                        Part5.�������**********************\n");
  fprintf(fp,"\n");
  fprintf(fp,"********************************Part1****ԭʼ����*******************************\n");
  fprintf(fp,"�ڵ�����:%d    ֧·����:%d\n",BN,LM);
  fprintf(fp,"�ڵ����ݡ����ڵ����     �й�����   �޹�����   ��ѹ��ֵ  ��ѹ���   ����\n");
  fprintf(fp,"�ڵ����:\n");
  for(i=0;i<BN;i++)
  {for(j=0;j<6;j++)
  {if(j==0||j==5)
	fprintf(fp,"%d     ",int(bus[i][j]));
  else fprintf(fp,"%f     ",bus[i][j]);}
  fprintf(fp,"\n");}
  fprintf(fp,"================================================================================\n");
  fprintf(fp,"֧·���ݡ����ڵ����   �ڵ����     ����   �翹   �絼   ����   �Ǳ���\n");
  fprintf(fp,"֧·����:\n");
  for(i=0;i<LM;i++)
  {for(j=0;j<7;j++)
  {if(j==0||j==1)
    fprintf(fp,"%d      ",int(line[i][j]));
  else
	  fprintf(fp,"%f      ",line[i][j]);}
  fprintf(fp,"\n");}
  fprintf(fp,"********************************************************************************\n");
}
//******************ReadData()***End*************************
//***********************************************************
//******************Form_Y()*****Start***********************
void Form_Y()
{int i,j,m,n;
  for(i=0;i<BN;i++)
	  for(j=0;j<BN;j++)
	  {yr[i][j]=0;yi[i][j]=0;}//�ڵ㵼�ɾ��󸳳�ֵ0
  for(i=0;i<LM;i++)
	  {m=int(line[i][0]-1);n=int(line[i][1]-1);//m,n�ֱ�Ϊ�ڵ�ż�1.�Զ�Ӧ������
      if(line[i][0]==0)//�Ե�֧·
	  {yr[n][n]+=line[i][4];
	  yi[n][n]+=line[i][5];}
	  else if(line[i][1]==0)//�Ե�֧·
	  {yr[m][m]+=line[i][4];
	  yi[m][m]+=line[i][5];}

	  else if(line[i][6]==0)      //��ͨ��·����������ѹ������·
//line[i][2]ΪR��line[i][3]ΪX��line[i][4]ΪG��line[i][5]ΪB
  {yr[m][n]+=-line[i][2]/(line[i][2]*line[i][2]+line[i][3]*line[i][3]);
   yi[m][n]+=line[i][3]/(line[i][2]*line[i][2]+line[i][3]*line[i][3]);
   yr[m][m]+=-yr[m][n];
   yi[m][m]+=-yi[m][n]+line[i][5];
   yr[n][n]+=-yr[m][n];
   yi[n][n]+=-yi[m][n]+line[i][5];
   yr[n][m]=yr[m][n];
   yi[n][m]=yi[m][n];}
       else if(line[i][6]>0) //����ѹ����·����·������i����㵽j�࣬k��j�࣬�����ѹ��Ϊk=line[i][6]
	   {yr[m][n]+=-line[i][2]/(line[i][2]*line[i][2]+line[i][3]*line[i][3])/line[i][6];
        yi[m][n]+=line[i][3]/(line[i][2]*line[i][2]+line[i][3]*line[i][3])/line[i][6];
        yr[m][m]+=-yr[m][n]*(line[i][6]-1)/line[i][6];
        yi[m][m]+=-yi[m][n]*(line[i][6]-1)/line[i][6];
        yr[n][n]+=-yr[m][n]*(1-line[i][6])/(line[i][6]*line[i][6]);
        yi[n][n]+=-yi[m][n]*(1-line[i][6])/(line[i][6]*line[i][6]);
	    yr[n][m]=yr[m][n];
        yi[n][m]=yi[m][n];}
	   else if(line[i][6]<0) //����ѹ����·����·������j����㵽i�࣬k��i�࣬�����ѹ��Ϊk=line[i][6]
	   {yr[m][n]+=-line[i][2]/(line[i][2]*line[i][2]+line[i][3]*line[i][3])/(-line[i][6]);
        yi[m][n]+=line[i][3]/(line[i][2]*line[i][2]+line[i][3]*line[i][3])/(-line[i][6]);
        yr[m][m]+=-yr[m][n]*(1+line[i][6])/(line[i][6]*line[i][6]);
        yi[m][m]+=-yi[m][n]*(1+line[i][6])/(line[i][6]*line[i][6]);
        yr[n][n]+=-yr[m][n]*(-line[i][6]-1)/(-line[i][6]);
        yi[n][n]+=-yi[m][n]*(-line[i][6]-1)/(-line[i][6]);
	    yr[n][m]=yr[m][n];
        yi[n][m]=yi[m][n];}
  }
  fprintf(fp,"********************************Part2****�ڵ㵼�ɾ���***************************\n");
 for(i=0;i<BN;i++)
 {for(j=0;j<BN;j++)
 {if(yi[i][j]>=0)
	fprintf(fp,"%f+j%f    ",yr[i][j],yi[i][j]);
 else
	fprintf(fp,"%f-j%f    ",yr[i][j],fabs(yi[i][j]));}
 fprintf(fp,"\n");}
 fprintf(fp,"\n");
 fprintf(fp,"********************************************************************************\n");
 fprintf(fp,"*********************************Part3****��˹���¶���**************************\n");
}
//******************Form_Y()*****End*************************
//***********************************************************
//******************Gauss()******Start***********************
//���ø�˹���¶�����ȡ��ֵ����ƽ�⹦�ʺ͵�ѹ��
void Gauss()
{int i,j,k;
for(k=0;k<2;k++)
{fprintf(fp,"===============��%d�ε������=============\n",k+1);
float cr[N],ci[N],dr[N],di[N];//��Ź��̱���
//PQ�ڵ�ļ���
  for(i=0;i<BN;i++)
  {//ע:PV�ڵ��ѹ��ǳ�ʼΪ0
   //ע:PQ�ڵ��ѹ��ֵ��ʼΪ1�����Ϊ0
  e[i]=bus[i][3]*cos(bus[i][4]);//�ڵ��ѹʵ��
  f[i]=bus[i][3]*sin(bus[i][4]); //�ڵ��ѹ�鲿
  p[i]=bus[i][1]; //�ڵ��й�����
  q[i]=bus[i][2];}//�ڵ��޹�����
  for(i=0;i<nPQ;i++)//cr[]ci[]dr[]di[]Ϊ�����м��������ʵ������
  {cr[i]=yr[i][i]/(yr[i][i]*yr[i][i]+yi[i][i]*yi[i][i]);
   ci[i]=-yi[i][i]/(yr[i][i]*yr[i][i]+yi[i][i]*yi[i][i]);
   dr[i]=(p[i]*e[i]+q[i]*f[i])/(e[i]*e[i]+f[i]*f[i]);
   di[i]=(p[i]*f[i]-q[i]*e[i])/(e[i]*e[i]+f[i]*f[i]);
   for(j=0;j<BN;j++)
   {if(j==i)
    continue;//����Y(i,i)*U(i)��
   dr[i]-=yr[i][j]*e[j]-yi[i][j]*f[j];
   di[i]-=yr[i][j]*f[j]+yi[i][j]*e[j];}
   e[i]=cr[i]*dr[i]-ci[i]*di[i];
   f[i]=ci[i]*dr[i]+cr[i]*di[i];
   bus[i][3]=sqrt(e[i]*e[i]+f[i]*f[i]);//PQ�ڵ��ֵ
   bus[i][4]=atan(f[i]/e[i]);}//PQ�ڵ����
//PV�ڵ�ļ���
  for(i=nPQ;i<BN-1;i++)
  {cr[i]=0;ci[i]=0;
  for(j=0;j<BN;j++)
  {cr[i]+=yr[i][j]*e[j]-yi[i][j]*f[j];
   ci[i]+=yr[i][j]*f[j]+yi[i][j]*e[j];}
   q[i]=cr[i]*f[i]-ci[i]*e[i];//�ɽڵ��ѹ���̵õ�PV�ڵ���޹����ʽ���ֵ
   cr[i]+=-yr[i][i]*e[i]+yi[i][i]*f[i];
   cr[i]=-cr[i]+(p[i]*e[i]+q[i]*f[i])/(e[i]*e[i]+f[i]*f[i]);
   ci[i]+=-yr[i][i]*f[i]-yi[i][i]*e[i];
   ci[i]=-ci[i]+(p[i]*f[i]-q[i]*e[i])/(e[i]*e[i]+f[i]*f[i]);
   dr[i]=yr[i][i]/(yr[i][i]*yr[i][i]+yi[i][i]*yi[i][i]);
   di[i]=-yi[i][i]/(yr[i][i]*yr[i][i]+yi[i][i]*yi[i][i]);//ͬǰ��cr��ci��dr��diΪ�м����
   e[i]=dr[i]*cr[i]-di[i]*ci[i];
   f[i]=di[i]*cr[i]+dr[i]*ci[i];
   bus[i][4]=atan(f[i]/e[i]);//����PV�ڵ�ĵ�ѹ���
   e[i]=bus[i][3]*cos(bus[i][4]);
   f[i]=bus[i][3]*sin(bus[i][4]);
  }
fprintf(fp,"PQ�ڵ��ѹ:\n");
for(i=0;i<nPQ;i++)
{if(f[i]>=0)
     fprintf(fp,"%d����%f+j%f\n",i+1,e[i],f[i]);
 else 
	 fprintf(fp,"%d����%f-j%f\n",i+1,e[i],fabs(f[i]));}
if(nPV!=0)
{fprintf(fp,"PV�ڵ��ѹ:\n");
for(i=0;i<nPV;i++)
{if(f[i+nPQ]>=0)
fprintf(fp,"%d����%f+j%f\n",i+1+nPQ,e[i+nPQ],f[i+nPQ]);
else
fprintf(fp,"%d����%f-j%f\n",i+1+nPQ,e[i+nPQ],fabs(f[i+nPQ]));}}
else fprintf(fp,"������PV�ڵ�\n");
}
 fprintf(fp,"\n");
 fprintf(fp,"********************************************************************************\n");
 fprintf(fp,"*********************************Part4****ţ������ѷ��**************************\n");
 fprintf(fp,"\n");
 fprintf(fp,"                       ****************�ſ˱Ⱦ���************\n");
 fprintf(fp,"                       **************����*H=N****************\n");
 fprintf(fp,"                       *******************J=L****************\n");
 fprintf(fp,"                       *******************R=S**����**********\n");
 fprintf(fp,"                       **************************************\n");
 fprintf(fp,"\n");
}
//********************Gauss********End***********************
//***********************************************************
//********Calculate_Unbalanced_Value()*******Start***********
void Calculate_Unbalanced_Value()
{int i,j;
 for(i=0;i<nPQ;i++)//PQ�ڵ㹦�ʵĲ�ƽ����
 {Per[i]=p[i];
  Qer[i]=q[i];
  for(j=0;j<BN;j++)
  {Per[i]-=e[i]*(yr[i][j]*e[j]-yi[i][j]*f[j])+f[i]*(yr[i][j]*f[j]+yi[i][j]*e[j]);
   Qer[i]-=f[i]*(yr[i][j]*e[j]-yi[i][j]*f[j])-e[i]*(yr[i][j]*f[j]+yi[i][j]*e[j]);}
 }
 for(i=nPQ;i<BN-1;i++)//PV�ڵ��й����ʺ͵�ѹ�Ĳ�ƽ����
 {Per[i]=p[i];
  Uer[i]=bus[i][3]*bus[i][3]-e[i]*e[i]-f[i]*f[i];

 for(j=0;j<BN;j++)
  Per[i]-=e[i]*(yr[i][j]*e[j]-yi[i][j]*f[j])+f[i]*(yr[i][j]*f[j]+yi[i][j]*e[j]);
 }
}
//********Calculate_Unbalanced_Value()*******End*************
//***********************************************************
//*************Form_Jacobi_Matric()**********Start***********
void Form_Jacobi_Matric()
{     float H[N][N],NN[N][N],J[N][N],L[N][N],R[N][N],S[N][N];
	  int i,j;
      //i!=j
      for(i=0;i<BN-1;i++)
	  {for(j=0;j<BN-1;j++)
	   if(i!=j)//�ſ˱Ⱦ���H[][],N[][]�ǶԽ�Ԫ�ر�ʾʽ
      {H[i][j]=yi[i][j]*e[i]-yr[i][j]*f[i];
	   NN[i][j]=-yr[i][j]*e[i]-yi[i][j]*f[i];}
	  }
	  for(i=0;i<nPQ;i++)
      {for(j=0;j<BN-1;j++)
	   if(i!=j)//�ſ˱Ⱦ���J[][],L[][]�ǶԽ�Ԫ�ر�ʾʽ
	  {J[i][j]=yi[i][j]*f[i]+yr[i][j]*e[i];
	   L[i][j]=-yr[i][j]*f[i]+yi[i][j]*e[i];}
	  }
	  for(i=0;i<nPV;i++)
	  {for(j=0;j<BN-1;j++)
	   if(i!=j)//�ſ˱Ⱦ���R[][],S[][]�ǶԽ�Ԫ�ر�ʾʽ
	  {R[i][j]=0;
	   S[i][j]=0;}
	  }
	  //i=j
	  for(i=0;i<BN-1;i++) //�ſ˱Ⱦ���H[][],N[][]�Խ�Ԫ�ر�ʾʽ
	  {H[i][i]=-yr[i][i]*f[i]+yi[i][i]*e[i];
	   NN[i][i]=-yr[i][i]*e[i]-yi[i][i]*f[i];
	   for(j=0;j<BN;j++)
	   {H[i][i]-=yr[i][j]*f[j]+yi[i][j]*e[j];
		NN[i][i]-=yr[i][j]*e[j]-yi[i][j]*f[j];}
	  }
      for(i=0;i<nPQ;i++)//�ſ˱Ⱦ���J[][],L[][]�Խ�Ԫ�ر�ʾʽ
      {J[i][i]=yr[i][i]*e[i]+yi[i][i]*f[i];
	   L[i][i]=-yr[i][i]*f[i]+yi[i][i]*e[i];
	   for(j=0;j<BN;j++)
	   {J[i][i]-=yr[i][j]*e[j]-yi[i][j]*f[j];
	    L[i][i]+=yr[i][j]*f[j]+yi[i][j]*e[j];}
	  }
	  for(i=0;i<nPV;i++)//�ſ˱Ⱦ���R[][],S[][]�Խ�Ԫ�ر�ʾʽ
	  {R[i][i]=-2*f[i];
	   S[i][i]=-2*e[i];}
	  //H,N,J,L,R,S�����ſɱȾ���
      for(i=0;i<nPQ;i++)
		  for(j=0;j<BN-1;j++)
		  {jac[i][j]=H[i][j];
		   jac[i+BN-1][j]=J[i][j];
		   jac[i][j+BN-1]=NN[i][j];
		   jac[i+BN-1][j+BN-1]=L[i][j];}
	  for(i=nPQ;i<BN-1;i++)
		  for(j=0;j<BN-1;j++)
		  {jac[i][j]=H[i][j];
		   jac[i+BN-1][j]=R[i-nPQ][j];
		   jac[i][j+BN-1]=NN[i][j];
		   jac[i+BN-1][j+BN-1]=S[i-nPQ][j];}
 fprintf(fp,"�ſ˱Ⱦ���:\n");
 for(i=0;i<2*BN-2;i++)
 {for(j=0;j<2*BN-2;j++)
     fprintf(fp,"%f  ",jac[i][j]);
 fprintf(fp,"\n");}
}
//*************Form_Jacobi_Matric()*********End**************
//***********************************************************
//*************Solve_Equations()***********Start*************
void Solve_Equations()//LU�ֽⷨ������
{int i,j,k;
 float u[N][N],l[N][N];
  for(i=0;i<BN-1;i++)
   x[i]=Per[i];
  for(i=BN-1;i<BN+nPQ-1;i++)
   x[i]=Qer[i-BN+1];
  for(i=BN+nPQ-1;i<2*BN-2;i++)
   x[i]=Uer[i-BN+1];
  for(i=0;i<2*(BN-1);i++)
  {u[0][i]=jac[0][i];
   l[i][0]=jac[i][0]/u[0][0];}
  for(i=1;i<2*(BN-1);i++)
  for(j=i;j<2*(BN-1);j++)
  {u[i][j]=jac[i][j];
   l[j][i]=jac[j][i];
  }
  for(i=1;i<2*(BN-1);i++)
  for(j=i;j<2*(BN-1);j++)
  {for(k=0;k<i;k++)
  {u[i][j]-=l[i][k]*u[k][j];
   l[j][i]-=l[j][k]*u[k][i];}
   l[j][i]=l[j][i]/u[i][i];}
   for(i=0;i<2*(BN-1);i++)
   l[i][i]=1;
//���ux
   for(i=1;i<2*(BN-1);i++)
   for(k=0;k<i;k++)
    x[i]=x[i]-l[i][k]*x[k];
//���x
   x[2*(BN-1)-1]=x[2*(BN-1)-1]/u[2*(BN-1)-1][2*(BN-1)-1];
   for(i=2*(BN-1)-2;i>=0;i--)
   {for(k=i+1;k<2*(BN-1);k++)
    x[i]-=u[i][k]*x[k];
    x[i]=x[i]/u[i][i];}
//��Eer[],Fer[]�е����ֵ
   big=fabs(x[0]);
   for(i=1;i<2*BN-2;i++)
   if(big<fabs(x[i]))
	   big=fabs(x[i]);
//�����µĽڵ��ѹֵ
   for(i=0;i<BN-1;i++)
   {e[i]-=x[i+BN-1];
    f[i]-=x[i];}
   fprintf(fp,"�ڵ��ѹ:\n");
   for(i=0;i<BN;i++)
   {if(f[i]>=0)
   fprintf(fp,"�ڵ�%d����%f+j%f\n",i+1,e[i],f[i]);
   else
	   fprintf(fp,"�ڵ�%d����%f-j%f\n",i+1,e[i],fabs(f[i]));}
}
//*************Solve_Equations()***********End***************
//***********************************************************
//***********Powerflow_Results()***********Start*************
void Powerflow_Results()
{int i,j;
 float z,s;
 float cr[N][N],ci[N][N];
 fprintf(fp,"\n");
 fprintf(fp,"=================���ս��===============\n");
 fprintf(fp,"\n");
 //�ڵ��ѹ
 for(i=0;i<BN;i++)
 {if(f[i]>=0)
 fprintf(fp,"�ڵ�%d�ĵ�ѹ��%f+j%f\n",i+1,e[i],f[i]);
  else
 fprintf(fp,"�ڵ�%d�ĵ�ѹ��%f-j%f\n",i+1,e[i],fabs(f[i]));}
 //ƽ��ڵ㹦��
 z=0;
 s=0;
 for(j=0;j<BN;j++)
 {z+=yr[BN-1][j]*e[j]-yi[BN-1][j]*f[j];
  s+=-yr[BN-1][j]*f[j]-yi[BN-1][j]*e[j];}
  p[BN-1]=e[BN-1]*z-f[BN-1]*s;
  q[BN-1]=z*f[BN-1]+s*e[BN-1];
  fprintf(fp,"\n");
  if(q[BN-1]>=0)
  fprintf(fp,"ƽ�⹦��:%f+j%f\n",p[BN-1],q[BN-1]);
  else
  fprintf(fp,"ƽ�⹦��:%f-j%f\n",p[BN-1],fabs(q[BN-1]));
 //PV�ڵ���޹�����
 for(i=nPQ;i<BN-1;i++)
 {q[i]=0;
  for(j=0;j<BN;j++)
  q[i]+=f[i]*(yr[i][j]*e[j]-yi[i][j]*f[j])-e[i]*(yr[i][j]*f[j]+yi[i][j]*e[j]); 
 }
 //�ڵ�ע�빦��
 for(i=0;i<BN-1;i++)
 {fprintf(fp,"\n");
 if(q[i]>=0)
  fprintf(fp,"�ڵ�%dע�빦��%f+j%f\n",i+1,p[i],q[i]);
 else
  fprintf(fp,"�ڵ�%dע�빦��%f-j%f\n",i+1,p[i],fabs(q[i]));}
 //��֧·����
 for(i=0;i<LM;i++)
 {if(line[i][0]==0||line[i][1]==0)
		 continue;
  else
 {line[i][4]=0;
  line[i][5]=0;}}//�Ե�֧·����
 for(i=0;i<BN;i++)//Ϊ�˽�ʡ�ڴ棬��bus[][]����м����
	 for(j=i+1;j<BN;j++)
	 {z=e[i]-e[j];
	  s=f[j]-f[i];
	  cr[i][j]=e[i]*z-f[i]*s;
	  ci[i][j]=f[i]*z+e[i]*s;
	  z=cr[i][j]*(-yr[i][j])+ci[i][j]*(-yi[i][j]);
	  s=-cr[i][j]*(-yi[i][j])+ci[i][j]*(-yr[i][j]);
	  cr[i][j]=z+(e[i]*e[i]-f[i]*f[i])*line[i][4]+2*e[i]*f[i]*line[i][5];
	  ci[i][j]=s-(e[i]*e[i]-f[i]*f[i])*line[i][5]+2*e[i]*f[i]*line[i][4];
	  z=e[j]-e[i];
	  s=f[i]-f[j];
	  cr[j][i]=e[j]*z-f[j]*s;
	  ci[j][i]=f[j]*z+e[j]*s;
	  z=cr[j][i]*(-yr[j][i])+ci[j][i]*(-yi[j][i]);
	  s=-cr[j][i]*(-yi[j][i])+ci[j][i]*(-yr[j][i]);
	  cr[j][i]=z+(e[j]*e[j]-f[j]*f[j])*line[j][4]+2*e[j]*f[j]*line[j][5];
	  ci[j][i]=s-(e[j]*e[j]-f[j]*f[j])*line[j][5]+2*e[j]*f[j]*line[j][4];

	  if(yr[i][j]!=0&&yi[i][j]!=0)
	  {fprintf(fp,"\n");
	   if(ci[i][j]>=0)
	   fprintf(fp,"֧·%d->%d����ͨ����%f+j%f\n",i+1,j+1,cr[i][j],ci[i][j]);
	   else
	   fprintf(fp,"֧·%d->%d����ͨ����%f-j%f\n",i+1,j+1,cr[i][j],fabs(ci[i][j]));

	   if(ci[j][i]>=0)
	   fprintf(fp,"֧·%d->%d����ͨ����%f+j%f\n",j+1,i+1,cr[j][i],ci[j][i]);
	   else
	   fprintf(fp,"֧·%d->%d����ͨ����%f-j%f\n",j+1,i+1,cr[j][i],fabs(ci[j][i]));}
	 }
	 fprintf(fp,"\n");
     fprintf(fp,"********************************************************************************\n");
	 fprintf(fp,"********************************Part5****�������*******************************\n");
	 fprintf(fp,"\n");
     for(i=0;i<BN;i++)
	 {for(j=i+1;j<BN;j++)
	 { if(yr[i][j]!=0&&yi[i][j]!=0)
	 {if(ci[j][i]+ci[i][j]>=0)
	  fprintf(fp,"֧·%d%d������%f+j%f\n\n",i+1,j+1,cr[i][j]+cr[j][i],ci[i][j]+ci[j][i]);
	  else
	  fprintf(fp,"֧·%d%d������%f-j%f\n\n",i+1,j+1,cr[i][j]+cr[j][i],fabs(ci[i][j]+ci[j][i]));
	  if(fabs(cr[i][j])>fabs(cr[j][i]))
	  fprintf(fp,"��·%d->%dЧ�ʣ�%f%%\n\n",i+1,j+1,fabs(cr[j][i]/cr[i][j])*100);
	  else
	  fprintf(fp,"��·%d->%dЧ�ʣ�%f%%\n\n",j+1,i+1,fabs(cr[i][j]/cr[j][i])*100);}}
	 }


	  

}
//***********Powerflow_Results()***********End***************
