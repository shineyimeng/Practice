#include<stdio.h>      
#include<math.h>
#define M 100          //M定义了矩阵的阶数
#define W 2            //W为高斯——塞德尔迭代次数，为了使牛顿—拉夫逊法初值适当，应取合适的W值
#define N 100          //N为迭代次数
struct jd                                  //定义一个节点的结构体，方便存储节点信息
      {  int num,s;                        // num为节点号，s为节点类型
	     double p,q,e,f,U,dp,dq,du,de,df;  //依次为节点有功、无功功率，电压纵、横分量，电压模值,牛顿——拉夫逊法中的功率不平衡量、电压不平衡量*/
      } jd[M];//定义节点矩阵

struct zhilu                              //定义一个支路的结构体，方便存储支路信息
      {  int s;                           //s为支路类型
	     int num;                         //num为支路号
	     int p1,p2;                       //支路的两个节点
	     double r,x;                      //支路的电阻r与电抗x
		 double	 rr,xx;                   //并联支路电阻rr与电抗xx
		 double	 k;                       //非标准变比支路变比k
		 double	 r01,x01,r02,x02;         //对地支路对地电阻r01,r02与电抗x01,x02
		 double  pl,ql;                   //各支路有功损耗、无功损耗
		 double  p;                       //各支路有功流入功率
		 double  eff;                     //各支路效率
      } zhilu[M];//定义支路矩阵
int i,j,k,a,b,c,ii,jj;              //循环控制变量 
int t,l,g;                          //中间变量         
int n,                      // 节点数
    m,                      //支路数 
    pq,                     //PQ节点数
    pv;                     //PV节点数 
    
    
