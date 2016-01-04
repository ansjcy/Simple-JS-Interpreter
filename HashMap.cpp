#include "HashMap.hpp"

Field::Field() : type(-1), next(NULL) {}

Field::Field(std::string name) : name(name), type(10), next(NULL) {}

Field::Field(std::string name, int i) : name(name), type(0), i(i), next(NULL) {}

Field::Field(std::string name, float f) : name(name), type(1), f(f), next(NULL) {}

Field::Field(std::string name, char c) : name(name), type(2), c(c), next(NULL) {}

Field::Field(std::string name, std::string str) : name(name), type(3), str(str), next(NULL) {}

void Deposite(Field* p)
{
	if (p != NULL)
		Deposite(p->next);
	delete p;
}

HashMap::HashMap(int size) : size(size), father(NULL)
{
	T = new Field*[size];
	for (int i = 0; i < size; i++)
		T[i] = NULL;
}

HashMap::~HashMap()
{
	for (int i = 0; i < size; i++)
		Deposite(T[i]);
	delete T;
}

Field* HashMap::Find(std::string name)
{
	int key = 0;
	for (int i = 0; i < name.length() && i < 20; i++)
		key += (name[i] - '0');
	key %= size;
	Field* p = T[key];
	while (p != NULL)
	{
		if (p->name == name)
			return p;
		p = p->next;
	}
	return NULL;
}

bool HashMap::Insert(Field f)
{
	int key = 0;
	for (int i = 0; i < f.name.length() && i < 20; i++)
		key += (f.name[i] - '0');
	key %= size;
	if (T[key] == NULL)
	{
		T[key] = new Field(f);
		return true;
	}
	Field* p = T[key];
	while (1)
	{
		if (p->name == f.name)
			return false;
		if (p->next != NULL)
			p = p->next;
		else
			break;
	}
	p->next = new Field(f);
	//p->next->next = NULL;
	return true;
}

bool HashMap::Modify(std::string name, Field f)
{
	Field* p = Find(name);
	if (p == NULL)
		return false;
	Field* q = p->next;
	*p = f;
	p->next = q;
	return true;
}