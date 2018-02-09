#include "CLuaInterface.h"
#include <iostream>
#include "../Waypoint/WaypointManager.h"

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

	// For Highscore
	theHighscoreState = lua_open();
	if (theHighscoreState)
	{
		// 2. load lua auxiliary libraries
		luaL_openlibs(theHighscoreState);

		// 3. load lua script
		luaL_dofile(theHighscoreState, "Image//DM2240_HighScore.lua");

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

	// For Player
	thePlayerState = lua_open();
	if (thePlayerState)
	{
		// 2. load lua auxiliary libraries
		luaL_openlibs(thePlayerState);

		// 3. load lua script
		luaL_dofile(thePlayerState, "Image//DM2240_Player.lua");

		result = true;
	}

	// For Enemy
	theEnemyState = lua_open();
	if (theEnemyState)
	{
		// 2. load lua auxiliary libraries
		luaL_openlibs(theEnemyState);
		lua_register(theEnemyState, "AddWaypoint", RunWaypoint);

		// 3. load lua script
		luaL_dofile(theEnemyState, "Image//DM2240_Enemy.lua");
		result = true;
	}


	// For Object
	theObjectState = lua_open();
	if (theObjectState)
	{
		// 2. load lua auxiliary libraries
		luaL_openlibs(theObjectState);

		// 3. load lua script
		luaL_dofile(theObjectState, "Image//DM2240_Object.lua");
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
	if (theScreenState == NULL)
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

void CLuaInterface::saveFloatValue(const char * _name, float _value, int _type, const bool bOverwrite)
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

void CLuaInterface::saveFieldValue(const char * _name, Vector3 _value, int _type, const bool bOverwrite)
{
	// read value of Lua variable
	lua_getglobal(theLuaState, "SaveToLuaFile");

	//// Print values
	char outputString[80];
	sprintf(outputString, "%s = {x=%f, y=%f, z=%f}\n", _name, _value.x, _value.y, _value.z);
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
		lua_getglobal(theHighscoreState, _name);
		return lua_tointeger(theHighscoreState, -1);
		break;
	case 2: /// For Resolution
		lua_getglobal(theScreenState, _name);
		return lua_tointeger(theScreenState, -1);
		break;
	case 3: /// For Player 
		lua_getglobal(thePlayerState, _name);
		return lua_tointeger(thePlayerState, -1);
		break;
	case 4: /// For Objects
		lua_getglobal(theObjectState, _name);
		return lua_tointeger(theObjectState, -1);
		break;
	default:
		break;
	}
}

float CLuaInterface::getFloatValue(const char * _name, int _type)
{
	switch (_type)
	{
	case 1: /// For main file
		lua_getglobal(theHighscoreState, _name);
		return static_cast<float>(lua_tonumber(theHighscoreState, -1));
		break;
	case 2: /// For Resolution
		lua_getglobal(theScreenState, _name);
		return static_cast<float>(lua_tonumber(theScreenState, -1));
		break;
	case 3: /// For Player 
		lua_getglobal(thePlayerState, _name);
		return static_cast<float>(lua_tonumber(thePlayerState, -1));
		break;
	case 4: /// For Objects
		lua_getglobal(theObjectState, _name);
		return static_cast<float>(lua_tonumber(theObjectState, -1));
		break;
	default:
		break;
	}
}

char CLuaInterface::getCharValue(const char * varName, int _type)
{
	size_t len = 0;
	const char* cstr;

	switch (_type)
	{
	case 1: /// Main Lua File
		lua_getglobal(theLuaState, varName);
		cstr = lua_tolstring(theLuaState, -1, &len);
		break;
	case 2: /// For Player 
		lua_getglobal(thePlayerState, varName);
		cstr = lua_tolstring(thePlayerState, -1, &len);
		break;
	}
	
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
	lua_getglobal(thePlayerState, varName);
	lua_rawgeti(thePlayerState, -1, 1); // get the first value in the stack
	int x = lua_tonumber(thePlayerState, -1);
	lua_pop(thePlayerState, 1);

	lua_rawgeti(thePlayerState, -1, 2);// get the second value 
	int y = lua_tonumber(thePlayerState, -1);
	lua_pop(thePlayerState, 1);

	lua_rawgeti(thePlayerState, -1, 3);
	int z = lua_tonumber(thePlayerState, -1);
	lua_pop(thePlayerState, 1);

	return Vector3(x,y,z);
}

Vector3 CLuaInterface::getVector3valuesE(const char * varName)
{
	// Get the stack
	lua_getglobal(theEnemyState, varName);
	lua_rawgeti(theEnemyState, -1, 1); // get the first value in the stack
	int x = lua_tonumber(theEnemyState, -1);
	lua_pop(theEnemyState, 1);

	lua_rawgeti(theEnemyState, -1, 2);// get the second value 
	int y = lua_tonumber(theEnemyState, -1);
	lua_pop(theEnemyState, 1);

	lua_rawgeti(theEnemyState, -1, 3);
	int z = lua_tonumber(theEnemyState, -1);
	lua_pop(theEnemyState, 1);

	return Vector3(x, y, z);
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

// For WayPoint handling
float CLuaInterface::GetField(const char * key)
{
	int result = false;
	lua_pushstring(theEnemyState, key);
	lua_gettable(theEnemyState, -2);
	result = (int)lua_tonumber(theEnemyState, -1);
	lua_pop(theEnemyState, 1); // remove number
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

static int RunWaypoint(lua_State *state)
{
	std::vector<float>value;
	// read value of Lua variable
	int n = lua_gettop(state);
	for (int i = 1; i <= n; ++i)
	{
		value.push_back(lua_tonumber(state, i));
	}
	CWaypointManager::GetInstance()->AddWaypoint(Vector3(value[0], value[1], value[2]));

	return 0; // dont need get back anything
}
