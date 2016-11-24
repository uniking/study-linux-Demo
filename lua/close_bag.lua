function newCounter()
	local i=0
	return function()
				i=i+1
				return i
			end
end

c = newCounter()		-- c get anonymous function, i is local var or global var ? is upvalue !
print(newCounter())

print(c())
print(c())
print(c())

print(type(newCounter()))
print(type(c()))
