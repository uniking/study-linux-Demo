package org.wzl;

import org.apache.eagle.security.userprofile.model.UserProfileKDEModel;
import org.apache.eagle.security.userprofile.model.kde.UserProfileKDEModeler;

import org.apache.eagle.security.userprofile.model.UserProfileEigenModel;
import org.apache.eagle.security.userprofile.model.eigen.UserProfileEigenModeler;

//detect
import org.apache.eagle.security.userprofile.impl.UserProfileAnomalyKDEEvaluator;
import org.apache.eagle.security.userprofile.model.UserActivityAggModel;
import org.apache.eagle.security.userprofile.UserProfileConstants;
import org.apache.eagle.security.userprofile.MLCallbackResult;

import org.apache.eagle.security.userprofile.impl.UserProfileAnomalyEigenEvaluator;


import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.math3.linear.RealMatrix;
import org.apache.commons.math3.linear.Array2DRowRealMatrix; 

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;


public class test {

	/**

	* 输出一行字符串“Hello World!”

	* @param args

	*/

	public static void saveModelToFile(UserProfileKDEModel model, String path) throws Exception
	{
		try
		{
			FileOutputStream fos = new FileOutputStream(path);
			ObjectOutputStream oos = new ObjectOutputStream(fos);

			oos.writeObject(model);
			oos.flush();
			oos.close();
		}
		catch(Exception e)
		{
			System.out.println("save model to file exception");
		}
	}

	public static boolean restoreModelFromFile(String path, UserProfileKDEModel fModel) throws Exception
	{
		boolean bRtn = false;
		try
		{
			FileInputStream fis = new FileInputStream(path);
			ObjectInputStream ois = new ObjectInputStream(fis);

			fModel = (UserProfileKDEModel)ois.readObject();
			bRtn = true;
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return bRtn;
	}


	public static void saveModelToFileEigen(UserProfileEigenModel model, String path) throws Exception
	{
		try
		{
			FileOutputStream fos = new FileOutputStream(path);
			ObjectOutputStream oos = new ObjectOutputStream(fos);

			oos.writeObject(model);
			oos.flush();
			oos.close();
		}
		catch(Exception e)
		{
			System.out.println("save model to file exception");
		}
	}

	public static boolean restoreModelFromFileEigen(String path, UserProfileEigenModel fModel) throws Exception
	{
		boolean bRtn = false;
		try
		{
			FileInputStream fis = new FileInputStream(path);
			ObjectInputStream ois = new ObjectInputStream(fis);

			fModel = (UserProfileEigenModel)ois.readObject();
			bRtn = true;
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		return bRtn;
	}

	public static boolean detect_kde(UserProfileKDEModel model)
	{
		System.out.println("detect_kde");
		UserProfileAnomalyKDEEvaluator discover = new UserProfileAnomalyKDEEvaluator();

		double[][] userData = { {32,15,36,59}, {1,2,3,4} , {8,6,3,0}, {2,2,3,4}, {3,1,2,3}};
		RealMatrix matrix = new Array2DRowRealMatrix(userData);
		String[] cmdTypes = new String[]{"cmd", "ls", "hhh", "jjj"};
		Long timestamp = 478545L;
		
		UserActivityAggModel uAct = new UserActivityAggModel("user1", matrix, cmdTypes, "hadoop-s", timestamp);

		List<MLCallbackResult> res = discover.detect("user1", UserProfileConstants.KDE_ALGORITHM, uAct, model);

		if(res != null)
		{
			System.out.println("show result:");
			for(MLCallbackResult x: res)
			{
				boolean isA = x.isAnomaly();
				System.out.println("is anomaly:"+ isA);
			}
		}
		else
			System.out.println("res null");

		return true;
	}


	public static boolean detect_eigen(UserProfileEigenModel model)
	{
		System.out.println("detect_eigen");

		UserProfileAnomalyEigenEvaluator discover = new UserProfileAnomalyEigenEvaluator();

		System.out.println("d0");
		double[][] userData = {{32,15,36,59}, {1,2,3,4} , {8,6,3,0}, {2,2,3,4}, {3,1,2,3}};
		RealMatrix matrix = new Array2DRowRealMatrix(userData);
		String[] cmdTypes = new String[]{"cmd", "ls","hhh","jjj"};
		Long timestamp = 478545L;
		
		System.out.println("d1");
		UserActivityAggModel uAct = new UserActivityAggModel("user1", matrix, cmdTypes, "hadoop-s", timestamp);

		System.out.println("d2");
		List<MLCallbackResult> res = discover.detect("user1", UserProfileConstants.USER_PROFILE_EIGEN_MODEL_SERVICE, uAct, model);

		System.out.println("d3");
		if(res != null)
		{
			System.out.println("show result:");
			for(MLCallbackResult x: res)
			{
				boolean isA = x.isAnomaly();
				System.out.println("is anomaly:"+ isA);
			}
		}
		else
			System.out.println("res null");

		return true;
	}

	public static void main(String[] args)  throws Exception
	{

		try
		{
			System.out.println("Hello World!");

			UserProfileKDEModeler kde = new UserProfileKDEModeler();

			double[][] userData = { {3,1,3,5}, {3,1,3,5} , {3,1,3,5}, {3,1,3,5}, {3,1,3,5}, {3,1,3,5}, {3,1,2,3}, {3,1,2,3}, {3,1,2,3}, {3,1,2,3}};

			RealMatrix matrix = new Array2DRowRealMatrix(userData);

			String mymodel = new String("./kModel");
			int i=0;
			List<UserProfileKDEModel> lModle = kde.generate("hadoop-s", "user1", matrix);

			for(UserProfileKDEModel x :lModle)
			{
				System.out.println("to save model");
				saveModelToFile(x, mymodel+i);
				i++;
			}


			mymodel = "./eModel";
			UserProfileEigenModeler eigen = new UserProfileEigenModeler();
			List<UserProfileEigenModel> eigen_model = eigen.generate("hadoop-s", "user1", matrix);
			for(UserProfileEigenModel y :eigen_model)
			{
				System.out.println("to save model");
				saveModelToFileEigen(y, mymodel+i);
				i++;
			}


			//detect
			for(UserProfileKDEModel x:lModle)
			{
				detect_kde(x);
			}

			//detect eigen
			for(UserProfileEigenModel y:eigen_model)
			{
				detect_eigen(y);
			}
		}
		catch(Exception e)
		{
			System.out.println("exception");
		}
		
		System.out.println("exit");

	}

}
