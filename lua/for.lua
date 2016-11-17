l={1,3,5,7,9}

-- for var=star, end, step do
for i=1,10,2 do -- from 1 to 10 ,step 2
	print(i)
end

-- step default 1
for i=1,10 do
	print(i)
end

-- for iterator
function list_iter(t)
	local i=0
	local n=table.getn(t)
	return function ()
				i=i+1
				if i <= n then
					return t[i]
				end
	end
end

-- for rv in close_bag_function do
for x in list_iter(l) do
	print(x)
end
