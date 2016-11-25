sk = require("socket")

local info = sk.dns.getaddrinfo("www.baidu.com")

function print_addr_info(ad)
	for k,v in pairs(ad) do
		print(k)
		if type(v)=="table" then
			for kk,vv in pairs(v) do
				print(kk, vv)
			end
		else
			print(v)
		end
	end
end

print_addr_info(info)

print(sk.dns.gethostname())

ip, inf = sk.dns.toip(sk.dns.gethostname())
print(ip)
print_addr_info(inf)
