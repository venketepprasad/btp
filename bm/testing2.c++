#include <bits/stdc++.h>
#define ll long long
#define f(i,m,n) for(int i=m;i<n;i++)

using namespace std;

vector<ll> scratch,temp;
vector<ll> seq,incseq,maxseq;
vector<vector<ll> > sets;
int ranking[200], dp[20][20], cs[20][20];

int dpin[200],previ[200],rev[200];
int edges[201][201];

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

ll move(vector<ll>& v,int ind)
{
	ll x = 0;
	f(i,0,v.size())
	{
		if(i!=ind)
			x = 10*x + v[i];
		if(v[i] > v[ind])
			x--;
	}
	return x;
}


ll shorten(vector<ll>& v)
{
	ll x = 0;
	f(i,0,v.size())
		x = 10*x + v[i];
	return x; 
}

void addedges(vector<ll>& v,ll st,ll en)
{
	ll i,j,maxval,maxind,jold,jnew;
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
			addedges(v,v[jnew],v[jold]);
	}
	reverse(newseq.begin(), newseq.end());
	for(i=0;i<newseq.size();i++)
	{
		// cout<<newseq[i]<<" ";
		for(j=i+1;j<newseq.size();j++)
			edges[newseq[i]][newseq[j]] = 1;
	}
	// cout<<endl;
}



void kernelize(vector<ll>& v)
{
	scratch.clear();
	temp.clear();
	scratch.push_back(v[0]);
	temp.push_back(v[0]);
	f(i,1,v.size())
	{
		if(v[i] != v[i-1]+1)
		{
			scratch.push_back(v[i]);
			temp.push_back(v[i]);
		}
	}
	sort(temp.begin(),temp.end());
	f(i,0,temp.size())
		ranking[temp[i]] = i+1;
	f(i,0,scratch.size())
		scratch[i] = ranking[scratch[i]];		
}

ll testheuristic2(vector<ll>& v)
{
	// Clearing sequences
	seq.clear();
	maxseq.clear();
	incseq.clear();
	sets.clear();

	ll en,n = v.size();
	f(i,0,n)
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
	
	memset(edges,0,sizeof edges);
	f(i,0,sets.size())
	{
		f(j,0,sets[i].size())
		{
			f(k,j+1,sets[i].size())
			{
				// if(sets[i][j] < sets[i][k])
					edges[sets[i][j]][sets[i][k]] = 1;
			}
		}
	}


	f(i,0,incseq.size())
		rev[incseq[i]] = i;

	// Adding additional edges according to heuristic
	maxseq = findmax(incseq);
	f(i,0,maxseq.size()-1)
		addedges(incseq,maxseq[i],maxseq[i+1]);	
	
	// maxseq.push_back(1);maxseq.push_back(2);maxseq.push_back(8);maxseq.push_back(10);

	f(i,0,maxseq.size())
	{
		f(j,i+1,maxseq.size())
			edges[maxseq[i]][maxseq[j]] = 1;
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

ll testheuristic1(vector<ll>& v)
{
	// Clearing sequences
	seq.clear();
	maxseq.clear();
	incseq.clear();
	sets.clear();

	ll en,n = v.size();
	f(i,0,n)
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
	
	memset(edges,0,sizeof edges);
	f(i,0,sets.size())
	{
		f(j,0,sets[i].size())
		{
			f(k,j+1,sets[i].size())
			{
				// if(sets[i][j] < sets[i][k])
					edges[sets[i][j]][sets[i][k]] = 1;
			}
		}
	}

	// Adding additional edges according to heuristic
	maxseq = findmax(incseq);
	f(i,0,maxseq.size())
	{
		f(j,i+1,maxseq.size())
			edges[maxseq[i]][maxseq[j]] = 1;
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
	double maxratio = 1;
	map<ll,ll> hm;
	ll x = 0,val,y,y2,counting = 0,countless = 0,countequal = 0,countequal2 = 0;
	f(i,1,11)
	{
		x = 10*x + i;
		hm[x] = 0;
	}
	f(l,2,11)
	{
		vector<ll> v;
		f(i,1,l+1)
			v.push_back(i);
		while(next_permutation(v.begin(),v.end()))
		{
			kernelize(v);
			val = INT_MAX;
			f(i,0,scratch.size())
			{
				x = move(scratch,i);
				val = min(val,hm[x]+1);
			}
			
			y = testheuristic1(scratch);
			y2 = testheuristic2(scratch);
			x = 0;
			f(i,0,v.size())
				x = x*10 + v[i];
			hm[x] = val;
			// cout<<x<<" "<<val<<" "<<y<<"\n";
			if(y2<y)
			{
				// cout<<"1: "<<x<<" "<<y<<" "<<y2<<endl;
				countless++;
			}

			// if(maxratio < (y2*1.0/val))
			// 	cout<<"2: "<<x<<" "<<val<<" "<<y<<"\n";
			maxratio = max(maxratio,(double)(y2*1.0/val));
			if((int)(y2*1.0/val) == 2)
			{
				// cout<<x<<endl;
				counting++;
			}
			if(y == val)
				countequal2++;
			if(y2 == val)
			{
				countequal++;
			}
			if(y2<val)
			{
				cout<<x<<" "<<val<<" "<<y2<<"\n";
				break;
			}
		}	
	}
	// for(auto it = hm.begin(); it!=hm.end() ;it++)
	// 	cout<<it->first<<" "<<it->second<<"\n"; 	
	cout<<"Number of cases of heuristic 4 giving optimal "<<countequal<<endl;   
	cout<<"Number of cases of heuristic 3 giving optimal "<<countequal2<<endl;   
	cout<<"Number of cases of heuristic 4 beating heuristic 3: "<<countless<<endl;   
	cout<<"Maximum approximation ratio: "<<maxratio<<"\n";
	cout<<"Number of such occurences: "<<counting<<"\n";
	return 0;
}
