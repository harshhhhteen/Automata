#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

char *E=NULL;

typedef struct
{
	char st[10];
}state;

state *Q=NULL;

typedef struct
{
	int size;
	int *ss;
}node;

node *delta=NULL, f, *closee=NULL, *newState=NULL, nf;

int e, q, epsilon=-1, q0, nq=1, *newDelta=NULL, yolo=0;;

void nfaSymbolsInput()
{
	puts("enter the number of symbols in the NFA including epsilon");
	scanf(" %d",&e);
	puts("use 'e' for epsilon");
	E=(char *)malloc(sizeof(char)*e);
	int i;
	for(i=0;i<e;i++)
	{
		printf("enter the symbol %d\n",i+1);
		scanf(" %c",&E[i]);
		if(E[i]=='e')
			epsilon=i;
	}
	return;
}

void nfaStatesInput()
{
	puts("enter the number of states in the NFA");
	scanf(" %d",&q);
	Q=(state *)malloc(sizeof(state)*q);
	int i;
	for(i=0;i<q;i++)
	{
		printf("enter the state %d of NFA\n",i+1);
		scanf(" %s",Q[i].st);
	}
	return;
}

int stateFinder(char *str, int *check)
{
	int i;
    for(i=0;i<q;i++)
    {
		if(strcmp(str,Q[i].st)==0)
        {
        	*check=i;
         	return 1;
        }
        else if(i==q-1)
        return 0;
	}
}

void initialState()
{
	puts("enter the initial state of the NFA");
	char temp[100];
	scanf(" %s",temp);
	if(stateFinder(temp,&q0)==0)
	{
		printf("invalid state, re-");
		initialState();
	}
	return;
}

void finalState()
{
	puts("enter the cardinality of the final states set");
	scanf(" %d",&f.size);
	f.ss=(int *)malloc(sizeof(int)*f.size);
	int i;
	char temp[100];
	for(i=0;i<f.size;i++)
	{
		printf("enter the final state %d\n",i+1);
		scanf(" %s",temp);
		if(stateFinder(temp,(f.ss+i))==0)
		{
			printf("invalid state, re-");
			i--;
		}
	}
	return;
}
		  
void nfaTransitionInput()
{
	int i,j,k;
	char ts[10];
	delta=(node *)malloc(sizeof(node)*q*e);
	for(i=0;i<q;i++)
	{
		for(j=0;j<e;j++)
		{
			printf("enter the cardinality of transition set for (%s, %c)\n",Q[i].st,E[j]);
			scanf(" %d",&((delta+i*e+j)->size));
			(delta+i*e+j)->ss=(int *)malloc(sizeof(int)*(delta+i*e+j)->size);
			for(k=0;k<((delta+i*e+j)->size);k++)
			{
				printf("enter the transition state %d for (%s, %c)\n",k+1,Q[i].st,E[j]);
				scanf(" %s",ts);
				if(stateFinder(ts,&((delta+i*e+j)->ss[k]))==0)
				{
					printf("invalid state transition, re-");
					k--;
				}
			}
		}
	}
	return;
}

void closureCal()
{
	closee=(node *)malloc(sizeof(node)*q);
	int i,k,j,point;
	for(i=0;i<q;i++)
	{
		closee[i].size=1;
		closee[i].ss=NULL;
		closee[i].ss=(int *)realloc(closee[i].ss,sizeof(int)*closee[i].size);
		closee[i].ss[closee[i].size-1]=i;
		point=0;
		while(epsilon>-1&&((delta+closee[i].ss[point]*e+epsilon)->size)>0&&point<closee[i].size)
		{
			for(k=0;k<((delta+closee[i].ss[point]*e+epsilon)->size);k++)
			{
				for(j=0;j<closee[i].size;j++)
				{
					if(closee[i].ss[j]==(delta+closee[i].ss[point]*e+epsilon)->ss[k])
						break;
					else if(j==closee[i].size-1)
					{
						closee[i].size++;
						closee[i].ss=(int *)realloc(closee[i].ss,sizeof(int)*closee[i].size);
						closee[i].ss[closee[i].size-1]=(delta+closee[i].ss[point]*e+epsilon)->ss[k];
					}
				}
			}
			point++;
		}
	}
	return;
}

