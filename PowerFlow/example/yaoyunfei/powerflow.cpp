#include<stdio.h>      
#include<math.h>
#define M 100          //M�����˾���Ľ���
#define W 2            //WΪ��˹�������¶�����������Ϊ��ʹţ�١�����ѷ����ֵ�ʵ���Ӧȡ���ʵ�Wֵ
#define N 100          //NΪ��������
struct jd                                  //����һ���ڵ�Ľṹ�壬����洢�ڵ���Ϣ
      {  int num,s;                        // numΪ�ڵ�ţ�sΪ�ڵ�����
	     double p,q,e,f,U,dp,dq,du,de,df;  //����Ϊ�ڵ��й����޹����ʣ���ѹ�ݡ����������ѹģֵ,ţ�١�������ѷ���еĹ��ʲ�ƽ��������ѹ��ƽ����*/
      } jd[M];//����ڵ����

struct zhilu                              //����һ��֧·�Ľṹ�壬����洢֧·��Ϣ
      {  int s;                           //sΪ֧·����
	     int num;                         //numΪ֧·��
	     int p1,p2;                       //֧·�������ڵ�
	     double r,x;                      //֧·�ĵ���r��翹x
		 double	 rr,xx;                   //����֧·����rr��翹xx
		 double	 k;                       //�Ǳ�׼���֧·���k
		 double	 r01,x01,r02,x02;         //�Ե�֧·�Եص���r01,r02��翹x01,x02
		 double  pl,ql;                   //��֧·�й���ġ��޹����
		 double  p;                       //��֧·�й����빦��
		 double  eff;                     //��֧·Ч��
      } zhilu[M];//����֧·����
int i,j,k,a,b,c,ii,jj;              //ѭ�����Ʊ��� 
int t,l,g;                          //�м����         
int n,                      // �ڵ���
    m,                      //֧·�� 
    pq,                     //PQ�ڵ���
    pv;                     //PV�ڵ��� 
    
    
