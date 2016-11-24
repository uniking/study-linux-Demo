md5 = require "md5.core"

local val = md5.sum("hello world")
sval = ""
local i=1
while i<=16 do
	--print(string.sub(val, i, i))
	local a = string.sub(val, i, i)
	ts = string.format("%x", string.byte(a))
	sval = sval .. ts
	i=i+1
end

sval=string.upper(sval)
print(sval)

print(val)

print(string.byte(val, 1, 16))

print(type(string.byte(val)))