double eps;                                    // 精度 
double aa[M],bb[M],cc[M],dd[M],max,temp, rr,tt,bianbi,c3,d3,c4,d4;                //中间变量         
double mo,c1,d1,c2,d2;                         //复数运算函数的返回值   
double G[M][M],B[M][M],Y[M][M];                //节点导纳矩阵中的实部、虚部及其模方值
double ykb[M][M],D[M],dU[M];                   //雅克比矩阵、不平衡量矩阵

	     
FILE *fp1,*fp2;
void introduction()//程序简介
{
	if((fp2=fopen("OutputData.txt","w"))==NULL)     
   { 
	   printf("  can not open file!\n");
       
   }
	
	fprintf(fp2,"                            ********程序简介********\n");
	fprintf(fp2,"   本程序可以实现简单的潮流计算。可以处理包含PQ节点、PV节点和平衡节点在内的网络，其中网络平衡节点只有一个.\n");
    fprintf(fp2,"   其主要步骤是：首先，使用高斯赛德尔迭代求初值，进而使用牛顿拉夫逊迭代到要求精度为止。本程序最大的优点是:\n");
    fprintf(fp2,"   清晰明朗，模块化,分别将不同的块功能变成子函数。有高斯赛德尔子函数、数据输入输出子函数、不平衡量子函数、\n");
    fprintf(fp2,"   雅克比矩阵形成子函数、线性方程组求解子函数、功率计算子函数、结果分析子函数。经检验，该程序精度较高。\n");
    fprintf(fp2,"   本程序考虑到了对地支路，可处理非标变比并联支路，通过修改导纳矩阵，从而使程序可以计算非标准变压器网络的潮流。\n");
	fprintf(fp2,"   本程序通过文件的输入输出，使得输出结果清晰，美观。\n");
	
}
/*####################################从文件中读取原始数据函数###################################*/
void ReadData()                           //读取数据
 { 
	int h,number;                     //定义变量number存放循环中读入的节点号,定义变量h存放循环中读入的节点类型
    fp1=fopen("InputData.txt","r");//读取格式打开输入文件
	if(fp1==NULL)
    { 
		printf(" 输入文件读取失败 !\n");
        
    }
	else//运行过程中显示
		printf("潮流计算正在运行中，请稍等...\n");//美化格式

/*################################################读取信息#######################################*/
    fscanf(fp1,"%d,%d,%d,%d,%lf\n",&n,&m,&pq,&pv,&eps);              //输入节点数,支路数,PQ节点数,PV节点数和精度
	
    j=1;k=pq+1;                   /*在节点结构体中先存储PQ节点,再存储PV节点,
	最后一个为平衡节点变量j控制结构体中PQ节点的存储位置,变量k控制结构体中PV节点的存储位置*/

	/*------------------------------------读取节点信息--------------------------------------*/
    for(i=1;i<=n;i++)                                                //输入节点编号、类型、输入功率和电压初值
    {
		fscanf(fp1,"%d,%d",&number,&h);//读取节点序号和类型
	    if(h==1)                                                     //类型h=1是PQ节点
		{ 
			fscanf(fp1,",%lf,%lf,%lf,%lf\n",&jd[j].p,&jd[j].q,&jd[j].e,&jd[j].f);/*输入PQ节点有功功率、
			无功功率，电压纵、横分量*/
	        jd[j].num=number;//判断该节点为PQ节点后，将信息转移到PQ节点存储位置
	        jd[j].s=h;
	        j++;
		}
	    if(h==2)                                                    //类型h=2是pv节点
		{
			fscanf(fp1,",%lf,%lf,%lf\n",&jd[k].p,&jd[k].U,&jd[k].q);/*输入PV节点有功功率、
			电压,给定一个无功功率，且该无功功率数值上一般为有功功率的一半*/
	        jd[k].num=number;//判断该节点为PV节点后，将信息转移到PV节点存储位置
	        jd[k].s=h;
	        k++;
		}
	   if(h==3)                                                    //类型h=3是平衡节点
	   {
		   fscanf(fp1,",%lf,%lf\n",&jd[n].e,&jd[n].f);/*输入平衡节点电压纵、横分量*/
	       jd[n].num=number;//判断该节点为平衡节点后，将信息转移到平衡节点存储位置
	       jd[n].s=h;
	   }
	}
	/*------------------------------------读取支路信息--------------------------------------*/
   for(i=1;i<=m;i++) //输入支路阻抗
   {
	   fscanf(fp1,"%d,%d",&number,&h);//输入支路序号，类型
	    if(h==1)                                                     /*类型h=1是普通支路*/
		{ 
			fscanf(fp1,",%d,%d,%lf,%lf\n",&zhilu[i].p1,&zhilu[i].p2,&zhilu[i].r,&zhilu[i].x);
			/*输入支路节点号p1、p2,支路的电阻r与电抗x*/
	        zhilu[i].num=number;
	        zhilu[i].s=h;
	      
		}
	    if(h==0)                                                    /*类型h=0是有对地阻抗的支路*/
		{
			fscanf(fp1,",%d,%d,%lf,%lf,%lf,%lf,%lf,%lf\n",&zhilu[i].p1,&zhilu[i].p2,&zhilu[i].r,&zhilu[i].x,&zhilu[i].r01,&zhilu[i].x01,&zhilu[i].r02,&zhilu[i].x02);
			/*输入支路节点号p1、p2,支路的电阻r与电抗x,对地支路对地电阻r01,r02与电抗x01,x02*/
	        zhilu[i].num=number;
	        zhilu[i].s=h;
	    
		}
	   if(h==2)                                                    /*类型h=2是非标准变比支路*/
	   {
		   fscanf(fp1,",%d,%d,%lf,%lf,%lf\n",&zhilu[i].p1,&zhilu[i].p2,&zhilu[i].r,&zhilu[i].x,&zhilu[i].k);
	        /*输入支路节点号p1、p2,支路的电阻r与电抗x,非标准变比支路变比k*/
		   zhilu[i].num=number;
	        zhilu[i].s=h;
	     
	   }
	    if(h==3)                                                    /*类型h=3是并联支路*/
	   {
		   fscanf(fp1,",%d,%d,%lf,%lf,%lf,%lf\n",&zhilu[i].p1,&zhilu[i].p2,&zhilu[i].r,&zhilu[i].x,&zhilu[i].rr,&zhilu[i].xx);
	        /*输入支路节点号p1、p2,支路的电阻r与电抗x,并联支路电阻rr与电抗xx*/
		   zhilu[i].num=number;
	        zhilu[i].s=h;

	   }
	}

	/*##################################输出原始信息########################################*/   
   
  /*输入已知数据到文件OutputData.txt中*/
  
  fprintf(fp2,"\n     潮流上机  华北电力大学 电力1007  姚云飞   201001001321\n");
  fprintf(fp2,"\n                         ##############原始数据 #############\n");
  fprintf(fp2,"  ==============================================================================\n");
  fprintf(fp2,"     节点数:%d 支路数:%d  PQ节点数:%d  PV节点数:%d    精度:%f\n",
	  								 n,m,pq,pv,eps);
  fprintf(fp2,"  ------------------------------------------------------------------------------\n");
  for(i=1;i<=pq;i++)
  fprintf(fp2,"     PQ节点:      节点%d       P[%d]=%f    Q[%d]=%f\n",
	       jd[i].num,jd[i].num,jd[i].p,jd[i].num,jd[i].q);
  for(i=pq+1;i<=pq+pv;i++)
  fprintf(fp2,"     PV节点:      节点%d       P[%d]=%f     U[%d]=%f    初值Q[%d]=%f\n",
	       jd[i].num,jd[i].num,jd[i].p,jd[i].num,jd[i].U,jd[i].num,jd[i].q);
  fprintf(fp2,"     平衡节点:    节点%d       e[%d]=%f     f[%d]=%f\n",
	       jd[n].num,jd[n].num,jd[n].e,jd[n].num,jd[n].f);
  fprintf(fp2,"  -------------------------------------------------------------------------------\n");
  
  for(i=1;i<=m;i++)
  {
	  if(zhilu[i].s==1)
    
        fprintf(fp2,"     普通支路%d:     相关节点:从%d到%d   R=%f  X=%f\n",
		      zhilu[i].num,zhilu[i].p1,zhilu[i].p2,zhilu[i].r,zhilu[i].x);

      if(zhilu[i].s==0)
        fprintf(fp2,"     存在对地导纳的支路%d:     相关节点:从%d到%d   R=%f  X=%f  %d节点对地电阻R01=%f  对地电抗X01=%f   %d节点对地电阻R02=%f  对地电抗X02=%f\n",
		      zhilu[i].num,zhilu[i].p1,zhilu[i].p2,zhilu[i].r,zhilu[i].x,zhilu[i].p1,zhilu[i].r01,zhilu[i].x01,zhilu[i].p2,zhilu[i].r02,zhilu[i].x02);
      
	  if(zhilu[i].s==2)
        fprintf(fp2,"     非标准变比支路%d:     相关节点:从%d到%d   RT=%f  XT=%f  变比为%f\n",
		      zhilu[i].num,zhilu[i].p1,zhilu[i].p2,zhilu[i].r,zhilu[i].x,zhilu[i].k);
       
	  if(zhilu[i].s==3)
        fprintf(fp2,"     并联支路%d:     相关节点:从%d到%d   R=%f  X=%f  并联支路电阻R=%f  并联支路电抗X=%f\n",
		      zhilu[i].num,zhilu[i].p1,zhilu[i].p2,zhilu[i].r,zhilu[i].x,zhilu[i].rr,zhilu[i].xx);

 
} 
   fprintf(fp2,"  ==============================================================================\n");
  }
