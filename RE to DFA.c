#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

char s[100], gra[100];

typedef struct abcde
{
	char ch;
	struct abcde *lc, *rc;
	int *fp,*lp,i,null,fps,lps;
}node;

node **stack=NULL;

typedef struct
{
	int *array, size;
}block;

block *follow=NULL, final, *newState=NULL;

int ss=0, counter=-1, syms=0, *newDelta=NULL, nq=1, yolo=0;

typedef struct
{
	char c;
	int *loc, locs;
}sym;

sym *symbol=NULL;

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
	}
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

void tree()
{
	int j,k,l,m;
	node *current=NULL;
	for(j=0;s[j]!='\0';j++)
	{
		current=(node *)malloc(sizeof(node));
		current->ch=s[j];
		current->lc=current->rc=NULL;
		current->fp=current->lp=NULL;
		current->fps=current->lps=0;
		if(s[j]=='+'||s[j]=='.')
		{
			current->lc=stack[ss-2];
			current->rc=stack[ss-1];
			ss--;
			if(s[j]=='+')
			{
				if(current->lc->null||current->rc->null)
					current->null=1;
				else
					current->null=0;
				if(current->lc->fps>0)
				{
					current->fps=current->lc->fps;
					current->fp=(int *)malloc(sizeof(int)*current->fps);
					for(k=0;k<current->fps;k++)
						current->fp[k]=current->lc->fp[k];
				}
				if(current->rc->fps>0)
				{
					for(k=0;k<current->rc->fps;k++)
					{
						for(l=0;l<current->lc->fps;l++)
						{
							if(current->rc->fp[k]==current->fp[l])
								break;
							else if(l==current->lc->fps-1)
							{
								current->fps++;
								current->fp=(int *)realloc(current->fp,sizeof(int)*current->fps);
								current->fp[current->fps-1]=current->rc->fp[k];
							}
						}
					}
				}
				if(current->lc->lps>0)
				{
					current->lps=current->lc->lps;
					current->lp=(int *)malloc(sizeof(int)*current->lps);
					for(k=0;k<current->lps;k++)
						current->lp[k]=current->lc->lp[k];
				}
				if(current->rc->lps>0)
				{
					for(k=0;k<current->rc->lps;k++)
					{
						for(l=0;l<current->lc->lps;l++)
						{
							if(current->rc->lp[k]==current->lp[l])
								break;
							else if(l==current->lc->lps-1)
							{
								current->lps++;
								current->lp=(int *)realloc(current->lp,sizeof(int)*current->lps);
								current->lp[current->lps-1]=current->rc->lp[k];
							}
						}
					}
				}
			}
			else
			{
				if(current->lc->null&&current->rc->null)
					current->null=1;
				else
					current->null=0;
				if(current->lc->null)
				{
					if(current->lc->fps>0)
					{
						current->fps=current->lc->fps;
						current->fp=(int *)malloc(sizeof(int)*current->fps);
						for(k=0;k<current->fps;k++)
							current->fp[k]=current->lc->fp[k];
					}
					if(current->rc->fps>0)
					{
						for(k=0;k<current->rc->fps;k++)
						{
							for(l=0;l<current->lc->fps;l++)
							{
								if(current->rc->fp[k]==current->fp[l])
									break;
								else if(l==current->lc->fps-1)
								{
									current->fps++;
									current->fp=(int *)realloc(current->fp,sizeof(int)*current->fps);
									current->fp[current->fps-1]=current->rc->fp[k];
								}
							}
						}
					}
				}
				else
				{
					if(current->lc->fps>0)
					{
						current->fps=current->lc->fps;
						current->fp=current->lc->fp;
					}
				}
				if(current->rc->null)
				{
					if(current->lc->lps>0)
					{
						current->lps=current->lc->lps;
						current->lp=(int *)malloc(sizeof(int)*current->lps);
						for(k=0;k<current->lps;k++)
							current->lp[k]=current->lc->lp[k];
					}
					if(current->rc->lps>0)
					{
						for(k=0;k<current->rc->lps;k++)
						{
							for(l=0;l<current->lc->lps;l++)
							{
								if(current->rc->lp[k]==current->lp[l])
									break;
								else if(l==current->lc->lps-1)
								{
									current->lps++;
									current->lp=(int *)realloc(current->lp,sizeof(int)*current->lps);
									current->lp[current->lps-1]=current->rc->lp[k];
								}
							}
						}
					}
				}
				else
				{
					if(current->rc->lps>0)
					{
						current->lps=current->rc->lps;
						current->lp=current->rc->lp;
					}
				}
				for(k=0;k<current->lc->lps;k++)
				{
					for(l=0;l<current->rc->fps;l++)
					{
						if(follow[current->lc->lp[k]].size>0)
						{
							for(m=0;m<follow[current->lc->lp[k]].size;m++)
							{
								if(follow[current->lc->lp[k]].array[m]==current->rc->fp[l])
									break;
								else if(m==follow[current->lc->lp[k]].size-1)
								{
									follow[current->lc->lp[k]].size++;
									follow[current->lc->lp[k]].array=(int *)realloc(follow[current->lc->lp[k]].array,sizeof(int)*follow[current->lc->lp[k]].size);
									follow[current->lc->lp[k]].array[follow[current->lc->lp[k]].size-1]=current->rc->fp[l];
								}
							}
						}
						else
						{
							follow[current->lc->lp[k]].size++;
							follow[current->lc->lp[k]].array=(int *)realloc(follow[current->lc->lp[k]].array,sizeof(int)*follow[current->lc->lp[k]].size);
							follow[current->lc->lp[k]].array[follow[current->lc->lp[k]].size-1]=current->rc->fp[l];
						}
					}
				}
			}
		}
		else if(s[j]=='*')
		{
			current->lc=stack[ss-1];
			current->null=1;
			if(current->lc->fps>0)
			{
				current->fps=current->lc->fps;
				current->fp=current->lc->fp;
			}
			if(current->lc->lps>0)
			{
				current->lps=current->lc->lps;
				current->lp=current->lc->lp;
			}
			for(k=0;k<current->lps;k++)
			{
				for(l=0;l<current->fps;l++)
				{
					if(follow[current->lp[k]].size>0)
					{
						for(m=0;m<follow[current->lp[k]].size;m++)
						{
							if(follow[current->lp[k]].array[m]==current->fp[l])
								break;
							else if(m==follow[current->lp[k]].size-1)
							{
								follow[current->lp[k]].size++;
								follow[current->lp[k]].array=(int *)realloc(follow[current->lp[k]].array,sizeof(int)*follow[current->lp[k]].size);
								follow[current->lp[k]].array[follow[current->lp[k]].size-1]=current->fp[l];
							}
						}
					}
					else
					{
						follow[current->lp[k]].size++;
						follow[current->lp[k]].array=(int *)realloc(follow[current->lp[k]].array,sizeof(int)*follow[current->lp[k]].size);
						follow[current->lp[k]].array[follow[current->lp[k]].size-1]=current->fp[l];
					}
				}
			}
		}
		else if(s[j]=='e')
		{
			current->null=1;
			ss++;
		}
		else
		{
			current->i=++counter;
			follow=(block *)realloc(follow,sizeof(block)*(counter+1));
			follow[counter].array=NULL;
			follow[counter].size=0;
			if(s[j]!='#')
			{
				if(syms==0)
				{
					syms++;
					symbol=(sym *)realloc(symbol,sizeof(sym));
					symbol[0].c=s[j];
					symbol[0].locs=1;
					symbol[0].loc=(int *)malloc(sizeof(int));
					symbol[0].loc[0]=current->i;
				}
				else
				{
					for(k=0;k<syms;k++)
					{
						if(s[j]==symbol[k].c)
						{
							symbol[k].locs++;
							symbol[k].loc=(int *)realloc(symbol[k].loc,sizeof(int)*symbol[k].locs);
							symbol[k].loc[symbol[k].locs-1]=current->i;
							break;
						}
						else if(k==syms-1)
						{
							syms++;
							symbol=(sym *)realloc(symbol,sizeof(sym)*syms);
							symbol[syms-1].c=s[j];
							symbol[syms-1].locs=1;
							symbol[syms-1].loc=(int *)malloc(sizeof(int));
							symbol[syms-1].loc[0]=current->i;
							break;
						}
					}
				}
			}
			current->null=0;
			current->fps=current->lps=1;
			current->fp=(int *)malloc(sizeof(int));
			current->fp[0]=current->i;
			current->lp=current->fp;
			ss++;
		}
		stack=(node **)realloc(stack,sizeof(node *)*ss);
		stack[ss-1]=current;
	}
	return;
}

