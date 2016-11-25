#include <stdio.h>
#include <string.h>
#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>

typedef struct _NumArray
{
	int size;
	double values[1];
}NumArray, *PNumArray;
extern "C" int newarray(lua_State* L) 
{
	int n = luaL_checkint(L,1);
	size_t nbytes = sizeof(NumArray) + (n-1)*sizeof(double);
	PNumArray a = (PNumArray)lua_newuserdata(L, nbytes);
	a->size = n;

	return 1;
}

extern "C" int setarray(lua_State* L)
{
	PNumArray a = (PNumArray)lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2);
	double value = luaL_checknumber(L, 3);
	
	luaL_argcheck(L, a!=NULL, 1, "''array' expected");
	luaL_argcheck(L, 1<= index && index <= a->size, 2, "index of out of range");
	a->values[index-1] = value;

	return 0;
}

extern "C" int getarray(lua_State* L)
{
	PNumArray a = (PNumArray)lua_touserdata(L, 1);
	int index = luaL_checkint(L, 2);
	
	luaL_argcheck(L, a!=NULL, 1, "''array' expected");
	luaL_argcheck(L, 1<= index && index <= a->size, 2, "index of out of range");
	lua_pushnumber(L, a->values[index-1]);

	return 1;
}

extern "C" int getsize(lua_State* L)
{
	PNumArray a = (PNumArray)lua_touserdata(L, 1);
	
	luaL_argcheck(L, a!=NULL, 1, "''array' expected");
	lua_pushnumber(L, a->size);

	return 1;
}

static luaL_Reg myLib[] = { 
	{"new", newarray},
	{"set", setarray},
	{"get", getarray},
	{"size", getsize},
	{NULL, NULL} 
}; 

//该C库的唯一入口函数。其函数签名等同于上面的注册函数。见如下几点说明：
//1. 我们可以将该函数简单的理解为模块的工厂函数。
//2. 其函数名必须为luaopen_xxx，其中xxx表示library名称。Lua代码require "xxx"需要与之对应。
//3. 在luaL_register的调用中，其第一个字符串参数为模块名"xxx"，第二个参数为待注册函数的数组。
//4. 需要强调的是，所有需要用到"xxx"的代码，不论C还是Lua，都必须保持一致，这是Lua的约定，
//   否则将无法调用。
extern "C" //__declspec(dllexport)
int luaopen_myArray(lua_State* L) 
{
    const char* libName = "myArray";
    luaL_register(L,libName,myLib);
    return 1;
}
