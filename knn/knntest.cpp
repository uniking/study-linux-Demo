/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2011 Heiko Strathmann
 * Copyright (C) 2011 Berlin Institute of Technology and Max-Planck-Society
 */
#include <shogun/base/init.h>
#include <shogun/evaluation/CrossValidation.h>
#include <shogun/evaluation/ContingencyTableEvaluation.h>
#include <shogun/evaluation/StratifiedCrossValidationSplitting.h>
#include <shogun/modelselection/GridSearchModelSelection.h>
#include <shogun/modelselection/ModelSelectionParameters.h>
#include <shogun/modelselection/ParameterCombination.h>
#include <shogun/labels/MulticlassLabels.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/clustering/KMeans.h>
#include <shogun/distance/EuclideanDistance.h>
#include <shogun/distance/MinkowskiMetric.h>

#include <shogun/labels/MulticlassLabels.h>
#include <shogun/features/DenseFeatures.h>
#include <shogun/multiclass/GaussianNaiveBayes.h>
#include <shogun/distance/HammingWordDistance.h>
#include <shogun/distance/StringDistance.h>
#include <shogun/distance/CosineDistance.h>

#include <shogun/multiclass/KNN.h>

#include <shogun/io/CSVFile.h>
#include <shogun/labels/MulticlassLabels.h>
#include <shogun/multiclass/ecoc/ECOCStrategy.h>
#include <shogun/multiclass/ecoc/ECOCOVREncoder.h>
#include <shogun/multiclass/ecoc/ECOCHDDecoder.h>
#include <shogun/machine/LinearMulticlassMachine.h>
#include <shogun/classifier/svm/LibLinear.h>

#include <shogun/base/init.h>
#include <shogun/lib/common.h>
#include <shogun/io/SGIO.h>




#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <functional>
#include <cfloat>
#include <string.h>

using namespace std;

#define EPSILON 1e-5

using namespace shogun;

void print_message(FILE* target, const char* str)
{
	fprintf(target, "%s", str);
}

typedef vector<vector<double> > Double2DVec;

int DVec2SGMatrix(Double2DVec& srcdata, SGMatrix<float64_t>& dstdata)
{
	int nrows = dstdata.num_rows;
	int ncols = dstdata.num_cols;

	Double2DVec::iterator pEle = srcdata.begin();
	int i=0;
	for(; pEle != srcdata.end(); pEle++,i++)
	{
		int j=0, k=0;
		for(;j < pEle->size();j++)
		{
			double el = (*pEle)[j];
			dstdata.matrix[i*nrows + j] = el;
		}
	}

	return 0;
}


