#include "main.hpp"

namespace parser // anslysing code
{
	aho _aho;
	queue<change> changs;

	/*vector<string> includes_directories;
	void parse_file();*/

	void parse(const string& code) // finds types and analysis preprocessor
	{
		// vector<_aho.tree::node>().swap(_aho.tree::graph); // clear tree::graph
		// _aho.tree::init(); // initialize tree
		_aho.tree.add_word("class",0,0);
		_aho.tree.add_word("struct",1,0);
		_aho.tree.add_word("union",2,0);
		_aho.tree.add_word("enum",3,0);
		_aho.tree.add_word("namespace",4,0);
		_aho.tree.add_word("typename",5,0);
		_aho.tree.add_word("typedef",6,0);
		_aho.tree.add_fails(); // add fails edges
		_aho.find(code);
		// vector<_aho.aho_tree::node> pre_graph;
		// pre_graph.swap(_aho.tree::graph);
		// _aho.tree::init();
		string type;
		int ps, id;
		for(int cl=code.size(), i=0; i<cl; ++i)
		{
			if(_aho.fin[i]!=0)
			{
				type="";
				switch(_aho.tree.graph[_aho.fin[i]].pattern_id)
				{
					case 0:
						ps=i;
						i+=5;
						while(i<cl && code[i]==' ')
							++i;
						while(i<cl && is_true_name[static_cast<int>(code[i])])
						{
							type+=code[i];
							++i;
						}
						if(!type.empty())
						{
							id=synax_highlight::_aho.tree.add_word(type,0,4);
							changs.push(change(ps,id,2));
							changs.push(change(i,id,4));
							synax_highlight::_aho.tree.add_word(type+"::",0,2);
						}
						break;
					case 1:
						ps=i;
						i+=6;
						while(i<cl && code[i]==' ')
							++i;
						while(i<cl && is_true_name[static_cast<int>(code[i])])
						{
							type+=code[i];
							++i;
						}
						if(!type.empty())
						{
							id=synax_highlight::_aho.tree.add_word(type,0,4);
							changs.push(change(ps,id,2));
							changs.push(change(i,id,4));
							synax_highlight::_aho.tree.add_word(type+"::",0,2);
						}
						break;
					case 2:
						ps=i;
						i+=5;
						while(i<cl && code[i]==' ')
							++i;
						while(i<cl && is_true_name[static_cast<int>(code[i])])
						{
							type+=code[i];
							++i;
						}
						if(!type.empty())
						{
							id=synax_highlight::_aho.tree.add_word(type,0,4);
							changs.push(change(ps,id,2));
							changs.push(change(i,id,4));
							synax_highlight::_aho.tree.add_word(type+"::",0,2);
						}
						break;
					case 3:
						ps=i;
						i+=4;
						while(i<cl && code[i]==' ')
							++i;
						while(i<cl && is_true_name[static_cast<int>(code[i])])
						{
							type+=code[i];
							++i;
						}
						if(!type.empty())
						{
							id=synax_highlight::_aho.tree.add_word(type,0,4);
							changs.push(change(ps,id,2));
							changs.push(change(i,id,4));
						}
						break;
					case 4:
						ps=i;
						i+=9;
						while(i<cl && code[i]==' ')
							++i;
						while(i<cl && is_true_name[static_cast<int>(code[i])])
						{
							type+=code[i];
							++i;
						}
						if(!type.empty())
						{
							id=synax_highlight::_aho.tree.add_word(type,0,4);
							_aho.troll.push(aho::ch_pat(i-1,id,true));
							_aho.troll.push(aho::ch_pat(i,id,false));
							changs.push(change(ps,id,2));
							changs.push(change(i,id,4));
							id=synax_highlight::_aho.tree.add_word(type+"::",0,2);
						}
						break;
					case 5:
						ps=i;
						i+=8;
						while(i<cl && code[i]==' ')
							++i;
						while(i<cl && is_true_name[static_cast<int>(code[i])])
						{
							type+=code[i];
							++i;
						}
						if(!type.empty())
						{
							id=synax_highlight::_aho.tree.add_word(type,0,4);
							changs.push(change(ps,id,2));
							changs.push(change(i,id,4));
							synax_highlight::_aho.tree.add_word(type+"::",0,2);
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
						while(i>0 && is_true_name[static_cast<int>(code[i])])
							--i;
						++i;
						while(i<=ed && code[i]!=';')
						{
							type+=code[i];
							++i;
						}
						if(!type.empty())
						{
							id=synax_highlight::_aho.tree.add_word(type,0,4);
							changs.push(change(ps,id,2));
							changs.push(change(i,id,4));
							id=synax_highlight::_aho.tree.add_word(type+"::",0,2);
						}
						break;
				}
				if(!type.empty())
					cout << type << " " << type+"::" << endl;
			}
		}
		synax_highlight::make_tree();
	}
}