/*输入原始数据完毕*/
/*######################################复数运算函数####################################*/
 void mozhi(double a0,double b0)                      //复数求模值函数
 { 	 mo=sqrt(a0*a0+b0*b0);
 } 
 void ji(double a1,double b1,double a2,double b2)     //复数求积函数
 {   c1=a1*a2-b1*b2;
	 d1=a1*b2+a2*b1;
 }
 void shang(double a3,double b3,double a4,double b4)  //复数求商函数
 {   c2=(a3*a4+b3*b4)/(a4*a4+b4*b4);
     d2=(a4*b3-a3*b4)/(a4*a4+b4*b4);
 }
 

 /*####################################计算节点导纳矩阵###############################*/
 void Form_Y()                    //形成节点导纳矩阵 
 {
for(i=1;i<=n;i++)//先将节点导纳矩阵的所有用到的元素置零
{
    G[i][j]=0;//实部置零
    B[i][j]=0;//虚部置零
}
  double S,S1,S2;
  //开始求互导纳，自导纳
  for(i=1;i<=m;i++)               //节点导纳矩阵的主对角线上的导纳
      {
	 jj=zhilu[i].p1;
	 ii=zhilu[i].p2;
	 S=zhilu[i].r*zhilu[i].r+zhilu[i].x*zhilu[i].x;
	 if(S==0)  continue;
	     G[jj][jj]+=zhilu[i].r/S;
	     B[jj][jj]+=-zhilu[i].x/S;
		 G[ii][ii]+=zhilu[i].r/S;
	     B[ii][ii]+=-zhilu[i].x/S;
		 }

  for(k=1;k<=m;k++)              //节点导纳矩阵非主对角线上的导纳
      { i=zhilu[k].p1;
	j=zhilu[k].p2;
	S=zhilu[k].r*zhilu[k].r+zhilu[k].x*zhilu[k].x;
	if(S==0)  continue;
	G[i][j]+=-zhilu[k].r/S;
	B[i][j]+=zhilu[k].x/S;
	G[j][i]=G[i][j];
	B[j][i]=B[i][j];
	}
  /*------------------------------------对非普通支路修改节点导纳矩阵--------------------------------------*/
for(i=1;i<=m;i++)   
{
    jj=zhilu[i].p1;
	 ii=zhilu[i].p2;
	 if(zhilu[i].s==0)    //该支路有对地支路,对地导纳一般不考虑电导，只考虑电纳，即只有虚部
	 {
          S1=zhilu[i].r01*zhilu[i].r01+zhilu[i].x01*zhilu[i].x01;
          if(S1!=0)
		  {
             G[jj][jj]+=zhilu[i].r01/S1;
	         B[jj][jj]+=-zhilu[i].x01/S1;
		  }
		  S2=zhilu[i].r02*zhilu[i].r02+zhilu[i].x02*zhilu[i].x02;
          if(S2!=0)
		  {
             G[ii][ii]+=zhilu[i].r02/S2;
	         B[ii][ii]+=-zhilu[i].x02/S2;
		  }
	 }
if(zhilu[i].s==2)    //该支路为非标准变比支路,原始变比为1
{//如果i条支路和j节点有关，那么就在j的自导纳上累加上i支路的导纳值
	bianbi=zhilu[i].k;
	 S=zhilu[i].r*zhilu[i].r+zhilu[i].x*zhilu[i].x;
	 if(S==0)  continue;
	     G[jj][jj]+=((1/(bianbi*bianbi))-1)*zhilu[i].r/S;
	     B[jj][jj]+=-((1/(bianbi*bianbi))-1)*zhilu[i].x/S;
		 G[jj][ii]+=-((1/bianbi)-1)*zhilu[i].r/S;
	     B[jj][ii]+=((1/bianbi)-1)*zhilu[i].x/S;
         G[ii][jj]=G[jj][ii];
	     B[ii][jj]=B[jj][ii];
}
if(zhilu[i].s==3)    //该支路为并联支路
{
  S=zhilu[i].rr*zhilu[i].rr+zhilu[i].xx*zhilu[i].xx;
	 if(S==0)  continue;
         G[jj][jj]+=zhilu[i].rr/S;
	     B[jj][jj]+=-zhilu[i].xx/S;
		 G[ii][ii]+=zhilu[i].rr/S;
	     B[ii][ii]+=-zhilu[i].xx/S;
         G[jj][ii]+=-zhilu[i].rr/S;
	     B[jj][ii]+=zhilu[i].xx/S;
         G[ii][jj]=G[jj][ii];
	     B[ii][jj]=B[jj][ii];
}

}
                         
/*#########################################输出节点导纳矩阵###################################*/
  fprintf(fp2,"\n\n                         ############ 潮流计算过程 ###########\n");
  fprintf(fp2,"\n  ==============================================================================\n");
  fprintf(fp2,"\n     节点导纳矩阵为:");
  for(i=1;i<=n;i++)
  { 
	  fprintf(fp2,"\n     ");
      for(k=1;k<=n;k++)
	  {
		  fprintf(fp2,"%f",G[i][k]);
          if(B[i][k]>=0)
		  { 
			  fprintf(fp2,"+j%f  ",B[i][k]);
		  }
	      else
		  { 
			  fprintf(fp2,"-j%f  ",-B[i][k]);
		  }
	  }
  }
  fprintf(fp2,"\n  ------------------------------------------------------------------------------\n");
 }
