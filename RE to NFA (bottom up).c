#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

typedef struct
{
	int start,end;
	char string[5];
}edge;

typedef struct
{
	int initial, final, edgeTableSize;
	edge *edgeTable;
}automata;

typedef struct
{
	automata *autoMata;
	edge *singleEdge;
}stack;

stack *stackTable;

char s[100], gra[100];

int stackSize=0, counter=0;

void inputRE()
{
	char new[100];
	puts("enter the Regular Expression");
	scanf(" %[^\n]%*c",s);
	int i,sb=0,eb=0,k=0;
	for(i=0;s[i]!='\0';i++)
	{
		if(s[i]==' ')
			continue;
		else if(i==0)
			new[k++]=s[i];
		else
		{
			if(s[i]=='+'||s[i]=='.'||s[i]=='*'||s[i]==')')
				new[k++]=s[i];
			else
			{
				if(new[k-1]=='+'||new[k-1]=='.'||new[k-1]=='(')
					new[k++]=s[i];
				else
				{
					new[k++]='.';
					new[k++]=s[i];
				}
			}
		}
	}
	new[k]='\0';
	strcpy(s,new);
	for(i=0;s[i]!='\0';i++)
	{
		if(sb<eb)
		{
			printf("invalid expression re-");
			inputRE();
			return;
		}
		if(s[i]=='(')
			sb++;
		else if(s[i]==')')
			eb++;
	}
	if(sb!=eb)
	{
		printf("invalid expression re-");
		inputRE();
		return;
	}
	strcpy(gra,s);
	return;
}

void postfix()
{
	char new[100], stack[100];
	int k=0,i,j=0;
	for(i=0;s[i]!='\0';i++)
	{
		if(s[i]=='('||s[i]=='+'||s[i]=='.')
		{
			while(s[i]=='+')
			{
				if(j-1>=0&&(stack[j-1]=='.'||stack[j-1]=='+'))
				{
					new[k++]=stack[j-1];
					j--;
				}
				else
					break;
			}
			while(s[i]=='.')
			{
				if(j-1>=0&&stack[j-1]=='.')
				{
					new[k++]=stack[j-1];
					j--;
				}
				else
					break;
			}
			stack[j++]=s[i];
		}
		else if(s[i]==')')
		{
			j--;
			while(stack[j]!='(')
				new[k++]=stack[j--];
		}
		else
			new[k++]=s[i];
	}
	j--;
	while(j>=0)
		new[k++]=stack[j--];
	new[k]='\0';
	strcpy(s,new);
	return;
}

