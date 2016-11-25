Set = {}
Set.mt = {} --增加一个元表

function Set.new(t)
	local set = {}
	setmetatable(set, Set.mt)
	for _, l in ipairs(t) do
		set[l] = true
	end
	return set
end

function Set.union(a, b)
	local res = Set.new{}
	for k in pairs(a) do res[k] = true end
	for k in pairs(b) do res[k] = true end
	return res
end

--给元表增加__add函数， +重载啊！
Set.mt.__add = Set.union

-- test
s1 = Set.new{10,20,30,40,50}
s2 = Set.new{60,70}
s3 = s1 + s2
for k in pairs(s3) do
	print(k)
end
