x=10			--all
local i=1		--local in file

while i<=x do
	local x = i*2		-- local in while  between do and end
	print("local x=" .. x)
	i = i+1
end

print("x=" .. x)
print("local i=" .. i)

--control local var
do
local x = "hello"
print(x)
end

print(x)