/*----------------------------------------------导纳矩阵求解完毕----------------------------*/
 /*############################################给节点附电压初值##################################*/
 void Voltage_Initial_Value()
 { 
	 for(i=1;i<=pq;i++)                      // e、f附初值 
	 { 
		 jd[i].e=1;  jd[i].f=0;	
	 }
     for(i=pq+1;i<n;i++)
	 {	  
		 jd[i].e=jd[i].U;  jd[i].f=0;
	 }
    

 }
 /*###################################高斯赛德尔迭代法求初值#############################*/


 void Gauss_Seidel()
 {
	 fprintf(fp2,"\n                        ##########高斯赛德尔迭代求初值############\n");
	 for(k=1;k<=W;k++)
	 {
		 double cs=0;double ds=0;                         //PQ节点的计算
		 for(i=1;i<=pq;i++)                              
		 {
			 shang(jd[i].p,-jd[i].q,jd[i].e,jd[i].f);
		     cs=c2;
	         ds=d2;
		     for(j=1;j<=n;j++)
			 {
				 if(i==j) continue;
			     ji(G[i][j],B[i][j],jd[j].e,jd[j].f);
			     cs-=c1;
			     ds-=d1;
			 }
		     shang(cs,ds,G[i][i],B[i][i]);
		     jd[i].e=c2;
		     jd[i].f=d2;
		 }
		 cs=0;ds=0;                                       //PV节点的计算                                 
		 for(i=pq+1;i<n;i++)
		 {
			 shang(jd[i].p,-jd[i].q,jd[i].e,jd[i].f);
		     cs=c2;
	         ds=d2;
		     for(j=1;j<=n;j++)
			 {
				 if(i==j) continue;
			     ji(G[i][j],B[i][j],jd[j].e,jd[j].f);
			     cs-=c1;
			     ds-=d1;
			 }
			 shang(cs,ds,G[i][i],B[i][i]);
			 jd[i].e=c2/sqrt(c2*c2+d2*d2)*jd[i].U;
			 jd[i].f=d2/sqrt(c2*c2+d2*d2)*jd[i].U;
			 ji(jd[i].e,jd[i].f,cs,ds);
			 jd[i].q=d1;
			 ji(jd[i].e,jd[i].f,G[i][i],B[i][i]);
			 ji(c1,d1,jd[i].e,jd[i].f);
			 jd[i].q=-d1;
		 }
	 fprintf(fp2,"\n\n      高斯赛德尔迭代的次数 %d",k);
	 fprintf(fp2,"\n      迭代所得的各点电压值");
	 for(i=1;i<=n;i++)
	 {
		 fprintf(fp2,"\n      节点%d的电压值：",i);
		 if(jd[i].f>=0)
		 {
			 fprintf(fp2,"%f+j%f  ",jd[i].e,jd[i].f);
		 }
		 else
		 {
			 fprintf(fp2,"%f-j%f  ",jd[i].e,-jd[i].f);
		 }
	 }
	 }
 }

/*########################################牛顿—拉夫逊法####################################*/

 /*----------------------------------计算节点不平衡量函数----------------------------------*/
