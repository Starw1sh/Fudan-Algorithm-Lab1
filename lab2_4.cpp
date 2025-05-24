#include<iostream>
#include<unordered_map>
#include<cstring>
using namespace std;
int qry[100010],ref[100010],rev_ref[100010],change[256];
int match[100010][101],path[100010],f[100010];
int n,m;
struct hash
{
	unsigned long long num[100010],pw[100010];
	int len;
	void init(int a[],int _len,int _p)
	{
		len=_len;
		pw[0]=1;
		for(int i=1;i<=_len;i++) pw[i]=pw[i-1]*_p;
		num[0]=a[0]; 
		for(int i=1;i<_len;i++) num[i]=(num[i-1]*_p)+a[i];
		return;
	}
	unsigned long long gethash(int l,int r)
	{
		if(l==0) return num[r];
		return num[r]-num[l-1]*pw[r-l+1];
	}
}hsh[3];
unordered_map<unsigned long long,int>idx;
char s[100010];
struct seg
{
	pair<int,int>qry;
	pair<int,int>ref;
	void mofy(int a,int b,int c,int d)
	{
		qry=make_pair(a,b);
		ref=make_pair(c,d);
	}
}ans[10010];
double caldis(int l1,int r1,int l2,int r2)
{
	int cnt=0;
	for(int i=l1,j=l2;i<r1;i++,j++)
	{
		cnt+=qry[i]!=ref[j];
	}
	return (double)cnt/(r1-l1);
}
int main()
{
	freopen("2.in","r",stdin);
	freopen("my2.out","w",stdout);
	change['A']=1;
	change['T']=4;
	change['C']=2;
	change['G']=3;
	scanf("%s",s);
	m=strlen(s);
	for(int i=0;i<m;i++) ref[i]=change[s[i]];
	scanf("%s",s);
	n=strlen(s);
	for(int i=0;i<n;i++) qry[i]=change[s[i]];
	printf("%d %d\n",n,m);
	for(int i=0;i<m;i++) rev_ref[i]=5-ref[m-1-i];
	hsh[0].init(qry,n,131);
	hsh[1].init(ref,m,131);
	hsh[2].init(rev_ref,m,131);
	int len1=30,len2=60;
	memset(match,-1,sizeof(match));
	for(int len=len1;len<=len2;len++)
	{
		idx.clear();
		for(int i=0;i<=m-len;i++)
		{
			idx[hsh[1].gethash(i,i+len-1)]=i;
			idx[hsh[2].gethash(i,i+len-1)]=m-i-len;
		}
		for(int i=0;i<=n-len;i++)
		{
			double missrate=1.0;
			int minidx=-1;
			for(int j=0;j<=m-len;j++)
			{
				double dis=caldis(i,i+len,j,j+len);
				if(dis<missrate) missrate=dis,minidx=j;
			}
			if(missrate<0.1)
			{
				match[i][len]=minidx;
			}
		}
	}
	for(int i=len1;i<=n;i++)
	{
		f[i]=f[i-1];
		path[i]=i-1;
		for(int j=max(0,i-len2);j<=i-len1;j++)
		{
			if(match[j][i-j]!=-1)
			{
				if(f[i]<f[j]+i-j)
				{
					f[i]=f[j]+i-j;
					path[i]=j;
				}
			}
		}
	}
	printf("%d\n",f[n]);
	int cnt=0;
	for(int i=n;i>0;i=path[i])
	{
		int ref_idx=match[path[i]][i-path[i]];
		if(ref_idx!=-1) ans[++cnt].mofy(path[i],i,ref_idx,ref_idx+i-path[i]);
	}
	for(int i=1;i<cnt-i+1;i++) swap(ans[i],ans[cnt-i+1]);
	for(int i=1;i<=cnt;i++) printf("(%d,%d,%d,%d),",ans[i].qry.first,ans[i].qry.second,ans[i].ref.first,ans[i].ref.second);
	puts("");
	for(int i=1;i<cnt;i++)
	{
		if(ans[i+1].qry.second-ans[i].qry.first==ans[i+1].ref.second-ans[i].ref.first)
		{
			ans[i+1].qry.first=ans[i].qry.first;
			ans[i+1].ref.first=ans[i].ref.first;
			ans[i].qry.first=-1;
		}
		else if(ans[i+1].qry.second-ans[i].qry.first==ans[i].ref.second-ans[i+1].ref.first)
		{
			ans[i+1].qry.first=ans[i].qry.first;
			ans[i+1].ref.second=ans[i].ref.second;
			ans[i].qry.first=-1;
		}
	}
	int tmp=0;
	for(int i=1;i<=cnt;i++)
		if(ans[i].qry.first!=-1)
			ans[++tmp]=ans[i];
	cnt=tmp;
	printf("%d\n",cnt);
	for(int i=1;i<=cnt;i++)
			printf("(%d,%d,%d,%d),",ans[i].qry.first,ans[i].qry.second,ans[i].ref.first,ans[i].ref.second);//1705
	ans[0].qry.first=-1;
	ans[cnt+1].qry.first=-1;
	for(int i=1;i<=cnt;i++)
	{
		if(ans[i].qry.second-ans[i].qry.first<30)
		{
			double missrate1=1.0,missrate2=1.0;
			if(ans[i-1].qry.first!=-1)
			{
				missrate1=caldis(ans[i-1].qry.first,ans[i].qry.second,ans[i-1].ref.first,ans[i-1].ref.first+ans[i].qry.second-ans[i-1].qry.first);
			}
			if(ans[i+1].qry.first!=-1)
			{
				missrate2=caldis(ans[i].qry.first,ans[i+1].qry.second,ans[i+1].ref.second-ans[i+1].qry.second+ans[i].qry.first,ans[i+1].ref.second);
			}
			printf("%d %lf %lf\n",i,missrate1,missrate2);
			if(missrate1<missrate2)
			{
				if(missrate1<0.1)
				{
					ans[i-1].qry.second=ans[i].qry.second;
					ans[i-1].ref.second=ans[i-1].ref.first+ans[i].qry.second-ans[i-1].qry.first;
					ans[i].qry.first=-1;
				}
			}
			else
			{
				if(missrate2<0.1)
				{
					ans[i+1].qry.first=ans[i].qry.first;
					ans[i+1].ref.first=ans[i+1].ref.second-ans[i+1].qry.second+ans[i].qry.first;
					ans[i].qry.first=-1;
				}
			}
		}
	}
	for(int i=1;i<=cnt;i++)
		if(ans[i].qry.first!=-1)
			printf("(%d,%d,%d,%d),",ans[i].qry.first,ans[i].qry.second,ans[i].ref.first,ans[i].ref.second);//1705
	return 0;
}
/*
*/ 
