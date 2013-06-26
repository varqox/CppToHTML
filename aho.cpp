#include <iostream>
#include <vector>
#include <queue>

using namespace std;

namespace aho
{
	namespace tree
	{
		struct node
		{
			int E[256], fail, long_sh_pat, pattern_id; // fail pointer, max shorter patter, pattern id
			bool is_pattern; // is pattern end in this vertex
			unsigned char character; // this node character
			node(unsigned char letter=0): is_pattern(false), character(letter)
			{
				for(int i=0; i<256; ++i)
					E[i]=0;
			}
			~node(){}
		};

		vector<node> graph;

		void init()
		{
			graph.resize(1); // add root
			graph[0].fail=graph[0].long_sh_pat=0; // max shorter pattern isn't exist
		}

		void add_word(const string& word, int id)
		{
			int ver=0; // actual node (vertex)
			for(int s=word.size(), i=0; i<s; ++i)
			{
				if(graph[ver].E[word[i]]!=0) ver=graph[ver].E[word[i]]; // actual view node = next node
				else
				{
					ver=graph[ver].E[word[i]]=graph.size(); // add id of new node
					graph.push_back(node(word[i])); // add new node
				}
			}
			graph[ver].is_pattern=true;
			graph[ver].pattern_id=id;
		}

		void add_fails() // and the longest shorter patterns, based on BFS algorithm
		{
			queue<int> V;
			// add root childrens
			for(int i=0; i<256; ++i)
			{
				if(graph[0].E[i]!=0) // if children exists
				{
					graph[graph[0].E[i]].fail=graph[graph[0].E[i]].long_sh_pat=0;
					V.push(graph[0].E[i]);
				}
			}
			while(!V.empty())
			{
				int actual=V.front(); // id of actual view node
				for(int i=0; i<256; ++i) // i is character of view node
				{
					if(graph[actual].E[i]!=0) // if children exists
					{
						actual=graph[actual].fail; // we have view node parent's fial edge
						while(actual>0 && graph[actual].E[i]==0) // while we don't have node with children of actual character (i)
							actual=graph[actual].fail;
						actual=graph[graph[V.front()].E[i]].fail=graph[actual].E[i]; // the longest sufix, if 0 then longest sufix = root
						// add the longest shorter pattern
						if(graph[actual].is_pattern) // if the fail node is pattern then is long_sh_pat
							graph[graph[V.front()].E[i]].long_sh_pat=actual;
						else // long_sh_pat is the fail node's long_sh_pat
							graph[graph[V.front()].E[i]].long_sh_pat=graph[actual].long_sh_pat;
						actual=V.front();
						V.push(graph[actual].E[i]); // add this children to queue
					}
				}
				V.pop(); // remove visited node
			}
		}
	}

	vector<vector<int> > fin; // finding patterns

	void find(const vector<string>& patterns, const string& text)
	{
		vector<tree::node>().swap(tree::graph); // clear tree::graph
		vector<vector<int> >().swap(fin); // clear fin
		fin.resize(patterns.size()); // set number of patterns
		tree::init(); // initialize tree
		for(int i=patterns.size()-1; i>=0; --i) // add patterns to tree
			tree::add_word(patterns[i], i);
		tree::add_fails(); // add fails edges
		int act=0, pat; // actual node - root
		for(int s=text.size(), i=0; i<s; ++i)
		{
			while(act>0 && tree::graph[act].E[text[i]]==0)
				act=tree::graph[act].fail; // while we can't add text[i] to path, go to fail node
			if(tree::graph[act].E[text[i]]!=0) // if we can add text[i] to path
				act=tree::graph[act].E[text[i]];
			if(tree::graph[act].is_pattern) // if actual node is pattern, then add it to fin
				fin[tree::graph[act].pattern_id].push_back(i);
			pat=tree::graph[act].long_sh_pat; // go to the pattern node
			while(pat>0) // finding patterns
			{
				fin[tree::graph[pat].pattern_id].push_back(i); // add pat node to fin
				pat=tree::graph[pat].long_sh_pat; // go to the next pattern
			}
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	string text;
	cin >> text;
	int n;
	cin >> n;
	vector<string> pats(n);
	for(int i=0; i<n; ++i)
		cin >> pats[i];
	aho::find(pats, text);
	for(int i=0; i<n; ++i)
	{
		cout << pats[i] << " :";
		for(int j=0; j<aho::fin[i].size(); ++j)
			cout << ' ' << aho::fin[i][j];
		cout << endl;
	}
return 0;
}