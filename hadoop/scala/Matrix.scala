	private def File2Matrix(inputFile: String)
	{
		val conf = new SparkConf().setAppName("Gaussian Mixture Model EM example")
		val ctx  = new SparkContext(conf)
		ctx.setCheckpointDir("checkpoint")

		val data = ctx.textFile(inputFile)
		val line = data.collect //RDD[String] to Array[String]

		val mat:ArrayBuffer[Array[Double]] = new ArrayBuffer(10)
		for(elem <- line)
			mat += elem.split(" ").map(_.toDouble)

		/*
			mat += elem.split(" ").toDouble error
			elem => String
			elem.split(" ") => Array[String]
			elem.split(" ").toDouble => Array[String].toDouble, there is't toDouble in Array

			map(function(element))
		*/
	}
