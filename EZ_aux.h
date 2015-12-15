#ifndef EZ_AUX_H
#define EZ_AUX_H
#include<string>
#include<vector>
using std::string;
using std::vector;


void split(const string& str, vector<string> &sv, char c = '\t')
{
	sv.clear();
	auto flag = str.begin();
	for (auto iter = str.begin(); iter != str.end(); ++iter)
	{
		if (*iter == c)
		{
			sv.push_back(string(flag, iter));
			flag = iter + 1;
		}
	}
	if (flag != str.end())
		sv.push_back(string(flag, str.end()));

	return;
}

#endif