int knnLoadFile(char* filepath, Double2DVec& filedata, vector<int>& vclass)
{
		Double2DVec srcdata;
		//char *cFileSample = "/home/sxis/workspace/inputTest/class/sample_file.txt";
		char *cFileModel = "/home/sxis/workspace/inputTest/class/Model_file.txt";
		char* sSegmenter = ":";
		int iClassNum = 4;
		int iFeaTypeNum = 900000;
		init_shogun_with_defaults();


		ifstream in (filepath, ios_base::binary);
		ofstream out (cFileModel);
		if (!in || !out)
		{
			cerr << "Can not open the file" << endl;
			return false;
		}

		int iTotalDocNum = 0;
		double * pClassPriorProb = new double [iClassNum];
		memset (pClassPriorProb, 0, iClassNum*sizeof(double));
		double * pFeaItemPriorProb = new double [iFeaTypeNum];
		memset (pFeaItemPriorProb, 0, iFeaTypeNum*sizeof(double));
		double ** ppChiMatrix = new double * [iClassNum];
		for (int i=0; i<iClassNum; i++)
		{
			ppChiMatrix[i] = new double [iFeaTypeNum];
			memset (ppChiMatrix[i], 0, iFeaTypeNum*sizeof(double));
		}

		double ** ppPProbMatrix = new double * [iClassNum];
		for (int i=0; i<iClassNum; i++)
		{
			ppPProbMatrix[i] = new double [iFeaTypeNum];
			memset (ppChiMatrix[i], 0, iFeaTypeNum*sizeof(double));
		}

		int * pFeaSelected = new int [iFeaTypeNum];
		memset (pFeaSelected, 0, iFeaTypeNum*sizeof(int));


		string sLine;
		int i = 0;
		while (getline (in, sLine))
		{
			// show some information on screen
			if (0 == i%10000)
				cout << i << "\n";
			i++;

			// 2.1 the total number of doc
			iTotalDocNum++;

			// 2.2 split the sample into class and feature items
			string::size_type iSeg = sLine.find_first_of (sSegmenter);
			string sTmp = sLine.substr (0, iSeg);
			int iClassId = atoi (sTmp.c_str());
			if (iClassId >= iClassNum)
				continue;

			vclass.push_back(iClassId);
			pClassPriorProb [iClassId]++;

			// 2.3 count the rest feature items
			iSeg += sTmp.length();
			sTmp = sLine.substr (iSeg);
			istringstream isLine (sTmp);

			string sTmpItem;
			vector<double> TmpFeaIdVec;
			while (isLine >> sTmpItem)
			{
				int iFeaItemId = atoi (sTmpItem.c_str());
				if (iFeaItemId >= iFeaTypeNum)
					continue;
				TmpFeaIdVec.push_back (iFeaItemId);
			}


			// sort and remove duplicated items
			sort (TmpFeaIdVec.begin(), TmpFeaIdVec.end());
			TmpFeaIdVec.erase (unique (TmpFeaIdVec.begin(), TmpFeaIdVec.end()), TmpFeaIdVec.end ());

			//形成矩阵
			srcdata.push_back(TmpFeaIdVec);
		}

		filedata = srcdata;

		return 0;
}


int KnnTest()
{
	int i=0;
	Double2DVec exampledata;
	vector<int> vclass;
	Double2DVec testdata;
	vector<int> temp;

	knnLoadFile("/home/sxis/workspace/inputTest/class/sample_file.txt", exampledata, vclass);
	//knnLoadFile("/home/sxis/workspace/inputTest/class/test_file.txt", testdata, temp);
	knnLoadFile("/home/sxis/workspace/predict/class/test_file.txt", testdata, temp);

	// create some data
	int nrows = 0;
	Double2DVec::iterator pEl = exampledata.begin();
	for(;pEl != exampledata.end(); pEl++)
	{
		if(nrows < pEl->size())
		{
			nrows = pEl->size();
		}
	}

	int ncols = exampledata.size();
	SGMatrix<float64_t> matrix(nrows,ncols);
	for(i=0; i<nrows*ncols; i++)
		matrix.matrix[i] = 0;
	DVec2SGMatrix(exampledata, matrix);

	nrows = 0;
	Double2DVec::iterator ptestEl = testdata.begin();
	for(;ptestEl != testdata.end(); ptestEl++)
	{
		if(nrows < ptestEl->size())
		{
			nrows = ptestEl->size();
		}
	}

	ncols = testdata.size();
	//featrue的个数必须与样本的个数相同，不然代码无法执行
	SGMatrix<float64_t> testmatrix(1070,ncols);
	for(i=0; i<1070*ncols; i++)
		testmatrix.matrix[i] = 0;//如果是随机的，那对判断的影响可太大了
	DVec2SGMatrix(testdata, testmatrix);



	CDenseFeatures<float64_t>* features= new CDenseFeatures<float64_t>(matrix);

	// create three labels
	int labsize = vclass.size();
	CMulticlassLabels* labels=new CMulticlassLabels(labsize);
	cout<<"file class:";
	for(i=0; i<labsize; i++)
	{
		float64_t j = vclass[i];
		cout<<j<<" ";
		labels->set_label(i, j);
	}
	cout<<endl;


	//CEuclideanDistance* distance=new CEuclideanDistance(features, features);
	//CHammingWordDistance* distance=new CHammingWordDistance();
	//CMinkowskiMetric* distance=new CMinkowskiMetric();
	CCosineDistance* distance=new CCosineDistance(features, features);
	//distance->init(features, features);

	//CGaussianNaiveBayes* ci = new CGaussianNaiveBayes(features,labels);
	CKNN* ci = new CKNN(4, distance, labels);
	ci->train();

	CMulticlassLabels* sef = CLabelsFactory::to_multiclass(ci->apply());
	for(i=0; i<29; i++)
		cout<<sef->get_label(i)<<" ";
	cout<<endl;


	CDenseFeatures<float64_t>* testfeatures= new CDenseFeatures<float64_t>(testmatrix);
	CMulticlassLabels* lab = CLabelsFactory::to_multiclass(ci->apply(testfeatures));
	cout<<"labels:"<<lab->get_num_labels()<<"属于:" <<lab->get_label(0)<<endl;
	// free up memory
	SG_UNREF(ci);

	exit_shogun();

	return 0;
}


