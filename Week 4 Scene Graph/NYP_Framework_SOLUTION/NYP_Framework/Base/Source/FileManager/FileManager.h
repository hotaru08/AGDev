#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "SingletonTemplate.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Vector3.h"
#include <vector>

using std::vector;
using std::string;

class FileManager : public Singleton <FileManager>
{
	friend Singleton<FileManager>;

	struct OBJinfo
	{
		string type;
		float posX;
		float posY;
		float posZ;

		float minAABBx;
		float minAABBy;
		float minAABBz;

		float maxAABBx;
		float maxAABBy;
		float maxAABBz;

		float scalex;
		float scaley;
		float scalez;

		float rotateAngle;
	}theOBJinfo;

	struct enemyInfo
	{
		int type;

		float displacementX;
		float displacementY;
		float displacementZ;

		float scaleX;
		float scaleY;
		float scaleZ;
	}theEnemyInfo;

	vector<OBJinfo>objlist;

public:
	FileManager() {};
	~FileManager() {};

	void init();
	bool ReadMapFile(const string myFile); // read the file containing map stuff

	void EditMapFile(const string myFile); // editing the map file with what editor creates

	Vector3 Token2Vector(string token);
	double Token2Double(string token);
	bool Token2Bool(string token);

	void clearVector();
};

#endif