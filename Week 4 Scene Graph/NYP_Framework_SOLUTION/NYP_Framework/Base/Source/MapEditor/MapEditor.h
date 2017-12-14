#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "SingletonTemplate.h"
#include "Vector3.h"
#include <sstream>

using std::ostringstream;

class CEnemy;

class MapEditor : public Singleton<MapEditor>
{
public:
	enum OBJECT_TYPE
	{
		OBJECT = 0,
		HOUSE,
		TREE,
		TOMBSTONE,
	};

	enum SCALE_AXIS
	{
		SCALE_X = 0,
		SCALE_Y,
		SCALE_Z,
		SCALE_ALL
	};

	MapEditor();
	~MapEditor();

	bool mapEditing;

	// rendering for reference
	void renderObj(void);
	// render option
	void renderOption(void);
	// update option
	void updateOption(double dt);

	OBJECT_TYPE objType;

	// saving original displacement
	Vector3 displacement;
	// for tuning the displacement
	Vector3 displacementMod;
	// scaling obj
	Vector3 scale;
	// rotate
	float rotate;

	// show what obj
	ostringstream ObjectType_String;
	// show scale
	ostringstream ScaleAxis_String;

	SCALE_AXIS scaleAxis;
};

#endif