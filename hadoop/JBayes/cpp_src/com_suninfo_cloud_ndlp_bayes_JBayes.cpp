#include "com_suninfo_cloud_ndlp_bayes_JBayes.h"

#include <list>
#include <string>
#include <iostream>

#include "CallBayes.h"
using namespace std;

int javalist2stdlist(JNIEnv *env, jobject* java_words, vector<string>& std_words);


/*
 * Class:     com_suninfo_cloud_ndlp_bayes_JBayes
 * Method:    LoadBayesModule
 * Signature: (Ljava/util/List;)I
 */
JNIEXPORT jint JNICALL Java_com_suninfo_cloud_ndlp_bayes_JBayes_LoadBayesModule
(JNIEnv *env, jobject obj, jobject moduleContent)
{
	vector<string> std_words;
	javalist2stdlist(env, &moduleContent, std_words);
	load_class_model(std_words);
}



JNIEXPORT jint JNICALL Java_com_suninfo_cloud_ndlp_bayes_JBayes_LoadBayesModule
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_suninfo_cloud_ndlp_bayes_JBayes
 * Method:    IsAttractiveContent
 * Signature: (Ljava/util/List;Lcom/suninfo/cloud/ndlp/bayes/BayesResult;)I
 */
JNIEXPORT jint JNICALL Java_com_suninfo_cloud_ndlp_bayes_JBayes_IsAttractiveContent
  (JNIEnv *env, jobject obj, jobject words, jobject result)
{

	vector<string> std_words;
	javalist2stdlist(env, &words, std_words);
	
	string uid;
	string level;
	string className;
	string Similarity;
	vector<string> KeyWords;
	if(is_attractive_content(std_words, uid, level, className, Similarity, KeyWords))
	{
		jclass rs = env->GetObjectClass(result);
		jmethodID mid = env->GetMethodID(rs, "init_data", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
		jstring juid = env->NewStringUTF(uid.c_str());
		jstring jlevel = env->NewStringUTF(level.c_str());
		jstring jclassName = env->NewStringUTF(className.c_str());
		jstring jSimilarity = env->NewStringUTF(Similarity.c_str());
		env->CallObjectMethod(result, mid, juid, jlevel, jclassName, jSimilarity);
	}
}
