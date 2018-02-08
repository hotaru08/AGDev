#include "CLuaInterface.h"
#include <iostream>

using std::cout;
using std::endl;

// Allocating and initialising CLuaInterface's static data member
// The pointer is allocated but not the object's constructor
CLuaInterface* CLuaInterface::s_instance = 0;

CLuaInterface::CLuaInterface() : theLuaState(NULL)
{
}

CLuaInterface::~CLuaInterface()
{
}

bool CLuaInterface::Init()
{
	bool result = false;

	/* Creation of States and setting Files to States */
	// For main file
	theLuaState = lua_open();
	if (theLuaState)
	{
		// 2. load lua auxiliary libraries
		luaL_openlibs(theLuaState);

		// 3. load lua script
		luaL_dofile(theLuaState, "Image//DM2240.lua");

		result = true;
	}

	// For resolution
	theScreenState = lua_open();
	if (theScreenState)
	{
		// 2. load lua auxiliary libraries
		luaL_openlibs(theScreenState);

		// 3. load lua script
		luaL_dofile(theScreenState, "Image//DM2240_Resolution.lua");

		result = true;
	}

	// For Error
	theErrorState = lua_open();
	if ((theLuaState) && (theErrorState))
	{
		// 2. Load lua auxiliary libraries
		luaL_openlibs(theLuaState);

		// Load the error lua script
		luaL_dofile(theErrorState, "Image//errorLookup.lua");
	}

	return result;
}

// Run the lua interface class
void CLuaInterface::Run()
{
	if (theLuaState == NULL || theScreenState == NULL)
		return;

	// 4. Read the variables
	// lua_getglobal(lua_State*, const char*)
	lua_getglobal(theScreenState, "title");
	// extract value, index -1 as variable is already retrieved using lua_getglobal
	const char *title = lua_tostring(theScreenState, -1);

	lua_getglobal(theScreenState, "width");
	int width = lua_tointeger(theScreenState, -1);

	lua_getglobal(theScreenState, "height");
	int height = lua_tointeger(theScreenState, -1);

	// Display on screen
	cout << title << endl;
	cout << "---------------------------------------" << endl;
	cout << "Width of screen : " << width << endl;
	cout << "Height of screen : " << height << endl;
}

void CLuaInterface::Drop()
{
	if (theLuaState)
	{
		// close lua state
		lua_close(theLuaState);
	}
}

// Setting to lua file
void CLuaInterface::saveIntValue(const char * _name, int _value, int _type, const bool bOverwrite)
{
	// read value of Lua variable
	lua_getglobal(theLuaState, "SaveToLuaFile");

	// Print values
	char outputString[80];
	sprintf(outputString, "%s = %d\n", _name, _value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_pushinteger(theLuaState, _type);
	lua_call(theLuaState, 3, 0); 
}

void CLuaInterface::saveFloatValue(const char * _name, int _value, int _type, const bool bOverwrite)
{
	// read value of Lua variable
	lua_getglobal(theLuaState, "SaveToLuaFile");
	
	//// Print values
	char outputString[80];
	sprintf(outputString, "%s = %f\n", _name, _value);
	lua_pushstring(theLuaState, outputString);
	lua_pushinteger(theLuaState, bOverwrite);
	lua_pushinteger(theLuaState, _type);
	lua_call(theLuaState, 3, 0);
}

// Getting from lua files
int CLuaInterface::getIntValue(const char * _name, int _type)
{ 
	switch (_type)
	{
	case 1: /// For main file
		lua_getglobal(theLuaState, _name);
		return lua_tointeger(theLuaState, -1);
		break;
	case 2: /// For Resolution
		lua_getglobal(theScreenState, _name);
		return lua_tointeger(theScreenState, -1);
		break;
	default:
		break;
	}
}

float CLuaInterface::getFloatValue(const char * _name)
{
	lua_getglobal(theLuaState, _name);
	return (float)lua_tointeger(theLuaState, -1);
}

char CLuaInterface::getCharValue(const char * varName)
{
	lua_getglobal(theLuaState, varName);

	size_t len;
	
	const char* cstr = lua_tolstring(theLuaState, -1, &len);

	// if the string is not empty, then return the first char
	if (len > 0)
		return cstr[0];

	// else return a default value of space
	else
		return ' ';
}

Vector3 CLuaInterface::getVector3values(const char * varName)
{
	// Get the stack
	lua_getglobal(theLuaState, varName);
	lua_rawgeti(theLuaState, -1, 1); // get the first value in the stack
	int x = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);

	lua_rawgeti(theLuaState, -1, 2);// get the second value 
	int y = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);

	lua_rawgeti(theLuaState, -1, 3);
	int z = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);

	return Vector3(x,y,z);
}

float CLuaInterface::getDistanceSquared(const char * varName, Vector3 source, Vector3 destination)
{
	lua_getglobal(theLuaState, varName);
	lua_pushnumber(theLuaState, source.x);
	lua_pushnumber(theLuaState, source.y);
	lua_pushnumber(theLuaState, source.z);
	lua_pushnumber(theLuaState, destination.x);
	lua_pushnumber(theLuaState, destination.y);
	lua_pushnumber(theLuaState, destination.z);
	lua_call(theLuaState, 6, 1);

	float distanceSquare = (float)lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	return distanceSquare;
}

// Multiple values
int CLuaInterface::getVariableValues(const char * varName, int & a, int & b, int & c, int & d)
{
	lua_getglobal(theLuaState, varName);
	lua_pushnumber(theLuaState, 1000);
	lua_pushnumber(theLuaState, 2000);
	lua_pushnumber(theLuaState, 3000);
	lua_pushnumber(theLuaState, 4000);
	lua_call(theLuaState, 4, 4);

	a = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	b = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	c = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	d = lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);

	return true;
}

float CLuaInterface::GetField(const char * key)
{
	int result = false;
	lua_pushstring(theLuaState, key);
	lua_gettable(theLuaState, -2);
	result = (int)lua_tonumber(theLuaState, -1);
	lua_pop(theLuaState, 1);
	return result;
}

void CLuaInterface::error(const char *errorCode)
{
	if (theErrorState == NULL)
		return;

	lua_getglobal(theErrorState, errorCode);
	const char *errorMsg = lua_tostring(theErrorState, -1);
	if (errorMsg != NULL)
		cout << errorMsg << endl;
	else
		cout << errorCode << " is not valid.\n*** Please contact the developer ***" << endl;
}
