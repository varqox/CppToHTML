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

namespace aho
{
	namespace tree
	{
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

		void init()
		{
			graph.resize(1); // add root
			graph[0].fail=graph[0].long_sh_pat=0; // max shorter pattern isn't exist
		}

		int add_word(const string& word, int id, char color)
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
			graph[ver].color=color;
			graph[ver].depth=word.size();
		return ver;
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

	vector<int> fin;
	struct ch_pat
	{
		int pos, id;
		bool is_pattern;
		ch_pat(int p1, int p2, bool p3): pos(p1), id(p2), is_pattern(p3)
		{}
	};
	queue<ch_pat> troll;

	void find(const string& text, int x=0)
	{
		vector<int>(text.size()).swap(fin); // clear fin
		int act=0, pat; // actual node - root
		for(int s=text.size(), i=0; i<s; ++i)
		{
			while(!troll.empty() && troll.front().pos<=x+i-text.size())
			{
				tree::graph[troll.front().id].is_pattern=troll.front().is_pattern;
				troll.pop();
			}
			//--------------------------------------
			while(act>0 && tree::graph[act].E[text[i]]==0)
				act=tree::graph[act].fail; // while we can't add text[i] to path, go to fail node
			if(tree::graph[act].E[text[i]]!=0) // if we can add text[i] to path
				act=tree::graph[act].E[text[i]];
			if(tree::graph[act].is_pattern) // if actual node is pattern, then add it to fin
			{
				fin[i-tree::graph[act].depth+1]=act;
			}
			else
			{
				pat=tree::graph[act].long_sh_pat; // go to the pattern node
				while(pat>0) // finding the longest pattern
				{
					if(tree::graph[pat].is_pattern)
					{
						fin[i-tree::graph[pat].depth+1]=pat; // add pat node to fin
						break;
					}
					pat=tree::graph[pat].long_sh_pat; // go to the next pattern
				}
			}
		}
	}
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

struct change 
{
	int pos, id; // pos - position, node id
	char color;
	change(int ps, int idx, char p2): pos(ps), id(idx), color(p2)
	{}
};
bool is_name[256]={}, is_true_name[256]={};
queue<change> changs;

void make_tree()
{
	/*vector<aho::tree::node>().swap(aho::tree::graph); // clear tree::graph
	aho::tree::init(); // initialize tree*/
	fstream keys_file("patterns/red_keys.txt", ios::in);
	string key;
	while(keys_file.good())
	{
		getline(keys_file,key);
		aho::tree::add_word(key,0,5);
	}
	keys_file.close();
	keys_file.open("patterns/keywords.txt", ios::in);
	while(keys_file.good())
	{
		getline(keys_file,key);
		aho::tree::add_word(key,0,3);
	}
	keys_file.close();
	keys_file.open("patterns/types.txt", ios::in);
	while(keys_file.good())
	{
		getline(keys_file,key);
		aho::tree::add_word(key,0,4);
	}
	keys_file.close();
	aho::tree::add_fails(); // add fails edges
}

void parse(const string& code)
{
	vector<aho::tree::node>().swap(aho::tree::graph); // clear tree::graph
	aho::tree::init(); // initialize tree
	aho::tree::add_word("class",0,0);
	aho::tree::add_word("struct",1,0);
	aho::tree::add_word("union",2,0);
	aho::tree::add_word("enum",3,0);
	aho::tree::add_word("namespace",4,0);
	aho::tree::add_word("typename",5,0);
	aho::tree::add_word("typedef",6,0);
	aho::tree::add_fails(); // add fails edges
	aho::find(code);
	vector<aho::tree::node> pre_graph;
	pre_graph.swap(aho::tree::graph);
	aho::tree::init();
	string type;
	int ps, id;
	for(int cl=code.size(), i=0; i<cl; ++i)
	{
		if(aho::fin[i]!=0)
		{
			type="";
			switch(pre_graph[aho::fin[i]].pattern_id)
			{
				case 0:
					ps=i;
					i+=5;
					while(i<cl && code[i]==' ')
						++i;
					while(i<cl && is_true_name[code[i]])
					{
						type+=code[i];
						++i;
					}
					if(!type.empty())
					{
						id=aho::tree::add_word(type,0,4);
						changs.push(change(ps,id,2));
						changs.push(change(i,id,4));
						aho::tree::add_word(type+"::",0,2);
					}
					break;
				case 1:
					ps=i;
					i+=6;
					while(i<cl && code[i]==' ')
						++i;
					while(i<cl && is_true_name[code[i]])
					{
						type+=code[i];
						++i;
					}
					if(!type.empty())
					{
						id=aho::tree::add_word(type,0,4);
						changs.push(change(ps,id,2));
						changs.push(change(i,id,4));
						aho::tree::add_word(type+"::",0,2);
					}
					break;
				case 2:
					ps=i;
					i+=5;
					while(i<cl && code[i]==' ')
						++i;
					while(i<cl && is_true_name[code[i]])
					{
						type+=code[i];
						++i;
					}
					if(!type.empty())
					{
						id=aho::tree::add_word(type,0,4);
						changs.push(change(ps,id,2));
						changs.push(change(i,id,4));
						aho::tree::add_word(type+"::",0,2);
					}
					break;
				case 3:
					ps=i;
					i+=4;
					while(i<cl && code[i]==' ')
						++i;
					while(i<cl && is_true_name[code[i]])
					{
						type+=code[i];
						++i;
					}
					if(!type.empty())
					{
						id=aho::tree::add_word(type,0,4);
						changs.push(change(ps,id,2));
						changs.push(change(i,id,4));
					}
					break;
				case 4:
					ps=i;
					i+=9;
					while(i<cl && code[i]==' ')
						++i;
					while(i<cl && is_true_name[code[i]])
					{
						type+=code[i];
						++i;
					}
					if(!type.empty())
					{
						id=aho::tree::add_word(type,0,4);
						aho::troll.push(aho::ch_pat(i-1,id,true));
						aho::troll.push(aho::ch_pat(i,id,false));
						changs.push(change(ps,id,2));
						changs.push(change(i,id,4));
						id=aho::tree::add_word(type+"::",0,2);
					}
					break;
				case 5:
					ps=i;
					i+=8;
					while(i<cl && code[i]==' ')
						++i;
					while(i<cl && is_true_name[code[i]])
					{
						type+=code[i];
						++i;
					}
					if(!type.empty())
					{
						id=aho::tree::add_word(type,0,4);
						changs.push(change(ps,id,2));
						changs.push(change(i,id,4));
						aho::tree::add_word(type+"::",0,2);
					}
					break;
				case 6:
					ps=i;
					i+=7;
					while(i<cl && code[i]!=';')
						++i;
					--i;
					while(i>0 && (code[i]==' ' || code[i]=='\n'))
						--i;
					int ed=i;
					while(i>0 && is_true_name[code[i]])
						--i;
					++i;
					while(i<=ed && code[i]!=';')
					{
						type+=code[i];
						++i;
					}
					if(!type.empty())
					{
						id=aho::tree::add_word(type,0,4);
						changs.push(change(ps,id,2));
						changs.push(change(i,id,4));
						id=aho::tree::add_word(type+"::",0,2);
					}
					break;
			}
			if(!type.empty())
				cout << type << " " << type+"::" << endl;
		}
	}
	make_tree();
}

void make_changes(int i)
{
	while(!changs.empty() && changs.front().pos<=i)
	{
		aho::tree::graph[changs.front().id].color=changs.front().color;
		changs.pop();
	}
}

string synax_highlight(const string& code, int x)
{
	aho::find(code, x);
	string ret;
	char old_color=0;
	for(int cl=code.size(), i=0; i<cl; ++i)
	{
		make_changes(x+i-code.size());
		//cout << i << ": " << aho::tree::graph[3].color+0 << endl;
		//if pattern isn't included in any name (front)
		if(aho::fin[i]!=0 && (aho::tree::graph[aho::fin[i]].color==5 || aho::tree::graph[aho::fin[i]].color==2 || i==0 || !is_name[code[i-1]]))
		{
			int end=i+aho::tree::graph[aho::fin[i]].depth;
			//if pattern is included in any name (end)
			if(aho::tree::graph[aho::fin[i]].color!=2 && aho::tree::graph[aho::fin[i]].color!=5 && (end>=cl || is_name[code[end]]))
			{
				switch(code[i])
				{
					case '<': ret+="&lt;";break;
					case '>': ret+="&gt;";break;
					case '&': ret+="&amp;";break;
					default: ret+=code[i];break;
				}
				continue;
			}
			if(code[i]=='(')
			{
				int h=ret.size()-1;
				while(h>=0 && ret[h]==' ')
					--h;
				while(h>=0 && is_true_name[ret[h]])
					--h;
				if(ret.size()-h>1)
				{
					if(old_color==4)
						ret.insert(h+1, "<span class=\"p1\">");
					else
						ret.insert(h+1, "<span class=\"p41\">");
				}
				ret+="</span>";
			}
			else if(aho::tree::graph[aho::fin[i]].color!=5 || (code[i]!='*' && code[i]!='&'))
				old_color=aho::tree::graph[aho::fin[i]].color;
			//cout << aho::fin[i] << "{ " << aho::tree::graph[aho::fin[i]].color+0 << endl;
			ret+="<span class=\"p"+to_string(aho::tree::graph[aho::fin[i]].color)+"\">";
			for(; i<end; ++i)
			{
				switch(code[i])
				{
					case '<': ret+="&lt;";break;
					case '>': ret+="&gt;";break;
					case '&': ret+="&amp;";break;
					default: ret+=code[i];break;
				}
			}
			ret+="</span>";
			--i;
		}
		else
		{
			switch(code[i])
			{
				case '<': ret+="&lt;";break;
				case '>': ret+="&gt;";break;
				case '&': ret+="&amp;";break;
				default: ret+=code[i];break;
			}
		}
	}
return ret;
}

string code_coloring(const string& code) // coloring comments, preprocesor, chars and strings
{
	parse(code);
	string ret, rest;
	for(int cl=code.size(), i=0; i<cl; ++i)
	{
		if(code[i]=='#') // preprocessor
		{
			ret+=synax_highlight(rest, i);
			rest="";
			ret+="<span class=\"p1\">";
			while(i<cl && code[i]!='\n')
			{
				if(i+1<cl && code[i]=='\\' && code[i+1]=='\n')
				{
					ret+="\\\n";
					++i;
				}
				else
				{
					switch(code[i])
					{
						case '<': ret+="&lt;";break;
						case '>': ret+="&gt;";break;
						case '&': ret+="&amp;";break;
						default: ret+=code[i];break;
					}
				}
				++i;
			}
			ret+="</span>";
			--i;
		}
		else if(i+1<cl && code[i]=='/' && code[i+1]=='/') // oneline comment
		{
			ret+=synax_highlight(rest, i);
			rest="";
			ret+="<span class=\"p8\">";
			while(i<cl && code[i]!='\n')
			{
				ret+=code[i];
				++i;
			}
			ret+="</span>";
			--i;
		}
		else if(i+1<cl && code[i]=='/' && code[i+1]=='*') // multiline comment
		{
			ret+=synax_highlight(rest, i);
			rest="";
			ret+="<span class=\"p8\">/*";
			i+=2;
			while(i<cl && !(code[i-1]=='*' && code[i]=='/'))
			{
				ret+=code[i];
				++i;
			}
			if(i<cl) // if comment ending
				ret+='/';
			ret+="</span>";
		}
		else if(code[i]=='"' || code[i]=='\'') // strings and chars
		{
			ret+=synax_highlight(rest, i);
			rest="";
			char str_or_char=code[i];
			ret+="<span class=\"p7\">";
			ret+=str_or_char;
			++i;
			while(i<cl && code[i]!=str_or_char)
			{
				if(code[i]=='\\')
				{
					++i;
					ret+="<span class=\"p6\">\\";
					if(i<cl)
					{
						switch(code[i])
						{
							case '<': ret+="&lt;";break;
							case '>': ret+="&gt;";break;
							case '&': ret+="&amp;";break;
							default: ret+=code[i];break;
						}
					}
					ret+="</span>";
				}
				else
				{
					switch(code[i])
					{
						case '<': ret+="&lt;";break;
						case '>': ret+="&gt;";break;
						case '&': ret+="&amp;";break;
						default: ret+=code[i];break;
					}
				}
				++i;
			}
			ret+=str_or_char;
			ret+="</span>";
		}
		else if(code[i]>='0' && code[i]<='9' && (i==0 || !is_true_name[code[i-1]])) // numbers
		{/*
			if(code[i-1]>='0' && code[i-1]<='9')
				rest+=code[i];
			else*/
			{
				ret+=synax_highlight(rest, i);
				rest="";
				 bool point=false;
				 ret+="<span class=\"p6\">";
				 ret+=code[i];
				 ++i;
				 while(i<cl && ((code[i]>='0' && code[i]<='9') || code[i]=='.'))
				 {
				 	if(code[i]=='.')
				 	{
				 		if(point) break;
				 		point=true;
				 	}
				 	ret+=code[i];
				 	++i;
				 }
				 if(i<cl && code[i]=='L')
				 {
				 	ret+='L';
				 	++i;
				 	if(i<cl && code[i]=='L')
				 	{
				 		ret+='L';
				 		++i;
				 	}
				 }
				 ret+="</span>";
				 --i;
			}
		}
		else rest+=code[i];
	}
	ret+=synax_highlight(rest, code.size());
return ret;
}

void compress(string file_name)
{
	fstream file;
	file.open(file_name.c_str(), ios::in);
	string k, g, m, h;
	stack<string> base;
	char hz=file.get();
	while(file.good())
	{
		k+=hz;
		hz=file.get();
	}
	file.close();
	file.open(file_name.c_str(), ios::out);
	for(int i=0; i<k.size(); i++)
	{
		if(i+6<k.size() && k[i]=='<' && k[i+1]=='/' && k[i+2]=='s' && k[i+3]=='p' && k[i+4]=='a' && k[i+5]=='n' && k[i+6]=='>')
		{
			i+=7;
			if(i+4<k.size() && k[i]=='<' && k[i+1]=='s' && k[i+2]=='p' && k[i+3]=='a' && k[i+4]=='n')
			{
				i+=5;
				m="<span";
				while(i<k.size() && k[i]!='>')
				{
					m+=k[i];
					i++;
				}
				m+='>';
				if(g!=m){file << "</span>" << m;g=m;}
			}
			else
			{
				file << "</span>";
				g="";
				i--;
			}
		}
		else if(i+4<k.size() && k[i]=='<' && k[i+1]=='s' && k[i+2]=='p' && k[i+3]=='a' && k[i+4]=='n')
		{
			i+=5;
			m="<span";
			while(i<k.size() && k[i]!='>')
			{
				m+=k[i];
				i++;
			}
			m+='>';
			if(g!=m){file << m;g=m;}
		}
		else file << k[i];
	}
	file.close();
}

int main(int argc, char **argv)
{
	ios_base::sync_with_stdio(false);
	if(argc<2)
	{
		cout << "Usage: CppToHTML <file>" << endl;
		return 0;
	}
	// make is_name array
	is_true_name['_']=is_name['_']=true;
	for(int i='A'; i<='Z'; ++i)
		is_true_name[i]=is_name[i]=true;
	for(int i='a'; i<='z'; ++i)
		is_true_name[i]=is_name[i]=true;
	for(int i='0'; i<='9'; ++i)
		is_true_name[i]=true;
	// ------------------
	string file_name=argv[1];
	fstream file(file_name.c_str(), ios_base::in), output;
	if(!file.good()) cout << "Connot open file!" << endl;
	else
	{
		output.open((file_name+".html").c_str(), ios::out);
		string input, lol;
		int i=2;
		output << "<html>\n<head>\n<meta charset=\"utf-8\">\n<style>\ndiv:after\n{\n	content: \".\";\n	display: inline-block;\n	clear: both;\n	visibility: hidden;\n	line-height: 0;\n	height: 0;\n}\n\ndiv\n{\n	display: inline-block;\n}\n\nbody\n{\n  background: #272822;\n  color: #f8f8f2;\n  font-family: Helvetica, Arial, sans-serif;\n  font-size: 14px;\n}\n.code\n{\n  width: 100%;\n  border: 2px solid #49483e;\n  border-radius: 4px;\n}\n\n.cpp_code\n{\n  text-align: left;\n	margin: 0;\n  margin-left: 0px;\n  overflow: auto;\n  padding-left: 1em;\n  padding-bottom: 5px;\n  padding-top: 5px;\n  padding-right: 5px;\n  border-left: 2px solid #49483e;\n}\n\n.num_lines\n{\n  color: #8f908a;\n  float: left;\n  margin: 0px;\n  text-align: right;\n  padding-left: 3px;\n  padding-right: 5px;\n  padding-bottom: 5px;\n  padding-top: 5px;\n}\n\n.p1{color: #a6e22e;}\n.p2{color: #ff9b4b;}\n.p3{color: #f92672;}\n.p4{color: #66d9ef;font-style: italic;}\n.p41{color: #66d9ef;}\n.p5{color: #b15555;}\n.p6{color: #ae81ff;}\n.p7{color: #e6db74;}\n.p8{color: #75715e;}\n</style>\n</head>\n<body>\n<div class=\"code\">\n<pre class=\"num_lines\">\n";
		if(!file.eof())
		{
			getline(file, lol);
			// input+=lol (tab_size=4)
			for(int ll=lol.size(), q=0; q<ll; ++q)
			{
				if(lol[q]=='\t') input+="    ";
				else input+=lol[q];
			}
			output << '1' << endl;
		}
		while(!file.eof())
		{
			input+='\n';
			getline(file, lol);
			// input+=lol (tab_size=4)
			for(int ll=lol.size(), q=0; q<ll; ++q)
			{
				if(lol[q]=='\t') input+="    ";
				else input+=lol[q];
			}
			output << i << '\n';
			++i;
		}
		file.close();
		output << "</pre>\n<pre class=\"cpp_code\">\n";
		//cout << input << endl;
		output << code_coloring(input) << endl;
		output << "</pre>\n</div>\n</body>\n</html>";
		output.close();
		compress(file_name+".html");
	}
return 0;
}