void finalCheck(node *new, int number)
{
	int i,j;
	for(i=0;i<new->size;i++)
	{
		for(j=0;j<f.size;j++)
		{
			if(new->ss[i]==f.ss[j])
			{
				nf.size++;
				nf.ss=(int *)realloc(nf.ss,sizeof(int)*nf.size);
				nf.ss[nf.size-1]=number;
				return;
			}
		}
	}
	return;
}

int cmp(const void *a, const void *b)
{
	return (*((int *)a)-*((int *)b));
}

void newStateTransition()
{
	nf.ss=NULL;
	int *temp=NULL, *tempc=NULL, tempSize, tSize, l, m, i, j, k, n, check, v;
	nf.size=0;
	newState=(node *)realloc(newState,sizeof(node)*nq);
	newState[0].size=closee[q0].size;
	newState[0].ss=closee[q0].ss;
	qsort(newState->ss,newState->size,sizeof(int),cmp);
	finalCheck(newState,0);
	for(i=0;i<nq;i++)
	{
		if(epsilon>=0)
			newDelta=(int *)realloc(newDelta,sizeof(int)*nq*(e-1));
		else
			newDelta=(int *)realloc(newDelta,sizeof(int)*nq*e);
		for(j=0,v=0;j<e;j++,v++)
		{
			check=0;
			if(j==epsilon)
			{
				v--;
				continue;
			}
			l=0;
			tempSize=0;
			tSize=0;
			for(k=0;k<newState[i].size;k++)
			{
				if(((delta+(newState[i].ss[k])*e+j)->size)==0)
					continue;
				tempSize+=((delta+(newState[i].ss[k])*e+j)->size);
				temp=(int *)realloc(temp,sizeof(int)*tempSize);
				for(m=0;l<tempSize&&m<(delta+newState[i].ss[k]*e+j)->size;l++,m++)
				{
					temp[l]=(delta+newState[i].ss[k]*e+j)->ss[m];
					for(n=0;n<l;n++)
					{
						if(temp[n]==temp[l])
						{
							l--;
							tempSize--;
							temp=(int *)realloc(temp,sizeof(int)*tempSize);
							break;
						}
					}
				}
			}
			m=0;
			for(l=0;l<tempSize;l++)
			{
				tSize+=closee[temp[l]].size;
				tempc=(int *)realloc(tempc,sizeof(int)*tSize);
				for(n=0;n<closee[temp[l]].size&&m<tSize;n++,m++)
				{
					tempc[m]=closee[temp[l]].ss[n];
					for(k=0;k<m;k++)
					{
						if(tempc[k]==tempc[m])
						{
							m--;
							tSize--;
							tempc=(int *)realloc(tempc,sizeof(int)*tSize);
							break;
						}
					}
				}
			}
			free(temp);
			temp=NULL;
			if(tSize>0)
			{
				qsort(tempc,tSize,sizeof(int),cmp);
				for(n=0;n<nq;n++)
				{
					if(tSize==newState[n].size)
					{
						for(k=0;k<tSize;k++)
						{
							if(tempc[k]!=newState[n].ss[k])
								break;
							else if(k==tSize-1)
							{
								check=1;
								if(epsilon>=0)
									*(newDelta+i*(e-1)+v)=n;
								else
									*(newDelta+i*e+j)=n;
								free(tempc);
								tempc=NULL;
							}
						}
						if(check==1)
							break;
					}
				}
				if(check!=1)
				{
					nq++;
					if(epsilon>=0)
						newDelta=(int *)realloc(newDelta,sizeof(int)*nq*(e-1));
					else
						newDelta=(int *)realloc(newDelta,sizeof(int)*nq*e);
					newState=(node *)realloc(newState,sizeof(node)*nq);
					newState[nq-1].size=tSize;
					newState[nq-1].ss=tempc;
					tempc=NULL;
					if(epsilon>=0)
						*(newDelta+i*(e-1)+v)=nq-1;
					else
						*(newDelta+i*e+j)=nq-1;
					finalCheck(newState+nq-1,nq-1);
				}
			}
			else
			{
				yolo=1;
				if(epsilon>=0)
					*(newDelta+i*(e-1)+v)=-1;
				else
					*(newDelta+i*e+j)=-1;
			}
		}
	}
	return;
}
	
