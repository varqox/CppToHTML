#include <iostream>
#include <fstream>
#include <queue>
#include <cstdlib>
#include <sys/time.h>
#include <cmath>

using namespace std;

namespace task
{
	string t_name, time, command, path="tests/";
	queue<string> wa;

	inline void clean();
	bool run(bool);
	void spr(string name, string exec);
}

inline void task::clean()
{
#ifdef WIN32
	char _empty=system("del tests\\out");
#else
	char _empty=system("rm tests/out");
#endif
}

bool task::run(bool out_wrongs=false)
{
	// runtime
	timeval ts, te;
	gettimeofday(&ts, NULL);
#ifdef WIN32
char _empty=system((command+path+t_name+".in > tests\\out").c_str());
#else
char _empty=system((command+path+t_name+".in > tests/out").c_str());
#endif
	gettimeofday(&te, NULL);
	double cl=(te.tv_sec+static_cast<double>(te.tv_usec)/1000000)-(ts.tv_sec+static_cast<double>(ts.tv_usec)/1000000);
	// round with precision 3
	cl*=1000;
	cl=round(cl)/1000;
	// end of runtime
	fstream out, ans;
	out.open("tests/out", ios_base::in);
	ans.open((path+t_name+".out").c_str(), ios_base::in);
	string s_out, s_ans;
	cout << fixed;
	cout.precision(3);
	int line=0;
	while(out.good() && ans.good())
	{
		++line;
		getline(ans,s_ans);
		getline(out,s_out);
		if(s_ans!=s_out || (!out.good() && (ans.good() && s_ans.empty())))
		{
			cout << t_name << ": Wrong! time - " << cl << "s >> line:" << line;
			if(out_wrongs)
			{
				cout << "\nGet: ";
				if(!out.good()) cout << "EOF";
				else cout << '\'' << s_out << '\'';
				cout << "\nExpected: '" << s_ans << '\'';
			}
			cout << endl;
			clean();
			return false;
		}
	}
	out.close();
	ans.close();
	cout << t_name << ": [ OK ] time - " << cl << "s" << endl;
	clean();
return true;
}

void task::spr(string name, string exec)
{
#ifdef WIN32
	{char _emp=system(("dir /b tests\\"+name+" > tests\\list.txt").c_str());}
#else
	command="./";
	{char _emp=system(("ls -1 -t -r tests/"+name+" > tests/list.txt").c_str());}
#endif
	fstream list;
	path+=name+"/";
	list.open("tests/list.txt", ios_base::in);
	if(!list.good())
	{
		cout << "Cannot list tests directory!\n";
		return;
	}
	command+=exec+" < ";
	queue<string> t_lst;
	while(!list.eof())
	{
		getline(list,t_name);
		if(t_name[t_name.size()-3]=='.' && t_name[t_name.size()-2]=='i' && t_name[t_name.size()-1]=='n')
		{
			t_name.erase(t_name.size()-3, 3);
			t_lst.push(t_name);
		}
	}
	while(!t_lst.empty())
	{
		t_name.swap(t_lst.front());
		t_lst.pop();
		if(!run()) wa.push(t_name);
	}
	list.close();
#ifdef WIN32
	{char _empty=system("del tests\\list.txt");}
#else
	{char _empty=system("rm tests/list.txt");}
#endif
	if(!wa.empty())
	{
		cout << "\nWrong tests: " << wa.front();
		wa.pop();
		while(!wa.empty())
		{
			cout << ", " << wa.front();
			wa.pop();
		}
		cout << endl;
	}
}

int main(int argc, char **argv)
{
	if(argc!=3)
	{
		if(argc==4)
		{
		#ifndef WIN32
			task::command="./";
		#endif
			task::path+=string(argv[1])+'/';
			task::command+=string(argv[2])+" < ";
			task::t_name=argv[3];
			task::run(true);
		return 0;
		}
	#ifdef WIN32
		cout << "Usage: spr.exe <task name> <exec name> [test name]\n";
	#else
		cout << "Usage: spr <task name> <exec name> [test name]\n";
	#endif
		return 1;
	}
	task::spr(string(argv[1]), string(argv[2]));
return 0;
}
