local smtp = require("socket.smtp")
local from = ""
local rcpt = {"",}
local mesgt = {
				headers = {
					to = "ds8dawdafe5@163.com", -- 收件人
					subject = "This is Mail Title"
					},
				body = "This is  Mail Content."
			}

local  r, e = smtp.send{
		server="smtp.163.com",
		user="dsdsds",
		password="dddsdsds",
		from = from,
		rcpt = rcpt,
		source = smtp.message(mesgt)
	}
if not r then
	print(e)
else
	print("send ok!")
end