int cmp(const void *a, const void *b)
{
	return (*((int *)a)-*((int *)b));
}

void finalCheck(block *new, int number)
{
	if(new->array[new->size-1]==counter)
	{
		final.size++;
		final.array=(int *)realloc(final.array,sizeof(int)*final.size);
		final.array[final.size-1]=number;
	}
	return;
}

void newStateTransition()
{
	final.array=NULL;
	int ii,j,k,kk, *temp=NULL, tempSize, l,m,n, check;
	final.size=0;
	newState=(block *)realloc(newState,sizeof(block)*nq);
	newState[0].size=stack[0]->fps;
	newState[0].array=stack[0]->fp;
	qsort(newState->array,newState->size,sizeof(int),cmp);
	finalCheck(newState,0);
	for(ii=0;ii<nq;ii++)
	{
		newDelta=(int *)realloc(newDelta,sizeof(int)*nq*syms);
		for(j=0;j<syms;j++)
		{
			check=0;
			l=0;
			tempSize=0;
			for(k=0;k<newState[ii].size;k++)
			{
				for(kk=0;kk<symbol[j].locs;kk++)
				{
					if(newState[ii].array[k]==symbol[j].loc[kk])
					{
						if(follow[newState[ii].array[k]].size>0)
						{
							tempSize+=follow[newState[ii].array[k]].size;
							temp=(int *)realloc(temp,sizeof(int)*tempSize);
							for(m=0;l<tempSize&&m<follow[newState[ii].array[k]].size;l++,m++)
							{
								temp[l]=follow[newState[ii].array[k]].array[m];
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
						break;
					}
				}
			}
			if(tempSize>0)
			{
				qsort(temp,tempSize,sizeof(int),cmp);
				for(n=0;n<nq;n++)
				{
					if(tempSize==newState[n].size)
					{
						for(k=0;k<tempSize;k++)
						{
							if(temp[k]!=newState[n].array[k])
								break;
							else if(k==tempSize-1)
							{
								check=1;
								*(newDelta+ii*syms+j)=n;
								free(temp);
								temp=NULL;
							}
						}
						if(check==1)
							break;
					}
				}
				if(check!=1)
				{
					nq++;
					newDelta=(int *)realloc(newDelta,sizeof(int)*nq*syms);
					newState=(block *)realloc(newState,sizeof(block)*nq);
					newState[nq-1].size=tempSize;
					newState[nq-1].array=temp;
					temp=NULL;
					*(newDelta+ii*syms+j)=nq-1;
					finalCheck(newState+nq-1,nq-1);
				}
			}
			else
			{
				yolo=1;
				*(newDelta+ii*syms+j)=-1;
			}
		}
	}
	return;
}

int main()
{
	puts("use 'e' for epsilon");
	inputRE();
	strcpy(gra,s);
	postfix();
	strcat(s,"#.");
	tree();
	newStateTransition();
	int ii,j;
	pid_t id;
	FILE *f=fopen("ass2_3.dot","w");
	fprintf(f,"digraph finite_state_machine {\nrankdir=LR;\nsize=\"8,5\";\nlabel=\"RE : %s\";\nnode [shape = doublecircle]; ",gra);
	printf("\ninitial state: {q0}\nfinal states: {");
	for(ii=0;ii<final.size-1;ii++)
	{
		if(ii==0)
			fprintf(f,"q%d",final.array[ii]);
		else
			fprintf(f,",q%d",final.array[ii]);
		printf("q%d, ",final.array[ii]);
	}
	if(final.size==1)
		fprintf(f,"q%d;\nnode [shape = circle];\n",final.array[ii]);
	else
		fprintf(f,",q%d;\nnode [shape = circle];\n",final.array[ii]);
	printf("q%d}\n\n",final.array[ii]);
	for(ii=0;ii<syms;ii++)
	{
		printf("\t%c",symbol[ii].c);
	}
	puts("\n---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
	for(ii=0;ii<nq;ii++)
	{
		printf("q%d",ii);
		for(j=0;j<syms;j++)
		{
			if(*(newDelta+ii*syms+j)==-1)
			{
				fprintf(f,"q%d -> D [ label = \"%c\" ];\n",ii,symbol[j].c);
				printf("\tD");
			}
			else
			{
				fprintf(f,"q%d -> q%d [ label = \"%c\" ];\n",ii,*(newDelta+ii*syms+j),symbol[j].c);
				printf("\tq%d",*(newDelta+ii*syms+j));
			}
		}
		puts("");
	}
	if(yolo==1)
	{
		printf("D");
		for(ii=0;ii<syms;ii++)
		{
			printf("\tD");
			fprintf(f,"D -> D [ label = \"%c\" ];\n",symbol[ii].c);
		}
		puts("");
	}
	fprintf(f,"node [shape = none label=\"\"]; start\nstart ->  q0  [ label = \"\" ];\n}");
	fclose(f);
	id=fork();
	if(id==0)
	{
		char *parsed[]={"xdot","ass2_3.dot",NULL};
		if(execvp(parsed[0],parsed)<0)
			puts("graph representation can not be opened\n");
		exit(0);
	}
	else if(id<0)
		puts("graph representation can not be opened\n");
	puts("");
	free(newDelta);
	newDelta=NULL;
	for(ii=0;ii<=counter;ii++)
	{
		free(follow[ii].array);
		follow[ii].array=NULL;
	}
	free(follow);
	follow=NULL;
	for(ii=0;ii<nq;ii++)
	{
		free(newState[ii].array);
		newState[ii].array=NULL;
	}
	free(newState);
	newState=NULL;
	for(ii=0;ii<syms;ii++)
	{
		free(symbol[ii].loc);
		symbol[ii].loc=NULL;
	}
	free(symbol);
	symbol=NULL;
	free(stack[0]);
	stack[0]=NULL;
	free(stack);
	stack=NULL;
	return 0;
}