void Calculate_Unbalanced_Para()
{ 
	for(i=1;i<=(pq+pv);i++)//先对ΔP、ΔQ、ΔU2初始化
	{
         jd[i].dp=0;
         jd[i].dq=0;
         jd[i].du=0;
	}

	g=n-1;
	for(i=1;i<=n;i++)                                  //计算PQ节点不平衡量
	{
		if(jd[i].s==1)                                           
		{
			t=jd[i].num;
		    cc[t]=dd[t]=0;
		    for(j=1;j<=n;j++)
			{
				for(a=1;a<=n;a++)		  
				{
					if(jd[a].num==j)
					    break; 
				}                                         
			    ji(G[t][j],-B[t][j],jd[a].e,-jd[a].f);
			    cc[t]+=c1;
				dd[t]+=d1;                  
			}	                                    
				ji(jd[i].e,jd[i].f,cc[t],dd[t]);
				jd[i].dp=jd[i].p-c1;
				jd[i].dq=jd[i].q-d1;      /*求出实际的功率值和迭代结果得到功率值之间的差值ΔP、ΔQ
					                        ，将其放在数组中*/
		}
		if(jd[i].s==2)                                  //计算PV节点不平衡量
		{
			t=jd[i].num;
		    cc[t]=dd[t]=0;
		    for(j=1;j<=n;j++)
			{
				for(a=1;a<=n;a++)		  
				{
					if(jd[a].num==j)
					    break; 
				}                                         
			    ji(G[t][j],-B[t][j],jd[a].e,-jd[a].f);
			    cc[t]+=c1;
				dd[t]+=d1;                  
			}	                                    
				ji(jd[i].e,jd[i].f,cc[t],dd[t]);
				jd[i].dp=jd[i].p-c1;
				jd[i].q=d1;
				jd[i].du=jd[i].U*jd[i].U-(jd[i].e*jd[i].e+jd[i].f*jd[i].f); /*循环只需结束P，不需计算U2；
																			求得有功功率个、和电压平方的实际值与迭代
																			得到的差值ΔP、ΔU2，将其放入数组中*/

		}
	}  
	//先对D[M]矩阵进行初始化
	for(i=1;i<=(pq+pv);i++)
	{
		D[M]=0;
	}
    for(i=1;i<=pq;i++)   //将PQ节点的P和Q放进D[M]                                //形成不平衡量矩阵D[M]
    {   		
		D[2*i-1]=jd[i].dp;//奇数位置为P
	    D[2*i]=jd[i].dq; //偶数位置为Q
	}
    for(i=pq+1;i<=n-1;i++)////将PV节点的P和U2放进D[M]
    {  
		D[2*i-1]=jd[i].dp;//奇数位置为P
	    D[2*i]=jd[i].du; //偶数位置为U2
	}
	fprintf(fp2,"\n     不平衡量为:");
	for(i=1;i<=pq;i++)
	{	
		t=jd[i].num;
		fprintf(fp2,"\n     dp[%d]=%f",i,D[2*t-1]);
        fprintf(fp2,"\n     dq[%d]=%f",i,D[2*t]);
	} 
	for(i=pq+1;i<=n-1;i++)
	{	
		t=jd[i].num;
		fprintf(fp2,"\n     dp[%d]=%f",i,D[2*t-1]);
		fprintf(fp2,"\n     du[%d]=%f",i,D[2*t]);
	} 
}
/*###################################形成雅克比矩阵函数##############################*/     
void Form_Jacobi_Matric()           /*形成雅克比矩阵*/
{ 
	//先对雅克比矩阵初始化
	for(i=1;i<=2*(pq+pv);i++) 
		for(j=1;j<=2*(pq+pv);j++)
			ykb[i][j]=0.0;
	for(i=1;i<=pq;i++)              //形成pq节点子阵,计算H、N、J、L
		for(j=1;j<n;j++)
		{  
			int i1=jd[i].num;//为了简单，将i节点和j节点的量用简单的元素代替
	        int j1=jd[j].num;
	        double ei=jd[i].e;
	        double ej=jd[j].e;
	        double fi=jd[i].f;
	        double fj=jd[j].f;//变量代替完毕
	 
		    if(i!=j)                                              //求i!=j时即非对角线上的H、N、J、L*/
			{ 
			    ykb[2*i-1][2*j-1]=-B[i1][j1]*ei+G[i1][j1]*fi;      //Hij 
	            ykb[2*i-1][2*j]=G[i1][j1]*ei+B[i1][j1]*fi;         //Nij
	            ykb[2*i][2*j-1]=-G[i1][j1]*ei-B[i1][j1]*fi;        //Jij 
	            ykb[2*i][2*j]=-B[i1][j1]*ei+G[i1][j1]*fi;          //Lij
			}
		    else                                                   //求i=j时即对角线上的的H、N、J、L
			{ //为了简洁计算，引入注入电流，利用注入电流计算雅克比矩阵（i=j）时的元素
			    aa[i]=0;bb[i]=0;
	            for(k=1;k<=n;k++)//通过循环求得的是i节点的注入电流Ii的实部和虚部的求和部分
				{   
				    int k1=jd[k].num;//为了方便计算，将k节点的一部分量用简单变量表示
				    ji(G[i1][k1],B[i1][k1],jd[k].e,jd[k].f);				   
		            aa[i]=aa[i]+c1;
		            bb[i]=bb[i]+d1;//注意，只是求了注入电流公式中的求和分量
				}
	            ykb[2*i-1][2*j-1]=-B[i1][i1]*ei+G[i1][i1]*fi+bb[i];      //Hij 
	            ykb[2*i-1][2*j]=G[i1][i1]*ei+B[i1][i1]*fi+aa[i];         //Nij
	            ykb[2*i][2*j-1]=-G[i1][i1]*ei-B[i1][i1]*fi+aa[i];        //Jij 
	            ykb[2*i][2*j]=-B[i1][i1]*ei+G[i1][i1]*fi-bb[i];          //Lij
			}
		}

  for(i=pq+1;i<=n-1;i++)                                        //形成pv节点子阵
      for(j=1;j<n;j++)
	  { 
		  int i1=jd[i].num;//同样的，为了简单，将i节点和j节点的量用简单的元素代替
	      int j1=jd[j].num;
	      double ei=jd[i].e;
	      double ej=jd[j].e;
	      double fi=jd[i].f;
	      double fj=jd[j].f;//变量代替完毕
	      if(i!=j)                                           //求i!=j时即非对角线上的H、N                                          
		  { 
			  ykb[2*i-1][2*j-1]=-B[i1][j1]*ei+G[i1][j1]*fi;    //求法和PQ节点相同，得到Hij
	          ykb[2*i-1][2*j]=G[i1][j1]*ei+B[i1][j1]*fi;       //求法和PQ节点相同，得到Nij
			  ykb[2*i][2*j-1]=ykb[2*i][2*j]=0;                 //i!=j时Rij和Sij都为0
		  }
	  
	      else                                               //求i=j时即对角线上的的H、N、R、S
		  { 
			  //为了简洁计算，引入注入电流，利用注入电流计算雅克比矩阵（i=j）时的元素
			   aa[i]=0;bb[i]=0;
	           for(k=1;k<=n;k++)//通过循环求得的是i节点的注入电流Ii的实部和虚部的求和部分
			   {   
				   int k1=jd[k].num;//为了方便计算，将k节点的一部分量用简单变量表示
				   ji(G[i1][k1],B[i1][k1],jd[k].e,jd[k].f);				   
		           aa[i]=aa[i]+c1;
		           bb[i]=bb[i]+d1;
			   }
	           ykb[2*i-1][2*j-1]=-B[i1][i1]*ei+G[i1][i1]*fi+bb[i];      //Hii
	           ykb[2*i-1][2*j]=G[i1][i1]*ei+B[i1][i1]*fi+aa[i];         //Nii
	           ykb[2*i][2*j-1]=2*fi;                                    //Rii
	           ykb[2*i][2*j]=2*ei;                                      //Sii
	      }
	  }
	/*------------------------------输出雅克比矩阵--------------------------------*/ 
    fprintf(fp2,"\n\n     雅克比矩阵为: "); 
	for(i=1;i<=2*g;i++)                                      
	{	
        fprintf(fp2,"\n"); 
		for(j=1;j<=2*g;j++)
		{
			fprintf(fp2,"     %f",ykb[i][j]); 
		}
	} 
}


	/*------------------------------列主元素消去法解修正方程组函数--------------------------------*/ 
