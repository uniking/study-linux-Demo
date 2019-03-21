#include <jni.h>
#include <vector>
#include <string>
#include <iostream>
using namespace std;



int javalist2stdlist(JNIEnv *env, jobject* java_words, vector<string>& std_words)
{

// retrieve the java.util.List interface class
jclass cList = env->FindClass("java/util/List");

// retrieve the toArray method and invoke it
jmethodID mToArray = env->GetMethodID(cList, "toArray", "()[Ljava/lang/Object;");
if(mToArray == NULL)
    return -1;
jobjectArray array = (jobjectArray)env->CallObjectMethod(*java_words, mToArray);

// now create the string array
for(int i=0;i<env->GetArrayLength(array);i++) {
    // retrieve the chars of the entry strings and assign them to the array!
    jstring strObj = (jstring)env->GetObjectArrayElement(array, i);
    const char * chr = env->GetStringUTFChars(strObj, NULL);
    cout<<chr<<endl;
	std_words.push_back(chr);
    env->ReleaseStringUTFChars(strObj, chr);
}

	return 0;
}
