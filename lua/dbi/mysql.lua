db = require("DBI")

local dbh,err = db.Connect("MySQL", "DNSdata", "root", "abcd", "127.0.0.1", "3306")
--print(dbh, err)
print(type(dbh))	-->userdata

local alive = dbh:ping()
--print(alive)

--local ar,err = db.Do(dbh, "select * from dns limit 1")
--print(ar,err)

local pr,err = assert(dbh:prepare('select * from dns limit 5'))
--print(pr, err)
print(type(pr))	-->userdata
pr:execute()

--  show columns from dns
print(pr:affected())
local result = pr:columns()
for k,v in pairs(result) do
	print(k,v)
end

-- select * from dns
local rw = pr:rows()
for x in pr:rows() do
	print("---------")
	for k,v in pairs(x) do
		print(k, v)
	end
end


local ok = dbh:close()
--print(ok)
