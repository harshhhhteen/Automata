#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int q,e,f,ultimate,l;
char s[100];

typedef struct abcd
{
	int z;
	char c;
}symbol;

typedef struct efgh
{
	int x;
	char st[100];
}state;

typedef struct asdf
{
	int car;
	state *t;
}node;

state q0,qp0;

state *nfaStatesInput()
{
	puts("enter the number of states in the NFA");
	scanf(" %d",&q);
	state *list=(state *)malloc(sizeof(state)*q);
	int i;
	for(i=0;i<q;i++)
	{
		printf("enter the state %d of NFA\n",i+1);
		scanf(" %s",list[i].st);
		list[i].x=i;
	}
	return list;
}

symbol *nfaSymbolsInput()
{
	puts("enter the number of symbols in the NFA");
	scanf(" %d",&e);
	symbol *E=(symbol *)malloc(sizeof(symbol)*e);
	int i;
	for(i=0;i<e;i++)
	{
		printf("enter the symbol %d\n",i+1);
		scanf(" %c",&E[i].c);
		E[i].z=i;
	}
	return E;
}

int stateFinder(char *str, state *list, int *check)
{
        int i;
        for(i=0;i<q;i++)
        {
                if(strcmp(str,list[i].st)==0)
                {
					*check=list[i].x;
                  	return 1;
                }
                else if(i==q-1)
                        return 0;
        }
}

node *nfaTransitionInput(symbol *E, state *list)
{
	int i,j,k;
	node *d=(node *)malloc(sizeof(node)*q*e);
	for(i=0;i<q;i++)
	{
		for(j=0;j<e;j++)
		{
			printf("enter the cardinality of transition set for (%s, %c)\n",list[i].st,E[j].c);
			scanf(" %d",&((d+e*i+j)->car));
			(d+e*i+j)->t=(state *)malloc(sizeof(state)*((d+i*e+j)->car));
			for(k=0;k<((d+i*e+j)->car);k++)
			{
				printf("enter the transition state %d for (%s, %c)\n",k+1,list[i].st,E[j].c);
				scanf(" %s",((d+e*i+j)->t)[k].st);
				if(stateFinder(((d+e*i+j)->t)[k].st,list,&(((d+e*i+j)->t)[k].x))==0)
				{
					printf("invalid state transition, re-");
					k--;
				}
			}
		}
	}
	return d;
}

void nfaInitialStateInput(state *list)
{
	puts("enter the initial state");
	scanf(" %s",q0.st);
	if(stateFinder(q0.st,list,&(q0.x))==0)
	{
		printf("invalid state, re-");
		nfaInitialStateInput(list);
	}
	strcpy(qp0.st,q0.st);
	qp0.x=q0.x;
	return;
}

state *nfaFinalStatesInput(state *list)
{
	puts("enter the number of final states");
	scanf(" %d",&f);
	int i;
	state *F=(state *)malloc(sizeof(state)*f);
	for(i=0;i<f;i++)
	{
		printf("enter the final state %d\n",i+1);
		scanf(" %s",F[i].st);
		if(stateFinder(F[i].st,list,&(F[i].x))==0)
		{
			printf("invalid state, re-");
			i--;
		}
	}
	return F;
}

void nfaStringInput()
{
	puts("enter the string");
	scanf(" %s",s);
	return;
}

int nfaFinalStateCheck(state *F)
{
	int i;
	for(i=0;i<f;i++)
	{
		if(q0.x==F[i].x)
			return 1;
	}
	return 0;
}

void nfaStringTraverse(symbol *E, node *d, state *list, int i, state *F )
{
	if(i<l&&ultimate==0)
	{
		int j,k;
		state temp;
		strcpy(temp.st,q0.st);
		temp.x=q0.x;
		for(j=0;j<e;j++)
		{
			if(s[i]==E[j].c)
			{
				for(k=0;k<(d+e*(temp.x)+j)->car;k++)
				{
					strcpy(q0.st,(((d+e*(temp.x)+j)->t)+k)->st);
					q0.x=(((d+e*(temp.x)+j)->t)+k)->x;
					nfaStringTraverse(E,d,list,i+1,F);
				}
				return;
			}
			else if(j==e-1)
			{
				puts("\ninvalid string\n");
				ultimate=1;
				return;
			}
		}
	}
	else if(i==l&&ultimate==0)
	{
		if(nfaFinalStateCheck(F))
		{
			puts("\naccepted\n");
			ultimate=1;
			return;
		}
	}
	return;
}

int main()
{
	state *list=nfaStatesInput();
	symbol *E=nfaSymbolsInput();
	node *d=nfaTransitionInput(E, list);
	nfaInitialStateInput(list);
	state *F=nfaFinalStatesInput(list);
	int yolo;
	do
	{
		strcpy(q0.st,qp0.st);
		q0.x=qp0.x;
		ultimate=0;
		nfaStringInput();
		l=strlen(s);
		nfaStringTraverse(E,d,list,0,F);
		if(ultimate==0)
			puts("\nnot accepted\n");
		puts("press 1 to enter another string or 0 to exit");
		scanf(" %d",&yolo);
	}while(yolo!=0);
	free(list);
	free(E);
	int i,j;
	for(i=0;i<q;i++)
		for(j=0;j<e;j++)
			free((d+e*i+j)->t);
	free(d);
	free(F);
	return 0;
}
