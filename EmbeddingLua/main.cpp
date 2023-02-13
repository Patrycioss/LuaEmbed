

#include <iostream>
#include <string>

extern "C"
{
#include "Lua-5.4.2-WIN32/include/lua.h"
#include "Lua-5.4.2-WIN32/include/lauxlib.h"
#include "Lua-5.4.2-WIN32/include/lauxlib.h"
#include "Lua-5.4.2-WIN32/include/lualib.h"
}

#ifdef _WIN32
#pragma comment(lib, "Lua-5.4.2-WIN32/liblua54.a")
#endif

struct Player
{
	std::string title;
	std::string name;
	std::string family;
	int level;
};


bool CheckLua(lua_State* L, int r)
{
	if (r != LUA_OK)
	{
		std::string errormsg = lua_tostring(L, -1);
		std::cout << errormsg << std::endl;
		return false;
	}
	return true;
}

void GivePlayerName(lua_State* L, Player& player)
{
	lua_getglobal(L, "player"); //Get player and place at -1

	if (lua_istable(L, -1)) //confirm player at -1 is table 
	{
		lua_pushstring(L, "Name"); //Push name onto stack making player -2 and name -1
		lua_gettable(L, -2); //uses value at the top of the stack (-1) to get value at specified index (-2)
		player.name = lua_tostring(L, -1); //gets the value from the name at -1
		lua_pop(L, 1); //pops the name value
	}
}

std::string GetPlayerName(lua_State* L, int n)
{
	std::string name;

	lua_getglobal(L, "GetPlayer");
	if (lua_isfunction(L, -1))
	{
		lua_pushnumber(L, 1);
		if (CheckLua(L, lua_pcall(L, 1, 1, 0))) //1 argument, 1 result, error 0
		{
			if (lua_istable(L, -1)) //confirm player at -1 is table 
			{
				lua_pushstring(L, "Name"); //Push name onto stack making player -2 and name -1
				lua_gettable(L, -2); //uses value at the top of the stack (-1) to get value at specified index (-2)
				name = lua_tostring(L, -1); //gets the value from the name at -1
				lua_pop(L, 1); //pops the name value
			}
		}
	}

	return name;
}

void DoAddStuff(lua_State* L)
{
	lua_getglobal(L, "AddStuff");
	if (lua_isfunction(L, -1))
	{
		lua_pushnumber(L, 3.5f);
		lua_pushnumber(L, 7.1f);
		if (CheckLua(L, lua_pcall(L, 2, 1, 0))) //2 arguments, 1 result, error 0
		{
			std::cout << "[C++] Called in Lua 'AddStuff(3.5f,7.1f)', got " << (float)lua_tonumber(L, -1) << std::endl;
		}
	}
}

int lua_HostFunction(lua_State* L)
{
	float a = (float)lua_tonumber(L, 1);
	float b = (float)lua_tonumber(L, 2);
	std::cout << "[C++] HostFunction(" << a << ", " << b << ") called" << std::endl;
	float c = a * b;
	lua_pushnumber(L, c);
	return 1; // only pushing one thing on stack
}


int main()
{
	Player player;

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_register(L, "HostFunction", lua_HostFunction);


	if (CheckLua(L, luaL_dofile(L, "Test.lua")))
	{
		// GivePlayerName(L, player);
		// DoAddStuff(L);
		// std::cout << GetPlayerName(L, 1) << std::endl;

		lua_getglobal(L, "DoAThing");
		if (lua_isfunction(L, -1))
		{
			lua_pushnumber(L, 5.0f);
			lua_pushnumber(L, 6.0f);

			if (CheckLua(L, lua_pcall(L, 2, 1, 0))) //2 argument, 1 result, error 0
			{
				std::cout << "[C++] Called in Lua 'DoAThing(5.0f,6.0f)', got " << (float)lua_tonumber(L, -1) << std::endl;

			}
		}
	}


	

	std::cout << player.name;


	lua_close(L); 
	return 0;
}
