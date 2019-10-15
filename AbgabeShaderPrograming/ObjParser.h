#ifndef _OBJPARSER_H_
#define _OBJPARSER_H_

#include "ObjModel.h"

using namespace std;
using namespace DirectX;

class ObjParser:public ObjModel
{
public:
	ObjParser(LPCTSTR filename);
	~ObjParser();

protected:
	bool CreateVertexArray();
	bool CreateIndexArray();
	bool ParseObjFile();

	LPCTSTR filename;

	struct FacePoint
	{
		UINT position;
		UINT uv;
	};
};
#endif