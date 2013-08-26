#include "main.hpp"

namespace parser // anslysing code
{
	aho _aho;
	queue<change> changs;

	namespace var_base
	{
		ver::ver(): is(false)
		{
			for(short int i=0; i<256; ++i)
				t[i]=0;
		}
		vector<ver> _v(1);
		queue<int> _free;
		bool is_there;

		void add_var(const string& s)
		{
			ver x;
			int k=0, sl=s.size();
			for(int i=0; i<sl; ++i)
			{
				if(_v[k].t[static_cast<int>(s[i])]==0)
				{
					if(_free.empty())
					{
						_v[k].t[static_cast<int>(s[i])]=_v.size();
						k=_v.size();
						_v.push_back(x);
					}
					else
					{
						_v[k].t[static_cast<int>(s[i])]=_free.front();
						k=_free.front();
						_v[k]=x;
						_free.pop();
					}
				}
				else k=_v[k].t[static_cast<int>(s[i])];
			}
			_v[k].is=true;
		}

		bool remove_var(const string& s)
		{
			int k=0, sl=s.size();
			stack<int> grt;
			grt.push(0);
			for(int i=0; i<sl; ++i)
			{
				if(_v[k].t[static_cast<int>(s[i])]==0) return false;
				else
				{
					k=_v[k].t[static_cast<int>(s[i])];
					grt.push(k);
				}
			}
			if(!_v[k].is) return false;
			_v[k].is=false;
			while(grt.size()>1)
			{
				--sl;
				for(short int i=0; i<256; ++i)
					if(_v[k].t[i]!=0) return true;
				grt.pop();
				_free.push(k);
				k=grt.top();
				_v[k].t[static_cast<int>(s[sl])]=0;
			}
		return true;
		}

		bool is_var(const string& s)
		{
			int k=0, sl=s.size();
			for(int i=0; i<sl; ++i)
			{
				if(_v[k].t[static_cast<int>(s[i])]==0) return false;
				else k=_v[k].t[static_cast<int>(s[i])];
			}
			if(!_v[k].is) return false;
		return true;
		}
	}

	vector<string> include_directories;

	void init()
	{
		_aho.tree.add_word("class",0,0);
		_aho.tree.add_word("struct",1,0);
		_aho.tree.add_word("union",2,0);
		_aho.tree.add_word("enum",3,0);
		_aho.tree.add_word("namespace",4,0);
		_aho.tree.add_word("typename",5,0);
		_aho.tree.add_word("typedef",6,0);
		_aho.tree.add_fails(); // add fails edges
	}

	string absolute_path(const string& path)
	{
		string out, act;
		vector<string> st;
		for(unsigned int i=0; i<path.size(); ++i)
		{
			act+=path[i];
			if(path[i]=='/')
			{
				if(act=="../") st.pop_back();
				else if(act!="./") st.push_back(act);
				act="";
			}
		}
		if(act!=".") st.push_back(act);
		for(unsigned int i=0; i<st.size(); ++i)
			out+=st[i];
	return out;
	}

	bool file_exist(const string& file_name)
	{
		fstream file(file_name.c_str(), ios_base::in);
		if(file.good())
		{
			file.close();
			return true;
		}
	return false;
	}


	void parse_file(string file_name)
	{
		queue<string> files;
		files.push(absolute_path(file_name));
		while(!files.empty())
		{
			file_name=files.front();
			if(!file_exist(file_name) || var_base::is_var(file_name))
			{
				files.pop();
				continue;
			}
			var_base::add_var(file_name);
		#ifdef DEBUG
			cout << endl << file_name << endl;
		#endif
			string code, directory, to_parse;
			{
				int k=file_name.size()-1;
				while(k>0 && file_name[k]!='/')
					--k;
				for(int i=0; i<=k; ++i)
					directory+=file_name[i];
			}
			fstream file(file_name.c_str(), ios_base::in);
			if(file.good())
			{
				string lol;
				while(!file.eof())
				{
					getline(file, lol);
					// input+=lol (tab_size=4)
					for(int ll=lol.size(), q=0; q<ll; ++q)
					{
						if(lol[q]=='\t') code+="    "; // tab size: 4
						else code+=lol[q];
					}
					code+='\n';
				}
				file.close();
			}
			for(int cl=code.size(), i=0; i<cl; ++i)
			{
				if(code[i]=='#') // preprocessor
				{
					while(++i<cl && code[i]!='\n' && (code[i]==' ' || code[i]=='\\'))
					{
						if(i+1<cl && code[i]=='\\' && code[i+1]=='\n')
							++i;
					}
					if(code[i]!='\n' && i+6<cl && code[i]=='i' && code[i+1]=='n' && code[i+2]=='c' && code[i+3]=='l' && code[i+4]=='u' && code[i+5]=='d' && code[i+6]=='e')
					{
						i+=6;
						while(++i<cl && (code[i]==' ' || code[i]=='\\'))
							if(i+1<cl && code[i]=='\\' && code[i+1]=='\n') ++i;
						string incl;
						if(code[i]=='"')
						{
							while(code[++i]!='"')
								incl+=code[i];
							files.push(directory+incl);
						}
						else
						{
							while(code[++i]!='>')
								incl+=code[i];
							for(vector<string>::iterator i=include_directories.begin(); i!=include_directories.end(); ++i)
								files.push(*i+(*--(i->end())=='/' ? "":"/")+incl);
						}
					}
					while(i<cl && code[i]!='\n')
					{
						if(i+1<cl && code[i]=='\\' && code[i+1]=='\n') ++i;
						++i;
					}
				}
				else if(i+1<cl && code[i]=='/' && code[i+1]=='/') // oneline comment
				{
					while(++i<cl && code[i]!='\n');
				}
				else if(i+1<cl && code[i]=='/' && code[i+1]=='*') // multiline comment
				{
					++i;
					while(++i<cl && !(code[i-1]=='*' && code[i]=='/'));
				}
				else if(code[i]=='"' || code[i]=='\'') // strings and chars
				{
					unsigned char str_or_char=code[i];
					++i;
					while(i<cl && code[i]!=str_or_char)
					{
						if(code[i]=='\\')
							++i;
						++i;
					}
				}
				else to_parse+=code[i];
			}
			parse(to_parse, false);
			files.pop();
		}
	}

