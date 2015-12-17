#ifndef EZ_SEARCH_H
#define EZ_SEARCH_H
#include<string>
#include<vector>
#include<unordered_map>
#include<map>
#include<algorithm>
#include<cstring>
#include<algorithm>
#include<list>

#include<iostream>

using std::cout;
using std::endl;
using std::map;
using std::list;
using std::vector;
using std::string;
using std::unordered_map;

#include"EZ_pages.h"
#include"EZ_jieba.h"
#include"EZ_lexicon.h"
#include"EZ_backwardIndex.h"

struct Comp{
	bool operator()(const pair<size_t, size_t>& x, const pair<size_t, size_t>& y){
		return x.second > y.second;
	}
};

struct HeapPred :public Comp{};

/*struct Pred{
	bool operator()(const pair<size_t, vector<pair<size_t, unsigned int>>> &x,
		const pair<size_t, vector<pair<size_t, unsigned int>>> &y)
	{
		return x.second.size() > y.second.size();
	}
};*/


class EZsearch{
	typedef vector<pair<size_t, size_t>>::iterator			zip_iter;
	typedef pair<zip_iter, zip_iter>						zip_range;
	typedef list<zip_range>									zip_list_type;
	typedef zip_list_type::iterator							zip_list_iter;
		
public:
	EZsearch() :lex(".\\lexicon_gbk.lst"), 
				bi(".\\backwardIndex.lst"), 
				pg(".\\UT_gbk.lst"){

		heap_base.reserve(500);
		vector<string> rubbish;
		ty_jieba.cut("nothing", rubbish);
		/*//888888888888888888888888888888888888888
		vector<pair<size_t, vector<pair<size_t, unsigned int>>>> count(10000);
		cout << "sorting" << endl;
		std::partial_sort_copy(bi.backward_Index.begin(), bi.backward_Index.end(), count.begin(), count.end(), Pred());
		
		fstream file_bi;
		file_bi.open("stop word.lst", std::ios::out);
			
		for (auto &n : count)
		{
			file_bi << lex[n.first] << ' ' << n.second.size() << endl;
		}

		cout << "count over" << endl;*/
	}

	void search(const string& query_, vector<pair<string, string>>& resList, size_t rn = 30){
		string query(query_);

		for (auto& n: query)
		{
			n = tolower(n);
		}

		heap_base.clear();
		resList.clear();
		result.clear();

		ty_jieba.cut(query, terms);//ty_jieba.cut will clear terms


		for (const auto& term : terms)
		{
			auto& zip = bi.getDocVec(lex[term]);
			zips.emplace_back(zip.begin(), zip.end());
		}		

		merge_zips(zips, heap_base, rn * 10);

		/*if (sz >= rn)
			result.resize(rn);
		else
			result.resize(sz);*/

		size_t result_cnt(0);
		for (const auto& res : heap_base)
		{
			cout << res.second << ' ';
			resList.push_back(pg.getDoc(res.first));
			if (++result_cnt == rn)
				break;
		}
		cout << endl;

		return;
	}

private:
	jieba ty_jieba;
	lexicon lex;
	backwardIndex bi;
	pages pg;

private:
	zip_list_type zips;
	vector<pair<size_t, size_t>> heap_base;
	vector<string> terms;
	vector<pair<size_t, size_t>> result;

private:
	void merge_zips(list<zip_range>& zips, vector<pair<size_t, size_t>>& heap_base, size_t sz)
	{
		heap_base.clear();
		HeapPred heapPred;
		list<zip_range>::size_type terms_cnt = zips.size();//need to talk to Li Xingni;

		for (auto iter = zips.begin(); iter != zips.end();)
		{
			if (iter->first == iter->second)
				iter = zips.erase(iter);
			else
				++iter;
		}

		if (zips.empty())
			return;

		size_t hit_cnt;
		while (!zips.empty())
		{
			//cout << zips.size() << endl;
			hit_cnt = 0;

			zip_list_iter min_iter = std::min_element(zips.begin(), zips.end(),
				[](const zip_range& x, const zip_range& y){return x.first->first < y.first->first; });

			size_t min_docID = min_iter->first->first;

			for (zip_list_iter iter = zips.begin(); iter != zips.end(); )
			{
				if (iter->first->first == min_docID)
				{
					++hit_cnt;
					if (++(iter->first) == iter->second)
					{
						iter = zips.erase(iter);
						continue;
					}
				}

				++iter;
			}
			
			if (heap_base.size() < sz)
			{
				heap_base.emplace_back(min_docID, hit_cnt);
				std::push_heap(heap_base.begin(), heap_base.end(), heapPred);
			}
			else
			{
				if (heap_base.begin()->second < hit_cnt)
				{
					std::pop_heap(heap_base.begin(), heap_base.end(), heapPred);
					heap_base.pop_back();
					heap_base.emplace_back(min_docID, hit_cnt);
					std::push_heap(heap_base.begin(), heap_base.end(), heapPred);
				}
			}
			//if (heap_base.size() == sz && heap_base.begin()->second >= (terms_cnt + 1) / 2)
			//	break;/
			//It's useful, but not here.
		}
		std::sort_heap(heap_base.begin(), heap_base.end(), heapPred);
	}
};

#endif