void parse()
{
	int i,j,l;
	for(i=0;s[i]!='\0';i++)
	{
		if(s[i]=='+')
		{
			if(stackTable[stackSize-2].autoMata==NULL)
			{
				stackTable[stackSize-2].autoMata=(automata *)malloc(sizeof(automata));
				stackTable[stackSize-2].autoMata->initial=++counter;
				stackTable[stackSize-2].autoMata->final=++counter;
				stackTable[stackSize-2].autoMata->edgeTable=(edge *)malloc(sizeof(edge)*3);
				stackTable[stackSize-2].autoMata->edgeTable[0].start=stackTable[stackSize-2].singleEdge->start;
				stackTable[stackSize-2].autoMata->edgeTable[0].end=stackTable[stackSize-2].singleEdge->end;
				strcpy(stackTable[stackSize-2].autoMata->edgeTable[0].string,stackTable[stackSize-2].singleEdge->string);
				stackTable[stackSize-2].autoMata->edgeTable[1].start=stackTable[stackSize-2].autoMata->initial;
				stackTable[stackSize-2].autoMata->edgeTable[1].end=stackTable[stackSize-2].singleEdge->start;
				strcpy(stackTable[stackSize-2].autoMata->edgeTable[1].string,"Ɛ");
				stackTable[stackSize-2].autoMata->edgeTable[2].start=stackTable[stackSize-2].singleEdge->end;
				stackTable[stackSize-2].autoMata->edgeTable[2].end=stackTable[stackSize-2].autoMata->final;
				strcpy(stackTable[stackSize-2].autoMata->edgeTable[2].string,"Ɛ");
				stackTable[stackSize-2].autoMata->edgeTableSize=3;
				free(stackTable[stackSize-2].singleEdge);
				stackTable[stackSize-2].singleEdge=NULL;
			}
			else
			{
				stackTable[stackSize-2].autoMata->edgeTableSize+=2;
				stackTable[stackSize-2].autoMata->edgeTable=(edge *)realloc(stackTable[stackSize-2].autoMata->edgeTable,sizeof(edge)*stackTable[stackSize-2].autoMata->edgeTableSize);
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-2].end=stackTable[stackSize-2].autoMata->initial;
				stackTable[stackSize-2].autoMata->initial=++counter;
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-2].start=stackTable[stackSize-2].autoMata->initial;
				strcpy(stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-2].string,"Ɛ");
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-1].start=stackTable[stackSize-2].autoMata->final;
				stackTable[stackSize-2].autoMata->final=++counter;
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-1].end=stackTable[stackSize-2].autoMata->final;
				strcpy(stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-1].string,"Ɛ");
			}
			if(stackTable[stackSize-1].autoMata==NULL)
			{
				stackTable[stackSize-2].autoMata->edgeTableSize+=3;
				stackTable[stackSize-2].autoMata->edgeTable=(edge *)realloc(stackTable[stackSize-2].autoMata->edgeTable,sizeof(edge)*stackTable[stackSize-2].autoMata->edgeTableSize);
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-3].start=stackTable[stackSize-2].autoMata->initial;
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-3].end=stackTable[stackSize-1].singleEdge->start;
				strcpy(stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-3].string,"Ɛ");
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-2].start=stackTable[stackSize-1].singleEdge->start;
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-2].end=stackTable[stackSize-1].singleEdge->end;
				strcpy(stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-2].string,stackTable[stackSize-1].singleEdge->string);
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-1].start=stackTable[stackSize-1].singleEdge->end;
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-1].end=stackTable[stackSize-2].autoMata->final;
				strcpy(stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-1].string,"Ɛ");
				free(stackTable[stackSize-1].singleEdge);
				stackTable[stackSize-1].singleEdge=NULL;
				stackSize--;
				stackTable=(stack *)realloc(stackTable,sizeof(stack)*stackSize);
				
			}
			else
			{
				stackTable[stackSize-2].autoMata->edgeTableSize+=stackTable[stackSize-1].autoMata->edgeTableSize;
				stackTable[stackSize-2].autoMata->edgeTable=(edge *)realloc(stackTable[stackSize-2].autoMata->edgeTable,sizeof(edge)*stackTable[stackSize-2].autoMata->edgeTableSize);
				for(j=stackTable[stackSize-1].autoMata->edgeTableSize;j>0;j--)
				{
					stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-j].start=stackTable[stackSize-1].autoMata->edgeTable[j-1].start;
					stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-j].end=stackTable[stackSize-1].autoMata->edgeTable[j-1].end;
					strcpy(stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-j].string,stackTable[stackSize-1].autoMata->edgeTable[j-1].string);
				}
				stackTable[stackSize-2].autoMata->edgeTableSize+=2;
				stackTable[stackSize-2].autoMata->edgeTable=(edge *)realloc(stackTable[stackSize-2].autoMata->edgeTable,sizeof(edge)*stackTable[stackSize-2].autoMata->edgeTableSize);
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-2].start=stackTable[stackSize-2].autoMata->initial;
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-2].end=stackTable[stackSize-1].autoMata->initial;
				strcpy(stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-2].string,"Ɛ");
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-1].start=stackTable[stackSize-1].autoMata->final;
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-1].end=stackTable[stackSize-2].autoMata->final;
				strcpy(stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-1].string,"Ɛ");
				free(stackTable[stackSize-1].autoMata->edgeTable);
				free(stackTable[stackSize-1].autoMata);
				stackTable[stackSize-1].autoMata=NULL;
				stackSize--;
				stackTable=(stack *)realloc(stackTable,sizeof(stack)*stackSize);
			}
		}
		else if(s[i]=='.')
		{
			if(stackTable[stackSize-2].autoMata==NULL)
			{
				stackTable[stackSize-2].autoMata=(automata *)malloc(sizeof(automata));
				stackTable[stackSize-2].autoMata->initial=stackTable[stackSize-2].singleEdge->start;
				stackTable[stackSize-2].autoMata->final=stackTable[stackSize-2].singleEdge->end;
				stackTable[stackSize-2].autoMata->edgeTableSize=1;
				stackTable[stackSize-2].autoMata->edgeTable=(edge *)malloc(sizeof(edge)*1);
				stackTable[stackSize-2].autoMata->edgeTable[0].start=stackTable[stackSize-2].singleEdge->start;
				stackTable[stackSize-2].autoMata->edgeTable[0].end=stackTable[stackSize-2].singleEdge->end;
				strcpy(stackTable[stackSize-2].autoMata->edgeTable[0].string,stackTable[stackSize-2].singleEdge->string);
				free(stackTable[stackSize-2].singleEdge);
				stackTable[stackSize-2].singleEdge=NULL;
			}
			if(stackTable[stackSize-1].autoMata==NULL)
			{
				stackTable[stackSize-2].autoMata->edgeTableSize+=1;
				stackTable[stackSize-2].autoMata->edgeTable=(edge *)realloc(stackTable[stackSize-2].autoMata->edgeTable,sizeof(edge)*stackTable[stackSize-2].autoMata->edgeTableSize);
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-1].start=stackTable[stackSize-2].autoMata->final;
				stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-1].end=(stackTable[stackSize-1].singleEdge->end)-1;
				counter--;
				strcpy(stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-1].string,stackTable[stackSize-1].singleEdge->string);
				stackTable[stackSize-2].autoMata->final=(stackTable[stackSize-1].singleEdge->end)-1;
				free(stackTable[stackSize-1].singleEdge);
				stackTable[stackSize-1].singleEdge=NULL;
				stackSize--;
				stackTable=(stack *)realloc(stackTable,sizeof(stack)*stackSize);
			}
			else
			{
				stackTable[stackSize-2].autoMata->edgeTableSize+=stackTable[stackSize-1].autoMata->edgeTableSize;
				stackTable[stackSize-2].autoMata->edgeTable=(edge *)realloc(stackTable[stackSize-2].autoMata->edgeTable,sizeof(edge)*stackTable[stackSize-2].autoMata->edgeTableSize);
				for(j=stackTable[stackSize-1].autoMata->edgeTableSize;j>0;j--)
				{
					if(stackTable[stackSize-1].autoMata->edgeTable[j-1].start==stackTable[stackSize-1].autoMata->initial)
						stackTable[stackSize-1].autoMata->edgeTable[j-1].start=stackTable[stackSize-2].autoMata->final;
					if(stackTable[stackSize-1].autoMata->edgeTable[j-1].end==stackTable[stackSize-1].autoMata->initial)
						stackTable[stackSize-1].autoMata->edgeTable[j-1].end=stackTable[stackSize-2].autoMata->final;
				}
				for(j=stackTable[stackSize-1].autoMata->edgeTableSize;j>0;j--)
				{
					if((stackTable[stackSize-1].autoMata->edgeTable[j-1].start)>(stackTable[stackSize-1].autoMata->initial))
						(stackTable[stackSize-1].autoMata->edgeTable[j-1].start)--;
					if((stackTable[stackSize-1].autoMata->edgeTable[j-1].end)>(stackTable[stackSize-1].autoMata->initial))
						(stackTable[stackSize-1].autoMata->edgeTable[j-1].end)--;
				}
				for(j=stackTable[stackSize-1].autoMata->edgeTableSize;j>0;j--)
				{
					stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-j].start=stackTable[stackSize-1].autoMata->edgeTable[j-1].start;
					stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-j].end=stackTable[stackSize-1].autoMata->edgeTable[j-1].end;
					strcpy(stackTable[stackSize-2].autoMata->edgeTable[stackTable[stackSize-2].autoMata->edgeTableSize-j].string,stackTable[stackSize-1].autoMata->edgeTable[j-1].string);
				}
				if((stackTable[stackSize-1].autoMata->final)>(stackTable[stackSize-1].autoMata->initial))
					(stackTable[stackSize-1].autoMata->final)--;
				counter--;
				stackTable[stackSize-2].autoMata->final=stackTable[stackSize-1].autoMata->final;
				free(stackTable[stackSize-1].autoMata->edgeTable);
				free(stackTable[stackSize-1].autoMata);
				stackTable[stackSize-1].autoMata=NULL;
				stackSize--;
				stackTable=(stack *)realloc(stackTable,sizeof(stack)*stackSize);
			}
		}
		else if(s[i]=='*')
		{
			if(stackTable[stackSize-1].autoMata==NULL)
			{
				stackTable[stackSize-1].autoMata=(automata *)malloc(sizeof(automata));
				stackTable[stackSize-1].autoMata->initial=++counter;
				stackTable[stackSize-1].autoMata->final=++counter;
				stackTable[stackSize-1].autoMata->edgeTableSize=5;
				stackTable[stackSize-1].autoMata->edgeTable=(edge *)malloc(sizeof(edge)*5);
				stackTable[stackSize-1].autoMata->edgeTable[0].start=stackTable[stackSize-1].singleEdge->start;
				stackTable[stackSize-1].autoMata->edgeTable[0].end=stackTable[stackSize-1].singleEdge->end;
				strcpy(stackTable[stackSize-1].autoMata->edgeTable[0].string,stackTable[stackSize-1].singleEdge->string);
				stackTable[stackSize-1].autoMata->edgeTable[1].start=stackTable[stackSize-1].autoMata->initial;
				stackTable[stackSize-1].autoMata->edgeTable[1].end=stackTable[stackSize-1].singleEdge->start;
				strcpy(stackTable[stackSize-1].autoMata->edgeTable[1].string,"Ɛ");
				stackTable[stackSize-1].autoMata->edgeTable[2].start=stackTable[stackSize-1].autoMata->initial;
				stackTable[stackSize-1].autoMata->edgeTable[2].end=stackTable[stackSize-1].autoMata->final;
				strcpy(stackTable[stackSize-1].autoMata->edgeTable[2].string,"Ɛ");
				stackTable[stackSize-1].autoMata->edgeTable[3].start=stackTable[stackSize-1].singleEdge->end;
				stackTable[stackSize-1].autoMata->edgeTable[3].end=stackTable[stackSize-1].autoMata->final;
				strcpy(stackTable[stackSize-1].autoMata->edgeTable[3].string,"Ɛ");
				stackTable[stackSize-1].autoMata->edgeTable[4].start=stackTable[stackSize-1].singleEdge->end;
				stackTable[stackSize-1].autoMata->edgeTable[4].end=stackTable[stackSize-1].singleEdge->start;
				strcpy(stackTable[stackSize-1].autoMata->edgeTable[4].string,"Ɛ");
				free(stackTable[stackSize-1].singleEdge);
				stackTable[stackSize-1].singleEdge=NULL;
			}
			else
			{
				stackTable[stackSize-1].autoMata->edgeTableSize+=4;
				stackTable[stackSize-1].autoMata->edgeTable=(edge *)realloc(stackTable[stackSize-1].autoMata->edgeTable,sizeof(edge)*stackTable[stackSize-1].autoMata->edgeTableSize);
				stackTable[stackSize-1].autoMata->edgeTable[stackTable[stackSize-1].autoMata->edgeTableSize-4].start=stackTable[stackSize-1].autoMata->final;
				stackTable[stackSize-1].autoMata->edgeTable[stackTable[stackSize-1].autoMata->edgeTableSize-4].end=stackTable[stackSize-1].autoMata->initial;
				strcpy(stackTable[stackSize-1].autoMata->edgeTable[stackTable[stackSize-1].autoMata->edgeTableSize-4].string,"Ɛ");
				stackTable[stackSize-1].autoMata->edgeTable[stackTable[stackSize-1].autoMata->edgeTableSize-3].end=stackTable[stackSize-1].autoMata->initial;
				stackTable[stackSize-1].autoMata->initial=++counter;
				stackTable[stackSize-1].autoMata->edgeTable[stackTable[stackSize-1].autoMata->edgeTableSize-3].start=stackTable[stackSize-1].autoMata->initial;
				strcpy(stackTable[stackSize-1].autoMata->edgeTable[stackTable[stackSize-1].autoMata->edgeTableSize-3].string,"Ɛ");
				stackTable[stackSize-1].autoMata->edgeTable[stackTable[stackSize-1].autoMata->edgeTableSize-2].start=stackTable[stackSize-1].autoMata->final;
				stackTable[stackSize-1].autoMata->final=++counter;
				stackTable[stackSize-1].autoMata->edgeTable[stackTable[stackSize-1].autoMata->edgeTableSize-2].end=stackTable[stackSize-1].autoMata->final;
				strcpy(stackTable[stackSize-1].autoMata->edgeTable[stackTable[stackSize-1].autoMata->edgeTableSize-2].string,"Ɛ");
				stackTable[stackSize-1].autoMata->edgeTable[stackTable[stackSize-1].autoMata->edgeTableSize-1].end=stackTable[stackSize-1].autoMata->final;
				stackTable[stackSize-1].autoMata->edgeTable[stackTable[stackSize-1].autoMata->edgeTableSize-1].start=stackTable[stackSize-1].autoMata->initial;
				strcpy(stackTable[stackSize-1].autoMata->edgeTable[stackTable[stackSize-1].autoMata->edgeTableSize-1].string,"Ɛ");
			}
		}
		else
		{
			stackSize++;
			stackTable=(stack *)realloc(stackTable,sizeof(stack)*stackSize);
			stackTable[stackSize-1].autoMata=NULL;
			stackTable[stackSize-1].singleEdge=(edge *)malloc(sizeof(edge));
			stackTable[stackSize-1].singleEdge->start=++counter;
			stackTable[stackSize-1].singleEdge->end=++counter;
			stackTable[stackSize-1].singleEdge->string[0]=s[i];
			stackTable[stackSize-1].singleEdge->string[1]='\0';
		}
	}
}

