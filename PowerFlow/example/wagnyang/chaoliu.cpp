#include"stdio.h"
#include"math.h"
#include"fstream.h"
//潮流计算源程序
//直角坐标下的计算
//输入文件名：“InputData1.txt”节点参数
//输入文件名：“InputData2.txt”支路参数
//输出文件名：“OutputData.txt”
/*子函数名：
  ReadData();（读原始数据）
  Form_Y();  （形成节点导纳矩阵）
  Gauss();   （高斯赛德尔法计算初值）
  Calculate_Unbalanced_Value();（计算不平衡功率和电压）
  Form_Jacobi_Matric();        （形成雅可比矩阵）
  Solve_Equations();           （解修正方程式）
  Powerflow_Results();         （输出潮流计算结果）*/
  int BN,LM;//BN为节点参数的行数，LM为支路参数的行数 
  int nPQ=0,nPV=0,nSW=0;//nPQ,nPV,nSW分别为各类节点个数;
  const N=100;
  float bus[N][6];//节点参数
  float line[N][7];//支路参数
  float yr[N][N],yi[N][N];//节点导纳矩阵实部和虚部
  float Per[N],Qer[N],Uer[N];//有功功率、无功功率、电压的不平衡量
  float e[N],f[N],p[N],q[N];//存放节点电压,有功和无功功率
  float Eer[N],Fer[N];//存放节点电压实部虚部的不平衡量
  float jac[N][N];//存放雅克比矩阵
  float x[N];//将f,e的不平衡量合成一个数组
  float big;//x[N]中的最大值
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
  printf("请输入精度lim=");
  scanf("%f",&lim);
  fp=fopen("OutputData.txt","w+");
  //调用子函数，并将数据结果写入到文件中
  ReadData();//读数据
  Form_Y();//形成节点导纳矩阵
  //高斯赛德尔法迭代两次计算初值
  Gauss();
  //牛拉法迭代至符合要求的精度
  do
  {k++;
   fprintf(fp,"===============第%d次迭代结果=============\n",k);
   Calculate_Unbalanced_Value();  //计算不平衡量
   Form_Jacobi_Matric();  //形成雅克比矩阵
   Solve_Equations();  //求解非线性方程
  }while(big>lim);  //lim为精度        
  Powerflow_Results();  //输出结果
}
//******************ReadData()***Start***********************
//读原始数据,并将节点、支路参数以PQ,PV,平衡节点的顺序重新排列
void ReadData()
{ float PQ[N][6],PV[N][6],SW[N][6];//PQ、PV、平衡节点参数分别存放
  int i,j,k;
  FILE *fp1,*fp2;
  fp1=fopen("InputData1.txt","r+");
  fp2=fopen("InputData2.txt","r+");
//获取节点总数BN
  ifstream in1("InputData1.txt");
  char str1[N];//1000是定义的常数，目的是为了读取足够长的行
  BN=0;
  while(!in1.eof())//eof()判断是否读到文件终止
  { 
	  in1.getline(str1, sizeof(str1));
	  BN++;		  
  }//确定节点总数BN
//获取支路总数LM
  ifstream in2("InputData2.txt");
  char str2[N];//1000是定义的常数，目的是为了读取足够长的行
  LM=0;
  while(!in2.eof())//eof()判断是否读到文件终止
  { 
	  in2.getline(str2, sizeof(str2));
	  LM++;		  
  }//确定支路个数LM
  for(i=0;i<BN;i++)
	  for(j=0;j<6;j++)
	   fscanf(fp1,"%f",&bus[i][j]);
  for(i=0;i<LM;i++)
	  for(j=0;j<7;j++)
	   fscanf(fp2,"%f",&line[i][j]);//从文件赋值到变量中
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
	  nSW++;}//PQ[][]PV[][]SW[][]分别存放PQ、PV、SW节点参数
  for(i=0;i<nPQ;i++)
	  for(j=0;j<6;j++)
		  bus[i][j]=PQ[i][j];
  for(i=nPQ;i<nPQ+nPV;i++)
	  for(j=0;j<6;j++)
		  bus[i][j]=PV[i-nPQ][j];
  for(i=nPQ+nPV;i<nPQ+nPV+nSW;i++)
	  for(j=0;j<6;j++)
		  bus[i][j]=SW[i-nPQ-nPV][j];//按PQ、PV、SW节点顺序进行重排列
  for(i=0;i<LM;i++)
      for(j=0;j<2;j++)
          for(k=0;k<BN;k++)
              if(line[i][j]==bus[k][0])
			  {line[i][j]=k+1;
			  break;}//改变支路参数中相应的节点序号
  for(i=0;i<BN;i++)
	  bus[i][0]=i+1; //节点重编号
  fprintf(fp,"**************************************潮流计算**********************************\n");
  fprintf(fp,"————班级：电力实10——————作者：王洋—————学号：201001001316————\n");
  fprintf(fp,"———————————日期：2012-01—————地点：教九六楼D区—————————\n");
  fprintf(fp,"\n");
  fprintf(fp,"                        ****************目录***************\n");
  fprintf(fp,"                        Part1.原始数据*********************\n");
  fprintf(fp,"                        Part2.节点导纳矩阵******************\n");
  fprintf(fp,"                        Part3.第一、二次高斯赛德尔迭代结果**\n");
  fprintf(fp,"                        Part4.牛拉法迭代结果****************\n");
  fprintf(fp,"                        Part5.结果分析**********************\n");
  fprintf(fp,"\n");
  fprintf(fp,"********************************Part1****原始数据*******************************\n");
  fprintf(fp,"节点总数:%d    支路总数:%d\n",BN,LM);
  fprintf(fp,"节点数据——节点序号     有功功率   无功功率   电压幅值  电压相角   类型\n");
  fprintf(fp,"节点参数:\n");
  for(i=0;i<BN;i++)
  {for(j=0;j<6;j++)
  {if(j==0||j==5)
	fprintf(fp,"%d     ",int(bus[i][j]));
  else fprintf(fp,"%f     ",bus[i][j]);}
  fprintf(fp,"\n");}
  fprintf(fp,"================================================================================\n");
  fprintf(fp,"支路数据——节点序号   节点序号     电阻   电抗   电导   电纳   非标变比\n");
  fprintf(fp,"支路参数:\n");
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
	  {yr[i][j]=0;yi[i][j]=0;}//节点导纳矩阵赋初值0
  for(i=0;i<LM;i++)
	  {m=int(line[i][0]-1);n=int(line[i][1]-1);//m,n分别为节点号减1.以对应数组编号
      if(line[i][0]==0)//对地支路
	  {yr[n][n]+=line[i][4];
	  yi[n][n]+=line[i][5];}
	  else if(line[i][1]==0)//对地支路
	  {yr[m][m]+=line[i][4];
	  yi[m][m]+=line[i][5];}

	  else if(line[i][6]==0)      //普通线路，即不含变压器的线路
//line[i][2]为R，line[i][3]为X，line[i][4]为G，line[i][5]为B
  {yr[m][n]+=-line[i][2]/(line[i][2]*line[i][2]+line[i][3]*line[i][3]);
   yi[m][n]+=line[i][3]/(line[i][2]*line[i][2]+line[i][3]*line[i][3]);
   yr[m][m]+=-yr[m][n];
   yi[m][m]+=-yi[m][n]+line[i][5];
   yr[n][n]+=-yr[m][n];
   yi[n][n]+=-yi[m][n]+line[i][5];
   yr[n][m]=yr[m][n];
   yi[n][m]=yi[m][n];}
       else if(line[i][6]>0) //含变压器线路，线路参数由i侧归算到j侧，k在j侧，理想电压比为k=line[i][6]
	   {yr[m][n]+=-line[i][2]/(line[i][2]*line[i][2]+line[i][3]*line[i][3])/line[i][6];
        yi[m][n]+=line[i][3]/(line[i][2]*line[i][2]+line[i][3]*line[i][3])/line[i][6];
        yr[m][m]+=-yr[m][n]*(line[i][6]-1)/line[i][6];
        yi[m][m]+=-yi[m][n]*(line[i][6]-1)/line[i][6];
        yr[n][n]+=-yr[m][n]*(1-line[i][6])/(line[i][6]*line[i][6]);
        yi[n][n]+=-yi[m][n]*(1-line[i][6])/(line[i][6]*line[i][6]);
	    yr[n][m]=yr[m][n];
        yi[n][m]=yi[m][n];}
	   else if(line[i][6]<0) //含变压器线路，线路参数由j侧归算到i侧，k在i侧，理想电压比为k=line[i][6]
	   {yr[m][n]+=-line[i][2]/(line[i][2]*line[i][2]+line[i][3]*line[i][3])/(-line[i][6]);
        yi[m][n]+=line[i][3]/(line[i][2]*line[i][2]+line[i][3]*line[i][3])/(-line[i][6]);
        yr[m][m]+=-yr[m][n]*(1+line[i][6])/(line[i][6]*line[i][6]);
        yi[m][m]+=-yi[m][n]*(1+line[i][6])/(line[i][6]*line[i][6]);
        yr[n][n]+=-yr[m][n]*(-line[i][6]-1)/(-line[i][6]);
        yi[n][n]+=-yi[m][n]*(-line[i][6]-1)/(-line[i][6]);
	    yr[n][m]=yr[m][n];
        yi[n][m]=yi[m][n];}
  }
  fprintf(fp,"********************************Part2****节点导纳矩阵***************************\n");
 for(i=0;i<BN;i++)
 {for(j=0;j<BN;j++)
 {if(yi[i][j]>=0)
	fprintf(fp,"%f+j%f    ",yr[i][j],yi[i][j]);
 else
	fprintf(fp,"%f-j%f    ",yr[i][j],fabs(yi[i][j]));}
 fprintf(fp,"\n");}
 fprintf(fp,"\n");
 fprintf(fp,"********************************************************************************\n");
 fprintf(fp,"*********************************Part3****高斯赛德尔法**************************\n");
}
//******************Form_Y()*****End*************************
//***********************************************************
//******************Gauss()******Start***********************
//采用高斯赛德尔法求取初值（不平衡功率和电压）
void Gauss()
{int i,j,k;
for(k=0;k<2;k++)
{fprintf(fp,"===============第%d次迭代结果=============\n",k+1);
float cr[N],ci[N],dr[N],di[N];//存放过程变量
//PQ节点的计算
  for(i=0;i<BN;i++)
  {//注:PV节点电压相角初始为0
   //注:PQ节点电压幅值初始为1，相角为0
  e[i]=bus[i][3]*cos(bus[i][4]);//节点电压实部
  f[i]=bus[i][3]*sin(bus[i][4]); //节点电压虚部
  p[i]=bus[i][1]; //节点有功功率
  q[i]=bus[i][2];}//节点无功功率
  for(i=0;i<nPQ;i++)//cr[]ci[]dr[]di[]为过程中间变量，无实际意义
  {cr[i]=yr[i][i]/(yr[i][i]*yr[i][i]+yi[i][i]*yi[i][i]);
   ci[i]=-yi[i][i]/(yr[i][i]*yr[i][i]+yi[i][i]*yi[i][i]);
   dr[i]=(p[i]*e[i]+q[i]*f[i])/(e[i]*e[i]+f[i]*f[i]);
   di[i]=(p[i]*f[i]-q[i]*e[i])/(e[i]*e[i]+f[i]*f[i]);
   for(j=0;j<BN;j++)
   {if(j==i)
    continue;//不含Y(i,i)*U(i)项
   dr[i]-=yr[i][j]*e[j]-yi[i][j]*f[j];
   di[i]-=yr[i][j]*f[j]+yi[i][j]*e[j];}
   e[i]=cr[i]*dr[i]-ci[i]*di[i];
   f[i]=ci[i]*dr[i]+cr[i]*di[i];
   bus[i][3]=sqrt(e[i]*e[i]+f[i]*f[i]);//PQ节点幅值
   bus[i][4]=atan(f[i]/e[i]);}//PQ节点相角
//PV节点的计算
  for(i=nPQ;i<BN-1;i++)
  {cr[i]=0;ci[i]=0;
  for(j=0;j<BN;j++)
  {cr[i]+=yr[i][j]*e[j]-yi[i][j]*f[j];
   ci[i]+=yr[i][j]*f[j]+yi[i][j]*e[j];}
   q[i]=cr[i]*f[i]-ci[i]*e[i];//由节点电压方程得到PV节点的无功功率近似值
   cr[i]+=-yr[i][i]*e[i]+yi[i][i]*f[i];
   cr[i]=-cr[i]+(p[i]*e[i]+q[i]*f[i])/(e[i]*e[i]+f[i]*f[i]);
   ci[i]+=-yr[i][i]*f[i]-yi[i][i]*e[i];
   ci[i]=-ci[i]+(p[i]*f[i]-q[i]*e[i])/(e[i]*e[i]+f[i]*f[i]);
   dr[i]=yr[i][i]/(yr[i][i]*yr[i][i]+yi[i][i]*yi[i][i]);
   di[i]=-yi[i][i]/(yr[i][i]*yr[i][i]+yi[i][i]*yi[i][i]);//同前，cr、ci、dr、di为中间变量
   e[i]=dr[i]*cr[i]-di[i]*ci[i];
   f[i]=di[i]*cr[i]+dr[i]*ci[i];
   bus[i][4]=atan(f[i]/e[i]);//计算PV节点的电压相角
   e[i]=bus[i][3]*cos(bus[i][4]);
   f[i]=bus[i][3]*sin(bus[i][4]);
  }
fprintf(fp,"PQ节点电压:\n");
for(i=0;i<nPQ;i++)
{if(f[i]>=0)
     fprintf(fp,"%d——%f+j%f\n",i+1,e[i],f[i]);
 else 
	 fprintf(fp,"%d——%f-j%f\n",i+1,e[i],fabs(f[i]));}
if(nPV!=0)
{fprintf(fp,"PV节点电压:\n");
for(i=0;i<nPV;i++)
{if(f[i+nPQ]>=0)
fprintf(fp,"%d——%f+j%f\n",i+1+nPQ,e[i+nPQ],f[i+nPQ]);
else
fprintf(fp,"%d——%f-j%f\n",i+1+nPQ,e[i+nPQ],fabs(f[i+nPQ]));}}
else fprintf(fp,"不存在PV节点\n");
}
 fprintf(fp,"\n");
 fprintf(fp,"********************************************************************************\n");
 fprintf(fp,"*********************************Part4****牛顿拉夫逊法**************************\n");
 fprintf(fp,"\n");
 fprintf(fp,"                       ****************雅克比矩阵************\n");
 fprintf(fp,"                       **************按照*H=N****************\n");
 fprintf(fp,"                       *******************J=L****************\n");
 fprintf(fp,"                       *******************R=S**排列**********\n");
 fprintf(fp,"                       **************************************\n");
 fprintf(fp,"\n");
}
//********************Gauss********End***********************
//***********************************************************
//********Calculate_Unbalanced_Value()*******Start***********
void Calculate_Unbalanced_Value()
{int i,j;
 for(i=0;i<nPQ;i++)//PQ节点功率的不平衡量
 {Per[i]=p[i];
  Qer[i]=q[i];
  for(j=0;j<BN;j++)
  {Per[i]-=e[i]*(yr[i][j]*e[j]-yi[i][j]*f[j])+f[i]*(yr[i][j]*f[j]+yi[i][j]*e[j]);
   Qer[i]-=f[i]*(yr[i][j]*e[j]-yi[i][j]*f[j])-e[i]*(yr[i][j]*f[j]+yi[i][j]*e[j]);}
 }
 for(i=nPQ;i<BN-1;i++)//PV节点有功功率和电压的不平衡量
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
	   if(i!=j)//雅克比矩阵H[][],N[][]非对角元素表示式
      {H[i][j]=yi[i][j]*e[i]-yr[i][j]*f[i];
	   NN[i][j]=-yr[i][j]*e[i]-yi[i][j]*f[i];}
	  }
	  for(i=0;i<nPQ;i++)
      {for(j=0;j<BN-1;j++)
	   if(i!=j)//雅克比矩阵J[][],L[][]非对角元素表示式
	  {J[i][j]=yi[i][j]*f[i]+yr[i][j]*e[i];
	   L[i][j]=-yr[i][j]*f[i]+yi[i][j]*e[i];}
	  }
	  for(i=0;i<nPV;i++)
	  {for(j=0;j<BN-1;j++)
	   if(i!=j)//雅克比矩阵R[][],S[][]非对角元素表示式
	  {R[i][j]=0;
	   S[i][j]=0;}
	  }
	  //i=j
	  for(i=0;i<BN-1;i++) //雅克比矩阵H[][],N[][]对角元素表示式
	  {H[i][i]=-yr[i][i]*f[i]+yi[i][i]*e[i];
	   NN[i][i]=-yr[i][i]*e[i]-yi[i][i]*f[i];
	   for(j=0;j<BN;j++)
	   {H[i][i]-=yr[i][j]*f[j]+yi[i][j]*e[j];
		NN[i][i]-=yr[i][j]*e[j]-yi[i][j]*f[j];}
	  }
      for(i=0;i<nPQ;i++)//雅克比矩阵J[][],L[][]对角元素表示式
      {J[i][i]=yr[i][i]*e[i]+yi[i][i]*f[i];
	   L[i][i]=-yr[i][i]*f[i]+yi[i][i]*e[i];
	   for(j=0;j<BN;j++)
	   {J[i][i]-=yr[i][j]*e[j]-yi[i][j]*f[j];
	    L[i][i]+=yr[i][j]*f[j]+yi[i][j]*e[j];}
	  }
	  for(i=0;i<nPV;i++)//雅克比矩阵R[][],S[][]对角元素表示式
	  {R[i][i]=-2*f[i];
	   S[i][i]=-2*e[i];}
	  //H,N,J,L,R,S构成雅可比矩阵
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
 fprintf(fp,"雅克比矩阵:\n");
 for(i=0;i<2*BN-2;i++)
 {for(j=0;j<2*BN-2;j++)
     fprintf(fp,"%f  ",jac[i][j]);
 fprintf(fp,"\n");}
}
//*************Form_Jacobi_Matric()*********End**************
//***********************************************************
//*************Solve_Equations()***********Start*************
void Solve_Equations()//LU分解法求解矩阵
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
//求解ux
   for(i=1;i<2*(BN-1);i++)
   for(k=0;k<i;k++)
    x[i]=x[i]-l[i][k]*x[k];
