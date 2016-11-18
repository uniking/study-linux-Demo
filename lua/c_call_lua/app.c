#include <stdio.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

int add(const char* filename, int x, int y)
{
	lua_State* L = lua_open();

	//load lua standard lib
	luaopen_base(L);
	luaopen_table(L);
	luaopen_os(L);
	//luaopen_io(L);
	luaopen_string(L);
	luaopen_math(L);

	if(luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run configuration file: %s", lua_tostring(L, -1));

	//push functions and arguments
	lua_getglobal(L, "add");
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);

	// do the call (2 arguments, 1 result)
	if(lua_pcall(L, 2, 1, 0) != 0)
		error(L, "error running function add: %s", lua_tostring(L, -1));

	if(!lua_isnumber(L, -1))
		error(L, "height should be a number\n");


	int r = (int)lua_tonumber(L, -1);

	lua_close(L);

	return r;
}

int main()
{
	int r = add("./conf.lua", 10, 20);
	printf("result:%d\n", r);

	return 0;
}
