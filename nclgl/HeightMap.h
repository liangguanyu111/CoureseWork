#pragma once
#include<string>
#include "../nclgl/Mesh.h"

class  HeightMap : public Mesh
{
public:
	 HeightMap(const std::string& name);
	 ~HeightMap(void) {};
	 Vector3 GetHeightMapSize() const {return heightmapSize;}

private:
protected:
	Vector3 heightmapSize;
};