int cmp(const void * a, const void * b)
{
	edge *c, *d;
	c=(edge *)a;
	d=(edge *)b;
	int cl=strlen(c->string);
	int dl=strlen(d->string);
	if(c->start == d->start)
	{
		if(cl==dl)
		{
			if(cl==1)
			{
				if(strcmp(c->string,d->string)!=0)
					return ((int)(c->string[0])-(int)(d->string[0]));
			}
			return (c->end - d->end);
		}
		return (cl-dl);
	}
	return (c->start - d->start);
}
	
int main()
{
	inputRE();
	postfix();
	parse();
	int i;
	pid_t id;
	for(i=0;i<stackTable[0].autoMata->edgeTableSize;i++)
	{
		if(stackTable[0].autoMata->edgeTable[i].start==stackTable[0].autoMata->initial)
			stackTable[0].autoMata->edgeTable[i].start=0;
		else if(stackTable[0].autoMata->edgeTable[i].start>stackTable[0].autoMata->initial)
			stackTable[0].autoMata->edgeTable[i].start--;
		if(stackTable[0].autoMata->edgeTable[i].end==stackTable[0].autoMata->initial)
			stackTable[0].autoMata->edgeTable[i].end=0;
		else if(stackTable[0].autoMata->edgeTable[i].end>stackTable[0].autoMata->initial)
			stackTable[0].autoMata->edgeTable[i].end--;
	}
	if(stackTable[0].autoMata->final==stackTable[0].autoMata->initial)
		stackTable[0].autoMata->final=0;
	else if(stackTable[0].autoMata->final>stackTable[0].autoMata->initial)
		stackTable[0].autoMata->final--;
	stackTable[0].autoMata->initial=0;
	qsort(stackTable[0].autoMata->edgeTable,stackTable[0].autoMata->edgeTableSize,sizeof(edge),cmp);
	FILE *fp=fopen("ass2_1.dot","w");
	fprintf(fp,"digraph finite_state_machine {\nrankdir=LR;\nsize=\"8,5\";\nlabel=\"RE : %s\";\nnode [shape = doublecircle]; q%d;\nnode [shape = circle];\n",gra,stackTable[0].autoMata->final);
	printf("\nstates = {");
	counter--;
	for(i=0;i<counter;i++)
		printf("q%d, ",i);
	printf("q%d}\n",counter);
	printf("\ninitial state: {q%d}\nfinal state: {q%d}\n\n",stackTable[0].autoMata->initial,stackTable[0].autoMata->final);
	printf("Transition functions:\n");
	for(i=0;i<stackTable[0].autoMata->edgeTableSize;i++)
	{
		printf("(q%d, %s) -> q%d\n",stackTable[0].autoMata->edgeTable[i].start,stackTable[0].autoMata->edgeTable[i].string,stackTable[0].autoMata->edgeTable[i].end);
		fprintf(fp,"q%d -> q%d [ label = \"%s\" ];\n",stackTable[0].autoMata->edgeTable[i].start,stackTable[0].autoMata->edgeTable[i].end,stackTable[0].autoMata->edgeTable[i].string);
	}
	fprintf(fp,"node [shape = none label=\"\"]; start\nstart ->  q0  [ label = \"\" ];\n}");
	puts("");
	fclose(fp);
	id=fork();
	if(id==0)
	{
		char *parsed[]={"xdot","ass2_1.dot",NULL};
		if(execvp(parsed[0],parsed)<0)
			puts("graph representation can not be opened\n");
		exit(0);
	}
	else if(id<0)
		puts("graph representation can not be opened\n");
	free(stackTable[0].autoMata);
	free(stackTable);
	return 0;
}			