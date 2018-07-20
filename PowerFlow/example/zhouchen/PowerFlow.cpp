 //this code is used to calculate PowerFlow
#include<stdio.h>
#include<math.h>
FILE *fp1,*fp2;		//define file point to read/write data
#define MAX 1000	//define max interation number
#define Pi 355/113   //define Pi

void Read_data();    //read primary data form Input.txt in defult direction
void Form_Y();			
void Calculate_Unbalanced_Para();
void Form_Jacobi_Matrix();
void Solve_Equations();
int Check(double fixx[200]);
void Calculate_Circuit_Power();
void Powerflow_Result();

typedef struct
{
	int No,type;
	double U,delta,P,Q,BN;
} point;              //define point para

typedef struct 
{
	int No,type,n,n1,n2,n3;
	double G,B,R,X,P1,P2,Q1,Q2,P,Q,RR,XX,G1,B1,G2,B2,k;
} line;          //define branch circuit para

int n,a,m,nl,count;     //n: number of point,nl: number of branch circuit,a:number of PQ point,m:numbe of PQ point plus balanced point
double eps;				//eps:computational accuracy 
double NdeltaP,NdeltaQ,LdeltaP,LdeltaQ,eta;//NdeltaX:Net Loss,LdeltaX:Line Loss,eta:efficient
point aa[100];       // aa: panel point
line bb[50];		 // bb: branch circuit
double AA[100][100],BB[100][100];    //AA:RE of Y,BB: IM of Y
double unba[200],fixx[200],J[200][200];  //unba: unbalanced Para,fixx: fix var,J:Jacobi Matrix


void main()
{	
	int i,j,t,qe;
	double maxQ;
	point tt[100];
	printf("=============Powerflow calculation is now runing,please waiting================\n\n");
	printf("---------------------------------------------------------------------\n");
	printf("Object:PowerFlow Calculate                           Status:preparing\n\n");
	Read_data();    //read data from Indata.txt
	printf("Reading data successfully!\n");
	Form_Y();         //form bus admittance matrix Y
	printf("Formming bus admittance matrix successfully!\n");
	count=0;		//initialize count
	printf("Interation now start\n\n");
	printf("---------------------------------------------------------------------\n");
	printf("Object:PowerFlow Calculate                           Status:calculating\n\n");
	printf("if you want to consider the max Reactive Power of PV point ? Y input 1;N input 0\n");
	scanf("%d",&qe);
	if(qe==0)
	{
		for(count=0;count<MAX;count++)     //define max calculation number
		{
			Calculate_Unbalanced_Para();   //calculate unbalanced para deltaQ deltaP 
			Form_Jacobi_Matrix();   //form jacobi matrix
			Solve_Equations();		//solve Modified Equations
			t=Check(fixx);			//check if the result satistify the request of computational accuracy
			if(t==1)			//if satistified break,else continue interation
			{
				printf("The interation is over,total interation time is %d\n\n",count+1);
				break;			
			}
		}
		Calculate_Circuit_Power();   //calculate some para such as Power of balanced panel point
		printf("---------------------------------------------------------------------\n");
		printf("Object:PowerFlow Calculate                           Status:Completed\n\n");
		Powerflow_Result();      //print the PowerFlow calculation report
		printf("The result has formed a report named 'output.txt'in default directory,please op-en and check it!\n\n");
		printf("---------------------------------------------------------------------\n");
		printf("==============Powerflow calculation is over,thank you for use==================\n\n");
		printf("Coded by\n");
		printf("NCEPU(BaoDing)\n");
		printf("Electric Power 1102\n");
		printf("Chen Zhou\n");
		printf("201104000228\n");
	}
	else
	{
		printf("please input the max Reactive Power\n");
		scanf("%lf",&maxQ);  //input maxQ
		for(count=0;count<MAX;count++)     //define max calculation number
		{
			for(i=0;i<n;i++)
			{
				if(aa[i].type==2)
				{
					if(aa[i].Q>maxQ)
					{
						aa[i].Q=maxQ;
						aa[i].type=1;   //transform PV to PQ
						j=0;
						for(i=0;i<n;i++)
						{
							if (aa[i].type==1) //adjust the struct of panel point data,in tt[0]-tt[a-1] is PQ point
							{	
								tt[j++]=aa[i];
							}
						}
						for(i=0;i<n;i++)
						{
							if (aa[i].type==2) //adjust the struct tt[a]-tt[n-2] is PV point
							{
								tt[j++]=aa[i];
							}
						}
						for(i=0;i<n;i++)
						{
							if (aa[i].type==3)
							{
								tt[n-1]=aa[i];		//put balance point to the last
							}
						}
						for(i=0;i<n;i++)
						{
							aa[i]=tt[i];   //return tt[]value to aa[] 
						}
					}
					else
						continue;
				}
			}
			Calculate_Unbalanced_Para();   //calculate unbalanced para deltaQ deltaP 
			Form_Jacobi_Matrix();   //form jacobi matrix
			Solve_Equations();		//solve Modified Equations
			t=Check(fixx);			//check if the result satistify the request of computational accuracy
			if(t==1)			//if satistified break,else continue interation
			{
				printf("The interation is over,total interation time is %d\n\n",count+1);
				break;			
			}
		}
		Calculate_Circuit_Power();   //calculate some para such as Power of balanced panel point
		printf("---------------------------------------------------------------------\n");
		printf("Object:PowerFlow Calculate                           Status:Completed\n\n");
		Powerflow_Result();      //print the PowerFlow calculation report
		printf("The result has formed a report named 'output.txt'in default directory,please op-en and check it!\n\n");
		printf("---------------------------------------------------------------------\n");
		printf("==============Powerflow calculation is over,thank you for use==================\n\n");
		printf("Coded by\n");
		printf("NCEPU(BaoDing)\n");
		printf("Electric Power 1102\n");
		printf("Chen Zhou\n");
		printf("201104000228\n");
	}

}

