#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

/*
p1 - green
p2 - orange
p3 - red
p4 - turquoise italic
p41 - turquoise
p5 - light gray
p6 - purple
p7 - yellow
p8 - gray
p9 - blue
*/

// aho.cpp
class aho
{
public:
	class aho_tree
	{
	public:
		struct node
		{
			int E[256], fail, long_sh_pat, pattern_id, depth; // fail pointer, max shorter patter, pattern id
			bool is_pattern; // is pattern end in this vertex
			unsigned char color; // highlight color
			unsigned unsigned char character; // this node character
			node(unsigned unsigned char letter=0): is_pattern(false), character(letter)
			{
				for(int i=0; i<256; ++i)
					E[i]=0;
			}
			~node(){}
		};

		vector<node> graph;

		aho_tree();
		~aho_tree(){}
		int add_word(const string& word, int id, unsigned char color);
		void add_fails(); // and the longest shorter patterns, based on BFS algorithm
	};

	aho_tree tree;
	vector<int> fin;

	struct ch_pat
	{
		unsigned int pos, id;
		bool is_pattern;
		ch_pat(int p1, int p2, bool p3): pos(p1), id(p2), is_pattern(p3)
		{}
	};
	queue<ch_pat> troll;

	aho(){}
	void find(const string&, int=0);
	~aho(){}
};

// parser.cpp
namespace parser // anslysing code
{
	struct change 
	{
		int pos, id; // pos - position, node id
		unsigned char color;
		change(int ps, int idx, unsigned char p2): pos(ps), id(idx), color(p2)
		{}
	};

	namespace var_base
	{
		struct ver
		{
			int t[256];
			bool is;
			ver();
			~ver(){};
		};
		extern vector<ver> _v;
		extern queue<int> _free;
		extern bool is_there;
		void add_var(const string&);
		bool remove_var(string);
		bool is_var(const string&);
	}

	extern aho _aho;
	extern queue<change> changs;

	void init();
	string absolute_path(const string& path);
	bool file_exist(const string& file_name);
	void parse_file(string file_name);
	void parse(const string& code, bool add_changs=true); // finds types and analysis 
}

// synax_highlight
namespace synax_highlight
{
	extern aho _aho;

	void make_tree();
	void make_changes(int i);
	string synax_highlight(const string& code, int x);
	string code_coloring(const string& code); // coloring comments, preprocesor, chars and strings
}

// main.cpp
extern bool is_name[256], is_true_name[256];
string to_string(int);
void compress(string file_name);