int main()
{
	pid_t id;
	int i,j,v;
	nfaSymbolsInput();
	nfaStatesInput();
	initialState();
	finalState();
	nfaTransitionInput();
	closureCal();
	newStateTransition();
	FILE *fp=fopen("ass2_2.dot","w");
	fprintf(fp,"digraph finite_state_machine {\nrankdir=LR;\nsize=\"8,5\";\nnode [shape = doublecircle]; ");
	printf("\ninitial state: {q0}\nfinal states: {");
	for(i=0;i<nf.size-1;i++)
	{
		if(i==0)
			fprintf(fp,"q%d",nf.ss[i]);
		else
			fprintf(fp,",q%d",nf.ss[i]);
		printf("q%d, ",nf.ss[i]);
	}
	if(nf.size==1)
		fprintf(fp,"q%d;\nnode [shape = circle];\n",nf.ss[i]);
	else
		fprintf(fp,",q%d;\nnode [shape = circle];\n",nf.ss[i]);
	printf("q%d}\n\n",nf.ss[i]);
	for(i=0;i<e;i++)
	{
		if(i==epsilon)
			continue;
		printf("\t%c",E[i]);
	}
	puts("\n---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
	for(i=0;i<nq;i++)
	{
		printf("q%d",i);
		for(j=0,v=0;v<e;j++,v++)
		{
			if(v==epsilon)
			{
				j--;
				continue;
			}
			if(epsilon>=0)
			{
				if(*(newDelta+i*(e-1)+j)==-1)
				{
					fprintf(fp,"q%d -> D [ label = \"%c\" ];\n",i,E[j]);
					printf("\tD");
				}
				else
				{
					fprintf(fp,"q%d -> q%d [ label = \"%c\" ];\n",i,*(newDelta+i*(e-1)+j),E[j]);
					printf("\tq%d",*(newDelta+i*(e-1)+j));
				}
			}
			else
			{
				if(*(newDelta+i*e+v)==-1)
				{
					fprintf(fp,"q%d -> D [ label = \"%c\" ];\n",i,E[v]);
					printf("\tD");
				}
				else
				{
					fprintf(fp,"q%d -> q%d [ label = \"%c\" ];\n",i,*(newDelta+i*(e-1)+v),E[v]);
					printf("\tq%d",*(newDelta+i*e+v));
				}
			}
		}
		puts("");
	}
	if(yolo==1)
	{
		printf("D");
		for(i=0;i<e;i++)
		{
			if(i==epsilon)
				continue;
			printf("\tD");
			fprintf(fp,"D -> D [ label = \"%c\" ];\n",E[i]);
		}
		puts("");
	}
	fprintf(fp,"node [shape = none label=\"\"]; start\nstart ->  q0  [ label = \"\" ];\n}");
	fclose(fp);
	id=fork();
	if(id==0)
	{
		char *parsed[]={"xdot","ass2_2.dot",NULL};
		if(execvp(parsed[0],parsed)<0)
			puts("graph representation can not be opened\n");
		exit(0);
	}
	else if(id<0)
		puts("graph representation can not be opened\n");
	puts("");
	free(Q);
	Q=NULL;
	for(i=0;i<q*e;i++)
	{
		free(delta[i].ss);
		delta[i].ss=NULL;
	}
	free(delta);
	delta=NULL;
	for(i=0;i<nq;i++)
	{
		free(newState[i].ss);
		newState[i].ss=NULL;
	}
	free(newState);
	newState=NULL;
	for(i=0;i<q;i++)
	{
		free(closee[i].ss);
		closee[i].ss=NULL;
	}
	free(closee);
	closee=NULL;
	free(E);
	E=NULL;
	free(newDelta);
	newDelta=NULL;
	return 0;
}
