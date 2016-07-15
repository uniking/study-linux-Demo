def sum(a:Int, b:Int):Int =
{
	if(a>b)
		0;
	else
		a+sum(a+1,b)
}

println(sum(1,9))
