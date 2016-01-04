#include "VarMgr.hpp"

VarMgr::VarMgr(int size) : tableSize(size), currentMap(NULL) {}

VarMgr::~VarMgr()
{
	if (currentMap == NULL)
		return;
	else
	{
		HashMap* p = currentMap;
		HashMap* q;
		while (p != NULL)
		{
			q = p->father;
			delete p;
			p = q;
		}
	}
}

bool VarMgr::NewFrame()
{
	HashMap* p = currentMap;
	currentMap = new HashMap(tableSize);
	currentMap->father = p;
	return true;
}

bool VarMgr::Return()
{
	if (currentMap == NULL)
		return false;
	else
	{
		HashMap* p = currentMap->father;
		delete currentMap;
		currentMap = p;
		return true;
	}
}

int VarMgr::GetType(std::string name)
{
	Field* p = currentMap->Find(name);
	if (p == NULL)
		return -1;
	else
		return p->type;
}

Field VarMgr::GetValue(std::string name)
{
	Field* p = currentMap->Find(name);
	if (p == NULL)
		return Field("");
	return *p;
}

bool VarMgr::NewVar(std::string name)
{
	return currentMap->Insert(Field(name));
}

bool VarMgr::NewVar(std::string name, int i)
{
	return currentMap->Insert(Field(name, i));
}

bool VarMgr::NewVar(std::string name, float f)
{
	return currentMap->Insert(Field(name, f));
}

bool VarMgr::NewVar(std::string name, char c)
{
	return currentMap->Insert(Field(name, c));
}

bool VarMgr::NewVar(std::string name, std::string str)
{
	return currentMap->Insert(Field(name, str));
}

bool VarMgr::ModifyVar(std::string name, int i)
{
	return currentMap->Modify(name, Field(name, i));
}

bool VarMgr::ModifyVar(std::string name, float f)
{
	return currentMap->Modify(name, Field(name, f));
}

bool VarMgr::ModifyVar(std::string name, char c)
{
	return currentMap->Modify(name, Field(name, c));
}

bool VarMgr::ModifyVar(std::string name, std::string str)
{
	return currentMap->Modify(name, Field(name, str));
}