int ecocTest()
{
	int i=0;
	Double2DVec exampledata;
	vector<int> vclass;
	Double2DVec testdata;
	vector<int> temp;

	knnLoadFile("/home/sxis/workspace/inputTest/class/sample_file.txt", exampledata, vclass);
	knnLoadFile("/home/sxis/workspace/predict/class/test_file.txt", testdata, temp);

	// create some data
	int nrows = 0;
	Double2DVec::iterator pEl = exampledata.begin();
	for(;pEl != exampledata.end(); pEl++)
	{
		if(nrows < pEl->size())
		{
			nrows = pEl->size();
		}
	}

	int ncols = exampledata.size();
	SGMatrix<float64_t> matrix(nrows,ncols);
	for(i=0; i<nrows*ncols; i++)
		matrix.matrix[i] = 0;
	DVec2SGMatrix(exampledata, matrix);

	nrows = 0;
	Double2DVec::iterator ptestEl = testdata.begin();
	for(;ptestEl != testdata.end(); ptestEl++)
	{
		if(nrows < ptestEl->size())
		{
			nrows = ptestEl->size();
		}
	}

	ncols = testdata.size();
	//featrue的个数必须与样本的个数相同，不然代码无法执行
	//SGMatrix<float64_t> testmatrix(nrows,ncols);
	SGMatrix<float64_t> testmatrix(1070,ncols);
	for(i=0; i<1070*ncols; i++)
		testmatrix.matrix[i] = 0;//如果是随机的，那对判断的影响可太大了
	DVec2SGMatrix(testdata, testmatrix);



	CDenseFeatures<float64_t>* features= new CDenseFeatures<float64_t>(matrix);

	// create three labels
	int labsize = vclass.size();
	CMulticlassLabels* labels=new CMulticlassLabels(labsize);
	cout<<"file class:";
	for(i=0; i<labsize; i++)
	{
		float64_t j = vclass[i];
		cout<<j<<" ";
		labels->set_label(i, j);
	}
	cout<<endl;


	CEuclideanDistance* distance=new CEuclideanDistance(features, features);
	//CGaussianNaiveBayes* ci = new CGaussianNaiveBayes(features,labels);
	//CKNN* ci = new CKNN(4, distance, labels);
	//ci->train();

	//创建线性svm分类器
	CLibLinear* svm = new CLibLinear(L2R_L2LOSS_SVC);
	SG_REF(svm);
	svm->set_epsilon(EPSILON);
	svm->set_bias_enabled(true);
	//创建多类svm分类器
	CLinearMulticlassMachine* mc_svm = new CLinearMulticlassMachine(
				new CECOCStrategy(new CECOCOVREncoder(), new CECOCHDDecoder()), (CDotFeatures*) features, svm, labels);
	SG_REF(mc_svm);

	mc_svm->train();

	CMulticlassLabels* sef = CLabelsFactory::to_multiclass(mc_svm->apply());
	for(i=0; i<29; i++)
		cout<<sef->get_label(i)<<" ";
	cout<<endl;


	CDenseFeatures<float64_t>* testfeatures= new CDenseFeatures<float64_t>(testmatrix);
	CMulticlassLabels* lab = CLabelsFactory::to_multiclass(mc_svm->apply(testfeatures));
	cout<<"labels:"<<lab->get_num_labels()<<"属于:" <<lab->get_label(0)<<endl;
	// free up memory
	SG_UNREF(svm)
	SG_UNREF(mc_svm);

	exit_shogun();

	return 0;
}

int main(int argc, char **argv)
{
	KnnTest();
	//ecocTest();
	return 0;
}
