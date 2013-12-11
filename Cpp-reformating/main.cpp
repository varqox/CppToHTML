#include <iostream>
#include <fstream> //trololo
#include <cstring>
#include <vector>

using namespace std;

namespace span
{
	const char
	preprocessor[]="<span style=\"color: #00a000\">",
	comment[]="<span style=\"color: #a0a0a0\">",
	string[]="<span style=\"color: #0000ff\">",
	character[]="<span style=\"color: #0000ff\">",
	special_character[]="<span style=\"color: #f000f0\">",
	number[]="<span style=\"color: #f000f0\">",
	keyword[]="<span style=\"\">",
	type[]="<span style=\"\">",
	function[]="<span style=\"\">",
	end[]="</span>";
}

const bool is_name[256]={false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, true, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

inline string safe_character(char _c)
{
	if(_c=='<') return "&lt";
	if(_c=='>') return "&gt";
	if(_c=='&') return "&amp";
	return string(&_c, 1);
}

string synax_highlight(const string& code)
{
	string out;
	for(string::const_iterator i=code.begin(); i!=code.end(); ++i)
		out+=safe_character(*i);
	return out;
}

void color_code(const string& code, ostream& output)
{
	string to_syn_high;
	for(int code_len=code.size(), i=0; i<code_len; ++i)
	{
		if(code[i]=='#') // preprocessor
		{
			output << synax_highlight(to_syn_high);
			to_syn_high="";
			output << span::preprocessor << code[i];
			while(++i<code_len && code[i]!='\n')
			{
				if(i+1<code_len && 0==memcmp(code.c_str()+i, "\\\n", 2))
				{
					output << "\\\n";
					++i;
				}
				else
					output << safe_character(code[i]);
			}
			--i;
			output << span::end;
		}
		else if(i+1<code_len && code[i]=='/' && code[i+1]=='/') // oneline comment
		{
			output << synax_highlight(to_syn_high);
			to_syn_high="";
			output << span::comment << code[i];
			while(++i<code_len && code[i]!='\n')
				output << safe_character(code[i]);
			--i;
			output << span::end;
		}
		else if(i+1<code_len && code[i]=='/' && code[i+1]=='*') // multiline comment
		{
			output << synax_highlight(to_syn_high);
			to_syn_high="";
			output << span::comment << "/*";
			i+=2;
			if(i<code_len) output << safe_character(code[i]);
			while(++i<code_len && 0!=memcmp(code.c_str()+i-1, "*/", 2))
				output << safe_character(code[i]);
			output << '/' << span::end;
		}
		else if(code[i]=='"' || code[i]=='\'') // strings and chars
		{
			output << synax_highlight(to_syn_high);
			to_syn_high="";
			unsigned char str_or_char=code[i];
			output << (str_or_char=='\'' ? span::character : span::string) << code[i];
			while(++i<code_len && code[i]!=str_or_char)
			{
				if(code[i]=='\\')
				{
					output << span::special_character << '\\';
					if(++i<code_len)
						output << safe_character(code[i]);
					output << span::end;
				}
				else
					output << safe_character(code[i]);
			}
			output << str_or_char << span::end;
		}
		else if(code[i]>='0' && code[i]<='9' && (i==0 || !is_name[static_cast<unsigned char>(code[i-1])])) // numbers
			{
				{
					output << synax_highlight(to_syn_high);
					to_syn_high="";
					output << span::number << code[i];
					bool point=false;
					while(++i<code_len && ((code[i]>='0' && code[i]<='9') || code[i]=='.'))
					{
						if(code[i]=='.')
						{
							if(point) break;
							point=true;
						}
						output << code[i];
					}
					if(i<code_len && code[i]=='L')
					{
						output << 'L';
						if(++i<code_len && code[i]=='L')
						{
							output << 'L';
							++i;
						}
					}
					--i;
					output << span::end;
				}
			}
		else to_syn_high+=code[i];
	}
	output << synax_highlight(to_syn_high);
}

int main(int argc, char** argv)
{
	ios_base::sync_with_stdio(false);
	if(argc<2)
	{
		cerr << "Usage:\ncr <file name>" << endl;
		return 1;
	}
	string file_name=argv[1];
	// cin >> file_name;
	fstream file(file_name.c_str(), ios::in);
	if(file.good())
	{
		fstream out((file_name+".html").c_str(), ios::out);
		out << "<table style=\"border-spacing: 0;\ndisplay: inline-block;\nfont-size: 14px;\nfont-family: monospace;\nline-height: 17px;\nborder: 1px solid #afafaf;\nborder-radius: 4px;\">\n<tbody>\n<tr>\n<td style=\"padding: 0\">\n<pre style=\"color: #4c4c4c;\nmargin: 0;\ntext-align: right;\npadding: 5px 4px 5px 4px;border-right: 1px solid #afafaf\">\n1\n";
		string input, tmp;
		getline(file, input);
		unsigned line=1;
		while(file.good())
		{
			out << ++line << '\n';
			input+='\n';
			getline(file, tmp);
			input+=tmp;
		}
		out << "</pre>\n</td>\n<td style=\"padding: 0\">\n<pre style=\"text-align: left;margin: 0;padding: 5px 5px 5px 1em\">\n";
		color_code(input, out);
		out << "</pre></td></tr></tbody></table>";
	}
	else cout << "Cannot open file" << endl;
return 0;
}