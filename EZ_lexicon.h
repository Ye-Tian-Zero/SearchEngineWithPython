#ifndef EZ_LEXICON_H
#define EZ_LEXICON_H
#include<unordered_map>
#include<string>
#include<fstream>
#include<sstream>
using std::fstream;
using std::string;
using std::unordered_map;
using std::stringstream;

class lexicon{
public:
	lexicon(const string& path){
		fstream fstrm;
		fstrm.open(path, std::ios::in);
		string tmp;
		string word;
		size_t wordID;
		stringstream buffer;
		buffer << fstrm.rdbuf();
		while (buffer >> word >> wordID)
		{
			lexicon_[word] = wordID;
			lexicon_reverse_[wordID] = word;
		}

		next_word_ID = wordID + 1;
	}

	inline size_t addWord(const string& word){
		lexicon_[word] = next_word_ID++;
		return lexicon_[word];
	}

	inline size_t operator[](const string& word){
		return lexicon_.find(word) != lexicon_.end() ? lexicon_[word] : addWord(word);
	}

	inline const string& operator[](const size_t wordID){
		return lexicon_reverse_.find(wordID) != lexicon_reverse_.end() ? lexicon_reverse_[wordID] : empty_;
	}

private:
	unordered_map<string, size_t> lexicon_;
	unordered_map<size_t, string> lexicon_reverse_;
	size_t next_word_ID;
	const string empty_;
};


#endif