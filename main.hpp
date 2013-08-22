#include <iostream>
#include <fstream>
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
			char color; // highlight color
			unsigned char character; // this node character
			node(unsigned char letter=0): is_pattern(false), character(letter)
			{
				for(int i=0; i<256; ++i)
					E[i]=0;
			}
			~node(){}
		};

		vector<node> graph;

		aho_tree();
		~aho_tree(){}
		int add_word(const string& word, int id, char color);
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
		char color;
		change(int ps, int idx, char p2): pos(ps), id(idx), color(p2)
		{}
	};
	extern aho _aho;
	extern queue<change> changs;
	void parse(const string& code); // finds types and analysis 
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