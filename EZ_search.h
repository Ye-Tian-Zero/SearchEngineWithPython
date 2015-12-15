#ifndef EZ_SEARCH_H
#define EZ_SEARCH_H
#include<string>
#include<vector>
#include<unordered_map>
#include<map>
#include<algorithm>
#include<cstring>
#include<algorithm>

using std::map;
using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::unordered_map;

#include"EZ_pages.h"
#include"EZ_jieba.h"
#include"EZ_lexicon.h"
#include"EZ_backwardIndex.h"

struct Comp{
	bool operator()(const std::pair<size_t, size_t>& x, const std::pair<size_t, size_t>& y){
		return x.second > y.second;
	}
};


struct Pred{
	bool operator()(const pair<size_t, vector<pair<size_t, unsigned int>>> &x,
		const pair<size_t, vector<pair<size_t, unsigned int>>> &y)
	{
		return x.second.size() > y.second.size();
	}
};


class EZsearch{
public:
	EZsearch() :lex(".\\lexicon_gbk.lst"), 
				bi(".\\backwardIndex.lst"), 
				pg(".\\UT_gbk.lst"){
		vector<string> rubbish;
		ty_jieba.cut("nothing", rubbish);

		//888888888888888888888888888888888888888
		vector<pair<size_t, vector<pair<size_t, unsigned int>>>> count(10000);
		cout << "sorting" << endl;
		std::partial_sort_copy(bi.backward_Index.begin(), bi.backward_Index.end(), count.begin(), count.end(), Pred());
		
		fstream file_bi;
		file_bi.open("stop word.lst", std::ios::out);
			
		for (auto &n : count)
		{
			file_bi << lex[n.first] << ' ' << n.second.size() << endl;
		}

		cout << "count over" << endl;
	}

	void search(const string& query_, vector<pair<string, string>>& resList, size_t rn = 30){
		
		string query(query_);
		for (auto& n: query)
		{
			n = tolower(n);
		}

		unordered_map<int, int> query_result;
		resList.clear();
		result.clear();

		ty_jieba.cut(query, terms);//ty_jieba.cut will clear terms

		for (const auto& term : terms)
		{
			const auto& docID_vec = bi.getDocVec(lex[term]);
			if (docID_vec.size() >= 10000)
				continue;
			for (const auto& ID : docID_vec)
			{
				++query_result[ID.first];
			}
		}		

		auto sz = query_result.size();
		cout << sz << endl;
		if (sz >= rn)
			result.resize(rn);
		else
			result.resize(sz);

		std::partial_sort_copy(query_result.begin(), query_result.end(), result.begin(), result.end(), Comp());
		for (const auto& res : result)
		{
			resList.push_back(pg.getDoc(res.first));
		}

		return;
	}

private:
	jieba ty_jieba;

	lexicon lex;
	backwardIndex bi;
	pages pg;

	vector<string> terms;
	vector<pair<size_t, size_t>> result;
};

#endif