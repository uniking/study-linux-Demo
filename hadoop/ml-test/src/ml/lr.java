package ml;


//$example on$
import java.util.Arrays;

import org.apache.spark.ml.regression.GeneralizedLinearRegression;
import org.apache.spark.ml.regression.GeneralizedLinearRegressionModel;
import org.apache.spark.ml.regression.GeneralizedLinearRegressionTrainingSummary;
import org.apache.spark.sql.Dataset;
import org.apache.spark.sql.Row;
//$example off$
import org.apache.spark.sql.SparkSession;

/**
* An example demonstrating generalized linear regression.
* Run with
* <pre>
* bin/run-example ml.JavaGeneralizedLinearRegressionExample
* </pre>
*/

public class lr {

public static void main(String[] args) {
 SparkSession spark = SparkSession
   .builder()
   .appName("JavaGeneralizedLinearRegressionExample")
   .getOrCreate();

 // $example on$
 // Load training data
 Dataset<Row> dataset = spark.read().format("libsvm")
   .load("data/mllib/sample_linear_regression_data.txt");

 GeneralizedLinearRegression glr = new GeneralizedLinearRegression()
   .setFamily("gaussian")
   .setLink("identity")
   .setMaxIter(10)
   .setRegParam(0.3);

 // Fit the model
 GeneralizedLinearRegressionModel model = glr.fit(dataset);

 // Print the coefficients and intercept for generalized linear regression model
 System.out.println("Coefficients: " + model.coefficients());
 System.out.println("Intercept: " + model.intercept());

 // Summarize the model over the training set and print out some metrics
 GeneralizedLinearRegressionTrainingSummary summary = model.summary();
 System.out.println("Coefficient Standard Errors: "
   + Arrays.toString(summary.coefficientStandardErrors()));
 System.out.println("T Values: " + Arrays.toString(summary.tValues()));
 System.out.println("P Values: " + Arrays.toString(summary.pValues()));
 System.out.println("Dispersion: " + summary.dispersion());
 System.out.println("Null Deviance: " + summary.nullDeviance());
 System.out.println("Residual Degree Of Freedom Null: " + summary.residualDegreeOfFreedomNull());
 System.out.println("Deviance: " + summary.deviance());
 System.out.println("Residual Degree Of Freedom: " + summary.residualDegreeOfFreedom());
 System.out.println("AIC: " + summary.aic());
 System.out.println("Deviance Residuals: ");
 summary.residuals().show();
 // $example off$

 spark.stop();
}
}
