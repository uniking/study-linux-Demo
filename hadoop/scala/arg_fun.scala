def fun(f:(Int)=>Int)= //定义“函数作为参数”的函数
{
	f(2)*4
}
var a = fun(x=>x*3) //传递一个 “匿名函数”
println("a:"+a)

var b=fun(x=>x)
println("b:"+b)

var c=fun(3 * _)
println("c:"+c)

var d=fun( M[_] )
println("d:"+d)		//???????????????


val vF=fun _ //函数作为值存储
a=vF(x=>x*4)
println(a)

(1 to 9) map("^" * _) foreach( println _ )

(1 to 9).map("^" * _).foreach( println _ )

(1 to 9).map( x=>"^" * x ).foreach( y=> println(y) )


/*
	"^" * _  ====>  x=>"^"*x

	_=>"^"*_  ====>  x=>(x=>"^"*_) ====>  y=>( x=>(x=>"^"*y) )   return function
*/
(1 to 9).map( _=>"^" * _ ).foreach( y=> println(y) )
