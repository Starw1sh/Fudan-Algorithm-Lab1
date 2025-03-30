#include<bits/stdc++.h>
using namespace std;
int qry[3030],refr[3030],rev_qry[3030],n,m,change[256];
struct hash
{
	unsigned long long num[3030],pw[3030];
	int len;
	void init(int a[],int _len,int _p)
	{
		len=_len;
		pw[0]=1;
		for(int i=1;i<=_len;i++) pw[i]=pw[i-1]*_p;
		num[0]=0; 
		for(int i=1;i<=_len;i++) num[i]=(num[i-1]*_p)+a[i];
		return;
	}
	unsigned long long gethash(int l,int r)
	{
		return num[r]-num[l-1]*pw[r-l+1];
	}
	unsigned long long getrevhash(int l,int r)
	{
		int tmp=r;
		r=m-l+1;
		l=m-tmp+1;
		return num[r]-num[l-1]*pw[r-l+1];
	}
}hsh[3];
bool match[3030][3030];
pair<int,bool>path[3030][3030];
map<unsigned long long,vector<int> >idx;
struct answer
{
	int id,length,cnt;
	bool reverse;
}ans[3030];
int main()
{
	freopen("lab1.in","r",stdin);
	freopen("test.out","w",stdout);
	change['A']=1;
	change['T']=4;
	change['C']=2;
	change['G']=3;
	char s[3030];
	scanf("%s",s+1);
	n=strlen(s+1);
	for(int i=1;i<=n;i++) refr[i]=change[s[i]];
	scanf("%s",s+1);
	m=strlen(s+1);
	for(int i=1;i<=m;i++) qry[i]=change[s[i]];
	hsh[0].init(refr,n,113);
	hsh[1].init(qry,m,113);
	for(int i=1;i<=m;i++) rev_qry[m+1-i]=5-qry[i];
	hsh[2].init(rev_qry,m,113);
	//init,calculate hash value
//	printf("%d %d\n",n,m);
	
	match[0][0]=1;//match[i][j]=1 means qry[1~i] can match refr[1~j]
	for(int i=1;i<=n;i++)
		for(int j=i;j<=n;j++)
		{
			idx[hsh[0].gethash(i,j)].push_back(j);
		}
	for(int i=1;i<=m;i++)
	{
//		printf("%d\n",i);
		for(int j=1;j<=i;j++)
		{
			if(qry[i]==refr[j]&&match[i-1][j-1])//qry[i] match refr[j] immediately
			{
				match[i][j]=1;
			}
		}
		for(int j=i;j>=1;j--)//the longer repeating will cover the shorter repeating
		{
			unsigned long long hshvalue=hsh[1].gethash(j,i);//try repeating string
			for(int id:idx[hshvalue])
			{
				if(match[j-1][id])
				{
					match[i][id]=1;
					path[i][id]=make_pair(id-i+j,0);
				}
			}
			hshvalue=hsh[2].getrevhash(j,i);//try reverse repeating string
			for(int id:idx[hshvalue])
			{
				if(match[j-1][id])
				{
					match[i][id]=1;
					path[i][id]=make_pair(id-i+j,1);
				}
			}
		}
	}
//	for(int i=1;i<=m;i++)
//	{
//		for(int j=1;j<=n;j++)
//			printf("%d ",match[i][j]);
//		puts("");
//	}
	
	//output
	int cntans=0;
	for(int i=m,j=n;i>0&&j>0;)
	{
//		printf("%d %d\n",i,j);
		if(path[i][j].first)
		{
			pair<int,bool> tmp=path[i][j];
			++cntans;
			ans[cntans].length=j-tmp.first+1;
			ans[cntans].id=tmp.first;
			ans[cntans].reverse=tmp.second;
			ans[cntans].cnt=1;
			i-=ans[cntans].length;
			while(path[i][j]==tmp)
			{
				++ans[cntans].cnt;
				i-=ans[cntans].length;
			}
		}
		else --i,--j;
	}
	for(int i=1;i<=cntans;i++)
		printf("reference index:%d length:%d repeating times:%d reverse:%d\n",ans[i].id,ans[i].length,ans[i].cnt,ans[i].reverse);
	return 0;
}
/*
AAAAA
AAAAAAAAAAA
*/
