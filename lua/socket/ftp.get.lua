-- load the ftp support
local ftp = require("socket.ftp")
local ltn12 = require("ltn12")


-- Log as user "anonymous" on server "ftp.tecgraf.puc-rio.br",
-- and get file "lua.tar.gz" from directory "pub/lua" as binary.
f, e = ftp.get{host = "192.168.220.177",
				port = 21,
				path = "111.txt",
				user = "admin",
				password = "abcd",
				sink = ltn12.sink.file(io.stdout)
				}
print(f,e)
