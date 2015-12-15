#include<iostream>
#include<string>
#include<utility>
#include<vector>
#include<cmath>
#include"EZ_search.h"
#include"EZ_label.h"

using std::vector;
using std::cout;
using std::pair;
using std::cin;
using std::endl;
using std::getline;
using std::string;

double getDcg(const vector<double>& rels){
	double dcg(rels[0]);
	for (int i(1); i < rels.size(); ++i)
	{
		dcg += rels[i] / std::log2(i + 1);
	}
	return dcg;
}

double getNDcg(const string& label, const vector<pair<string, string>> res, EZlabel& el){
	vector<double> rels;
	for (auto& re : res)
	{
		rels.push_back(el.getRel(label, re.second));
	}
	auto dcg = getDcg(rels);
	vector<double> best_order = el.getBestOrder(label, res.size());
	auto idcg = getDcg(best_order);

	return dcg / idcg;
}


int main()
{
	fstream fstrm("ndcg results", std::ios::out);
	cout << "loading necessary datas" << endl;
	EZsearch es;
	cout << "done" << endl;
	cout << "loading test labels" << endl;
	EZlabel el(".\\qu.label");
	cout << el.getSz() << endl;
	cout << "done" << endl;
	cout << "-------------------------------" << endl << endl;
	
	vector<pair<string, string>> resList3;
	vector<pair<string, string>> resList5;
	vector<pair<string, string>> resList10;

	size_t cnt(0);
	/*for (const auto& label : el.labls)
	{
		resList3.clear();
		resList5.clear();
		resList10.clear();
		string query = label.first;

		fstrm << query << ':' << endl;
		cout << query << endl;

		es.search(query, resList10, 10);
		if (resList10.size() >= 3)
		{
			resList3.insert(resList3.begin(), resList10.begin(), resList10.begin() + 3);
			fstrm << getNDcg(query, resList3, el) << endl;
			if (resList10.size() >= 5)
			{
				resList5.insert(resList5.begin(), resList10.begin(), resList10.begin() + 5);
				fstrm << getNDcg(query, resList5, el) << endl;
				if (resList10.size() >= 10)
					fstrm << getNDcg(query, resList10, el) << endl;
			}

		}

		cout << cnt++ << endl;
	}*/
	vector<pair<string, string>> resList;
	while (true)
	{
		//result.clear();
		string query;
		cout << "EZ search: ";
		getline(cin, query);
		es.search(query, resList, 5);
		
		for (const auto& res : resList)
		{
			cout << res.first << endl << res.second
				<< endl << endl;
		}
	}
	system("pause");
	return 0;
}