void Read_data() 
{	
	int i,j;  //i j :count varity
	point tt[100];  //t tt[]:temporary container
	fp1=fopen("Input.txt","r");  //open the file 
	fscanf(fp1,"%d,%d,%d,%lf\n",&n,&nl,&a,&eps);  //read data from area No.1
	for(i=0;i<n;i++)
	{
		aa[i].BN=0;  //initialize BN
	}
	for(i=0;i<n;i++)
	{
		fscanf(fp1,"%d,%d,%lf,%lf,%lf,%lf\n",&aa[i].No,&aa[i].type,&aa[i].P,&aa[i].Q,&aa[i].U,&aa[i].delta);
	}                                             //read data from area No.2
	m=a+1;     //calculate number of PV point
	for(i=0;i<nl;i++)
	{
		fscanf(fp1,"%d,%d",&bb[i].No,&bb[i].type);  //read circuit's Number and type
		
		switch(bb[i].type)   //read data according to it's type
		{
		case 1:   //Ordinary branch 
			{	
				fscanf(fp1,",%d,%d,%lf,%lf\n",&bb[i].n,&bb[i].n1,&bb[i].R,&bb[i].X); 
				break;
			}
		case 2:   //Double circuit line
			{
				fscanf(fp1,",%d,%d,%lf,%lf,%lf,%lf\n",&bb[i].n,&bb[i].n1,&bb[i].R,&bb[i].X,&bb[i].RR,&bb[i].XX);
				break;
			}
		case 3:	  //Non standard ratio transformer branch
			{
				fscanf(fp1,",%d,%d,%d,%d,%lf,%lf,%lf\n",&bb[i].n,&bb[i].n1,&bb[i].n2,&bb[i].n3,&bb[i].k,&bb[i].R,&bb[i].X);
				break;
			}
		case 4:	  //Earthing branch
			{
				fscanf(fp1,",%d,%d,%lf,%lf",&bb[i].n,&bb[i].n1,&bb[i].R,&bb[i].X);
				fscanf(fp1,",%lf,%lf",&aa[bb[i].n].BN,&aa[bb[i].n1].BN);
				break;
			}
		default:
			break;
		}
	}
	fclose(fp1);  //close the file 
	j=0;
	for(i=0;i<n;i++)
	{
		if (aa[i].type==1) //adjust the struct of panel point data,in tt[0]-tt[a-1] is PQ point
		{	
			tt[j++]=aa[i];
		}
	}
	for(i=0;i<n;i++)
	{
		if (aa[i].type==2) //adjust the struct tt[a]-tt[n-2] is PV point
		{
			tt[j++]=aa[i];
		}
	}
	for(i=0;i<n;i++)
	{
		if (aa[i].type==3)
		{
			tt[n-1]=aa[i];		//put balance point to the last
		}
	}
	for(i=0;i<n;i++)
	{
		aa[i]=tt[i];   //return tt[]value to aa[] 
	}
	for(i=0;i<2*(n-1);i++)
	{
		fixx[i]=0;      //Initialize fix value
	}
	fp2=fopen("Output.txt","w");
	fprintf(fp2,"Power System Analization Starts Successfully\n\n-------------------------------------------Test No.67 Computer No.A072\n");
	fprintf(fp2,"NCEPU(Baoding)\n");
	fprintf(fp2,"Electric Power 1102\n");
	fprintf(fp2,"Chen Zhou\n");
	fprintf(fp2,"201104000228\n");
	fprintf(fp2,"-----------------------------------------------------------------------\n");
	fprintf(fp2,"\nObject: PowerFlow Calculate                         Status: preparing\n\n");
	fprintf(fp2,"===============now print primary data information===============\n\n");
	fprintf(fp2,"****************primary power system information****************\n\n");
	fprintf(fp2,"number of panel point:  %d\n",n);
	fprintf(fp2,"number of branch circuit:   %d\n",nl);
	fprintf(fp2,"number of PQ panel point:  %d\n",a);
	fprintf(fp2,"number of PV panel point:  %d\n",n-m);
	fprintf(fp2,"\n*****************primary panel point information****************\n\n");
	for(i=0;i<n;i++)
	{	
		if(aa[i].type==1) //PQ point 
		{
			fprintf(fp2,"Type:%d  Point number:%d  Active Power:%10.6lf  Reactive Power:%10.6lf\n",aa[i].type,aa[i].No,aa[i].P,aa[i].Q);
		}
		else if(aa[i].type==2)  //PV point 
		{
			fprintf(fp2,"Type:%d  Point number:%d  Active Power:%10.6lf  Voltage mode value:%10.6lf\n",aa[i].type,aa[i].No,aa[i].P,aa[i].U);
		}
		else			//balanced point
		{
			fprintf(fp2,"Type:%d  Point number:%d  Voltage mode value:%10.6lf  Voltage Phase angle:%10.6lf\n",aa[i].type,aa[i].No,aa[i].U,aa[i].delta);
		}
	}
	fprintf(fp2,"\n*******************branch circuit information*******************\n\n");
	for(i=0;i<nl;i++)
	{
		switch(bb[i].type)  //detective branch's type
		{
		case 1:    //ordinary branch
		{
			fprintf(fp2,"Branch circuit number:%d  Type:%d  Conneted Point1:%d  Connected Point2:%d  Resistance:%lf  Reactance:j%lf  \n",bb[i].No,bb[i].type,bb[i].n,bb[i].n1,bb[i].R,bb[i].X);
			break;
		}
		case 2:   //Double circuit line
			{
				fprintf(fp2,"Branch circuit number:%d  Type:%d  Conneted Point1:%d  Connected Point2:%d  Resistance 1 :%lf  Reactance 1 :j%lf Resistance 2 :%lf  Reactance 2 :j%lf  \n",bb[i].No,bb[i].type,bb[i].n,bb[i].n1,bb[i].R,bb[i].X,bb[i].RR,bb[i].XX);
				break;
			}
		case 3:	  //Non standard ratio transformer branch
			{
				fprintf(fp2,"Branch circuit number:%d  Type:%d  Conneted Point1:%d  Connected Point2:%d  Transformer is close to point:%d  High Side:%d  Transformation Ratio:%lf  Resistance:%lf  Reactance:j%10.6lf\n",bb[i].No,bb[i].type,bb[i].n,bb[i].n1,bb[i].n2,bb[i].n3,bb[i].k,bb[i].R,bb[i].X);
				break;
			}
		case 4:	  //Earthing branch
			{
				fprintf(fp2,"Branch circuit number:%d  Type:%d  Conneted Point1:%d  Connected Point2:%d  Resistance:%lf  Reactance:j%lf  Earthing Susceptance of Point %d:j%lf  Earthing Susceptance of Point %d:j%lf\n",bb[i].No,bb[i].type,bb[i].n,bb[i].n1,bb[i].R,bb[i].X,bb[i].n,aa[bb[i].n].BN,bb[i].n1,aa[bb[i].n1].BN);
				break;
			}
		default:
			break;
		}
	}
}

