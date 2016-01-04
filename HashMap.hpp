#include <string>

class Field
{
public:
	std::string name;
	int type;
	/* 0 for integer, 1 for float, 2 for char, 3 for string. */
	union
	{
		int i;
		float f;
		char c;
	};
	std::string str;
	Field* next;

	Field();
	Field(std::string name);
	Field(std::string name, int i);
	Field(std::string name, float f);
	Field(std::string name, char c);
	Field(std::string name, std::string str);
};

class HashMap
{
private:
	Field** T;
	int size;
	
public:
	HashMap* father;

	HashMap(int size);
	/* Parameter is the size of hash table. */

	~HashMap();
	/* Destructor. */

	Field* Find(std::string name);
	/* Parameter is the name of the varieble. NULL for not found. */

	bool Insert(Field f);
	/* 1 for success, 0 for record existence or other error. */

	bool Modify(std::string name, Field f);
	/* 1 for success, 0 for record inexistence or other error. */
};