double eps;                                    // ���� 
double aa[M],bb[M],cc[M],dd[M],max,temp, rr,tt,bianbi,c3,d3,c4,d4;                //�м����         
double mo,c1,d1,c2,d2;                         //�������㺯���ķ���ֵ   
double G[M][M],B[M][M],Y[M][M];                //�ڵ㵼�ɾ����е�ʵ�����鲿����ģ��ֵ
double ykb[M][M],D[M],dU[M];                   //�ſ˱Ⱦ��󡢲�ƽ��������

	     
FILE *fp1,*fp2;
void introduction()//������
{
	if((fp2=fopen("OutputData.txt","w"))==NULL)     
   { 
	   printf("  can not open file!\n");
       
   }
	
	fprintf(fp2,"                            ********������********\n");
	fprintf(fp2,"   ���������ʵ�ּ򵥵ĳ������㡣���Դ������PQ�ڵ㡢PV�ڵ��ƽ��ڵ����ڵ����磬��������ƽ��ڵ�ֻ��һ��.\n");
    fprintf(fp2,"   ����Ҫ�����ǣ����ȣ�ʹ�ø�˹���¶��������ֵ������ʹ��ţ������ѷ������Ҫ�󾫶�Ϊֹ�������������ŵ���:\n");
    fprintf(fp2,"   �������ʣ�ģ�黯,�ֱ𽫲�ͬ�Ŀ鹦�ܱ���Ӻ������и�˹���¶��Ӻ�����������������Ӻ�������ƽ�����Ӻ�����\n");
    fprintf(fp2,"   �ſ˱Ⱦ����γ��Ӻ��������Է���������Ӻ��������ʼ����Ӻ�������������Ӻ����������飬�ó��򾫶Ƚϸߡ�\n");
    fprintf(fp2,"   �������ǵ��˶Ե�֧·���ɴ���Ǳ��Ȳ���֧·��ͨ���޸ĵ��ɾ��󣬴Ӷ�ʹ������Լ���Ǳ�׼��ѹ������ĳ�����\n");
	fprintf(fp2,"   ������ͨ���ļ������������ʹ�����������������ۡ�\n");
	
}
/*####################################���ļ��ж�ȡԭʼ���ݺ���###################################*/
void ReadData()                           //��ȡ����
 { 
	int h,number;                     //�������number���ѭ���ж���Ľڵ��,�������h���ѭ���ж���Ľڵ�����
    fp1=fopen("InputData.txt","r");//��ȡ��ʽ�������ļ�
	if(fp1==NULL)
    { 
		printf(" �����ļ���ȡʧ�� !\n");
        
    }
	else//���й�������ʾ
		printf("�����������������У����Ե�...\n");//������ʽ

/*################################################��ȡ��Ϣ#######################################*/
    fscanf(fp1,"%d,%d,%d,%d,%lf\n",&n,&m,&pq,&pv,&eps);              //����ڵ���,֧·��,PQ�ڵ���,PV�ڵ����;���
	
    j=1;k=pq+1;                   /*�ڽڵ�ṹ�����ȴ洢PQ�ڵ�,�ٴ洢PV�ڵ�,
	���һ��Ϊƽ��ڵ����j���ƽṹ����PQ�ڵ�Ĵ洢λ��,����k���ƽṹ����PV�ڵ�Ĵ洢λ��*/

	/*------------------------------------��ȡ�ڵ���Ϣ--------------------------------------*/
    for(i=1;i<=n;i++)                                                //����ڵ��š����͡����빦�ʺ͵�ѹ��ֵ
    {
		fscanf(fp1,"%d,%d",&number,&h);//��ȡ�ڵ���ź�����
	    if(h==1)                                                     //����h=1��PQ�ڵ�
		{ 
			fscanf(fp1,",%lf,%lf,%lf,%lf\n",&jd[j].p,&jd[j].q,&jd[j].e,&jd[j].f);/*����PQ�ڵ��й����ʡ�
			�޹����ʣ���ѹ�ݡ������*/
	        jd[j].num=number;//�жϸýڵ�ΪPQ�ڵ�󣬽���Ϣת�Ƶ�PQ�ڵ�洢λ��
	        jd[j].s=h;
	        j++;
		}
	    if(h==2)                                                    //����h=2��pv�ڵ�
		{
			fscanf(fp1,",%lf,%lf,%lf\n",&jd[k].p,&jd[k].U,&jd[k].q);/*����PV�ڵ��й����ʡ�
			��ѹ,����һ���޹����ʣ��Ҹ��޹�������ֵ��һ��Ϊ�й����ʵ�һ��*/
	        jd[k].num=number;//�жϸýڵ�ΪPV�ڵ�󣬽���Ϣת�Ƶ�PV�ڵ�洢λ��
	        jd[k].s=h;
	        k++;
		}
	   if(h==3)                                                    //����h=3��ƽ��ڵ�
	   {
		   fscanf(fp1,",%lf,%lf\n",&jd[n].e,&jd[n].f);/*����ƽ��ڵ��ѹ�ݡ������*/
	       jd[n].num=number;//�жϸýڵ�Ϊƽ��ڵ�󣬽���Ϣת�Ƶ�ƽ��ڵ�洢λ��
	       jd[n].s=h;
	   }
	}
	/*------------------------------------��ȡ֧·��Ϣ--------------------------------------*/
   for(i=1;i<=m;i++) //����֧·�迹
   {
	   fscanf(fp1,"%d,%d",&number,&h);//����֧·��ţ�����
	    if(h==1)                                                     /*����h=1����֧ͨ·*/
		{ 
			fscanf(fp1,",%d,%d,%lf,%lf\n",&zhilu[i].p1,&zhilu[i].p2,&zhilu[i].r,&zhilu[i].x);
			/*����֧·�ڵ��p1��p2,֧·�ĵ���r��翹x*/
	        zhilu[i].num=number;
	        zhilu[i].s=h;
	      
		}
	    if(h==0)                                                    /*����h=0���жԵ��迹��֧·*/
		{
			fscanf(fp1,",%d,%d,%lf,%lf,%lf,%lf,%lf,%lf\n",&zhilu[i].p1,&zhilu[i].p2,&zhilu[i].r,&zhilu[i].x,&zhilu[i].r01,&zhilu[i].x01,&zhilu[i].r02,&zhilu[i].x02);
			/*����֧·�ڵ��p1��p2,֧·�ĵ���r��翹x,�Ե�֧·�Եص���r01,r02��翹x01,x02*/
	        zhilu[i].num=number;
	        zhilu[i].s=h;
	    
		}
	   if(h==2)                                                    /*����h=2�ǷǱ�׼���֧·*/
	   {
		   fscanf(fp1,",%d,%d,%lf,%lf,%lf\n",&zhilu[i].p1,&zhilu[i].p2,&zhilu[i].r,&zhilu[i].x,&zhilu[i].k);
	        /*����֧·�ڵ��p1��p2,֧·�ĵ���r��翹x,�Ǳ�׼���֧·���k*/
		   zhilu[i].num=number;
	        zhilu[i].s=h;
	     
	   }
	    if(h==3)                                                    /*����h=3�ǲ���֧·*/
	   {
		   fscanf(fp1,",%d,%d,%lf,%lf,%lf,%lf\n",&zhilu[i].p1,&zhilu[i].p2,&zhilu[i].r,&zhilu[i].x,&zhilu[i].rr,&zhilu[i].xx);
	        /*����֧·�ڵ��p1��p2,֧·�ĵ���r��翹x,����֧·����rr��翹xx*/
		   zhilu[i].num=number;
	        zhilu[i].s=h;

	   }
	}

	/*##################################���ԭʼ��Ϣ########################################*/   
   
  /*������֪���ݵ��ļ�OutputData.txt��*/
  
  fprintf(fp2,"\n     �����ϻ�  ����������ѧ ����1007  Ҧ�Ʒ�   201001001321\n");
  fprintf(fp2,"\n                         ##############ԭʼ���� #############\n");
  fprintf(fp2,"  ==============================================================================\n");
  fprintf(fp2,"     �ڵ���:%d ֧·��:%d  PQ�ڵ���:%d  PV�ڵ���:%d    ����:%f\n",
	  								 n,m,pq,pv,eps);
  fprintf(fp2,"  ------------------------------------------------------------------------------\n");
  for(i=1;i<=pq;i++)
  fprintf(fp2,"     PQ�ڵ�:      �ڵ�%d       P[%d]=%f    Q[%d]=%f\n",
	       jd[i].num,jd[i].num,jd[i].p,jd[i].num,jd[i].q);
  for(i=pq+1;i<=pq+pv;i++)
  fprintf(fp2,"     PV�ڵ�:      �ڵ�%d       P[%d]=%f     U[%d]=%f    ��ֵQ[%d]=%f\n",
	       jd[i].num,jd[i].num,jd[i].p,jd[i].num,jd[i].U,jd[i].num,jd[i].q);
  fprintf(fp2,"     ƽ��ڵ�:    �ڵ�%d       e[%d]=%f     f[%d]=%f\n",
	       jd[n].num,jd[n].num,jd[n].e,jd[n].num,jd[n].f);
  fprintf(fp2,"  -------------------------------------------------------------------------------\n");
  
  for(i=1;i<=m;i++)
  {
	  if(zhilu[i].s==1)
    
        fprintf(fp2,"     ��֧ͨ·%d:     ��ؽڵ�:��%d��%d   R=%f  X=%f\n",
		      zhilu[i].num,zhilu[i].p1,zhilu[i].p2,zhilu[i].r,zhilu[i].x);

      if(zhilu[i].s==0)
        fprintf(fp2,"     ���ڶԵص��ɵ�֧·%d:     ��ؽڵ�:��%d��%d   R=%f  X=%f  %d�ڵ�Եص���R01=%f  �Եص翹X01=%f   %d�ڵ�Եص���R02=%f  �Եص翹X02=%f\n",
		      zhilu[i].num,zhilu[i].p1,zhilu[i].p2,zhilu[i].r,zhilu[i].x,zhilu[i].p1,zhilu[i].r01,zhilu[i].x01,zhilu[i].p2,zhilu[i].r02,zhilu[i].x02);
      
	  if(zhilu[i].s==2)
        fprintf(fp2,"     �Ǳ�׼���֧·%d:     ��ؽڵ�:��%d��%d   RT=%f  XT=%f  ���Ϊ%f\n",
		      zhilu[i].num,zhilu[i].p1,zhilu[i].p2,zhilu[i].r,zhilu[i].x,zhilu[i].k);
       
	  if(zhilu[i].s==3)
        fprintf(fp2,"     ����֧·%d:     ��ؽڵ�:��%d��%d   R=%f  X=%f  ����֧·����R=%f  ����֧·�翹X=%f\n",
		      zhilu[i].num,zhilu[i].p1,zhilu[i].p2,zhilu[i].r,zhilu[i].x,zhilu[i].rr,zhilu[i].xx);

 
} 
   fprintf(fp2,"  ==============================================================================\n");
  }
