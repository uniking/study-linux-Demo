function add(a, b)
	return a+b
end

function mul_var()
	return 1,2,3,4
end

x,y=mul_var()
print(x .. " " .. y)
o,p,q,r=mul_var()
print(o .. " " .. p .. " " .. q .. " " .. r)

print(mul_var())
