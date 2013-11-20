#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void parse(const string& code, ostream& output)
{
	for(int cl=code.size(), i=0; i<cl; ++i)
	{
		if(code[i]=='#') // preprocessor
		{
			/*ret+=synax_highlight(rest, i);
			rest="";
			ret+="<span class=\"p1\">#";
			bool in_quotation=false, is_include=false;
			while(++i<cl && code[i]!='\n' && (code[i]==' ' || code[i]=='\\'))
			{
				if(i+1<cl && code[i]=='\\' && code[i+1]=='\n')
				{
					ret+="\\\n";
					++i;
				}
				else
					switch(code[i])
					{
						case '<': ret+="&lt;";break;
						case '>': ret+="&gt;";break;
						case '&': ret+="&amp;";break;
						default: ret+=code[i];
					}
			}
			if(code[i]!='\n' && i+6<cl && code[i]=='i' && code[i+1]=='n' && code[i+2]=='c' && code[i+3]=='l' && code[i+4]=='u' && code[i+5]=='d' && code[i+6]=='e') is_include=true;
			while(i<cl && code[i]!='\n')
			{
				if(i+1<cl && code[i]=='\\' && code[i+1]=='\n')
				{
					ret+="\\\n";
					++i;
				}
				else
				{
					if(is_include)
						switch(code[i])
						{
							case '<': ret+="<span class=\"p7\">&lt;";break;
							case '>': ret+="&gt;</span>";break;
							case '"': ret+=(in_quotation^=true) ? "<span class=\"p7\">\"":"\"</span>";break;
							case '&': ret+="&amp;";break;
							default: ret+=code[i];
						}
					else
						switch(code[i])
						{
							case '<': ret+="&lt;";break;
							case '>': ret+="&gt;";break;
							case '&': ret+="&amp;";break;
							default: ret+=code[i];
						}
				}
				++i;
			}
			ret+="</span>";
			--i;*/
		}
		else if(i+1<cl && code[i]=='/' && code[i+1]=='/') // oneline comment
		{
			/*ret+=synax_highlight(rest, i);
			rest="";
			ret+="<span class=\"p8\">";
			while(i<cl && code[i]!='\n')
			{
				switch(code[i])
				{
					case '<': ret+="&lt;";break;
					case '>': ret+="&gt;";break;
					case '&': ret+="&amp;";break;
					default: ret+=code[i];
				}
				++i;
			}
			ret+="</span>";
			--i;*/
		}
		else if(i+1<cl && code[i]=='/' && code[i+1]=='*') // multiline comment
		{
			/*ret+=synax_highlight(rest, i);
			rest="";
			ret+="<span class=\"p8\">/*";
			i+=2;
			while(i<cl && !(code[i-1]=='*' && code[i]=='/'))
			{
				switch(code[i])
				{
					case '<': ret+="&lt;";break;
					case '>': ret+="&gt;";break;
					case '&': ret+="&amp;";break;
					default: ret+=code[i];
				}
				++i;
			}
			if(i<cl) // if comment ending
				ret+='/';
			ret+="</span>";*/
		}
		else if(code[i]=='"' || code[i]=='\'') // strings and chars
		{
			/*ret+=synax_highlight(rest, i);
			rest="";
			unsigned char str_or_char=code[i];
			ret+="<span class=\"p"+string(str_or_char=='\'' ? "71":"7")+"\">";
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
							default: ret+=code[i];
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
						default: ret+=code[i];
					}
				}
				++i;
			}
			ret+=str_or_char;
			ret+="</span>";*/
		}
		else ;//rest+=code[i];
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	string file_name;
	cin >> file_name;
	fstream file(file_name.c_str(), ios::in);
	if(file.good())
	{
		fstream out((file_name+'1').c_str(), ios::out);
		string input, tmp;
		getline(file, input);
		while(file.good())
		{
			input+='\n';
			getline(file, tmp);
			input+=tmp;
		}
		parse(input, out);
	}
	else cout << "Cannot open file" << endl;
return 0;
}