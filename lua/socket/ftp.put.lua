-- load the ftp support
local ftp = require("socket.ftp")
local ltn12 = require("ltn12")

-- Log as user "fulano" on server "ftp.example.com",
-- using password "silva", and append to the remote file "LOG", sending the
-- contents of the local file "LOCAL-LOG"
f, e = ftp.put{
	host = "192.168.220.177", 
	user = "admin",
	password = "abcd",
	path="ftp.put.lua",
	source = ltn12.source.file(io.open("./ftp.put.lua", "r"))
}

print(f,e)
