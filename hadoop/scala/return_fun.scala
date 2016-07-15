def sumInts(a:Int, b:Int):Int =
{
	if(a>b)
		0
	else
		a+sumInts(a+a, b)
}

println( sumInts(1,9) )

/////////////////////////////////////////////////

def sum(f:Int=>Int):(Int, Int)=>Int =
{
	def sumF(a:Int, b:Int):Int =
	{
		if(a>b)
			0
		else
			a+sumInts(a+1, b)
	}

	sumF
}

val sumInts_t=sum(x=>x)

println( sumInts_t(1, 9) )


/////////////////////////////////////////////////
def sum2(a:Int) = (b:Int)=>a+b

println( sum2(1)(2) )

/////////////////////////////////////////////////
def sum3(a:Int)(b:Int) = a+b

println( sum3(1)(2) )

/////////////////////////////////////////////////
def sum4(a:Int)(b:Int)(c:Int) = a+b+c

println( sum4(1)(2)(3) )