/*����ԭʼ�������*/
/*######################################�������㺯��####################################*/
 void mozhi(double a0,double b0)                      //������ģֵ����
 { 	 mo=sqrt(a0*a0+b0*b0);
 } 
 void ji(double a1,double b1,double a2,double b2)     //�����������
 {   c1=a1*a2-b1*b2;
	 d1=a1*b2+a2*b1;
 }
 void shang(double a3,double b3,double a4,double b4)  //�������̺���
 {   c2=(a3*a4+b3*b4)/(a4*a4+b4*b4);
     d2=(a4*b3-a3*b4)/(a4*a4+b4*b4);
 }
 

 /*####################################����ڵ㵼�ɾ���###############################*/
 void Form_Y()                    //�γɽڵ㵼�ɾ��� 
 {
for(i=1;i<=n;i++)//�Ƚ��ڵ㵼�ɾ���������õ���Ԫ������
{
    G[i][j]=0;//ʵ������
    B[i][j]=0;//�鲿����
}
  double S,S1,S2;
  //��ʼ�󻥵��ɣ��Ե���
  for(i=1;i<=m;i++)               //�ڵ㵼�ɾ�������Խ����ϵĵ���
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

  for(k=1;k<=m;k++)              //�ڵ㵼�ɾ�������Խ����ϵĵ���
      { i=zhilu[k].p1;
	j=zhilu[k].p2;
	S=zhilu[k].r*zhilu[k].r+zhilu[k].x*zhilu[k].x;
	if(S==0)  continue;
	G[i][j]+=-zhilu[k].r/S;
	B[i][j]+=zhilu[k].x/S;
	G[j][i]=G[i][j];
	B[j][i]=B[i][j];
	}
  /*------------------------------------�Է���֧ͨ·�޸Ľڵ㵼�ɾ���--------------------------------------*/
for(i=1;i<=m;i++)   
{
    jj=zhilu[i].p1;
	 ii=zhilu[i].p2;
	 if(zhilu[i].s==0)    //��֧·�жԵ�֧·,�Եص���һ�㲻���ǵ絼��ֻ���ǵ��ɣ���ֻ���鲿
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
if(zhilu[i].s==2)    //��֧·Ϊ�Ǳ�׼���֧·,ԭʼ���Ϊ1
{//���i��֧·��j�ڵ��йأ���ô����j���Ե������ۼ���i֧·�ĵ���ֵ
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
if(zhilu[i].s==3)    //��֧·Ϊ����֧·
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
                         
/*#########################################����ڵ㵼�ɾ���###################################*/
  fprintf(fp2,"\n\n                         ############ ����������� ###########\n");
  fprintf(fp2,"\n  ==============================================================================\n");
  fprintf(fp2,"\n     �ڵ㵼�ɾ���Ϊ:");
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
/*----------------------------------------------���ɾ���������----------------------------*/
 /*############################################���ڵ㸽��ѹ��ֵ##################################*/
 void Voltage_Initial_Value()
 { 
	 for(i=1;i<=pq;i++)                      // e��f����ֵ 
	 { 
		 jd[i].e=1;  jd[i].f=0;	
	 }
     for(i=pq+1;i<n;i++)
	 {	  
		 jd[i].e=jd[i].U;  jd[i].f=0;
	 }
    

 }
 /*###################################��˹���¶����������ֵ#############################*/


 void Gauss_Seidel()
 {
	 fprintf(fp2,"\n                        ##########��˹���¶��������ֵ############\n");
	 for(k=1;k<=W;k++)
	 {
		 double cs=0;double ds=0;                         //PQ�ڵ�ļ���
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
		 cs=0;ds=0;                                       //PV�ڵ�ļ���                                 
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
	 fprintf(fp2,"\n\n      ��˹���¶������Ĵ��� %d",k);
	 fprintf(fp2,"\n      �������õĸ����ѹֵ");
	 for(i=1;i<=n;i++)
	 {
		 fprintf(fp2,"\n      �ڵ�%d�ĵ�ѹֵ��",i);
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

/*########################################ţ�١�����ѷ��####################################*/

 /*----------------------------------����ڵ㲻ƽ��������----------------------------------*/
void Calculate_Unbalanced_Para()
{ 
	for(i=1;i<=(pq+pv);i++)//�ȶԦ�P����Q����U2��ʼ��
	{
         jd[i].dp=0;
         jd[i].dq=0;
         jd[i].du=0;
	}

	g=n-1;
	for(i=1;i<=n;i++)                                  //����PQ�ڵ㲻ƽ����
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
				jd[i].dq=jd[i].q-d1;      /*���ʵ�ʵĹ���ֵ�͵�������õ�����ֵ֮��Ĳ�ֵ��P����Q
					                        ���������������*/
		}
		if(jd[i].s==2)                                  //����PV�ڵ㲻ƽ����
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
				jd[i].du=jd[i].U*jd[i].U-(jd[i].e*jd[i].e+jd[i].f*jd[i].f); /*ѭ��ֻ�����P���������U2��
																			����й����ʸ����͵�ѹƽ����ʵ��ֵ�����
																			�õ��Ĳ�ֵ��P����U2���������������*/

		}
	}  
	//�ȶ�D[M]������г�ʼ��
	for(i=1;i<=(pq+pv);i++)
	{
		D[M]=0;
	}
    for(i=1;i<=pq;i++)   //��PQ�ڵ��P��Q�Ž�D[M]                                //�γɲ�ƽ��������D[M]
    {   		
		D[2*i-1]=jd[i].dp;//����λ��ΪP
	    D[2*i]=jd[i].dq; //ż��λ��ΪQ
	}
    for(i=pq+1;i<=n-1;i++)////��PV�ڵ��P��U2�Ž�D[M]
    {  
		D[2*i-1]=jd[i].dp;//����λ��ΪP
	    D[2*i]=jd[i].du; //ż��λ��ΪU2
	}
	fprintf(fp2,"\n     ��ƽ����Ϊ:");
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
/*###################################�γ��ſ˱Ⱦ�����##############################*/     
void Form_Jacobi_Matric()           /*�γ��ſ˱Ⱦ���*/
{ 
	//�ȶ��ſ˱Ⱦ����ʼ��
	for(i=1;i<=2*(pq+pv);i++) 
		for(j=1;j<=2*(pq+pv);j++)
			ykb[i][j]=0.0;
	for(i=1;i<=pq;i++)              //�γ�pq�ڵ�����,����H��N��J��L
		for(j=1;j<n;j++)
		{  
			int i1=jd[i].num;//Ϊ�˼򵥣���i�ڵ��j�ڵ�����ü򵥵�Ԫ�ش���
	        int j1=jd[j].num;
	        double ei=jd[i].e;
	        double ej=jd[j].e;
	        double fi=jd[i].f;
	        double fj=jd[j].f;//�����������
	 
		    if(i!=j)                                              //��i!=jʱ���ǶԽ����ϵ�H��N��J��L*/
			{ 
			    ykb[2*i-1][2*j-1]=-B[i1][j1]*ei+G[i1][j1]*fi;      //Hij 
	            ykb[2*i-1][2*j]=G[i1][j1]*ei+B[i1][j1]*fi;         //Nij
	            ykb[2*i][2*j-1]=-G[i1][j1]*ei-B[i1][j1]*fi;        //Jij 
	            ykb[2*i][2*j]=-B[i1][j1]*ei+G[i1][j1]*fi;          //Lij
			}
		    else                                                   //��i=jʱ���Խ����ϵĵ�H��N��J��L
			{ //Ϊ�˼����㣬����ע�����������ע����������ſ˱Ⱦ���i=j��ʱ��Ԫ��
			    aa[i]=0;bb[i]=0;
	            for(k=1;k<=n;k++)//ͨ��ѭ����õ���i�ڵ��ע�����Ii��ʵ�����鲿����Ͳ���
				{   
				    int k1=jd[k].num;//Ϊ�˷�����㣬��k�ڵ��һ�������ü򵥱�����ʾ
				    ji(G[i1][k1],B[i1][k1],jd[k].e,jd[k].f);				   
		            aa[i]=aa[i]+c1;
		            bb[i]=bb[i]+d1;//ע�⣬ֻ������ע�������ʽ�е���ͷ���
				}
	            ykb[2*i-1][2*j-1]=-B[i1][i1]*ei+G[i1][i1]*fi+bb[i];      //Hij 
	            ykb[2*i-1][2*j]=G[i1][i1]*ei+B[i1][i1]*fi+aa[i];         //Nij
	            ykb[2*i][2*j-1]=-G[i1][i1]*ei-B[i1][i1]*fi+aa[i];        //Jij 
	            ykb[2*i][2*j]=-B[i1][i1]*ei+G[i1][i1]*fi-bb[i];          //Lij
			}
		}

  for(i=pq+1;i<=n-1;i++)                                        //�γ�pv�ڵ�����
      for(j=1;j<n;j++)
	  { 
		  int i1=jd[i].num;//ͬ���ģ�Ϊ�˼򵥣���i�ڵ��j�ڵ�����ü򵥵�Ԫ�ش���
	      int j1=jd[j].num;
	      double ei=jd[i].e;
	      double ej=jd[j].e;
	      double fi=jd[i].f;
	      double fj=jd[j].f;//�����������
	      if(i!=j)                                           //��i!=jʱ���ǶԽ����ϵ�H��N                                          
		  { 
			  ykb[2*i-1][2*j-1]=-B[i1][j1]*ei+G[i1][j1]*fi;    //�󷨺�PQ�ڵ���ͬ���õ�Hij
	          ykb[2*i-1][2*j]=G[i1][j1]*ei+B[i1][j1]*fi;       //�󷨺�PQ�ڵ���ͬ���õ�Nij
			  ykb[2*i][2*j-1]=ykb[2*i][2*j]=0;                 //i!=jʱRij��Sij��Ϊ0
		  }
	  
	      else                                               //��i=jʱ���Խ����ϵĵ�H��N��R��S
		  { 
			  //Ϊ�˼����㣬����ע�����������ע����������ſ˱Ⱦ���i=j��ʱ��Ԫ��
			   aa[i]=0;bb[i]=0;
	           for(k=1;k<=n;k++)//ͨ��ѭ����õ���i�ڵ��ע�����Ii��ʵ�����鲿����Ͳ���
			   {   
				   int k1=jd[k].num;//Ϊ�˷�����㣬��k�ڵ��һ�������ü򵥱�����ʾ
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
	/*------------------------------����ſ˱Ⱦ���--------------------------------*/ 
    fprintf(fp2,"\n\n     �ſ˱Ⱦ���Ϊ: "); 
	for(i=1;i<=2*g;i++)                                      
	{	
        fprintf(fp2,"\n"); 
		for(j=1;j<=2*g;j++)
		{
			fprintf(fp2,"     %f",ykb[i][j]); 
		}
	} 
}


	/*------------------------------����Ԫ����ȥ�������������麯��--------------------------------*/ 
void Solve_Equations()                              //������������� (������Ԫ��ȡ��)                                      
 { double d,t;
	g=n-1;
	for(i=1;i<=2*g;i++)                             //�Ѻ����������������������/
		ykb[i][2*g+1]=D[i];
	
     k=1;
   do
    {
     d=ykb[k][k];
     l=k;
     i=k+1;
     do
      {
       if(fabs(ykb[i][k])>fabs(d))                  // ѡ����Ԫ�� 
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
    for(j=k+1;j<=2*(pq+pv)+1;j++)                   // ���н�����ȥ,�������������
       ykb[k][j]/=ykb[k][k];
    for(i=k+1;i<=2*(pq+pv);i++)
       for(j=k+1;j<=2*(pq+pv)+1;j++)
	  ykb[i][j]-=ykb[i][k]*ykb[k][j];
    k++;
    }  while(k<=2*(pq+pv));
    if(k!=1)
    {
     for(i=2*(pq+pv);i>=1;i--)                      // ͨ���ش�,���������
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
	/*------------------------------ţ�١�����ѷ��������--------------------------------*/ 
void Newton_Raphson()
{    
	 int z=1;     
	 fprintf(fp2,"\n                       ##########ţ�١�������ѷ���������###########\n");
	 g=pq+pv;  
     do
	 {    max=1;		 
		  if((z<N)&&(max>=eps))
         {
			 fprintf(fp2,"\n     ��������:  %d\n",z);
		 }
         
		 Calculate_Unbalanced_Para();
		 Form_Jacobi_Matric();
		 Solve_Equations();
		
		 
        fprintf(fp2,"\n\n     ���  df,de: ");
        for(c=1;c<=n;c++)
		{
			for(a=1;a<=n;a++)		  
			{
				if(jd[a].num==c)
					break; 
			}   
			fprintf(fp2,"\n");
            fprintf(fp2,"     �ڵ�Ϊ %2d   df=%8.5f    de=%8.5f",c,D[2*a-1],D[2*a]);
		}
		fprintf(fp2,"\n\n     ������������еĵ�ѹֵ: ");
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
	/*################################�����������������###################################*/
void Powerflow_Result()
{
	int n1=jd[n].num;
	fprintf(fp2,"\n\n  ==============================================================================\n\n");
	fprintf(fp2,"                         ###############����������################");
	fprintf(fp2,"\n\n  ==============================================================================\n\n");
	fprintf(fp2,"\n     ���ڵ��ѹֵ: ");
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
	for(i=1;i<=n;i++)        //��ƽ��ڵ㹦��
	{
		int i1=jd[i].num;
		ji(G[n1][i1],-B[n1][i1],jd[i].e,-jd[i].f);
		rr+=c1;
		tt+=d1;
	}
	ji(jd[n].e,jd[n].f,rr,tt);
	
	fprintf(fp2,"\n\n     ���ڵ�ע�빦��:\n");          //������ڵ�ע�빦��
  for(i=1;i<=pq;i++)                                    //PQ�ڵ�
  { 
	  int i1=jd[i].num;
	  fprintf(fp2,"     PQ�ڵ�:     �ڵ�%d     S[%d]=%f",  i1,i1,jd[i].p);
	  if(jd[i].q>=0)
	  fprintf(fp2,"+j%f\n",jd[i].q);
	  else
	  fprintf(fp2,"-j%f\n",-jd[i].q);
  }
  for(i=pq+1;i<=g;i++)                        //PV�ڵ�
  { 
	  int i1=jd[i].num;
	  fprintf(fp2,"     PV�ڵ�:     �ڵ�%d     S[%d]=%f",  i1,i1,jd[i].p);
	  if(jd[i].q>=0)
	  fprintf(fp2,"+j%f\n",jd[i].q);
	  else
	  fprintf(fp2,"-j%f\n",-jd[i].q);
  }
  
  fprintf(fp2,"     ƽ��ڵ�:   �ڵ�%d",jd[n].num,jd[n].num);       //ƽ��ڵ�
  fprintf(fp2,"     S[%d]=%f",n1,c1);
	if(d1>=0)
		fprintf(fp2,"+j%f",d1);
	else
	    fprintf(fp2,"-j%f",-d1);
	jd[n].p=c1;
	jd[n].q=d1;
/*------------------------------�����·����--------------------------------*/ 
	fprintf(fp2,"\n\n     ��·����:\n");
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
		/*------------------------------��֧ͨ·����·����--------------------------------*/ 
		if(zhilu[i].s==1)          //��֧ͨ·
		{
		ji(jd[a].e-jd[b].e,-jd[a].f+jd[b].f,c3,-d3);
		ji(jd[a].e,jd[a].f,c1,d1);
		fprintf(fp2,"     ��ͨ��· %d����%d �Ĺ���Ϊ:   %f",i1,j1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
		aa[i]+=c1;
		bb[i]+=d1;
		zhilu[i].p=fabs(c1);
	
		ji(jd[b].e-jd[a].e,-jd[b].f+jd[a].f,c3,-d3);
		ji(jd[b].e,jd[b].f,c1,d1);
		fprintf(fp2,"     ��ͨ��· %d����%d �Ĺ���Ϊ:   %f",j1,i1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
		if(zhilu[i].p<fabs(c1))   zhilu[i].p=fabs(c1);
        aa[i]+=c1;
		bb[i]+=d1;
		}
		/*------------------------------�Ե�֧·����·����--------------------------------*/ 
        if(zhilu[i].s==0)    //��֧·�жԵ�֧·
		{
			mozhi(zhilu[i].r01,zhilu[i].x01);         //֧·ǰһ���Ե��迹
		if(mo!=0)                                     //����迹��Ϊ��
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
		else                                //����迹Ϊ�㣬��Ϊ��֧ͨ·
		{
			ji(jd[a].e-jd[b].e,-jd[a].f+jd[b].f,c3,-d3);
		    ji(jd[a].e,jd[a].f,c1,d1);
		}
		   fprintf(fp2,"     �Ե���· %d����%d �Ĺ���Ϊ:   %f",i1,j1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);

		aa[i]+=c1;
		bb[i]+=d1;
		zhilu[i].p=fabs(c1);
		
        mozhi(zhilu[i].r02,zhilu[i].x02);    //֧·��һ���Ե��迹
		if(mo!=0)                            //����迹��Ϊ��
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
			else                              //����迹Ϊ�㣬��Ϊ��֧ͨ·
		{
			ji(jd[b].e-jd[a].e,-jd[b].f+jd[a].f,c3,-d3);
		    ji(jd[b].e,jd[b].f,c1,d1);
		}
			fprintf(fp2,"     �Ե���· %d����%d �Ĺ���Ϊ:   %f",j1,i1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
		if(zhilu[i].p<fabs(c1))   zhilu[i].p=fabs(c1);
        aa[i]+=c1;
		bb[i]+=d1;
		}
			/*------------------------------����֧·����·����--------------------------------*/ 
	    if(zhilu[i].s==3)    //��֧·Ϊ����֧·
		{
		   ji(jd[a].e-jd[b].e,-jd[a].f+jd[b].f,c3,-d3);      //���㲢��֧·��·�Ĺ���
		ji(jd[a].e,jd[a].f,c1,d1);
		fprintf(fp2,"     ������·�ĵ�һ�� %d����%d �Ĺ���Ϊ:   %f",i1,j1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
		zhilu[i].p=fabs(c1);
		aa[i]+=c1;
		bb[i]+=d1;
		ji(jd[b].e-jd[a].e,-jd[b].f+jd[a].f,c3,-d3);
		ji(jd[b].e,jd[b].f,c1,d1);
		fprintf(fp2,"     ������·�ĵ�һ�� %d����%d �Ĺ���Ϊ:   %f",j1,i1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
		if(zhilu[i].p<fabs(c1))   zhilu[i].p=fabs(c1);
        aa[i]+=c1;
		bb[i]+=d1;
			mozhi(zhilu[i].rr,zhilu[i].xx);      //���㲢��֧·��·�Ĺ���
		if(mo!=0)
			
		shang(1,0,zhilu[i].rr,zhilu[i].xx);
		c4=c2;
		d4=d2;
        ji(jd[a].e-jd[b].e,-jd[a].f+jd[b].f,c4,-d4);      
		ji(jd[a].e,jd[a].f,c1,d1);
		fprintf(fp2,"     ������·�ĵڶ��� %d����%d �Ĺ���Ϊ:   %f",i1,j1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
		aa[i]+=c1;
		bb[i]+=d1;
		ji(jd[b].e-jd[a].e,-jd[b].f+jd[a].f,c4,-d4);
		ji(jd[b].e,jd[b].f,c1,d1);
		fprintf(fp2,"     ������·�ĵڶ��� %d����%d �Ĺ���Ϊ:   %f",j1,i1,c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);
        aa[i]+=c1;
		bb[i]+=d1;
	   }
	   /*------------------------------�Ǳ�׼���֧·����·����--------------------------------*/
	   if(zhilu[i].s==2)    //��֧·Ϊ�Ǳ�׼���֧·,ԭʼ���Ϊ1
	   {   
		   bianbi=zhilu[i].k;
           ji(jd[a].e-jd[b].e,-jd[a].f+jd[b].f,c3/bianbi,-d3/bianbi);//ǰһ��
           	c4=c1;
		    d4=d1;
		  
		   ji(jd[a].e,-jd[a].f,c3*((1-bianbi)/(bianbi*bianbi)),-d3*((1-bianbi)/(bianbi*bianbi)));/*��Ǳ�׼���֧·��ǰ�˶Ե�֧·*/
		   c4+=c1;
		   d4+=d1;
           ji(jd[a].e,jd[a].f,c4,d4);
		   fprintf(fp2,"     �Ǳ�׼�����· %d����%d �Ĺ���Ϊ:   %f",i1,j1,c1);
		   zhilu[i].p=fabs(c1);
		if(d1>=0)
			fprintf(fp2,"+j%f\n",d1);
	    else
	        fprintf(fp2,"-j%f\n",-d1);

		aa[i]+=c1;
		bb[i]+=d1;
			ji(jd[b].e-jd[a].e,-jd[b].f+jd[a].f,c3/bianbi,-d3/bianbi);//��һ��
           	c4=c1;
		    d4=d1;
		   ji(jd[b].e,-jd[b].f,c3*((-1+bianbi)/bianbi),-d3*((-1+bianbi)/bianbi));
		   c4+=c1;
		   d4+=d1;
           ji(jd[b].e,jd[b].f,c4,d4);
		   fprintf(fp2,"     �Ǳ�׼�����· %d����%d �Ĺ���Ϊ:   %f",j1,i1,c1);
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
	 /*------------------------------��·��Ĺ���--------------------------------*/
    fprintf(fp2,"\n\n     ��·��Ĺ���:\n");
	for(i=1;i<=m;i++)
	{
		int i1=zhilu[i].p1;
		int j1=zhilu[i].p2;
		zhilu[i].pl=aa[i];
		zhilu[i].ql=bb[i];
	    fprintf(fp2,"     ��· %d����%d ��ĵĹ���Ϊ:   %f",i1,j1,aa[i]);
		zhilu[i].eff=1-fabs(aa[i])/zhilu[i].p;
		if(bb[i]>=0)
			fprintf(fp2,"+j%f\n",bb[i]);
	    else
			fprintf(fp2,"-j%f\n",-bb[i]);
	}
	 /*------------------------------��������Ĺ���--------------------------------*/		
	fprintf(fp2,"\n\n     ��������Ĺ���Ϊ:   %f",rr);
		if(tt>=0)
			fprintf(fp2,"+j%f\n",tt);
	    else
	        fprintf(fp2,"-j%f\n",tt);
	fprintf(fp2,"\n ==============================================================================\n");	
    fprintf(fp2,"\n\n                  ############ ����������� ##############");
}
void Result_Analysis()
{
	double sum1=0;double sum2=0;
	for(i=1;i<=n;i++)
	{
		if(jd[i].p>=0) sum1+=jd[i].p;
		else sum2+=-jd[i].p;
	}
	fprintf(fp2,"\n                  ############   �������  ###############\n");
	fprintf(fp2,"\n     ������Ч��Ϊ��      %f",sum2/sum1);
	for(i=1;i<=m;i++)
		fprintf(fp2,"\n     ��·%d��Ч���ǣ�     %f",i,zhilu[i].eff);
}

/*######################������������������е��Ӻ������������γ�ѭ������#####################*/
void main()
{
	 introduction();                        //������
	 ReadData();                            //��ȡ����
     Form_Y();                              //�γɽڵ㵼�ɾ���
     Voltage_Initial_Value();               //�ڵ��ѹ����ֵ
     Gauss_Seidel();                        //��˹���¶��������ֵ
	 Newton_Raphson();                      //ţ�١�������ѷ����
	 Powerflow_Result();                    //�������
     Result_Analysis();                     //�������
	 printf("������������н����OutputData.txt�ļ���\n");
 }
