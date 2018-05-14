#include <bits/stdc++.h>
#define ll long long
#define f(i,m,n) for(int i=m;i<n;i++)

using namespace std;

vector<ll> scratch,temp;
vector<ll> seq,incseq,maxseq;
vector<pair<ll,ll> > edgevec;
vector<vector<ll> > sets;
int ranking[200], edges[20][20], dp[20][20], cs[20][20];
int dpin[200],previ[200],rev[200];

vector<ll> findmax(vector<ll>& v)
{
	int n = v.size(),i,j,maxind,maxval;
	vector<ll> maxseq;
	memset(dpin,0,sizeof dpin);
	dpin[0] = 1;
	previ[0] = -1;
	for(i=0;i<n;i++)
	{
		for(j=0;j<i;j++)
		{
			if(v[j]<v[i] && dpin[i]<dpin[j]+1)
			{
				dpin[i] = dpin[j]+1;
				previ[i] = j;
			}
		}
		if(dpin[i] == 0)
		{
			dpin[i] = 1;
			previ[i] = -1;
		}
	}
	maxval = -1;
	for(i=0;i<n;i++)
	{
		if(dpin[i]>=maxval)
		{
			maxval = dpin[i];
			maxind = i;
		}
	}
	j = maxind;
	while(j!=-1)
	{
		maxseq.push_back(v[j]);
		j = previ[j];
	}
	reverse(maxseq.begin(), maxseq.end());
	return maxseq;
}


void addedges(vector<ll>& v,ll st,ll en,int* pos)
{
	ll i,j,maxval,maxind,jold,jnew,found = 0;
	if(abs(rev[en]-rev[st]) < 2)
		return;
	vector<ll> newseq;
	maxval = -1;
	for(i=rev[st];i<=rev[en];i++)
	{
		if(dpin[i]>=maxval)
		{
			maxval = dpin[i];
			maxind = i;
		}
	}
	jold = jnew = maxind;
	while(jnew>=rev[st])
	{

		newseq.push_back(v[jnew]);
		jold = jnew;
		jnew = previ[jnew];
		if(jnew >= rev[st] && (v[jnew] != st || v[jold] != en))
			addedges(v,v[jnew],v[jold],pos);
	}
	reverse(newseq.begin(), newseq.end());
	for(i=0;i<newseq.size();i++)
	{
		// cout<<newseq[i]<<" ";
		for(j=i+1;j<newseq.size();j++)
		{
			found = 0;
			f(k,0,edgevec.size())
			{
				tie(st,en) = edgevec[k];
				if(pos[newseq[i]] > pos[st] && pos[newseq[j]] < pos[en])
				{
					if(newseq[i] <= st || newseq[j] >= en)
					{
						found = 1;
						break;			
					}
				}
				else if(pos[newseq[i]] < pos[st] && pos[newseq[j]] > pos[en])
				{
					if(newseq[i] >= st || newseq[j] <= en)
					{
						found = 1;
						break;			
					}
				}
			}
			if(found == 0)
			{
				edges[newseq[i]][newseq[j]] = 1;
				cout<<newseq[i]<<" "<<newseq[j]<<endl;
				edgevec.push_back(make_pair(newseq[i],newseq[j]));
			}
			
		}
	}
	// cout<<endl;
}


ll testheuristic2(vector<ll>& v)
{
	// Clearing sequences
	seq.clear();
	maxseq.clear();
	incseq.clear();
	sets.clear();
	edgevec.clear();

	ll st,found,en,n = v.size();
	int pos[n+2];
	f(i,0,n)
	{
		seq.push_back(v[i]);
		pos[v[i]] = i;
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
	
	memset(edges,0,sizeof edges);
	f(i,0,sets.size())
	{
		f(j,0,sets[i].size())
		{
			f(k,j+1,sets[i].size())
			{
				// if(sets[i][j] < sets[i][k])
					edges[sets[i][j]][sets[i][k]] = 1;
					edgevec.push_back(make_pair(sets[i][j],sets[i][k]));
			}
		}
	}

	f(i,0,incseq.size())
		rev[incseq[i]] = i;

	// Adding additional edges according to heuristic
	maxseq = findmax(incseq);
	f(i,0,maxseq.size()-1)
		addedges(incseq,maxseq[i],maxseq[i+1],pos);	


	f(i,0,maxseq.size())
	{
		f(j,i+1,maxseq.size())
		{
			found = 0;
			f(k,0,edgevec.size())
			{
				tie(st,en) = edgevec[k];
				if(pos[maxseq[i]] > pos[st] && pos[maxseq[j]] < pos[en])
				{
					if(maxseq[i] <= st || maxseq[j] >= en)
					{
						found = 1;
						break;			
					}
				}
				else if(pos[maxseq[i]] < pos[st] && pos[maxseq[j]] > pos[en])
				{
					if(maxseq[i] >= st || maxseq[j] <= en)
					{
						found = 1;
						break;			
					}
				}
			}
			if(found == 0)
			{
				edges[maxseq[i]][maxseq[j]] = 1;
				cout<<maxseq[i]<<" "<<maxseq[j]<<endl;
				edgevec.push_back(make_pair(maxseq[i],maxseq[j]));
			}
		}
	}

	// Computing maximum non-crossing set using DP
	memset(dp,0,sizeof dp);
	memset(cs,0,sizeof cs);
	// Initialisation
	f(i,1,n)
	{
		if(edges[i][i+1])
			dp[i][i+1] = cs[i][i+1] = 1;
	}
	f(l,3,n+1)
	{
		f(i,1,n-l+2)
		{
			en = i+l-1;
			dp[i][en] = dp[i+1][en-1];
			if(edges[i][en])
			{
				dp[i][en] = dp[i+1][en-1] + 1;
				cs[i][en] = 1;
			}
			f(k,i+1,en)
			{
				if(dp[i][en] < (dp[i][k] + dp[k][en]))
				{
					dp[i][en] = (dp[i][k] + dp[k][en]);
					cs[i][en] = n+k;
				}
			}
		}
	}
	return n-1-dp[1][n];
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

	cout<<"Optimal Number of moves: "<< testheuristic2(v)<<"\n";
	// cout<<"Number of moves taken by algorithm: "<<num<<"\n";

	return 0;
}