void Form_Y()
{
	int i,j,noe,ep[100];//i j:count varity,noe ep[]:temporary container for calculation of self-admittance
	noe=0; //initialize noe
	for(i=0;i<n;i++)
	{
		switch(bb[i].type)  //detective branch's type
		{
		case 1:  //Ordinary Branch
			{	
				bb[i].G=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2));
				bb[i].B=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2));    //transform Z to Y
				break;
			}
		case 2:	 //Double circuit line
			{
				bb[i].G=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2))+(bb[i].RR)/(pow(bb[i].RR,2)+pow(bb[i].XX,2));
				bb[i].B=(-bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2))+(-bb[i].XX)/(pow(bb[i].RR,2)+pow(bb[i].XX,2));		//parallel connection
				break;
			}
		case 3:  //Non standard ratio transformer branch
			{
				if(bb[i].n2==bb[i].n&&bb[i].n3==bb[i].n1)
				{
					bb[i].G=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2))*bb[i].k;
					bb[i].B=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2))*bb[i].k;
					bb[i].G1=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(pow(bb[i].k,2)-bb[i].k);
					bb[i].B1=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(pow(bb[i].k,2)-bb[i].k);
					bb[i].G2=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(1-bb[i].k);
					bb[i].B2=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(1-bb[i].k);
				}
				else if (bb[i].n2==bb[i].n&&bb[i].n3==bb[i].n)
				{
					bb[i].k=1/bb[i].k;
					bb[i].G=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2))*bb[i].k;
					bb[i].B=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2))*bb[i].k;
					bb[i].G1=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(pow(bb[i].k,2)-bb[i].k);
					bb[i].B1=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(pow(bb[i].k,2)-bb[i].k);
					bb[i].G2=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(1-bb[i].k);
					bb[i].B2=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(1-bb[i].k);
				}
				else if (bb[i].n2==bb[i].n1&&bb[i].n3==bb[i].n)
				{
					bb[i].G=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2))*bb[i].k;
					bb[i].B=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2))*bb[i].k;
					bb[i].G2=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(pow(bb[i].k,2)-bb[i].k);
					bb[i].B2=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(pow(bb[i].k,2)-bb[i].k);
					bb[i].G1=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(1-bb[i].k);
					bb[i].B1=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(1-bb[i].k);
				}
				else 
				{
					bb[i].k=1/bb[i].k;
					bb[i].G=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2))*bb[i].k;
					bb[i].B=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2))*bb[i].k;
					bb[i].G2=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(pow(bb[i].k,2)-bb[i].k);
					bb[i].B2=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(pow(bb[i].k,2)-bb[i].k);
					bb[i].G1=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(1-bb[i].k);
					bb[i].B1=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2))*(1-bb[i].k);
				}
				break;
			}
		case 4: //Earthing branch
			{
				bb[i].G=(bb[i].R)/(pow(bb[i].R,2)+pow(bb[i].X,2));
				bb[i].B=-(bb[i].X)/(pow(bb[i].R,2)+pow(bb[i].X,2));    //transform Z to Y
				break;
			}
		default:
			break;
		}
	}

	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			AA[i][j]=0;
			BB[i][j]=0;    //Initialize Matrix Y
		}
	}

	for(i=0;i<nl;i++)
	{
		if (bb[i].type==1||bb[i].type==2) //Ordinary branch and Double circuit line 
		{
			AA[bb[i].n-1][bb[i].n1-1]=-bb[i].G;
			BB[bb[i].n-1][bb[i].n1-1]=-bb[i].B;  //add mutual admittance

			for(j=0;j<n;j++)
			{
				if(bb[i].n-1==j||bb[i].n1-1==j)
				{	
					AA[j][j]+=bb[i].G;
					BB[j][j]+=bb[i].B;				//add self-admittance
				}  
			}
		}
		else if (bb[i].type==3)			//Non standard ratio transformer branch
		{
			AA[bb[i].n-1][bb[i].n1-1]=-bb[i].G;
			BB[bb[i].n-1][bb[i].n1-1]=-bb[i].B;  //add mutual admittance

			for(j=0;j<n;j++)
			{
				if(bb[i].n-1==j)
				{	
					AA[j][j]+=bb[i].G+bb[i].G1;
					BB[j][j]+=bb[i].B+bb[i].B1;				//add self-admittance
				}  
				else if (bb[i].n1-1==j)
				{	
					AA[j][j]+=bb[i].G+bb[i].G2;
					BB[j][j]+=bb[i].B+bb[i].B2;				//add self-admittance
				}  
				else
					continue;
			}
		}
		else if (bb[i].type==4)     //Earthing branch 
		{
			ep[noe++]=bb[i].n;
			ep[noe++]=bb[i].n1;
			AA[bb[i].n-1][bb[i].n1-1]=-bb[i].G;
			BB[bb[i].n-1][bb[i].n1-1]=-bb[i].B;     //add mutual admittance
			for(j=0;j<n;j++)
			{
				if(bb[i].n-1==j||bb[i].n1-1==j)
				{	
					AA[j][j]+=bb[i].G;
					BB[j][j]+=bb[i].B;				//add self-admittance
				}
			}
		}
	}
	if(noe!=0)					//add B1 B2 to self-admittance connected with Earthing Point
	{

		for(i=0;i<noe;i++)
		{
			for(j=i+1;j<noe;j++)
			{
				if(ep[i]==ep[j])
				{	
					ep[j]=0;
				}
			}
		}
		j=0;
		for(i=0;i<noe;i++)
		{
			if(ep[i]!=0)
			{
				ep[j++]=ep[i];
			}
		}
		for(i=0;i<j;i++)
		{
			BB[ep[i]-1][ep[i]-1]+=aa[ep[i]].BN;
		}
	}

	for(i=0;i<n;i++)  //the Y is a symmetric matrix
	{
		for(j=0;j<n;j++)
		{
				AA[j][i]=AA[i][j];   
				BB[j][i]=BB[i][j];
		}
	}

	fprintf(fp2,"\n=================now print bus admittance matrix Y==============\n\n");
	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
		{
			fprintf(fp2,"%10.6lf+j%10.6lf  ",AA[i][j],BB[i][j]);//print Y 
		}
		fprintf(fp2,"\n");
	}
}

