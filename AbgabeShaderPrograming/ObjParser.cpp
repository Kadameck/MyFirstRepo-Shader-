#include "ObjParser.h"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <DirectXMath.h>


ObjParser::ObjParser(LPCTSTR _filename)
{
	filename = _filename;
}


ObjParser::~ObjParser()
{
}

bool ObjParser::CreateVertexArray()
{
	return ParseObjFile();
}

bool ObjParser::CreateIndexArray()
{
	return true;
}

bool ObjParser::ParseObjFile()
{
	ifstream file(filename);
	if (!file.is_open())
	{
		return false;
	}

	string line;
	vector<DirectX::XMFLOAT3> positions;
	vector<DirectX::XMFLOAT2> uvs;
	vector<vector<FacePoint>> faces;
	int actFace = 0;

	while (!file.eof())
	{
		getline(file, line);

		if (line.empty())
		{
			continue;
		}

		stringstream lineStream(line);
		getline(lineStream, line, ' ');

		if (line == "v")
		{
			float x, y, z;
			lineStream >> x >> y >> z;
			positions.push_back({ x, y, z });
		}
		else if (line == "vt")
		{
			float u, v;
			lineStream >> u >> v;
			uvs.push_back({ u, v });
		}
		else if (line == "f")
		{
			faces.push_back(vector<FacePoint>());
			do
			{
				UINT p, u;
				lineStream >> p;
				getline(lineStream, line, '/');
				lineStream >> u;
				faces[actFace].push_back({ p - 1, u - 1 });
			} while (getline(lineStream, line, ' ') && !lineStream.eof());
			vertexCount += faces[actFace].size();
			indexCount += (faces[actFace].size() - 2) * 3;
			++actFace;
		}
	}

	vertexArray = new Vertex[vertexCount];
	indexArray = new UINT[indexCount];

	int actVertex = 0;
	int actIndex = 0;
	int startIndex = 0;

	for (int f = 0; f < actFace; f++)
	{
		for (int v = 0; v < faces[f].size(); v++)
		{
			vertexArray[actVertex++] = { positions[faces[f][v].position],{ 1, 1, 1, 1 }, uvs[faces[f][v].uv] };
		}

		int actVertices = faces[f].size();

		for (int i = startIndex + actVertices - 1; i > startIndex + 1; i--)
		{
			indexArray[actIndex++] = startIndex;
			indexArray[actIndex++] = i - 1;
			indexArray[actIndex++] = i;
		}

		startIndex += actVertices;
	}

	return true;
}