void Solve_Equations()                              //求解修正方程组 (用列主元消取法)                                      
 { double d,t;
	g=n-1;
	for(i=1;i<=2*g;i++)                             //把函数残量矩阵编入修正方程/
		ykb[i][2*g+1]=D[i];
	
     k=1;
   do
    {
     d=ykb[k][k];
     l=k;
     i=k+1;
     do
      {
       if(fabs(ykb[i][k])>fabs(d))                  // 选列主元素 
	{ d=ykb[i][k];
	  l=i;
	 }
       i++;
       } while(i<=2*(pq+pv));
     if(l!=k)
       {  for(j=k;j<=2*(pq+pv)+1;j++)
	     { t=ykb[l][j];
	       ykb[l][j]=ykb[k][j];
	       ykb[k][j]=t;
	      }
	 }
    for(j=k+1;j<=2*(pq+pv)+1;j++)                   // 各列进行消去,化成最简三角阵
       ykb[k][j]/=ykb[k][k];
    for(i=k+1;i<=2*(pq+pv);i++)
       for(j=k+1;j<=2*(pq+pv)+1;j++)
	  ykb[i][j]-=ykb[i][k]*ykb[k][j];
    k++;
    }  while(k<=2*(pq+pv));
    if(k!=1)
    {
     for(i=2*(pq+pv);i>=1;i--)                      // 通过回代,求出各个解
     { t=0;
       for(j=i+1;j<=2*(pq+pv);j++)
	  t+=ykb[i][j]*D[j];
       D[i]=ykb[i][2*(pq+pv)+1]-t;
       }
     }

  for(i=1;i<=(pq+pv);i++)
     { jd[i].e+=D[2*i];
       jd[i].f+=D[2*i-1];
      }
  max=fabs(D[1]);
  for(i=1;i<=2*(pq+pv);i++)
     if(fabs(D[i])>max)
	max=fabs(D[i]);

}
	/*------------------------------牛顿—拉夫逊迭代函数--------------------------------*/ 
void Newton_Raphson()
{    
	 int z=1;     
	 fprintf(fp2,"\n                       ##########牛顿——拉夫逊法迭代结果###########\n");
	 g=pq+pv;  
     do
	 {    max=1;		 
		  if((z<N)&&(max>=eps))
         {
			 fprintf(fp2,"\n     迭代次数:  %d\n",z);
		 }
         
		 Calculate_Unbalanced_Para();
		 Form_Jacobi_Matric();
		 Solve_Equations();
		
		 
        fprintf(fp2,"\n\n     输出  df,de: ");
        for(c=1;c<=n;c++)
		{
			for(a=1;a<=n;a++)		  
			{
				if(jd[a].num==c)
					break; 
			}   
			fprintf(fp2,"\n");
            fprintf(fp2,"     节点为 %2d   df=%8.5f    de=%8.5f",c,D[2*a-1],D[2*a]);
		}
		fprintf(fp2,"\n\n     输出迭代过程中的电压值: ");
        for(c=1;c<=n;c++)
		{
			for(a=1;a<=n;a++)		  
			{
				if(jd[a].num==c)
					break; 
			}   
			fprintf(fp2,"\n     U[%d]=%f",c,jd[a].e);
			if(jd[a].f>=0)
				fprintf(fp2,"+j%f",jd[a].f);
			
			else
				fprintf(fp2,"-j%f",-jd[a].f);
		}
		fprintf(fp2,"\n\n  ------------------------------------------------------------------------------");
		 z++;
	 } while((z<N)&&(max>=eps));
  
 }
	/*################################潮流计算结果输出函数###################################*/