void Calculate_Unbalanced_Para()
{
	int i,j;  //i j:count varity
	double t1,t2;  //t1 t2:temporary container
	for(i=0;i<m-1;i++)        //add fix value to PQ point
	{
		aa[i].delta+=fixx[2*i];
		aa[i].U=(fixx[2*i+1]+1)*aa[i].U;
	}
	for(i=m-1;i<n-1;i++)		//add fix value to PV point
	{
		aa[i].delta+=fixx[2*i];
	}
	

	for(i=0;i<n;i=i++)
	{
		t1=0;t2=0;              //intialize container t1 t2
		if(aa[i].type==1)  // it's a PQ point 
		{
			for(j=0;j<n;j++)
			{
				t1+=aa[j].U*(AA[aa[i].No-1][aa[j].No-1]*cos(aa[i].delta-aa[j].delta)+BB[aa[i].No-1][aa[j].No-1]*sin(aa[i].delta-aa[j].delta));
				t2+=aa[j].U*(AA[aa[i].No-1][aa[j].No-1]*sin(aa[i].delta-aa[j].delta)-BB[aa[i].No-1][aa[j].No-1]*cos(aa[i].delta-aa[j].delta));
			}
			unba[2*i]=aa[i].P-aa[i].U*t1;
			unba[2*i+1]=aa[i].Q-aa[i].U*t2;
		}
		else if (aa[i].type==2)  //it's a PV point 
		{
			for(j=0;j<n;j++)\
			{
				t1+=aa[j].U*(AA[aa[i].No-1][aa[j].No-1]*cos(aa[i].delta-aa[j].delta)+BB[aa[i].No-1][aa[j].No-1]*sin(aa[i].delta-aa[j].delta));
			}
			unba[2*i]=aa[i].P-aa[i].U*t1;
		}
		else 
		continue;
	}
	if (count<4)  //print the result in the first 4 interation 
	{
		fprintf(fp2,"\n========================iteration number=%d========================\n",count+1);
		fprintf(fp2,"\nObject: PowerFlow Calculate                         Status: calculating\n");
		fprintf(fp2,"\n*******************now print unbalanced para****************\n\n");
		fprintf(fp2,"point No.     point type     value\n");
		for (i=0;i<n-1;i++)
		{
			if (aa[i].type==1)
			{
				fprintf(fp2,"%d               %d            %10.6lf\n%d               %d            %10.6lf\n",aa[i].No,aa[i].type,unba[2*i],aa[i].No,aa[i].type,unba[2*i+1]);
			}
			else if(aa[i].type==2)
			{
				fprintf(fp2,"%d               %d            %10.6lf\n",aa[i].No,aa[i].type,unba[2*i]);
			}
		}
	}
}

