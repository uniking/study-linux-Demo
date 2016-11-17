local foo

function foo(n)
	if n>0 then
		print(n)
		return foo(n-1)
	end
end

-- use goto, not use stack, so foo(9999999999999999999999999999999999999999999999999) no consume stack size

foo(99999)
