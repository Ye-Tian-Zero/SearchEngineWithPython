#ifndef EZ_LABEL_H
#define EZ_LABEL_H
#include <string>
#include <unordered_map>
#include <map>
#include <fstream>
#include <algorithm>
#include <functional>
#include"EZ_aux.h"

using std::partial_sort;
using std::fstream;
using std::map;
using std::unordered_map;
using std::string;
using std::getline;

class EZlabel{
public:
	EZlabel(const string& path){
		fstream fstrm;
		fstrm.open(path, std::ios::in);
		string tmp;
		vector<string> svec;
		
		size_t cnt(0);
		while (getline(fstrm, tmp))
		{
			++cnt;
			split(tmp, svec);
			if (svec.size() != 3)
				continue;

			labls[svec[0]][svec[1]] = std::stoi(svec[2]);
		}
		cout << cnt << endl;
	}
		
	inline
	unordered_map<string, map<string, int>>::size_type getSz() const{
		return labls.size();
	}

	inline
	int getRel(const string& label, const string& url){
		return labls[label].find(url) == labls[label].end()? 0 : labls[label][url];
	}

	inline
	vector<double> getBestOrder(const string& label, size_t sz){
		if (labls.find(label) == labls.end())
			return vector<double>();

		vector<double> result;
		for (const auto& item : labls[label])
		{
			result.push_back(item.second);
		}

		partial_sort(result.begin(), result.begin() + sz, result.end(), std::greater<double>());

		return vector<double>(result.begin(), result.begin() + sz);
	}
	unordered_map<string, map<string, int>> labls;
};

#endif