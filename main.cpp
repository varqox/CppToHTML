#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

string code_coloring(const string& code) // coloring comments, preprocesor, chars and strings
{
	string ret;
	for(int cl=code.size(), i=0; i<cl; ++i)
	{
		if(code[i]=='#') // preprocessor
		{
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
					if(i<cl) ret+=code[i];
					ret+="</span>";
				}
				else ret+=code[i];
				++i;
			}
			ret+=str_or_char;
			ret+="</span>";
		}
		else ret+=code[i];
	}
return ret;
}

int main(int argc, char **argv)
{
	ios_base::sync_with_stdio(false);
	if(argc<2)
	{
		cout << "Usage: CppToHTML <file>" << endl;
		return 0;
	}
	string file_name=argv[1];
	fstream file(file_name.c_str(), ios_base::in), output;
	if(!file.good()) cout << "Connot open file!" << endl;
	else
	{
		output.open((file_name+".html").c_str(), ios::out);
		string input, lol;
		int i=2;
		output << "<html>\n<head>\n<meta charset=\"utf-8\">\n<style>\ndiv:after\n{\n	content: \".\";\n	display: inline-block;\n	clear: both;\n	visibility: hidden;\n	line-height: 0;\n	height: 0;\n}\n\ndiv\n{\n	display: inline-block;\n}\n\nbody\n{\n  background: #272822;\n  color: #f8f8f2;\n  font-family: Helvetica, Arial, sans-serif;\n  font-size: 14px;\n}\n.code\n{\n  width: 40em;\n  border: 2px solid #49483e;\n  border-radius: 4px;\n}\n\n.cpp_code\n{\n  text-align: left;\n	margin: 0;\n  margin-left: 0px;\n  overflow: auto;\n  padding-left: 1em;\n  padding-bottom: 5px;\n  padding-top: 5px;\n  padding-right: 5px;\n  border-left: 2px solid #49483e;\n}\n\n.num_lines\n{\n  color: #8f908a;\n  float: left;\n  margin: 0px;\n  text-align: right;\n  padding-left: 3px;\n  padding-right: 5px;\n  padding-bottom: 5px;\n  padding-top: 5px;\n}\n\n.p1{color: #a6e22e;}\n.p2{color: #ff9b4b;}\n.p3{color: #f92672;}\n.p4{color: #66d9ef;}\n.p5{color: #b8b8b8;}\n.p6{color: #ae81ff;}\n.p7{color: #e6db74;}\n.p8{color: gray;}\n</style>\n</head>\n<body>\n<div class=\"code\">\n<pre class=\"num_lines\">\n";
		if(!file.eof())
		{
			getline(file, input);
			output << '1' << endl;
		}
		while(!file.eof())
		{
			input+='\n';
			getline(file, lol);
			input+=lol;
			output << i << "\n";
			++i;
		}
		file.close();
		output << "</pre>\n<pre class=\"cpp_code\">\n";
		//cout << input << endl;
		output << code_coloring(input) << endl;
		output << "</pre>\n</div>\n</body>\n</html>";
		output.close();
	}
return 0;
}