void Form_Jacobi_Matrix()
{
	int i,j,k; //i j k:counter varity
	double t1,t2;  //t1 t2:temporary container
	for(i=0;i<n-1;i++)
	{
		if (aa[i].type==1)  //it's a PQ point,means to calculate H J N L para.
		{
			for(j=0;j<n-1;j++)
			{
				if (i==j)     //when i equals to j
				{
					t1=0;t2=0; //initialize counter t1,t2
					for(k=0;k<n;k++)
					{
						if (k!=i)
						{
							t1+=aa[k].U*(AA[aa[i].No-1][aa[k].No-1]*sin(aa[i].delta-aa[k].delta)-BB[aa[i].No-1][aa[k].No-1]*cos(aa[i].delta-aa[k].delta));
							t2+=aa[k].U*(AA[aa[i].No-1][aa[k].No-1]*cos(aa[i].delta-aa[k].delta)+BB[aa[i].No-1][aa[k].No-1]*sin(aa[i].delta-aa[k].delta));
						}
						else 
							continue;
					}
					J[2*i][2*i]=-aa[i].U*t1;   //when i equals to j  value of Hii
					J[2*i+1][2*i]=aa[i].U*t2;   //when i equals to j  value of Jii
					J[2*i][2*i+1]=aa[i].U*t2+2*pow(aa[i].U,2)*AA[aa[i].No-1][aa[i].No-1];   //when i equals to j  value of Nii
					J[2*i+1][2*i+1]=aa[i].U*t1-2*pow(aa[i].U,2)*BB[aa[i].No-1][aa[i].No-1];  //when i equals to j  value of Lii
				}
				else  //when i isn't equal to j
				{
					if (aa[j].type==1)  //if j is a PQ point,means to calculate H J N L 
					{
						t1=AA[aa[i].No-1][aa[j].No-1]*sin((aa[i].delta-aa[j].delta))-BB[aa[i].No-1][aa[j].No-1]*cos((aa[i].delta-aa[j].delta));
						t2=AA[aa[i].No-1][aa[j].No-1]*cos((aa[i].delta-aa[j].delta))+BB[aa[i].No-1][aa[j].No-1]*sin((aa[i].delta-aa[j].delta));
						J[2*i][2*j]=aa[i].U*aa[j].U*t1;   //when i isn't equals to j   value of Hij
						J[2*i+1][2*j]=-aa[i].U*aa[j].U*t2;   //when i isn't equals to j   value of Jij
						J[2*i][2*j+1]=aa[i].U*aa[j].U*t2;   //when i isn't equals to j  value of Nij
						J[2*i+1][2*j+1]=aa[i].U*aa[j].U*t1;  //when i isn't equals to jvalue of Lij
					}
					else if (aa[j].type==2)  //if j is a PV point,means to calculate H,J
					{
						t1=AA[aa[i].No-1][aa[j].No-1]*sin((aa[i].delta-aa[j].delta))-BB[aa[i].No-1][aa[j].No-1]*cos((aa[i].delta-aa[j].delta));
						t2=AA[aa[i].No-1][aa[j].No-1]*cos((aa[i].delta-aa[j].delta))+BB[aa[i].No-1][aa[j].No-1]*sin((aa[i].delta-aa[j].delta));
						J[2*i][a+j]=aa[i].U*aa[j].U*t1;   //when i isn't equals to j   value of Hij
						J[2*i+1][a+j]=-aa[i].U*aa[j].U*t2;   //when i isn't equals to j   value of Jij
	
					}
				}
			}
		}
		else if (aa[i].type==2)  //it's a PV point,means to calculate H N  para.
		{
			for (j=0;j<n-1;j++)
			{
				if (i==j)     //when i equals to j
				{
					t1=0; //initialize counter t1
					for(k=0;k<n;k++)
					{
						if (k!=i)
						{	
							t1+=aa[k].U*(AA[aa[i].No-1][aa[k].No-1]*sin((aa[i].delta-aa[k].delta))-BB[aa[i].No-1][aa[k].No-1]*cos(aa[i].delta-aa[k].delta));
						}
						else 
							continue;
					}
					J[a+i][a+i]=-aa[i].U*t1;   //when i equals to j  value of Hii
//					J[2*i+1][2*i]=0;   //when i equals to j  value of Jii
//					J[2*i][2*i+1]=0;   //when i equals to j  value of Nii
//					J[2*i+1][2*i+1]=0;  //when i equals to j  value of Lii equals to 0
				}
				else 
				{
					t1=AA[aa[i].No-1][aa[j].No-1]*sin((aa[i].delta-aa[j].delta))-BB[aa[i].No-1][aa[j].No-1]*cos((aa[i].delta-aa[j].delta));
					t2=AA[aa[i].No-1][aa[j].No-1]*cos((aa[i].delta-aa[j].delta))+BB[aa[i].No-1][aa[j].No-1]*sin((aa[i].delta-aa[j].delta));
					J[a+i][2*j]=aa[i].U*aa[j].U*t1;   //when i isn't equals to j   value of Hij
					J[a+i][2*j+1]=aa[i].U*aa[j].U*t2;   //when i isn't equals to j  value of Nij
				}
			}
		}
		else
			continue;
	}

	if (count<4)  //print the result in thefirst 4 interation
	{
		fprintf(fp2,"\n********************now print Jacobi_Matrix*****************\n\n");
		for (i=0;i<n+m-2;i++)
		{
			for (j=0;j<n+m-2;j++)
			{
				fprintf(fp2,"%10.6lf   ",J[i][j]);
			}
			fprintf(fp2,"\n");
		}
	}
}

