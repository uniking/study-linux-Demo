def fun(f:(Int)=>Int)=f(2)//定义“函数作为参数”的函数
var a = fun(x=>x*3) //传递一个 “匿名函数”
println(a)

val vF=fun _ //函数作为值存储
a=vF(x=>x*4)
println(a)
