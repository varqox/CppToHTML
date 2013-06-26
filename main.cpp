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
	fstream file(file_name, ios_base::in | ios_base::out);
	if(!file.good()) cout << "Connot open file!" << endl;
	else
	{
		file.close();
	}
return 0;
}