#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stack>
#include <fstream>

using namespace std;

template<typename type>
type abs(type a)
{
	if(a<0) return -a;
return a;
}

int rd()
{
return abs(rand());
}

string to_string(int a)
{
	stack<char> st;
	while(a>0)
	{
		st.push('0'+a%10);
		a/=10;
	}
	string w;
	while(!st.empty())
	{
		w+=st.top();
		st.pop();
	}
	if(w.empty()) w="0";
return w;
}

int main()
{
	srand(time(NULL));
	int t;
	cin >> t;
	fstream test;
	{int ___e=system("rm tests/aho/*");}
	for(int q=1; q<=t; ++q)
	{
		string in, in1, out, spr, test_name;
		test_name="tests/aho/"+to_string(q);
		for(int i=rd()%1000000; i>=0; --i) // gen text
			in+=static_cast<char>(rd()%26+'a');
		in+='\n';
		for(int i=rd()%100; i>=0; --i)
		{
			for(int j=rd()%17+1; j>=0; --j)
				in1+=static_cast<char>(rd()%26+'a');
			in1+='\n';
		}
		test.open((test_name+".in").c_str(), ios_base::out);
		test << in << in1 << endl;
		test.close();
		{int ___e=system(("./kmp < "+test_name+".in > "+test_name+".out").c_str());}
		cout << q << ": generated!\n";
	}
return 0;
}