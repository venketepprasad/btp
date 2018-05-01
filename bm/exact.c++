#include <bits/stdc++.h>
#define ll long long
#define f(i,m,n) for(int i=m;i<n;i++)

using namespace std;
vector<ll> scratch,temp;
int ranking[200];


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

int main()
{
	map<ll,ll> hm;
	ll x = 0,val;
	f(i,1,11)
	{
		x = 10*x + i;
		hm[x] = 0;
	}
	f(l,2,10)
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
				if(scratch[i]!=1)
				{
					x = move(scratch,i);
					val = min(val,hm[x]+1);
				}
			}
			x = 0;
			f(i,0,v.size())
				x = x*10 + v[i];
			hm[x] = val;	
		}	
	}
	for(auto it = hm.begin(); it!=hm.end() ;it++)
		cout<<it->first<<" "<<it->second<<"\n"; 	   
	return 0;
}