void Solve_Equations()
{
	// use L-U to solve equations
	int i,j,k,N,r;   //i,j,k,r:counter varity N:size of Jacobi Matrix
	double s,y[200];	//s y[]:temporary container 
	double L[200][200],U[200][200];   //L U matrix

	N=n+m-2;   //get the value of N
	//form U,L
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)  
		{
			U[i][j]=0;
			L[i][j]=0;
			U[0][i]=J[0][i];
			L[i][0]=J[i][0]/U[0][0];
		}
	}

	for(r=1;r<N;r++)
	{		
		for(i=r;i<N;i++)
		{
			s=0;
			for(k=0;k<r;k++)
			{
				s+=L[r][k]*U[k][i];
			}
			U[r][i]=J[r][i]-s;
		}

		for(i=r+1;i<N;i++)
		{
			s=0;
			for(k=0;k<r;k++)
			{
				s+=L[i][k]*U[k][r];
			}
			L[i][r]=(J[i][r]-s)/U[r][r];
		}
	}

	for(i=0;i<N;i++)
	{
		L[i][i]=1;
	}

	//solve equations
	for(i=0;i<N;i++)
	{
		s=0.0;
		for(k=0;k<i;k++)
		{
			s+=L[i][k]*y[k];
		}
		y[i]=unba[i]-s;
	 }
	for(i=N-1;i>=0;i--)
	{
		s=0.0;
		for(k=i+1;k<N;k++)
		{ 
			s+=U[i][k]*fixx[k];
		}
		fixx[i]=(y[i]-s)/U[i][i];
	}

	if(count<4)    //print the result in thefirst 4 interation
	{
		fprintf(fp2,"\n===============now use L-U to solve fix equations================\n\n");
		fprintf(fp2,"***************************U Matrix*************************\n");
		for(i=0;i<N;i++)
		{
			fprintf(fp2,"\n");
			for(j=0;j<N;j++)
			  fprintf(fp2,"%10.6lf",U[i][j]);
		 }
	
		fprintf(fp2,"\n\n***************************L Matrix*************************\n");
		for(i=0;i<N;i++)
		{
			fprintf(fp2,"\n");
			for(j=0;j<N;j++)
			fprintf(fp2,"%10.6lf",L[i][j]);
		}
		fprintf(fp2,"\n\n****************the result of the equations*****************\n\n");
		for(i=0;i<N;i++)
		{
			fprintf(fp2,"%10.6lf\n",fixx[i]);
		}
	}
}

