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
//	n=m=5200;
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
//			if(i==7002&&len==30) printf("%llu\n",hsh[1].gethash(i,i+len-1));
		}
		for(int i=0;i<=n-len;i++)
		{
			unsigned long long hashvalue=hsh[0].gethash(i,i+len-1);
//			if(i==7001&&len==30) printf("hashvalue%llu\n",hashvalue);
			if(idx.count(hashvalue))
			{
				match[i][len]=idx[hashvalue];
			}
//			if(i==65&&len==60) printf("match%d\n",match[i][len]);
		}
	}
//	printf("%d\n",match[65][60]);
	unsigned long long hshvalue=hsh[0].gethash(65,65+60-1);
//	printf("idx%d\n",idx[hshvalue]);
	for(int i=len1;i<=n;i++)
	{
		f[i]=f[i-1];
		path[i]=i-1;
		for(int j=max(0,i-len2);j<=i-len1;j++)
		{
//			if(j==0&&i-j==30) printf("matchhh %d\n",match[j][i-j]);
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
	for(int i=n;i>0;i=path[i])
	{
//		printf("%d %d %d\n",path[i],i,i-path[i]);
		int ref_idx=match[path[i]][i-path[i]];
		if(ref_idx!=-1) printf("(%d,%d,%d,%d),",path[i],i,ref_idx,ref_idx+i-path[i]);
	}
//	for(int i=3500;i<=3600;i++)
//		printf("%d %d\n",i,f[i]);
//	for(int i=0;i<=100;i++)
//		printf("%d %d %d\n",i,f[i],path[i]);
	return 0;
}
/*
*/ 
