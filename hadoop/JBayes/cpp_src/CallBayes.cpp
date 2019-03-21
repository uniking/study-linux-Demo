#include "Bayes2.h"

static CBayes bayes;

extern "C"
int load_class_model(vector<string>& Models)
{
	return bayes.BYLoadClassModel4(Models);
}

bool is_attractive_content(vector<string> Words, string &uid, string& level, string& ClassName, string& Similarity, vector<string>& KeyWords)
{
	bool bRtn = false;
	float oSimilarity = 0;
	bRtn = bayes.IsAttractiveContent4(Words, uid, level, ClassName, oSimilarity, KeyWords);
	
	//Similarity = oSimilarity; //float 2 string
	
	return bRtn;
}