int Check(double fixx[200])
{
	int i;  //i:counter varity
	double maxU,maxdelta;  //maxU maxdelta:the max of ...
	//find fabs(max delta) and abs(maxU)
	maxdelta=fabs(fixx[0]);
//	maxU=fabs(fixx[1]);
	maxU=fabs(fixx[1]*aa[0].U);
	for(i=2;i<2*(n-1);i=i+2)
	{
		if(fabs(fixx[i])>maxdelta)
		{
			maxdelta=fabs(fixx[i]);
		}
		if(fabs(fixx[i+1]*aa[i+1].U)>maxU)
//		if(fabs(fixx[i+1])>maxU)
		{
			maxU=fabs(fixx[i+1]);
		}
	}
	if((maxdelta<=eps)&&(maxU<=eps))   //judge if the max value satistify the request of computational accuracy
		return 1;
	else
		return 0;
}
void Calculate_Circuit_Power()
{
	int i,j,k,ss;  //i j k:counter varity
	double t1,t2,T[6],ReBN; //t1 t2:temporary container in calculating panel point's Power and efficient
						//T[]:temporary container in calculating branch's Power
	ReBN=0;//Reactive Power of BN;
	for(i=0;i<n;i++)   
	{
		if(aa[i].type==2)   //calculate PV point's Reactive Power
		{
			t2=0;   //initialize t2
			for(j=0;j<n;j++)
			{
				if(i!=j)
				{
					t2+=aa[j].U*(AA[aa[i].No-1][aa[j].No-1]*sin((aa[i].delta-aa[j].delta))-BB[aa[i].No-1][aa[j].No-1]*cos((aa[i].delta-aa[j].delta)));
				}
			}
			aa[i].Q=-pow(aa[i].U,2)*BB[aa[i].No-1][aa[i].No-1]+aa[i].U*t2;
		}
		if(aa[i].type==3)  //calculate balanced point's Active Power and Reactive Power
		{
			t1=0;t2=0;   //initialize t1 t2
			for(j=0;j<n;j++)
			{
				t1+=aa[j].U*(AA[aa[i].No-1][aa[j].No-1]*cos((aa[i].delta-aa[j].delta))+BB[aa[i].No-1][aa[j].No-1]*sin((aa[i].delta-aa[j].delta)));
				t2+=aa[j].U*(AA[aa[i].No-1][aa[j].No-1]*sin((aa[i].delta-aa[j].delta))-BB[aa[i].No-1][aa[j].No-1]*cos((aa[i].delta-aa[j].delta)));
			}
			aa[i].P=aa[i].U*t1;
			aa[i].Q=aa[i].U*t2;
		}
	}

	//calculate branch circuit para

	for(k=0;k<nl;k++)
	{
		if(bb[k].type==1||bb[k].type==2)   //Ordinary branch or Double circuit line
		{
			i=bb[k].n;
			j=bb[k].n1;
			for(ss=0;ss<n;ss++)       //find the primary No in aa[]
			{	
				if(aa[ss].No==i)
				{
					i=ss;
					break;
				}
			}
			for(ss=0;ss<n;ss++)
			{	
				if(aa[ss].No==j)
				{
					j=ss;
					break;
				}
			}
			if(i>j)
			{
				ss=i;
				i=j;
				j=ss;
			}
			T[0]=aa[i].U*cos(aa[i].delta);
			T[1]=aa[i].U*sin(aa[i].delta);
			T[2]=T[0]-aa[j].U*cos(aa[j].delta);
			T[3]=T[1]-aa[j].U*sin(aa[j].delta);
			T[4]=bb[k].G;
			T[5]=bb[k].B;
			bb[k].P1=(T[0]*T[2]+T[1]*T[3])*T[4]-(T[0]*T[3]-T[1]*T[2])*T[5];
			bb[k].Q1=-((T[0]*T[2]+T[1]*T[3])*T[5]+(T[0]*T[3]-T[1]*T[2])*T[4]);
			T[0]=aa[j].U*cos(aa[j].delta);
			T[1]=aa[j].U*sin(aa[j].delta);
			T[2]=T[0]-aa[i].U*cos(aa[i].delta);
			T[3]=T[1]-aa[i].U*sin(aa[i].delta);
			T[4]=bb[k].G;
			T[5]=bb[k].B;
			bb[k].P2=(T[0]*T[2]+T[1]*T[3])*T[4]-(T[0]*T[3]-T[1]*T[2])*T[5];
			bb[k].Q2=-((T[0]*T[2]+T[1]*T[3])*T[5]+(T[0]*T[3]-T[1]*T[2])*T[4]);
			bb[k].P=bb[k].P1+bb[k].P2;
			bb[k].Q=bb[k].Q1+bb[k].Q2;
		}
		if(bb[k].type==3) //Non standard ratio transformer branch
		{ 
			i=bb[k].n;
			j=bb[k].n1;
			for(ss=0;ss<n;ss++)       //find the primary No in aa[]
			{	
				if(aa[ss].No==i)
				{
					i=ss;
					break;
				}
			}
			for(ss=0;ss<n;ss++)
			{	
				if(aa[ss].No==j)
				{
					j=ss;
					break;
				}
			}
			if(i>j)
			{
				ss=i;
				i=j;
				j=ss;
			}
			T[0]=aa[i].U*cos(aa[i].delta);
			T[1]=aa[i].U*sin(aa[i].delta);
			T[2]=T[0]-aa[j].U*cos(aa[j].delta);
			T[3]=T[1]-aa[j].U*sin(aa[j].delta);
			T[4]=bb[k].G;
			T[5]=bb[k].B;
			bb[k].P1=pow(aa[i].U,2)*(bb[k].G1)+(T[0]*T[2]+T[1]*T[3])*T[4]-(T[0]*T[3]-T[1]*T[2])*T[5];
			bb[k].Q1=-pow(aa[i].U,2)*(bb[k].B1+aa[i].BN)-((T[0]*T[2]+T[1]*T[3])*T[5]+(T[0]*T[3]-T[1]*T[2])*T[4]);
			T[0]=aa[j].U*cos(aa[j].delta);
			T[1]=aa[j].U*sin(aa[j].delta);
			T[2]=T[0]-aa[i].U*cos(aa[i].delta);
			T[3]=T[1]-aa[i].U*sin(aa[i].delta);
			T[4]=bb[k].G;
			T[5]=bb[k].B;
			bb[k].P2=pow(aa[j].U,2)*(bb[k].G2)+(T[0]*T[2]+T[1]*T[3])*T[4]-(T[0]*T[3]-T[1]*T[2])*T[5];
			bb[k].Q2=-pow(aa[j].U,2)*(bb[k].B2+aa[j].BN)-((T[0]*T[2]+T[1]*T[3])*T[5]+(T[0]*T[3]-T[1]*T[2])*T[4]);
			bb[k].P=bb[k].P1+bb[k].P2;
			bb[k].Q=bb[k].Q1+bb[k].Q2;
		}
		else if(bb[k].type==4)  //Earthing branch
		{ 
			i=bb[k].n;
			j=bb[k].n1;
			for(ss=0;ss<n;ss++)       //find the primary No in aa[]
			{	
				if(aa[ss].No==i)
				{
					i=ss;
					break;
				}
			}
			for(ss=0;ss<n;ss++)
			{	
				if(aa[ss].No==j)
				{
					j=ss;
					break;
				}
			}
			if(i>j)
			{
				ss=i;
				i=j;
				j=ss;
			}
			T[0]=aa[i].U*cos(aa[i].delta);
			T[1]=aa[i].U*sin(aa[i].delta);
			T[2]=T[0]-aa[j].U*cos(aa[j].delta);
			T[3]=T[1]-aa[j].U*sin(aa[j].delta);
			T[4]=bb[k].G;
			T[5]=bb[k].B;
			bb[k].P1=(T[0]*T[2]+T[1]*T[3])*T[4]-(T[0]*T[3]-T[1]*T[2])*T[5];
			bb[k].Q1=-pow(aa[i].U,2)*(aa[i].BN)-((T[0]*T[2]+T[1]*T[3])*T[5]+(T[0]*T[3]-T[1]*T[2])*T[4]);
			T[0]=aa[j].U*cos(aa[j].delta);
			T[1]=aa[j].U*sin(aa[j].delta);
			T[2]=T[0]-aa[i].U*cos(aa[i].delta);
			T[3]=T[1]-aa[i].U*sin(aa[i].delta);
			T[4]=bb[k].G;
			T[5]=bb[k].B;
			bb[k].P2=(T[0]*T[2]+T[1]*T[3])*T[4]-(T[0]*T[3]-T[1]*T[2])*T[5];
			bb[k].Q2=-pow(aa[j].U,2)*(aa[j].BN)-((T[0]*T[2]+T[1]*T[3])*T[5]+(T[0]*T[3]-T[1]*T[2])*T[4]);
			bb[k].P=bb[k].P1+bb[k].P2;
			bb[k].Q=bb[k].Q1+bb[k].Q2;
		}
	}
	if(bb[0].type==2)
	{
		ReBN=-pow(aa[2].U,2)*(aa[3].BN);
	}
	NdeltaP=0;  //initilize NdeltaX,LdeltaX,t1,t2
	NdeltaQ=0;
	LdeltaP=0;
	LdeltaQ=0;
	t1=0;
	t2=0;
	for(i=0;i<n;i++)
	{
		NdeltaP+=aa[i].P;
		NdeltaQ+=aa[i].Q;    //calculate Net Loss
		if(aa[i].P<0)
		{
			t1+=aa[i].P;  //t1:output power
		}
		else 
		{
			t2+=aa[i].P;  //t2:input power
		}
	}
	eta=fabs((t1/t2)*100); //calculate efficient
	for(i=0;i<nl;i++)
	{
		LdeltaP+=bb[i].P;
		LdeltaQ+=bb[i].Q;   //calculate Line Loss
	}
	LdeltaQ-=ReBN;
}

