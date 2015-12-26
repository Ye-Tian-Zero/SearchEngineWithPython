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

#include"EZ_pages.h"
#include"EZ_jieba.h"
#include"EZ_lexicon.h"
#include"EZ_backwardIndex.h"

using std::cout;
using std::endl;
using std::map;
using std::list;
using std::vector;
using std::string;
using std::unordered_map;


struct Comp{
	bool operator()(const pair<size_t, double>& x, const pair<size_t, double>& y){
		return x.second > y.second;
	}
};

struct HeapPred :public Comp{};

class EZsearch{
public:
	EZsearch() :lex(".\\lexicon_gbk.lst"),
				bi(".\\backwardIndex.lst"),
				pg(".\\UT_gbk.lst"){}

	virtual void search(const string& query_, vector<pair<string, string>>& resList, size_t rn = 30) = 0;

	void str2lower(string& src){
		for (auto& n : src)
		{
			n = tolower(n);
		}
	}

protected:

	vector<term>	terms;
	jieba			ty_jieba;
	lexicon			lex;
	backwardIndex	bi;
	pages			pg;

};

template<class InIter>
class EZzip{
public:
	EZzip(InIter beg, InIter end) :first(beg), last(end){}

	bool advance(){
		if (first == last || ++first == last)
		{
			return false;
		}
		return true;
	}

	bool empty() const{
		return first == last;
	}

protected:
	InIter first;
	InIter last;
};

template<class InIter>
class EZzip_IDF : public EZzip<InIter>{
public:

	EZzip_IDF(double weight, InIter beg, InIter end):EZzip(beg, end), Weight_(weight){}

	auto getDocID() const -> const decltype(first->first) {
		return first->first;
	}

	auto getHitTimes() const -> const decltype(first->second) {
		return first->second;
	}

	inline double getWeight() const{ return Weight_; }
private:
	double Weight_;
};

class EZsearch_IDF : public EZsearch{
	
	using docID_t		= size_t;
	using hitTimes_t	= size_t;
	using DocVec_t		= vector<pair<size_t, size_t>>;
	using zip_iter_t	= vector<pair<size_t, size_t>>::iterator;

	list<EZzip_IDF<zip_iter_t>> zips;
	vector<pair<size_t, double>> heap_base;
	vector<pair<size_t, size_t>> result;

public:
	EZsearch_IDF(){
		heap_base.reserve(500);
	}

	void search(const string& query_, vector<pair<string, string>>& resList, size_t rn = 30){
		string query(query_);
		str2lower(query);

		heap_base.clear();
		resList.clear();
		result.clear();

		ty_jieba.cut(query, terms);//ty_jieba.cut will clear terms

		for (const auto& term : terms)
		{
			auto& doc_vec = bi.getDocVec(lex[term.getWord()]);
			zips.emplace_back(term.getWeight(), doc_vec.begin(), doc_vec.end());
		}		

		merge_zips(rn);

		size_t result_cnt(0);
		for (const auto& res : heap_base)
		{
			resList.push_back(pg.getDoc(res.first));
			if (++result_cnt == rn)
				break;
		}

		return;
	}


private:

	void merge_zips(size_t sz)
	{
		heap_base.clear();
		HeapPred heapPred;

		auto terms_cnt = zips.size();//need to talk to Li Xingni;

		for (auto zip_iter = zips.begin(); zip_iter != zips.end();)
		{
			if (zip_iter->empty())
				zip_iter = zips.erase(zip_iter);
			else
				++zip_iter;
		}

		if (zips.empty())
			return;

		while (!zips.empty())
		{
			//cout << zips.size() << endl;
			double weight = 0;

			auto min_iter = std::min_element(zips.begin(), zips.end(),
				[](const EZzip_IDF<zip_iter_t>& x, const EZzip_IDF<zip_iter_t> &y)
			{return x.getDocID() < y.getDocID(); }); // to be rewrite;

			size_t min_docID = min_iter->getDocID(); // to be rewrite;

			for (auto iter = zips.begin(); iter != zips.end(); )
			{
				if (iter->getDocID() == min_docID)
				{
					double tf = iter->getHitTimes();
					double tf_trans = 1 + 0.3 * (tf - 1) / tf;
					weight += iter->getWeight() * tf_trans;
					if (! iter->advance())
					{
						iter = zips.erase(iter);
						continue;
					}
				}
				++iter;
			}

			if (heap_base.size() < sz)
			{
				heap_base.emplace_back(min_docID, weight);
				std::push_heap(heap_base.begin(), heap_base.end(), heapPred);
			}
			else
			{
				if (heap_base.begin()->second < weight)
				{
					std::pop_heap(heap_base.begin(), heap_base.end(), heapPred);
					heap_base.pop_back();
					heap_base.emplace_back(min_docID, weight);
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