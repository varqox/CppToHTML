#include "main.hpp"

aho::aho_tree::aho_tree(): graph(1) // add root
{
	this->graph[0].fail=this->graph[0].long_sh_pat=0; // max shorter pattern isn't exist
}

int aho::aho_tree::add_word(const string& word, int id, char color)
{
	int ver=0; // actual node (vertex)
	for(int s=word.size(), i=0; i<s; ++i)
	{
		if(this->graph[ver].E[word[i]]!=0) ver=this->graph[ver].E[word[i]]; // actual view node = next node
		else
		{
			ver=this->graph[ver].E[word[i]]=this->graph.size(); // add id of new node
			this->graph.push_back(node(word[i])); // add new node
		}
	}
	this->graph[ver].is_pattern=true;
	this->graph[ver].pattern_id=id;
	this->graph[ver].color=color;
	this->graph[ver].depth=word.size();
return ver;
}

void aho::aho_tree::add_fails() // and the longest shorter patterns, based on BFS algorithm
{
	queue<int> V;
	// add root childrens
	for(int i=0; i<256; ++i)
	{
		if(this->graph[0].E[i]!=0) // if children exists
		{
			this->graph[this->graph[0].E[i]].fail=this->graph[this->graph[0].E[i]].long_sh_pat=0;
			V.push(this->graph[0].E[i]);
		}
	}
	while(!V.empty())
	{
		int actual=V.front(); // id of actual view node
		for(int i=0; i<256; ++i) // i is character of view node
		{
			if(this->graph[actual].E[i]!=0) // if children exists
			{
				actual=this->graph[actual].fail; // we have view node parent's fial edge
				while(actual>0 && this->graph[actual].E[i]==0) // while we don't have node with children of actual character (i)
					actual=this->graph[actual].fail;
				actual=this->graph[this->graph[V.front()].E[i]].fail=this->graph[actual].E[i]; // the longest sufix, if 0 then longest sufix = root
				// add the longest shorter pattern
				if(this->graph[actual].is_pattern) // if the fail node is pattern then is long_sh_pat
					this->graph[this->graph[V.front()].E[i]].long_sh_pat=actual;
				else // long_sh_pat is the fail node's long_sh_pat
					this->graph[this->graph[V.front()].E[i]].long_sh_pat=this->graph[actual].long_sh_pat;
				actual=V.front();
				V.push(this->graph[actual].E[i]); // add this children to queue
			}
		}
		V.pop(); // remove visited node
	}
}

void aho::find(const string& text, int x)
{
	vector<int>(text.size()).swap(this->fin); // clear fin
	int act=0, pat; // actual node - root
	for(int s=text.size(), i=0; i<s; ++i)
	{
		while(!this->troll.empty() && this->troll.front().pos<=x+i-text.size())
		{
			tree.graph[this->troll.front().id].is_pattern=this->troll.front().is_pattern;
			this->troll.pop();
		}
		//--------------------------------------
		while(act>0 && tree.graph[act].E[text[i]]==0)
			act=tree.graph[act].fail; // while we can't add text[i] to path, go to fail node
		if(tree.graph[act].E[text[i]]!=0) // if we can add text[i] to path
			act=tree.graph[act].E[text[i]];
		if(tree.graph[act].is_pattern) // if actual node is pattern, then add it to fin
		{
			this->fin[i-tree.graph[act].depth+1]=act;
		}
		else
		{
			pat=tree.graph[act].long_sh_pat; // go to the pattern node
			while(pat>0) // finding the longest pattern
			{
				if(tree.graph[pat].is_pattern)
				{
					this->fin[i-tree.graph[pat].depth+1]=pat; // add pat node to fin
					break;
				}
				pat=tree.graph[pat].long_sh_pat; // go to the next pattern
			}
		}
	}
}