void Powerflow_Result()
{
	int i;
	//the interation is over,now to print the final result
	fprintf(fp2,"\n==============the interation is over,now to print calculate report=================\n");
	fprintf(fp2,"\nObject: PowerFlow Calculate                         Status: Completed\n");
	fprintf(fp2,"\nthe total interation number is %d\n",count+1);

	fprintf(fp2,"\n******************now print the Voltage of every panel point***********************\n\n");
	
	for(i=0;i<n;i++)
	{
		fprintf(fp2,"panel point No:%d  panel point type:%d  final Voltage mode value:%10.6lf  final Voltage Phase Angle:%10.6lf\n",aa[i].No,aa[i].type,aa[i].U,aa[i].delta);
	}

	fprintf(fp2,"\n*******************now print the Power of every panel point************************\n\n");
	for(i=0;i<n;i++)
	{
		fprintf(fp2,"panel point No:%d  panel point type:%d  final Active Power:%10.6lf  final Reactive Power:%10.6lf\n",aa[i].No,aa[i].type,aa[i].P,aa[i].Q);
	}
	
	fprintf(fp2,"\n*****************now print the branch circuit Power distribution*******************\n\n");
	for(i=0;i<nl;i++)
	{
		switch(bb[i].type) //detective branch's type
		{
			case 1:  //Ordinary branch 
				{
					fprintf(fp2,"usual circuit NO:%d,connected with point %d--%d,it's apparent power is %10.6lf+j%10.6lf\n",bb[i].No,bb[i].n,bb[i].n1,bb[i].P1,bb[i].Q1);
					fprintf(fp2,"usual circuit NO:%d,connected with point %d--%d,it's apparent power is %10.6lf+j%10.6lf\n",bb[i].No,bb[i].n1,bb[i].n,bb[i].P2,bb[i].Q2);		
					break;
				}
			case 2:  //double circuit line
				{
					fprintf(fp2,"double circuit line NO:%d,connected with point %d--%d,it's apparent power is %10.6lf+j%10.6lf\n",bb[i].No,bb[i].n,bb[i].n1,bb[i].P1,bb[i].Q1);
					fprintf(fp2,"double circuit line NO:%d,connected with point %d--%d,it's apparent power is %10.6lf+j%10.6lf\n",bb[i].No,bb[i].n1,bb[i].n,bb[i].P2,bb[i].Q2);		
					break;
				}
			case 3:  //Non standard ratio transformer branch
				{
					fprintf(fp2,"Non standard ratio transformer branch NO:%d,connected with point %d--%d,it's apparent power is %10.6lf+j%10.6lf\n",bb[i].No,bb[i].n,bb[i].n1,bb[i].P1,bb[i].Q1);
					fprintf(fp2,"Non standard ratio transformer branch NO:%d,connected with point %d--%d,it's apparent power is %10.6lf+j%10.6lf\n",bb[i].No,bb[i].n1,bb[i].n,bb[i].P2,bb[i].Q2);		
					break;
				}
			case 4:  //Earthing branch
				{
					fprintf(fp2,"Earthing branch NO:%d,connected with point %d--%d,it's apparent power is %10.6lf+j%10.6lf\n",bb[i].No,bb[i].n,bb[i].n1,bb[i].P1,bb[i].Q1);
					fprintf(fp2,"Earthing branch NO:%d,connected with point %d--%d,it's apparent power is %10.6lf+j%10.6lf\n",bb[i].No,bb[i].n1,bb[i].n,bb[i].P2,bb[i].Q2);		
					break;
				}
			default :
				break;
		}
	}

	fprintf(fp2,"\n********************now print the branch circuit Power Loss************************\n\n");
	for(i=0;i<nl;i++)
	{
		fprintf(fp2,"branch circuit NO:%d,connected with point %d--%d,it's Power Loss is %10.6lf+j%10.6lf\n",bb[i].No,bb[i].n,bb[i].n1,bb[i].P,bb[i].Q);
	}

	fprintf(fp2,"\n**********************now print the total Net para*********************************\n\n");
	fprintf(fp2,"total Net Loss:%10.6lf+j%10.6lf\n",NdeltaP,NdeltaQ);
	fprintf(fp2,"total Line Loss:%10.6lf+j%10.6lf\n",LdeltaP,LdeltaQ);
	fprintf(fp2,"total Net Efficient:%10.6lf%%\n",eta);

	fprintf(fp2,"\n------------------------Power System Analization Over-------------------------------");
	fclose(fp2);
}

		