//求解x
   x[2*(BN-1)-1]=x[2*(BN-1)-1]/u[2*(BN-1)-1][2*(BN-1)-1];
   for(i=2*(BN-1)-2;i>=0;i--)
   {for(k=i+1;k<2*(BN-1);k++)
    x[i]-=u[i][k]*x[k];
    x[i]=x[i]/u[i][i];}
//求Eer[],Fer[]中的最大值
   big=fabs(x[0]);
   for(i=1;i<2*BN-2;i++)
   if(big<fabs(x[i]))
	   big=fabs(x[i]);
//计算新的节点电压值
   for(i=0;i<BN-1;i++)
   {e[i]-=x[i+BN-1];
    f[i]-=x[i];}
   fprintf(fp,"节点电压:\n");
   for(i=0;i<BN;i++)
   {if(f[i]>=0)
   fprintf(fp,"节点%d——%f+j%f\n",i+1,e[i],f[i]);
   else
	   fprintf(fp,"节点%d——%f-j%f\n",i+1,e[i],fabs(f[i]));}
}
//*************Solve_Equations()***********End***************
//***********************************************************
//***********Powerflow_Results()***********Start*************
void Powerflow_Results()
{int i,j;
 float z,s;
 float cr[N][N],ci[N][N];
 fprintf(fp,"\n");
 fprintf(fp,"=================最终结果===============\n");
 fprintf(fp,"\n");
 //节点电压
 for(i=0;i<BN;i++)
 {if(f[i]>=0)
 fprintf(fp,"节点%d的电压：%f+j%f\n",i+1,e[i],f[i]);
  else
 fprintf(fp,"节点%d的电压：%f-j%f\n",i+1,e[i],fabs(f[i]));}
 //平衡节点功率
 z=0;
 s=0;
 for(j=0;j<BN;j++)
 {z+=yr[BN-1][j]*e[j]-yi[BN-1][j]*f[j];
  s+=-yr[BN-1][j]*f[j]-yi[BN-1][j]*e[j];}
  p[BN-1]=e[BN-1]*z-f[BN-1]*s;
  q[BN-1]=z*f[BN-1]+s*e[BN-1];
  fprintf(fp,"\n");
  if(q[BN-1]>=0)
  fprintf(fp,"平衡功率:%f+j%f\n",p[BN-1],q[BN-1]);
  else
  fprintf(fp,"平衡功率:%f-j%f\n",p[BN-1],fabs(q[BN-1]));
 //PV节点的无功功率
 for(i=nPQ;i<BN-1;i++)
 {q[i]=0;
  for(j=0;j<BN;j++)
  q[i]+=f[i]*(yr[i][j]*e[j]-yi[i][j]*f[j])-e[i]*(yr[i][j]*f[j]+yi[i][j]*e[j]); 
 }
 //节点注入功率
 for(i=0;i<BN-1;i++)
 {fprintf(fp,"\n");
 if(q[i]>=0)
  fprintf(fp,"节点%d注入功率%f+j%f\n",i+1,p[i],q[i]);
 else
  fprintf(fp,"节点%d注入功率%f-j%f\n",i+1,p[i],fabs(q[i]));}
 //各支路功率
 for(i=0;i<LM;i++)
 {if(line[i][0]==0||line[i][1]==0)
		 continue;
  else
 {line[i][4]=0;
  line[i][5]=0;}}//对地支路导纳
 for(i=0;i<BN;i++)//为了节省内存，用bus[][]存放中间变量
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
	   fprintf(fp,"支路%d->%d的流通功率%f+j%f\n",i+1,j+1,cr[i][j],ci[i][j]);
	   else
	   fprintf(fp,"支路%d->%d的流通功率%f-j%f\n",i+1,j+1,cr[i][j],fabs(ci[i][j]));

	   if(ci[j][i]>=0)
	   fprintf(fp,"支路%d->%d的流通功率%f+j%f\n",j+1,i+1,cr[j][i],ci[j][i]);
	   else
	   fprintf(fp,"支路%d->%d的流通功率%f-j%f\n",j+1,i+1,cr[j][i],fabs(ci[j][i]));}
	 }
	 fprintf(fp,"\n");
     fprintf(fp,"********************************************************************************\n");
	 fprintf(fp,"********************************Part5****结果分析*******************************\n");
	 fprintf(fp,"\n");
     for(i=0;i<BN;i++)
	 {for(j=i+1;j<BN;j++)
	 { if(yr[i][j]!=0&&yi[i][j]!=0)
	 {if(ci[j][i]+ci[i][j]>=0)
	  fprintf(fp,"支路%d%d的网损%f+j%f\n\n",i+1,j+1,cr[i][j]+cr[j][i],ci[i][j]+ci[j][i]);
	  else
	  fprintf(fp,"支路%d%d的网损%f-j%f\n\n",i+1,j+1,cr[i][j]+cr[j][i],fabs(ci[i][j]+ci[j][i]));
	  if(fabs(cr[i][j])>fabs(cr[j][i]))
	  fprintf(fp,"线路%d->%d效率：%f%%\n\n",i+1,j+1,fabs(cr[j][i]/cr[i][j])*100);
	  else
	  fprintf(fp,"线路%d->%d效率：%f%%\n\n",j+1,i+1,fabs(cr[i][j]/cr[j][i])*100);}}
	 }


	  

}
//***********Powerflow_Results()***********End***************
