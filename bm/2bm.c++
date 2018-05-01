#include <bits/stdc++.h>
#define ll long long
using namespace std;

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
	vector<int> seq;
	vector<vector<int> > sets;
	for(i=0;i<n;i++)
	{
		seq.push_back(v[i]);
		if(i<n-1)
		{
			if(v[i+1] < v[i])
			{
				sets.push_back(seq);
				seq.clear();
			}
		}
	}
	if(seq.size())
		sets.push_back(seq);

	// Construction of graph by finding all edges in the graph
	int edges[n+1][n+1];
	memset(edges,0,sizeof edges);
	for(i=0;i<sets.size();i++)
	{
		for(j=0;j<sets[i].size();j++)
		{
			for(k=j+1;k<sets[i].size();k++)
			{
				if(sets[i][j] < sets[i][k])
					edges[sets[i][j]][sets[i][k]] = 1;
			}
		}
	}

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

	// Computing the noncrossing edges for the initial graph
	int noncross[n+1][n+1],hitting[n+1],leaving[n+1];
	memset(noncross,0,sizeof noncross);
	memset(hitting,0,sizeof hitting);
	memset(leaving,0,sizeof leaving);

	queue<pair<int,int> > myq;
	myq.push(make_pair(1,n));
	while(!myq.empty())
	{
		tie(x,y) = myq.front();
		myq.pop();
		if(x>=y)
			continue;
		if(cs[x][y] < 2)
		{
			myq.push(make_pair(x+1,y-1));
			if(cs[x][y] == 1)
			{
				noncross[x][y] = 1;
				hitting[y] = x;
				leaving[x] = y;
			}
		}
		else
		{
			myq.push(make_pair(x,cs[x][y]-n));
			myq.push(make_pair(cs[x][y]-n,y));
		}
	}


	// Computing all blocks
	list<tuple<int,int,int> > blocks;
	for(i=0;i<sets.size();i++)
	{
		st = sets[i][0];
		for(j=1;j<sets[i].size();j++)
		{
			if(sets[i][j] != sets[i][j-1] + 1)
			{
				blocks.push_back(make_tuple(st,sets[i][j-1],i));
				st = sets[i][j];
			}
		}
		blocks.push_back(make_tuple(st,sets[i][j-1],i));
	}


	while(blocks.size() > 1)
	{	
		for(auto it = blocks.begin();it!=blocks.end();it++)
		{
			tie(x,y,z) = *it;
			// Finding Free blocks 
			if(hitting[x] == 0 && leaving[y] == 0)
			{
				it = blocks.erase(it);
				cout<<"Block ["<<x<<","<<y<<"] ";
				predz = succz = -1;
				for(auto it2 = blocks.begin(); it2!=blocks.end();it2++)
				{
					tie(x1,y1,z1) = *it2;
					if(y1 == x-1)
					{
						tie(predx,predy,predz) = *it2;
						it2 = blocks.erase(it2);
						it2--;
					}
					if(x1 == y+1)
					{
						tie(succx,succy,succz) = *it2;
						it2 = blocks.erase(it2);
						it2--;	 
					}
				}
	
				if(predz == succz)
				{
					cout<<"is merged with its predecessor Block ["<<predx<<","<<predy<<"] ";
					cout<<"and with its successor Block ["<<succx<<","<<succy<<"] ";
					blocks.push_back(make_tuple(predx,succy,succz));
					cout<<"to form a bigger Block ["<<predx<<","<<succy<<"] .....\n";	
				}
				else if(predz == -1)
				{
					cout<<"is merged with its successor Block ["<<succx<<","<<succy<<"] ";
					blocks.push_back(make_tuple(x,succy,succz));
					cout<<"to form a bigger Block ["<<x<<","<<succy<<"] .....\n";
				}
				else if(succz == -1)
				{
					cout<<"is merged with its predecessor Block ["<<predx<<","<<predy<<"] ";
					blocks.push_back(make_tuple(predx,y,predz));
					cout<<"to form a bigger Block ["<<predx<<","<<y<<"] .....\n";
				}
				else
				{
					cout<<"is merged with its predecessor Block ["<<predx<<","<<predy<<"] ";
					blocks.push_back(make_tuple(predx,y,predz));
					blocks.push_back(make_tuple(succx,succy,succz));
					leaving[y] = leaving[predx];
					cout<<"to form a bigger Block ["<<predx<<","<<y<<"] .....\n";
				}

				break;
			}
		}
		num++;
	}
	cout<<"Optimal Number of moves: "<< n-1-dp[1][n]<<"\n";
	cout<<"Number of moves taken by algorithm: "<<num<<"\n";

	return 0;
}