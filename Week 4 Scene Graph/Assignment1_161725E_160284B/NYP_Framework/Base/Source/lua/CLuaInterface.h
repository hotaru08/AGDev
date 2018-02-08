#pragma once

// includes the lua headers
#include "lua.hpp"
#include "lua.h"
#include "luaconf.h"
#include "lualib.h"
#include "Vector3.h"

#include <iostream>
#include <string>

using std::string;

class CLuaInterface 
{
protected:
	static CLuaInterface *s_instance;
	CLuaInterface();

public:
	static CLuaInterface *GetInstance()
	{
		if (!s_instance)
			s_instance = new CLuaInterface;
		return s_instance;
	}
	static bool DropInstance()
	{
		if (s_instance)
		{
			// Drop the Lua Interface Class
			s_instance->Drop();
			delete s_instance;
			s_instance = NULL;
			return true;
		}
		return false;
	}

	virtual ~CLuaInterface();
	// Initialisation of the Lua Interface Class
	bool Init();
	// Run the Lua Interface Class
	void Run();
	// Drop the Lua Interface Class
	void Drop();

	// Pointer to the Lua States
	lua_State *theLuaState;
	lua_State *theScreenState; /// For resolution
	lua_State *thePlayerState; /// For Player info
	lua_State *theEnemyState; /// For Enemy
	lua_State *theErrorState;

	// Getting and setting Values 
	void saveIntValue(const char* _name, int _value, int _type, const bool bOverwrite = NULL);
	void saveFloatValue(const char* _name, int _value, int _type, const bool bOverwrite = NULL);

	// get a int value through the lua interface class
	int getIntValue(const char* _name, int _type);

	// get a float value through the lua interface class
	float getFloatValue(const char* _name);

	// get a char value through the lua interface class
	char getCharValue(const char* varName, int _type);

	// get Vector3 values through the Lua Interface class
	Vector3 getVector3values(const char* varName);

	// Get Distance square value through the Lua interface
	float getDistanceSquared(const char* varName, Vector3 source, Vector3 destination);

	// Get variable number of values through the Lua interface
	int getVariableValues(const char* varName, int &a, int &b, int &c, int &d);

	// extract a field from a table
	float CLuaInterface::GetField(const char* key);

	// Get error message using an error code
	void error(const char *errorCode);
};