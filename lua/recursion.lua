local fact

fact = function (n)
			if n == 0 then
				return 1
			else
				return n*fact(n-1)
			end
		end

-- 4*3*2*1
print(fact(4))
