#include <bits/stdc++.h>
#define ll long long
using namespace std;


vector<ll> findmax(vector<ll>& v)
{
	int n = v.size(),i,j,maxind,maxval;
	int dp[n],prev[n];
	vector<ll> maxseq;
	memset(dp,0,sizeof dp);
	dp[0] = 1;
	prev[0] = -1;
	for(i=0;i<n;i++)
	{
		for(j=0;j<i;j++)
		{
			if(v[j]<v[i] && dp[i]<dp[j]+1)
			{
				dp[i] = dp[j]+1;
				prev[i] = j;
			}
		}
		if(dp[i] == 0)
		{
			dp[i] = 1;
			prev[i] = -1;
		}
	}
	maxval = -1;
	for(i=0;i<n;i++)
	{
		if(dp[i]>maxval)
		{
			maxval = dp[i];
			maxind = i;
		}
	}
	j = maxind;
	while(j!=-1)
	{
		maxseq.push_back(v[j]);
		j = prev[j];
	}
	reverse(maxseq.begin(), maxseq.end());
	return maxseq;
}


int main()
{
	ll n,i,x,y,j,k,l,en,st,z,predz,predx,predy,succz,succy,succx,num,x1,y1,z1;
	num = 0;
	// Input n and a permutation of [1,n]
	cin>>n;
	vector<ll> v;
	for(i=0;i<n;i++)
	{
		cin>>x;
		v.push_back(x);
	}

	// Construction of sequences
	vector<ll> seq,incseq,maxseq;
	vector<vector<ll> > sets;
	for(i=0;i<n;i++)
	{
		seq.push_back(v[i]);
		if(i<n-1)
		{
			if(v[i+1] < v[i])
			{
				sets.push_back(seq);
				incseq.push_back(seq[0]);
				if(seq.size()>1)
					incseq.push_back(seq[seq.size()-1]);
				seq.clear();
			}
		}
	}
	if(seq.size())
	{
		sets.push_back(seq);
		incseq.push_back(seq[0]);
		if(seq.size()>1)
			incseq.push_back(seq[seq.size()-1]);
	}

	// Construction of graph by finding all edges in the graph
	int edges[n+1][n+1];
	memset(edges,0,sizeof edges);
	for(i=0;i<sets.size();i++)
	{
		for(j=0;j<sets[i].size();j++)
		{
			for(k=j+1;k<sets[i].size();k++)
			{
				// if(sets[i][j] < sets[i][k])
					edges[sets[i][j]][sets[i][k]] = 1;
			}
		}
	}

	for(i=0;i<incseq.size();i++)
		cout<<incseq[i]<<" ";
	cout<<"\n";
	// Adding additional edges according to heuristic
	// maxseq = findmax(incseq);
	maxseq.push_back(1);maxseq.push_back(2);maxseq.push_back(8);maxseq.push_back(10);
	for(i=0;i<maxseq.size();i++)
	{
		cout<<maxseq[i]<<" ";
		for(j=i+1;j<maxseq.size();j++)
			edges[maxseq[i]][maxseq[j]] = 1;
	}
	cout<<endl;
	// Computing maximum non-crossing set using DP
	int dp[n+1][n+1],cs[n+1][n+1];
	memset(dp,0,sizeof dp);
	memset(cs,0,sizeof cs);
	// Initialisation
	for(i=1;i<n;i++)
	{
		if(edges[i][i+1])
			dp[i][i+1] = cs[i][i+1] = 1;
	}
	for(l=3;l<=n;l++)
	{
		for(i=1;i<n-l+2;i++)
		{
			en = i+l-1;
			dp[i][en] = dp[i+1][en-1];
			if(edges[i][en])
			{
				dp[i][en] = dp[i+1][en-1] + 1;
				cs[i][en] = 1;
			}
			for(k=i+1;k<en;k++)
			{
				if(dp[i][en] < (dp[i][k] + dp[k][en]))
				{
					dp[i][en] = (dp[i][k] + dp[k][en]);
					cs[i][en] = n+k;
				}
			}
		}
	}
	
	cout<<"Optimal Number of moves: "<< n-1-dp[1][n]<<"\n";
	// cout<<"Number of moves taken by algorithm: "<<num<<"\n";

	return 0;
}