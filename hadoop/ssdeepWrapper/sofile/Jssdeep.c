#include "Jssdeep.h"
#include <fuzzy.h>
#include <string.h>

JNIEXPORT jint JNICALL Java_Jssdeep_fuzzy_1hash_1filename__Ljava_lang_String_2Ljava_lang_StringBuffer_2
  (JNIEnv *env, jobject obj, jstring filename, jobject result)
{
	const char* fstr;
	jboolean isCopy=false;
	fstr = env->GetStringUTFChars(filename, &isCopy);
	if(fstr == NULL || result == NULL)
		return 0;

	jclass sb = env->GetObjectClass(result);
	jmethodID mid = env->GetMethodID(sb, "append", "(Ljava/lang/String;)Ljava/lang/StringBuffer;");

	char filehash[2048]={0};
	int rint = fuzzy_hash_filename(fstr, filehash);
	//printf("so:filename=%s, hash=%s\n", fstr, filehash);

	jstring rst = env->NewStringUTF(filehash);

	env->CallObjectMethod(result, mid, rst);
	return rint; 
}

JNIEXPORT jint JNICALL Java_Jssdeep_fuzzy_1hash_1buf
  (JNIEnv *env, jobject obj, jbyteArray buf, jlong length, jobject result)
{
	jboolean isCopy=false;
	const char* jbuf = (const char*)env->GetByteArrayElements(buf, &isCopy);
	if(jbuf == NULL || result == NULL)
		return 0;

	jclass sb = env->GetObjectClass(result);
	jmethodID mid = env->GetMethodID(sb, "append", "(Ljava/lang/String;)Ljava/lang/StringBuffer;");

	char filehash[2048]={0};
	int rint = fuzzy_hash_buf((const unsigned char*)jbuf, length, filehash);

	jstring rst = env->NewStringUTF(filehash);

	env->CallObjectMethod(result, mid, rst);
	return rint; 
}


JNIEXPORT jint JNICALL Java_Jssdeep_fuzzy_1compare
  (JNIEnv *env, jobject obj, jstring sig1, jstring sig2)
{
	const char* s1;
	const char* s2;
	jboolean isCopy=false;
	s1 = env->GetStringUTFChars(sig1, &isCopy);
	s2 = env->GetStringUTFChars(sig2, &isCopy);
	if(s1 == NULL || s2 == NULL)
		return 0;

	int iRlt = fuzzy_compare(s1, s2);
	return iRlt;
}
