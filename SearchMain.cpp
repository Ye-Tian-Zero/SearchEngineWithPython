#include<iostream>
#include<string>
#include<utility>
#include<vector>
#include<numeric>
#include<cmath>
#include<memory>
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
	for (vector<double>::size_type i(1); i < rels.size(); ++i)
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


	return idcg == 0? -1:dcg / idcg;
}

void runDcgTest(std::shared_ptr<EZsearch> searchP){
	cout << "loading test labels" << endl;
	EZlabel el(".\\qu.label");
	cout << el.getSz() << endl;
	cout << "done" << endl;
	fstream fstrm("ndcg results", std::ios::out);
	vector<pair<string, string>> resList3;
	vector<pair<string, string>> resList5;
	vector<pair<string, string>> resList10;
	vector<double> dcg3_vec, dcg5_vec, dcg10_vec;

	size_t cnt(0);
	for (const auto& label : el.labls)
	{
		resList3.clear();
		resList5.clear();
		resList10.clear();
		string query = label.first;

		fstrm << query << ':' << endl;
		//cout << query << endl;

		searchP->search(query, resList10, 10);
		if (resList10.size() >= 3)
		{
			resList3.insert(resList3.begin(), resList10.begin(), resList10.begin() + 3);
			double dcg3 = getNDcg(query, resList3, el);
			fstrm << dcg3 << endl;
			if (dcg3 != -1)
				dcg3_vec.push_back(dcg3);
			if (resList10.size() >= 5)
			{
				resList5.insert(resList5.begin(), resList10.begin(), resList10.begin() + 5);
				double dcg5 = getNDcg(query, resList5, el);
				fstrm << dcg5 << endl;
				if (dcg5 != -1)
					dcg5_vec.push_back(dcg3);
				if (resList10.size() >= 10)
				{
					double dcg10 = getNDcg(query, resList10, el);
					fstrm << dcg10 << endl;
					if (dcg10 != -1)
						dcg10_vec.push_back(dcg10);
				}
			}
		}
		if (cnt++ % 100 == 0)
			cout << cnt << endl;
	}
	
	fstrm << "dcg3 size: " << dcg3_vec.size() << endl;
	fstrm << "dcg5 size: " << dcg5_vec.size() << endl;
	fstrm << "dcg10 size: " << dcg10_vec.size() << endl;

	fstrm << "dcg3 average: " << std::accumulate(dcg3_vec.begin(), dcg3_vec.end(), 0.0) / dcg3_vec.size() << endl;
	fstrm << "dcg5 average: " << std::accumulate(dcg5_vec.begin(), dcg5_vec.end(), 0.0) / dcg5_vec.size() << endl;
	fstrm << "dcg10 average: " << std::accumulate(dcg10_vec.begin(), dcg10_vec.end(), 0.0) / dcg10_vec.size() << endl;
}

int main()
{
	cout << "loading necessary datas" << endl;
	EZsearch_IDF es;
	cout << "done" << endl;
	cout << "-------------------------------" << endl << endl;
	runDcgTest(std::shared_ptr<EZsearch>(&es));
	
	
	//vector<pair<string, string>> resList;
	//while (true)
	//{
	//	//result.clear();
	//	string query;
	//	cout << "EZ search: ";
	//	getline(cin, query);
	//	es.search(query, resList, 10);
	//	
	//	for (const auto& res : resList)
	//	{
	//		cout << res.first << endl << res.second
	//			<< endl << endl;
	//	}
	//}
	system("pause");
	return 0;
}