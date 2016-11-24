-- load the http module
local io = require("io")
local http = require("socket.http")
local ltn12 = require("ltn12")

-- connect to server "www.cs.princeton.edu" and retrieves this manual
-- file from "~diego/professional/luasocket/http.html" and print it to stdout
http.request{
    url = "http://www.natpi.cn/index.html",
    sink = ltn12.sink.file(io.stdout)
}

-- connect to server "www.example.com" and tries to retrieve
-- "/private/index.html". Fails because authentication is needed.
--b, c, h = http.request("http://www.natpi.cn/index.html")
-- b returns some useless page telling about the denied access, 
-- h returns authentication information
-- and c returns with value 401 (Authentication Required)

-- tries to connect to server "wrong.host" to retrieve "/"
-- and fails because the host does not exist.
--r, e = http.request("http://www.natpi.cn/")
-- r is nil, and e returns with value "host not found"
