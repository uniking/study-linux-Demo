#include <stdio.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

void load_config(const char* filename, int* width, int* height, char* home)
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

	lua_getglobal(L, "width");
	lua_getglobal(L, "height");
	lua_getglobal(L, "home");

	if(!lua_isnumber(L, -3))
		error(L, "width should be a number\n");
	if(!lua_isnumber(L, -2))
		error(L, "height should be a number\n");
	if(!lua_isstring(L, -1))
		error(L, "height should be a number\n");


	*width = (int)lua_tonumber(L, -3);
	*height = (int)lua_tonumber(L, -2);

	const char* phome = lua_tostring(L, -1);
	strncpy(home, phome, 64);

	lua_close(L);
}

int main()
{
	int width = 0;
	int height = 0;
	char home[64]={0};

	load_config("./conf.lua", &width, &height, home);
	printf("width:%d height:%d home:%s\n", width, height, home);

	return 0;
}
