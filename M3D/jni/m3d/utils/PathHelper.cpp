
#include "m3d/utils/PathHelper.h"
#include "m3d/utils/StringHelper.h"
#include "m3d/model/Model.h"
#include "m3d/scene/SceneNode.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;

namespace M3D{

//const int PathHelper::MVO_SMALL_BUFFER_SIZE = 255;

const string PathHelper::M3D_PATH_PRE ="M3D|MAIN|";

string PathHelper::GetSVLPath(Model* model)
{
	string svlPath;
	if(model)
	{
		if(model->GetSceneNode())
		{
			if(model->GetSceneNode()->GetParent())
			{
				const string m3dPath = model->GetSceneNode()->GetParent()->GetName();
				return PathHelper::M3DPathToSVLPath(m3dPath);
			}
		}
	}

	return svlPath;
}

string PathHelper::M3DHexPathToSVLDec(const string& hexPath)
{
	return PathHelper::SVLPathHexToDec(PathHelper::M3DPathToSVLPath(hexPath));
}

string SVLDecToM3DHexPath(const string& svlDecPath)
{
	return PathHelper::SVLPathToM3D(PathHelper::SVLPathDecToHex(svlDecPath));
}

string PathHelper::GetM3DPath(Model* model)
{
	if(model)
	{
		if(model->GetSceneNode())
		{
			if(model->GetSceneNode()->GetParent())
			{
				return model->GetSceneNode()->GetParent()->GetName();
			}
		}
	}

	return "";
}


string PathHelper::SVLPathHexToDec(const string& hexPath)
{
	char newPlcPath[256]={0};
	char *cPlcIdP = NULL;
	char srcPath[256]={0};
    char targetPath[256] ={0};
	int wiPlcId = 0;
	char *endptr;

	strcpy(srcPath,hexPath.c_str());
	cPlcIdP = strtok(srcPath,"|");
     cPlcIdP = strtok(NULL,"|");
	while(cPlcIdP !=NULL)
	{
		wiPlcId = strtoul(cPlcIdP, &endptr, 16);
		wiPlcId &= 0x00FFFFFF;
		if (strcmp(newPlcPath, "") == 0)
		{
			sprintf(targetPath, "%d", wiPlcId);
		}
		else
		{
			sprintf(targetPath, "%s|%d", newPlcPath, wiPlcId);
		}

		strcpy(newPlcPath,targetPath);
		cPlcIdP = strtok(NULL,"|");
	}
    string ret (newPlcPath);
    return "PATH|"+ret;
}

string PathHelper::SVLPathDecToHex(const string& decPPath)
{
    char newPlcPath[256]={0};
    char *cPlcIdP = NULL;
    char srcPath[256]={0};
    char targetPath[256] ={0};
	int wiPlcId = 0;
	char *endptr;

	strcpy(srcPath,decPPath.c_str());
	cPlcIdP = strtok(srcPath,"|");
    cPlcIdP = strtok(NULL,"|");
	while(cPlcIdP !=NULL)
	{
		wiPlcId = strtoul(cPlcIdP, &endptr, 10);
		wiPlcId &= 0x00FFFFFF;
		if (strcmp(newPlcPath, "") == 0)
		{
			sprintf(targetPath, "%x", wiPlcId);
		}
		else
		{
			sprintf(targetPath, "%s|%x", newPlcPath, wiPlcId);
		}

		strcpy(newPlcPath,targetPath);
		cPlcIdP = strtok(NULL,"|");
	}
    string ret (newPlcPath);
    return "PATH|"+ret;
}
string PathHelper::SVLPathToM3D(const string& svlPath)
{
	return M3D_PATH_PRE + svlPath;
}

string PathHelper::M3DPathToSVLPath(const string& m3dPath)
{
	if(m3dPath.size()>M3D_PATH_PRE.size()+1){
	return StringHelper::subString(m3dPath,M3D_PATH_PRE.size(),m3dPath.size());
	}
	return "";
}

string PathHelper::M3DPathToAnimation(const string& m3dPath)
{
	return "";
}

string PathHelper::AnimationPathToM3D(const string& animationPath)
{
    char newPlcPath[256] =
    { 0 };
    char *cPlcIdP = NULL;
    char srcPath[256] =
    { 0 };
    char targetPath[256] =
    { 0 };
    int wiPlcId = 0;
    
    strcpy(srcPath, animationPath.c_str());
    cPlcIdP = strtok(srcPath, "\\");
    while (cPlcIdP != NULL)
    {
        wiPlcId = strtoul(cPlcIdP, NULL, 10);
        wiPlcId &= 0x00FFFFFF;
        if (strcmp(newPlcPath, "") == 0)
        {
            sprintf(targetPath, "%x", wiPlcId);
        }
        else
        {
            sprintf(targetPath, "%s|%x", newPlcPath, wiPlcId);
        }
        
        strcpy(newPlcPath, targetPath);
        cPlcIdP = strtok(NULL, "\\");
    }
    
    if (strlen(newPlcPath) > 0 && newPlcPath[0] != '0')
    {
        sprintf(targetPath, "0|%s", newPlcPath);
    }
    
    string ret(targetPath);
    
    return PathHelper::M3D_PATH_PRE + "PATH|" + ret;

}

int PathHelper::GetPathLevel(const string& m3dPath)
{
	vector<string::size_type> findKeyVector = StringHelper::GetIndexList(
			PathHelper::M3DPathToSVLPath(m3dPath),
			"|");

	LOGE("path : %s",m3dPath.c_str());

	int level = findKeyVector.size();

	LOGE("level : %d",level);

	return level;
}

}
