#include "main.hpp"

namespace synax_highlight
{
	aho _aho;
	void make_tree()
	{
		/*vector<_aho.tree.node>().swap(_aho.tree.graph); // clear tree::graph
		_aho.tree.init(); // initialize tree*/
		_aho.tree.add_word("true", 0, 6);
		_aho.tree.add_word("false", 0, 6);
		fstream keys_file("patterns/red_keys.txt", ios::in);
		string key;
		while(keys_file.good())
		{
			getline(keys_file,key);
			_aho.tree.add_word(key, 0, 5);
		}
		keys_file.close();
		keys_file.open("patterns/keywords.txt", ios::in);
		while(keys_file.good())
		{
			getline(keys_file,key);
			_aho.tree.add_word(key, 0, 3);
		}
		keys_file.close();
		keys_file.open("patterns/types.txt", ios::in);
		while(keys_file.good())
		{
			getline(keys_file,key);
			_aho.tree.add_word(key, 0, 4);
		}
		keys_file.close();
		_aho.tree.add_fails(); // add fails edges
	}

	void make_changes(int i)
	{
		while(!parser::changs.empty() && parser::changs.front().pos<=i)
		{
			_aho.tree.graph[parser::changs.front().id].color=parser::changs.front().color;
			parser::changs.pop();
		}
	}

	string synax_highlight(const string& code, int x)
	{
		_aho.find(code, x);
		string ret;
		char old_color=0;
		for(int cl=code.size(), i=0; i<cl; ++i)
		{
			make_changes(x+i-code.size());
			//cout << i << ": " << _aho.tree.graph[3].color+0 << endl;
			//if pattern isn't included in any name (front)
			if(_aho.fin[i]!=0 && (_aho.tree.graph[_aho.fin[i]].color==5 || _aho.tree.graph[_aho.fin[i]].color==2 || i==0 || !is_name[code[i-1]]))
			{
				int end=i+_aho.tree.graph[_aho.fin[i]].depth;
				//if pattern is included in any name (end)
				if(_aho.tree.graph[_aho.fin[i]].color!=2 && _aho.tree.graph[_aho.fin[i]].color!=5 && (end>=cl || is_name[code[end]]))
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
				else if(_aho.tree.graph[_aho.fin[i]].color!=5 || (code[i]!='*' && code[i]!='&'))
					old_color=_aho.tree.graph[_aho.fin[i]].color;
				//cout << _aho.fin[i] << "{ " << _aho.tree.graph[_aho.fin[i]].color+0 << endl;
				ret+="<span class=\"p"+to_string(_aho.tree.graph[_aho.fin[i]].color)+"\">";
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
		parser::parse(code);
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
}