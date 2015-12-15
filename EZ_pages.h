#ifndef EZ_PAGES_H
#define EZ_PAGES_H
#include<string>
#include<vector>
#include<fstream>
#include<unordered_map>
#include<utility>
#include"EZ_aux.h"
using std::unordered_map;
using std::fstream;
using std::string;
using std::vector;
using std::getline;
using std::pair;

class pages{

public:
	pages(const string& path){
		fstream page_file;
		page_file.open(path);
		string tmp;
		vector<string> split_res;
		while (getline(page_file, tmp))
		{
			split(tmp, split_res, '\t');
			page_data[std::stoul(split_res[0])] = pair<string, string>(split_res[1], split_res[2]);
		}
	}

	const pair<string, string>& getDoc(size_t docID) {
		return page_data.find(docID) == page_data.end() ? empty_flag_ : page_data[docID];
	}
private:
	static const pair<string, string> empty_flag_;
	unordered_map<size_t, pair<string, string>> page_data;
	
};

const pair<string, string> pages::empty_flag_("","");

#endif