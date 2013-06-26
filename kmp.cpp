#include <iostream>
#include <queue>

using namespace std;

queue<int> kmp(const string& text, const string& pattern)
{
	queue<int> out;
	int *P=new int[pattern.size()], k=0, pl=pattern.size();
	P[0]=0;
	for(int i=1; i<pl; ++i)
	{
		while(k>0 && pattern[k]!=pattern[i])
			k=P[k-1];
		if(pattern[k]==pattern[i]) ++k;
		P[i]=k;
	}
	k=0;
	for(int tl=text.size(), i=0; i<tl; ++i)
	{
		while(k>0 && pattern[k]!=text[i])
			k=P[k-1];
		if(pattern[k]==text[i]) ++k;
		if(k==pl)
		{
			out.push(i);
			k=P[k-1];
		}
	}
	delete[] P;
return out;
}

int main()
{
	ios_base::sync_with_stdio(0);
	string pat, text;
	queue<int> wys;
	cin >> text;
	int n;
	cin >> n;
	for(int i=0; i<n; ++i)
	{
		cin >> pat;
		cout << pat << " :";
		wys=kmp(text, pat);
		while(!wys.empty())
		{
			cout << ' ' << wys.front();
			wys.pop();
		}
		cout << endl;
	}
return 0;
}