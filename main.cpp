#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <stack>
using namespace std;
struct stare_dfa
{
	char nume;
	char stringstari[50];
	char tranzitie[10];
	int final;
}stari_dfa[50];
struct tranzitie
{
	char c;
	int stari[50];
	int n;
};
struct stare
{
	int val;
	struct tranzitie t[50];
}stari[50];
int ns,q0,nf,final[50],na,i,j,nr,op,nr_dfa,k,gasit1[100]={0},gasit2[100]={0};
char alf[50],cuv[50]="";
stack <int> st1,st2;
ifstream fin("date.in");
void citire_nfa()
{
	cout<<"Introduceti nr de stari din NFA: ";
	cin>>ns;
	for(i=0;i<ns;i++)
	{
		stari[i].val=i;
	}
	cout<<endl<<"Introduceti starea de start: ";
	cin>>q0;
	cout<<endl<<"Introduceti nr de stari finale: ";
	cin>>nf;
	cout<<endl<<"Introduceti starile finale: ";
	for(i=0;i<nf;i++) cin>>final[i];
	cout<<endl<<"Introduceti nr de litere din alfabet: ";
	cin>>na;
	cout<<endl<<"Introduceti alfabetul: ";
	for(i=0;i<na;i++) cin>>alf[i];
	alf[na]='l';
	cout<<endl<<"Introduceti tranzitiile (-1 pentru a se opri): ";
	for(i=0;i<ns;i++)
	{
		for(j=0;j<=na;j++)
		{
			stari[i].t[j].c=alf[j];
			nr=0;
			op=1;
			while(op)
			{
                cout<<endl<<"tranzitie("<<i<<","<<alf[j]<<"): ";
				cin>>stari[i].t[j].stari[nr];
				nr++;
				if(stari[i].t[j].stari[nr-1]==-1)
				{
					nr--;
					op=0;
					break;
				}
			}
			stari[i].t[j].n=nr;
		}
	}
}
void l_inchidere()
{
	int i,q,qc;
	while (!st1.empty())
    {
        qc=st1.top();
        gasit1[qc]=0;
        st1.pop();
        i=0;
        q=qc;
        while(i<=stari[qc].t[na].n)
        {
            if (gasit2[q]==0) {st2.push(q); gasit2[q]=1;}
            q=stari[qc].t[na].stari[i];
            i++;
            if (gasit1[q]==0 && i<=stari[qc].t[na].n) {st1.push(q); gasit1[q]=1;}
        }
    }
}
void afisare_dfa()
{
	printf("\nTabelul de tranzitii DFA ");
	printf("\nStari \tTranzitii\n ");
	for(i=0;i<na;i++) printf("\t%c",alf[i]);
	cout<<endl;
	for(i=0;i<nr_dfa;i++)
	{
		if(stari_dfa[i].final==0) printf("\n%c",stari_dfa[i].nume);
		else printf("\nf%c",stari_dfa[i].nume);
		cout<<"(";
		for (j=0;j<strlen(stari_dfa[i].stringstari);j++)
            if (stari_dfa[i].stringstari[j]-'0'>9) cout<<stari_dfa[i].stringstari[j]-'0';
            else cout<<stari_dfa[i].stringstari[j];
        cout<<")";
		for(j=0;j<na;j++) printf("\t%c",stari_dfa[i].tranzitie[j]);
	}
	cout<<endl;
}
void transforma()
{
    int l=0;
    for (int i=0;i<ns;i++)
        if (gasit2[i]==1)
        {
            cuv[l]=i+'0';
            l++;
        }
    cuv[l]='\0';
}
void adauga(int x, int a)
{
    int ctr;
    for (ctr=0;ctr<stari[x].t[a].n;ctr++) {st1.push(stari[x].t[a].stari[ctr]); gasit1[stari[x].t[a].stari[ctr]]=1;}
}
void copiaza(int x)
{
    char a[100]=" ";
    int p=0;
    while (!st2.empty()) {gasit2[st2.top()]=0; st2.pop();}
    strcpy(a,stari_dfa[x].stringstari);
    for (p=0;p<strlen(a);p++) {st2.push(a[p]-'0'); gasit2[a[p]-'0']=1;}
}
int main()
{
    citire_nfa();
    st1.push(q0);
    gasit1[q0]=1;
    l_inchidere();
    transforma();
    stari_dfa[0].nume='A';
    strcpy(stari_dfa[0].stringstari,cuv);
    nr_dfa=1;
    i=0;
    while (i<nr_dfa)
    {
        for (j=0;j<na;j++)
        {
            copiaza(i);
            while(!st2.empty())
            {
                    adauga(st2.top(),j);
                    gasit2[st2.top()]=0;
                    st2.pop();
            }
            while (!st1.empty()) l_inchidere();
            transforma();
            for(k=0;k<nr_dfa;k++)
			{
				if((strcmp(cuv,stari_dfa[k].stringstari)==0))
				{
					stari_dfa[i].tranzitie[j]=stari_dfa[k].nume;
					break;
				}
			}
            if (k==nr_dfa)
            {
                for (k=0;k<nf;k++)
                {
                    if (gasit2[final[k]]==1)
                    {
                        stari_dfa[nr_dfa].final=1;
                        break;
                    }
                }
                strcpy(stari_dfa[nr_dfa].stringstari,cuv);
                stari_dfa[nr_dfa].nume='A'+nr_dfa;
                stari_dfa[i].tranzitie[j]=stari_dfa[nr_dfa].nume;
                nr_dfa++;
            }
        }
        i++;
    }
    afisare_dfa();
    return 0;
}
