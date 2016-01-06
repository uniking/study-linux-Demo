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
#include <shogun/base/init.h>
#include <shogun/lib/common.h>
#include <shogun/io/SGIO.h>


using namespace shogun;

void print_message(FILE* target, const char* str)
{
	fprintf(target, "%s", str);
}

int bayesmain(int argc, char** argv)
{
	init_shogun_with_defaults();

	// create some data
	SGMatrix<float64_t> matrix(3,4);
	SGMatrix<float64_t> textmatrix(3,1);
	//for (int32_t i=0; i<6; i++)
	//	matrix.matrix[i]=i;


	//matrix.matrix[0*4+0] = 1;
	//matrix.matrix[1*4+0] = 2;
	//matrix.matrix[2*4+0] = 10; // (1,2,3)

	//matrix.matrix[0*4+1] = 20;
	//matrix.matrix[1*4+1] = 38;
	//matrix.matrix[2*4+1] = 40; //(10,20,30)

	//matrix.matrix[0*4+2] = 130;
	//matrix.matrix[1*4+2] = 240;
	//matrix.matrix[2*4+2] = 370; //(100,200,300)

	//matrix.matrix[0*4+3] = 1800;
	//matrix.matrix[1*4+3] = 2300;
	//matrix.matrix[2*4+3] = 3500;//(1000,2000,3000)

	matrix.matrix[0] = 1;
	matrix.matrix[1] = 2;
	matrix.matrix[2] = 3; // (1,2,3)

	matrix.matrix[3] = 10;
	matrix.matrix[4] = 20;
	matrix.matrix[5] = 30; //(10,20,30)

	matrix.matrix[6] = 100;
	matrix.matrix[7] = 200;
	matrix.matrix[8] = 300; //(100,200,300)

	matrix.matrix[9] = 1000;
	matrix.matrix[10] = 2000;
	matrix.matrix[11] = 3000;//(1000,2000,3000)



	// create three 2-dimensional vectors
	// shogun will now own the matrix created
	CDenseFeatures<float64_t>* features= new CDenseFeatures<float64_t>(matrix);

	// create three labels
	CMulticlassLabels* labels=new CMulticlassLabels(4);
	labels->set_label(0, 0);
	labels->set_label(1, 1);
	labels->set_label(2, 1);
	labels->set_label(3, 2);

	CGaussianNaiveBayes* ci = new CGaussianNaiveBayes(features,labels);
	ci->train();

	textmatrix.matrix[0] = 1000;
	textmatrix.matrix[1] = 2000;
	textmatrix.matrix[2] = 3000;
	CDenseFeatures<float64_t>* testfeatures= new CDenseFeatures<float64_t>(textmatrix);
	ci->apply(testfeatures);

	// classify on training examples
	for (int32_t i=0; i<1; i++)
		SG_SPRINT("output[%d]=%f\n", i, ci->apply_one(i));

	// free up memory
	SG_UNREF(ci);

	exit_shogun();
	return 0;
}


int kmeansmain(int argc, char **argv)
{
	init_shogun(&print_message, &print_message, &print_message);

	int32_t num_clusters=4;
	int32_t num_features=11;
	int32_t dim_features=3;
	int32_t num_vectors_per_cluster=5;//每个簇有5个向量，每个向量有3个直，共4*5*3=60;
	float64_t cluster_std_dev=2.0;

	/* build random cluster centers */
	SGMatrix<float64_t> cluster_centers(dim_features, num_clusters);
	SGVector<float64_t>::random_vector(cluster_centers.matrix, dim_features*num_clusters,
			-10.0, 10.0);
	SGMatrix<float64_t>::display_matrix(cluster_centers.matrix, cluster_centers.num_rows,
			cluster_centers.num_cols, "cluster centers");

	/* create data around clusters */
	SGMatrix<float64_t> data(dim_features, num_clusters*num_vectors_per_cluster);
	SG_SPRINT("data\n");
	for (index_t i=0; i<num_clusters; ++i)
	{ //有 4 * 3 * 5 = 60个数据
		for (index_t j=0; j<dim_features; ++j)
		{
			for (index_t k=0; k<num_vectors_per_cluster; ++k)
			{
				index_t idx=i*dim_features*num_vectors_per_cluster;
				idx+=j;
				idx+=k*dim_features;
				float64_t entry=cluster_centers.matrix[i*dim_features+j];
				data.matrix[idx]=CMath::normal_random(entry, cluster_std_dev);

				//打印data中的数据
				SG_SPRINT("%f  ", data.matrix[idx]);
			}
			SG_SPRINT("\n");
		}
	}
	SG_SPRINT("\n");

	/* create features, SG_REF to avoid deletion */
	CDenseFeatures<float64_t>* features=new CDenseFeatures<float64_t> ();
	features->set_feature_matrix(data);
	SG_REF(features);

	/* create labels for cluster centers */
	//CMulticlassLabels* labels=new CMulticlassLabels(num_features);
	//for (index_t i=0; i<num_features; ++i)
	//	labels->set_label(i, i%2==0 ? 0 : 1);

	/* create distance */
	CEuclideanDistance* distance=new CEuclideanDistance(features, features);

	/* create distance machine */
	CKMeans* clustering=new CKMeans(num_clusters, distance);
	clustering->train(features);

	/* build clusters */
	CMulticlassLabels* result=CLabelsFactory::to_multiclass(clustering->apply());
	for (index_t i=0; i<result->get_num_labels(); ++i)
		SG_SPRINT("cluster index of vector %i: %f\n", i, result->get_label(i));

	/* print cluster centers */
	CDenseFeatures<float64_t>* centers=
			(CDenseFeatures<float64_t>*)distance->get_lhs();

	SGMatrix<float64_t> centers_matrix=centers->get_feature_matrix();

	SGMatrix<float64_t>::display_matrix(centers_matrix.matrix, centers_matrix.num_rows,
			centers_matrix.num_cols, "learned centers");

	SGMatrix<float64_t>::display_matrix(cluster_centers.matrix, cluster_centers.num_rows,
			cluster_centers.num_cols, "real centers");

	/* clean up */
	SG_UNREF(result);
	SG_UNREF(centers);
	SG_UNREF(clustering);
	//SG_UNREF(labels);
	SG_UNREF(features);

	exit_shogun();

	return 0;
}


int main(int argc, char **argv)
{
	//bayesmain(argc, argv);
	kmeansmain(argc, argv);

	return 0;
}
