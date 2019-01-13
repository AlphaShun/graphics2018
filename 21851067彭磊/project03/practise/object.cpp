#include"object.h"

Object::Object(const char* path)
{
	FILE *file;
	fopen_s(&file, path, "rb");
	if (!file)
	{
		std::cout << "read obj file fail" << std::endl;
	}
	char line[1024];
	while (!feof(file))
	{
		fgets(line, 1024, file);
		std::vector<string> parts;
		std::vector<vec3> tempNors;
		std::vector<vec2> tempTex;
		std::string strLine = line;
		std::string endMark = " ";
		std::string ans = "";
		strLine = strLine.append(endMark);
		for (int i = 0; i < strLine.size(); i++)
		{
			if (strLine[i] != ' ')
				ans += strLine[i];
			else
			{
				parts.push_back(ans);
				ans = "";
			}
		}
		if (parts[0] == "v")
		{
			Vertex vertex;
			vertex.Position = vec3 ( atof(parts[1].c_str()), atof(parts[2].c_str()), atof(parts[3].c_str()));
			vertices.push_back(vertex);
		}
		if (parts[0] == "vn")
		{
			vec3 normal(atof(parts[1].c_str()), atof(parts[2].c_str()), atof(parts[3].c_str()));
			tempNors.push_back(normal);
		}
		if (parts[0] == "vt")
		{
			vec2 textCood(atof(parts[1].c_str()), atof(parts[2].c_str()));
			tempTex.push_back(textCood);
		}
		if (parts[0] == "f")
		{
			unsigned short indexs[4];
			unsigned short tempTexts[4] = { 0 };
			unsigned short tempNormals[4] = { 0 };
			for (int i = 1; i < 4; i++)
			{
				string s = parts[i];
				string ans1 = "";
				int sw = 1;
				for (int j = 0; j < s.size(); j++)
				{
					
					if (s[j] != '/')
					{
						ans1 += s[j];
					}
					else
					{
						if (ans1 != "")
						{
							if (sw == 1)
								indexs[i] = atof(ans1.c_str()) - 1;
							else if (sw == 2)
							{
								tempTexts[i] = atof(ans1.c_str()) - 1;
							}
							else
							{
								tempNormals[i] = atof(ans1.c_str()) - 1;
							}
						}
						sw++;
						ans1 = "";
					}
				}
			}
			indices.push_back(vec3u(indexs[1], indexs[2], indexs[3]));
			for (int i = 1; i < 4; i++)
			{
				if(tempNormals[i] != -1)
					vertices[indexs[i]].Normal = tempNors[tempNormals[i]];
				if(tempTexts[i] != -1)
					vertices[indexs[i]].TexCoords = tempTex[tempTexts[i]];
			}
		}
	}

}