	void parse(const string& code, bool add_changs) // finds types and analysis preprocessor
	{
		_aho.find(code);
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
						if(code[i]!=' ' && code[i]!='\t' && code[i]!='\n') break;
						while(i<cl && code[i]==' ')
							++i;
						while(i<cl && is_true_name[static_cast<int>(code[i])])
						{
							type+=code[i];
							++i;
						}
						if(!type.empty())
						{
							id=synax_highlight::_aho.tree.add_word(type,0,9);
							if(add_changs)
							{
								changs.push(change(ps,id,2));
								changs.push(change(i,id,9));
							}
							synax_highlight::_aho.tree.add_word(type+"::",0,2);
						}
						break;
					case 1:
						ps=i;
						i+=6;
						if(code[i]!=' ' && code[i]!='\t' && code[i]!='\n') break;
						while(i<cl && code[i]==' ')
							++i;
						while(i<cl && is_true_name[static_cast<int>(code[i])])
						{
							type+=code[i];
							++i;
						}
						if(!type.empty())
						{
							id=synax_highlight::_aho.tree.add_word(type,0,9);
							if(add_changs)
							{
								changs.push(change(ps,id,2));
								changs.push(change(i,id,9));
							}
							synax_highlight::_aho.tree.add_word(type+"::",0,2);
						}
						break;
					case 2:
						ps=i;
						i+=5;
						if(code[i]!=' ' && code[i]!='\t' && code[i]!='\n') break;
						while(i<cl && code[i]==' ')
							++i;
						while(i<cl && is_true_name[static_cast<int>(code[i])])
						{
							type+=code[i];
							++i;
						}
						if(!type.empty())
						{
							id=synax_highlight::_aho.tree.add_word(type,0,9);
							if(add_changs)
							{
								changs.push(change(ps,id,2));
								changs.push(change(i,id,9));
							}
							synax_highlight::_aho.tree.add_word(type+"::",0,2);
						}
						break;
					case 3:
						ps=i;
						i+=4;
						if(code[i]!=' ' && code[i]!='\t' && code[i]!='\n') break;
						while(i<cl && code[i]==' ')
							++i;
						while(i<cl && is_true_name[static_cast<int>(code[i])])
						{
							type+=code[i];
							++i;
						}
						if(!type.empty())
						{
							id=synax_highlight::_aho.tree.add_word(type,0,9);
							if(add_changs)
							{
								changs.push(change(ps,id,2));
								changs.push(change(i,id,9));
							}
						}
						break;
					case 4:
						ps=i;
						i+=9;
						if(code[i]!=' ' && code[i]!='\t' && code[i]!='\n') break;
						while(i<cl && code[i]==' ')
							++i;
						while(i<cl && is_true_name[static_cast<int>(code[i])])
						{
							type+=code[i];
							++i;
						}
						if(!type.empty())
						{
							id=synax_highlight::_aho.tree.add_word(type,0,9);
							// synax_highlight::_aho.troll.push(aho::ch_pat(i-1,id,true));
							// synax_highlight::_aho.troll.push(aho::ch_pat(i,id,false));
							if(add_changs)
							{
								changs.push(change(ps,id,2));
								changs.push(change(i,id,4));
							}
							synax_highlight::_aho.tree.add_word(type+"::",0,2);
						}
						break;
					case 5:
						ps=i;
						i+=8;
						if(code[i]!=' ' && code[i]!='\t' && code[i]!='\n') break;
						while(i<cl && code[i]==' ')
							++i;
						while(i<cl && is_true_name[static_cast<int>(code[i])])
						{
							type+=code[i];
							++i;
						}
						if(!type.empty())
						{
							id=synax_highlight::_aho.tree.add_word(type,0,9);
							if(add_changs)
							{
								changs.push(change(ps,id,2));
								changs.push(change(i,id,9));
							}
							synax_highlight::_aho.tree.add_word(type+"::",0,2);
						}
						break;
					case 6:
						ps=i;
						i+=7;
						if(code[i]!=' ' && code[i]!='\t' && code[i]!='\n') break;
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
							id=synax_highlight::_aho.tree.add_word(type,0,9);
							if(add_changs)
							{
								changs.push(change(ps,id,2));
								changs.push(change(i,id,9));
							}
							synax_highlight::_aho.tree.add_word(type+"::",0,2);
						}
						break;
				}
			#ifdef DEBUG
				if(!type.empty())
					cout << type << endl;
			#endif
			}
		}
	}
}