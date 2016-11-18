#include <stdio.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

int main()
{
	char buff[512];
	int error;
	lua_State* L = lua_open();

	//load lua standard lib
	luaopen_base(L);
	luaopen_table(L);
	//luaopen_io(L);
	luaopen_string(L);
	luaopen_math(L);

	while(fgets(buff, sizeof(buff), stdin) != NULL)
	{
		error = luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0, 0);
		if(error)
		{
			fprintf(stderr, "%s", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
	
	lua_close(L);

	return 0;
}
