#ifndef EZ_backwardIndex_H
#define EZ_backwardIndex_H
#include<string>
#include<fstream>
#include<utility>
#include<vector>
#include<map>
#include<sstream>
#include<unordered_map>

using std::unordered_map;
using std::pair;
using std::string;
using std::fstream;
using std::getline;
using std::vector;

class backwardIndex{
public:
	backwardIndex(const string& file_path){
		file_bi.open(file_path, std::ios::in);

		string tmp;
		size_t wordID;
		size_t docID;
		unsigned int cnt;

		size_t info(0);

		std::stringstream buffer;
		//buffer << file_bi.rdbuf();
		while (file_bi >> wordID >> docID >> cnt)
		{
			backward_Index[wordID].push_back(pair<size_t, unsigned int>(docID, cnt));
			++info;
		}
		

	}

	vector<pair<size_t, unsigned int>>& getDocVec(size_t wordID){
		return backward_Index.find(wordID) == backward_Index.end()? _empty_flag : backward_Index[wordID];
	}

private:
	fstream file_bi;
	static vector<pair<size_t, unsigned int>> _empty_flag;
	unordered_map<size_t, vector<pair<size_t, unsigned int>>> backward_Index;
};

vector<pair<size_t, unsigned int>> backwardIndex::_empty_flag;
#endif