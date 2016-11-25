[sxis@centos-w as_app]$ lua app.lua 
lua: app.lua:1: module 'mylib' not found:
	no field package.preload['mylib']
	no file './mylib.lua'
	no file '/usr/share/lua/5.1/mylib.lua'
	no file '/usr/share/lua/5.1/mylib/init.lua'
	no file '/usr/lib64/lua/5.1/mylib.lua'
	no file '/usr/lib64/lua/5.1/mylib/init.lua'
	no file './mylib.so'
	no file '/usr/lib64/lua/5.1/mylib.so'
	no file '/usr/lib64/lua/5.1/loadall.so'
stack traceback:
	[C]: in function 'require'
	app.lua:1: in main chunk
	[C]: ?
[sxis@centos-w as_app]$ 


cp mylib.so /usr/lib64/lua/5.1/
