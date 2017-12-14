#include "FileManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Vector3.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "KeyboardController.h"
#include "../GenericEntity.h"
#include "../EntityManager.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::getline;

void FileManager::init()
{
}

bool FileManager::ReadMapFile(const string myFile)
{
	ifstream file;
	static int count = 0;
	string data = "";
	string tempData = "";
	int nextData = 0;
	bool skipFirstLine = false;
	file.open(myFile);

	if (file.is_open())
	{
		while (getline(file, data))
		{
			if (!skipFirstLine)
			{
				skipFirstLine = true;
				continue;
			}

			for (unsigned int i = 0; i < data.size(); ++i)
			{
				if (nextData == 0)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.type = tempData;
				}

				else if (nextData == 1)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.posX = stof(tempData);
				}

				else if (nextData == 2)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.posY = stof(tempData);
				}

				else if (nextData == 3)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.posZ = stof(tempData);
				}

				else if (nextData == 4)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.minAABBx = stof(tempData);
				}

				else if (nextData == 5)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.minAABBy = stof(tempData);
				}

				else if (nextData == 6)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.minAABBz = stof(tempData);
				}

				else if (nextData == 7)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.maxAABBx = stof(tempData);
				}

				else if (nextData == 8)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.maxAABBy = stof(tempData);
				}

				else if (nextData == 9)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.maxAABBz = stof(tempData);
				}

				else if (nextData == 10)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.scalex = stof(tempData);
				}

				else if (nextData == 11)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.scaley = stof(tempData);
				}

				else if (nextData == 12)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.scalez = stof(tempData);
				}

				else if (nextData == 13)
				{
					for (unsigned int j = i; j < data.size(); ++j)
					{
						if (data[j] != ',')
						{
							tempData += data[j];
						}
						else
						{
							i = j;
							break;
						}
					}
					theOBJinfo.rotateAngle = stof(tempData);
				}

				nextData += 1;
				tempData = "";
			}
			GenericEntity* temp = Create::Entity(theOBJinfo.type, Vector3(theOBJinfo.posX, theOBJinfo.posY, theOBJinfo.posZ), Vector3(theOBJinfo.scalex, theOBJinfo.scaley, theOBJinfo.scalez));
			temp->SetRotate(theOBJinfo.rotateAngle);
			temp->SetAABB(Vector3(theOBJinfo.maxAABBx, theOBJinfo.maxAABBy, theOBJinfo.maxAABBz), Vector3(theOBJinfo.minAABBx, theOBJinfo.minAABBy, theOBJinfo.minAABBz));

			if (temp->GetName() == "Tree_Hi")
			{
				temp->InitLOD("Tree_Hi", "Tree_Med", "Tree_Lo");
			}

			if (temp->GetName() == "TombStone_Hi")
			{
				temp->InitLOD("TombStone_Hi", "TombStone_Med", "TombStone_Lo");
			}

			if (temp->GetName() == "House_Hi")
			{
				temp->InitLOD("House_Hi", "House_Med", "House_Lo");
			}

			nextData = 0;
		}
		return true;
	}

	return false;
}

void FileManager::EditMapFile(const string myFile)
{
	ofstream File;
	File.open(myFile);
	File << "Name,posX,posY,posZ,minAABBx,minAABBy,minAABBz,maxAABBx,maxAABBy,maxAABBz,scaleX,scaleY,scaleZ,rotateAngle\n";
	for (list<EntityBase*>::iterator it = EntityManager::GetInstance()->returnEntity().begin(); it != EntityManager::GetInstance()->returnEntity().end(); ++it)
	{
		GenericEntity* temp = (GenericEntity*)*it;
		File << temp->GetName() << ","
			<< temp->GetPosition().x << ","
			<< temp->GetPosition().y << ","
			<< temp->GetPosition().z << ","
			<< temp->GetMinAABB().x << ","
			<< temp->GetMinAABB().y << ","
			<< temp->GetMinAABB().z << ","
			<< temp->GetMaxAABB().x << ","
			<< temp->GetMaxAABB().y << ","
			<< temp->GetMaxAABB().z << ","
			<< temp->GetScale().x << ","
			<< temp->GetScale().y << ","
			<< temp->GetScale().z << ","
			<< temp->GetRotate() << "\n";
	}
}