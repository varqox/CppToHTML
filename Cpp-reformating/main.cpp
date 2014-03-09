#include "aho.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

using namespace std;

#ifdef DEBUG
#define D(...) __VA_ARGS__
#else
#define D(...)
#endif

#define LOG(x) D(cerr << #x << ": " << x << flush)
#define LOGN(x) D(cerr << #x << ": " << x << endl)

string file_get_contents(const char* name)
{
	FILE* f = fopen(name, "rb");
	if(f == NULL)
		perror("Cannot open file\n"), exit(1);
	fseek(f, 0, SEEK_END);
	size_t len = ftell(f);
	rewind(f);
	char *in = new char[len];
	fread(in, sizeof(char), len, f);
	fclose(f);
	string out(in, in+len);
	delete[] in;
return out;
}

// enum bracket_keyword{if, elseif, else, for, while/*, do, try, catch, switch*/};

enum aho_classes
{
	left_space_operator,
	right_space_operator,
	space_operator,
	no_space_operator
};

special_aho<aho_classes> _aho;

void build__aho()
{
	string w1[]={},
	w2[]={",", "new", "delete", "delete[]"},
	w3[]={"=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "^=", "|=", "?", ":", "+", "-", "*", "/", "%", "<<", ">>", "<", "<=", ">", ">=", "==", "!=", "&", "^", "|", "&&", "||"},
	w4[]={".", "::", ".*", "->", "->*", "~", "!", "++", "--", "[", "]", "(", ")"};
	vector<pair<string, aho_classes> > patterns;
	for(int s = sizeof(w1) / sizeof(string), i = 0; i < s; ++i)
		patterns.push_back(make_pair(w1[i], left_space_operator));
	for(int s = sizeof(w2) / sizeof(string), i = 0; i < s; ++i)
		patterns.push_back(make_pair(w2[i], right_space_operator));
	for(int s = sizeof(w3) / sizeof(string), i = 0; i < s; ++i)
		patterns.push_back(make_pair(w3[i], space_operator));
	for(int s = sizeof(w4) / sizeof(string), i = 0; i < s; ++i)
		patterns.push_back(make_pair(w4[i], no_space_operator));
	_aho.set_patterns(patterns);
}

void reformat(const char * file)
{
	string code(file_get_contents(file)), out;
	D(printf("%s\n", code.c_str());)
	unsigned tabs = 0;
	unsigned char last = '\n';
	_aho.find(code);
	for(unsigned i = 0, s = code.size(); i < s; ++i)
	{
		if(code[i] == ' ' || code[i] == '\t' || code[i] == '\n')
			continue;
		if(code[i] == '#') // preprocessor
		{
			if(last == ';')
				out += "\n\n";
			out += '#';
			while(++i < s && code[i] != '\n')
			{
				if(i + 1 < s && 0 == memcmp(code.c_str() + i, "\\\n", 2))
					out += "\\\n", ++i;
				else
					out += code[i];
			}
			out += code[i];
			last = '#';
		}
		else if(i + 1 < s && code[i] == '/' && code[i + 1] == '/') // oneline comment
		{
			if(last == ';' || last == '{' || (last == '}' && code[i] != ';') || last == '#' || code[i] == '{')
				out += '\n', out.append(tabs, '\t');
			else if(i > 0 && last != ' ' && (code[i - 1] == ' ' || code[i - 1] == '\t' || code[i - 1] == '\n') && last != '{')
				out += ' ';
			out += code[i];
			while(++i < s && code[i] != '\n')
				out += code[i];
			--i;
		}
		else if(i + 1 < s && code[i] == '/' && code[i + 1] == '*') // multiline comment
		{
			out += "/*";
			i+=2;
			if(i < s) out += code[i];
			while(++i < s && 0 != memcmp(code.c_str() + i - 1, "*/", 2))
				out += code[i];
			out += '/';
		}
		else if(code[i] == '"' || code[i] == '\'') // strings and chars
		{
			unsigned char str_or_char = code[i];
			out += code[i];
			while(++i < s && code[i] != str_or_char)
			{
				if(code[i] == '\\')
				{
					out += '\\';
					if(++i < s)
						out += code[i];
				}
				else
					out += code[i];
			}
			out += str_or_char;
		}
		else
		{
			if(code[i] == '}')
			{
				--tabs;
				if(last == '{')
					goto next1;
			}
			if(last == ';' || last == '{' || (last == '}' && code[i] != ';') || last == '#' || code[i] == '{')
				out += '\n', out.append(tabs, '\t');
			else if(i > 0 && last != ' ' && (code[i - 1] == ' ' || code[i - 1] == '\t' || code[i - 1] == '\n') && last != '{')
				out += ' ';
			if(code[i] == '{')
				++tabs;
		next1:
			if(_aho[i] != -1)
			{
				if(_aho.pattern(_aho[i]).second == left_space_operator)
				{
					if(out.size() == 0 || *out.rbegin() != ' ')
						out += ' ';
					out += _aho.pattern(_aho[i]).first;
					i += _aho.pattern(_aho[i]).first.size() - 1;
					last = code[i];
				}
				else if(_aho.pattern(_aho[i]).second == right_space_operator)
				{
					out += _aho.pattern(_aho[i]).first;
					out += last = ' ';
					i += _aho.pattern(_aho[i]).first.size() - 1;
				}
				else if(_aho.pattern(_aho[i]).second == space_operator)
				{
					if(out.size() == 0 || *out.rbegin() != ' ')
						out += ' ';
					out += _aho.pattern(_aho[i]).first;
					out += last = ' ';
					i += _aho.pattern(_aho[i]).first.size() - 1;
				}
				else if(_aho.pattern(_aho[i]).second == no_space_operator)
				{
					out += _aho.pattern(_aho[i]).first;
					i += _aho.pattern(_aho[i]).first.size() - 1;
					last = code[i];
				}
				D(out += "\033[01;31m";out+=code[i];out+="\033[0m";)
				continue;
			}
			out += last = code[i];
		}
	}
	printf("%s\n", out.c_str());
}

int main(int argc, char const **argv)
{
	if(argc < 2)
	{
		printf("Usage: cr [option]... <file>\nOptions:\n");
		return 1;
	}
	build__aho();
	reformat(argv[1]);
	return 0;
}