void Powerflow_Result()
{
	int n1=jd[n].num;
	fprintf(fp2,"\n\n  ==============================================================================\n\n");
	fprintf(fp2,"                         ###############潮流计算结果################");
	fprintf(fp2,"\n\n  ==============================================================================\n\n");
	fprintf(fp2,"\n     各节点电压值: ");
        for(c=1;c<=n;c++)
		{
			for(a=1;a<=n;a++)		  
			{
				if(jd[a].num==c)
					break; 
			}   
			fprintf(fp2,"\n     U[%d]=%f",c,jd[a].e);
			if(jd[a].f>=0)
				fprintf(fp2,"+j%f",jd[a].f);			
			else
				fprintf(fp2,"-j%f",-jd[a].f);
		}
	
	rr=tt=0;	
	for(i=1;i<=n;i++)        //求平衡节点功率
	{
		int i1=jd[i].num;
		ji(G[n1][i1],-B[n1][i1],jd[i].e,-jd[i].f);
		rr+=c1;
		tt+=d1;
	}
	ji(jd[n].e,jd[n].f,rr,tt);
	
	fprintf(fp2,"\n\n     各节点注入功率:\n");          //输出各节点注入功率
  for(i=1;i<=pq;i++)                                    //PQ节点
  { 
	  int i1=jd[i].num;
	  fprintf(fp2,"     PQ节点:     节点%d     S[%d]=%f",  i1,i1,jd[i].p);
	  if(jd[i].q>=0)
	  fprintf(fp2,"+j%f\n",jd[i].q);
	  else
	  fprintf(fp2,"-j%f\n",-jd[i].q);
  }
  for(i=pq+1;i<=g;i++)                        //PV节点
  { 
	  int i1=jd[i].num;
	  fprintf(fp2,"     PV节点:     节点%d     S[%d]=%f",  i1,i1,jd[i].p);
	  if(jd[i].q>=0)
	  fprintf(fp2,"+j%f\n",jd[i].q);
	  else
	  fprintf(fp2,"-j%f\n",-jd[i].q);
  }
  
  fprintf(fp2,"     平衡节点:   节点%d",jd[n].num,jd[n].num);       //平衡节点
  fprintf(fp2,"     S[%d]=%f",n1,c1);
	if(d1>=0)
		fprintf(fp2,"+j%f",d1);
	else
	    fprintf(fp2,"-j%f",-d1);
	jd[n].p=c1;
	jd[n].q=d1;
/*------------------------------输出线路功率--------------------------------*/ 
	fprintf(fp2,"\n\n     线路功率:\n");
	rr=tt=0;
    for(i=1;i<=m;i++)
	{
		int i1=zhilu[i].p1;
		int j1=zhilu[i].p2;
		aa[i]=bb[i]=0;
		for(a=1;a<=n;a++)			  
		{
			if(jd[a].num==i1)
				break;
		} 
		for(b=1;b<=n;b++)			  
		{
			if(jd[b].num==j1)
				break;
		}
		mozhi(zhilu[i].r,zhilu[i].x);
		if(mo==0)
			continue;
		shang(1,0,zhilu[i].r,zhilu[i].x);
		c3=c2;
		d3=d2;
		/*------------------------------普通支路的线路功率--------------------------------*/ 
		if(zhilu[i].s==1)          //普通支路
		{
		ji(jd[a].e-jd[b].e,-jd[a].f+jd[b].f,c3,-d3);
		ji(jd[a].e,jd[a].f,c1,d1);
		fprintf(fp2,"     普通线路 %d——%d 的功率为:   %f",i1,j1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
		aa[i]+=c1;
		bb[i]+=d1;
		zhilu[i].p=fabs(c1);
	
		ji(jd[b].e-jd[a].e,-jd[b].f+jd[a].f,c3,-d3);
		ji(jd[b].e,jd[b].f,c1,d1);
		fprintf(fp2,"     普通线路 %d——%d 的功率为:   %f",j1,i1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
		if(zhilu[i].p<fabs(c1))   zhilu[i].p=fabs(c1);
        aa[i]+=c1;
		bb[i]+=d1;
		}
		/*------------------------------对地支路的线路功率--------------------------------*/ 
        if(zhilu[i].s==0)    //该支路有对地支路
		{
			mozhi(zhilu[i].r01,zhilu[i].x01);         //支路前一个对地阻抗
		if(mo!=0)                                     //如果阻抗不为零
		{ 
		   ji(jd[a].e-jd[b].e,-jd[a].f+jd[b].f,c3,-d3);
           	c4=c1;
		    d4=d1;
		   shang(1,0,zhilu[i].r01,zhilu[i].x01);
		   ji(jd[a].e,-jd[a].f,c2,-d2);
		   c4+=c1;
		   d4+=d1;
           ji(jd[a].e,jd[a].f,c4,d4);
		}
		else                                //如果阻抗为零，变为普通支路
		{
			ji(jd[a].e-jd[b].e,-jd[a].f+jd[b].f,c3,-d3);
		    ji(jd[a].e,jd[a].f,c1,d1);
		}
		   fprintf(fp2,"     对地线路 %d——%d 的功率为:   %f",i1,j1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);

		aa[i]+=c1;
		bb[i]+=d1;
		zhilu[i].p=fabs(c1);
		
        mozhi(zhilu[i].r02,zhilu[i].x02);    //支路后一个对地阻抗
		if(mo!=0)                            //如果阻抗不为零
		{
			ji(jd[b].e-jd[a].e,-jd[b].f+jd[a].f,c3,-d3);
           	c4=c1;
		    d4=d1; 
		   shang(1,0,zhilu[i].r02,zhilu[i].x02);
		   ji(jd[b].e,-jd[b].f,c2,-d2);
		   c4+=c1;
		   d4+=d1;
           ji(jd[b].e,jd[b].f,c4,d4);
		}
			else                              //如果阻抗为零，变为普通支路
		{
			ji(jd[b].e-jd[a].e,-jd[b].f+jd[a].f,c3,-d3);
		    ji(jd[b].e,jd[b].f,c1,d1);
		}
			fprintf(fp2,"     对地线路 %d——%d 的功率为:   %f",j1,i1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
		if(zhilu[i].p<fabs(c1))   zhilu[i].p=fabs(c1);
        aa[i]+=c1;
		bb[i]+=d1;
		}
			/*------------------------------并联支路的线路功率--------------------------------*/ 
	    if(zhilu[i].s==3)    //该支路为并联支路
		{
		   ji(jd[a].e-jd[b].e,-jd[a].f+jd[b].f,c3,-d3);      //计算并联支路主路的功率
		ji(jd[a].e,jd[a].f,c1,d1);
		fprintf(fp2,"     并联线路的第一条 %d——%d 的功率为:   %f",i1,j1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
		zhilu[i].p=fabs(c1);
		aa[i]+=c1;
		bb[i]+=d1;
		ji(jd[b].e-jd[a].e,-jd[b].f+jd[a].f,c3,-d3);
		ji(jd[b].e,jd[b].f,c1,d1);
		fprintf(fp2,"     并联线路的第一条 %d——%d 的功率为:   %f",j1,i1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
		if(zhilu[i].p<fabs(c1))   zhilu[i].p=fabs(c1);
        aa[i]+=c1;
		bb[i]+=d1;
			mozhi(zhilu[i].rr,zhilu[i].xx);      //计算并联支路辅路的功率
		if(mo!=0)
			
		shang(1,0,zhilu[i].rr,zhilu[i].xx);
		c4=c2;
		d4=d2;
        ji(jd[a].e-jd[b].e,-jd[a].f+jd[b].f,c4,-d4);      
		ji(jd[a].e,jd[a].f,c1,d1);
		fprintf(fp2,"     并联线路的第二条 %d——%d 的功率为:   %f",i1,j1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
		aa[i]+=c1;
		bb[i]+=d1;
		ji(jd[b].e-jd[a].e,-jd[b].f+jd[a].f,c4,-d4);
		ji(jd[b].e,jd[b].f,c1,d1);
		fprintf(fp2,"     并联线路的第二条 %d——%d 的功率为:   %f",j1,i1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
        aa[i]+=c1;
		bb[i]+=d1;
	   }
	   /*------------------------------非标准变比支路的线路功率--------------------------------*/
	   if(zhilu[i].s==2)    //该支路为非标准变比支路,原始变比为1
	   {   
		   bianbi=zhilu[i].k;
           ji(jd[a].e-jd[b].e,-jd[a].f+jd[b].f,c3/bianbi,-d3/bianbi);//前一条
           	c4=c1;
		    d4=d1;
		  
		   ji(jd[a].e,-jd[a].f,c3*((1-bianbi)/(bianbi*bianbi)),-d3*((1-bianbi)/(bianbi*bianbi)));/*求非标准变比支路的前端对地支路*/
		   c4+=c1;
		   d4+=d1;
           ji(jd[a].e,jd[a].f,c4,d4);
		   fprintf(fp2,"     非标准变比线路 %d——%d 的功率为:   %f",i1,j1,c1);
		   zhilu[i].p=fabs(c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);

		aa[i]+=c1;
		bb[i]+=d1;
			ji(jd[b].e-jd[a].e,-jd[b].f+jd[a].f,c3/bianbi,-d3/bianbi);//后一条
           	c4=c1;
		    d4=d1;
		   ji(jd[b].e,-jd[b].f,c3*((-1+bianbi)/bianbi),-d3*((-1+bianbi)/bianbi));
		   c4+=c1;
		   d4+=d1;
           ji(jd[b].e,jd[b].f,c4,d4);
		   fprintf(fp2,"     非标准变比线路 %d——%d 的功率为:   %f",j1,i1,c1);
		   if(zhilu[i].p<fabs(c1))   zhilu[i].p=fabs(c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
        aa[i]+=c1;
		bb[i]+=d1;
	   }
		rr+=aa[i];
		tt+=bb[i];

	}
	 /*------------------------------线路损耗功率--------------------------------*/
    fprintf(fp2,"\n\n     线路损耗功率:\n");
	for(i=1;i<=m;i++)
	{
		int i1=zhilu[i].p1;
		int j1=zhilu[i].p2;
		zhilu[i].pl=aa[i];
		zhilu[i].ql=bb[i];
	    fprintf(fp2,"     线路 %d——%d 损耗的功率为:   %f",i1,j1,aa[i]);
		zhilu[i].eff=1-fabs(aa[i])/zhilu[i].p;
		if(bb[i]>=0)
			fprintf(fp2,"+j%f\n",bb[i]);
	    else
			fprintf(fp2,"-j%f\n",-bb[i]);
	}
	 /*------------------------------网络总损耗功率--------------------------------*/		
	fprintf(fp2,"\n\n     网络总损耗功率为:   %f",rr);
		if(tt>=0)
			fprintf(fp2,"+j%f\n",tt);
	    else
	        fprintf(fp2,"-j%f\n",tt);
	fprintf(fp2,"\n ==============================================================================\n");	
    fprintf(fp2,"\n\n                  ############ 潮流计算结束 ##############");
}
void Result_Analysis()
{
	double sum1=0;double sum2=0;
	for(i=1;i<=n;i++)
	{
		if(jd[i].p>=0) sum1+=jd[i].p;
		else sum2+=-jd[i].p;
	}
	fprintf(fp2,"\n                  ############   结果分析  ###############\n");
	fprintf(fp2,"\n     网络总效率为：      %f",sum2/sum1);
	for(i=1;i<=m;i++)
		fprintf(fp2,"\n     线路%d的效率是：     %f",i,zhilu[i].eff);
}

/*######################最后是主函数，将所有的子函数串起来，形成循环计算#####################*/
void main()
{
	 introduction();                        //程序简介
	 ReadData();                            //读取数据
     Form_Y();                              //形成节点导纳矩阵
     Voltage_Initial_Value();               //节点电压附初值
     Gauss_Seidel();                        //高斯赛德尔迭代求初值
	 Newton_Raphson();                      //牛顿——拉夫逊迭代
	 Powerflow_Result();                    //输出数据
     Result_Analysis();                     //结果分析
	 printf("计算结束，所有结果见OutputData.txt文件中\n");
 }
