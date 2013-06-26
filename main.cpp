#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

int main(int argc, char **argv)
{
	ios_base::sync_with_stdio(false);
	if(argc<2)
	{
		cout << "Usage: CppToHTML <file>" << endl;
		return 0;
	}
	string file_name=argv[1];
	fstream file(file_name.c_str(), ios_base::in);
	if(!file.good()) cout << "Connot open file!" << endl;
	else
	{
		string input, lol;
		getline(file, input);
		while(!file.eof())
		{
			input+='\n';
			getline(file, lol);
			input+=lol;
		}
		file.close();
		cout << input << endl;
	}
return 0;
}