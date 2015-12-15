#include"EZ_lexicon.h"
#include<iostream>
#include<string>

using std::string;
using std::cin;
using std::cout;
using std::endl;

int main(){
	string str;
	cout << "building lexicon " << endl;
	lexicon lex(".\\lexicon_gbk.lst");
	cout << "done " << endl;
	while (cin >> str)
	{
		cout << lex[str] << endl;
	}
	system("pause");
	return 0;
}