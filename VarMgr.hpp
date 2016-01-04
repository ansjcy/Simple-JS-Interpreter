#include "HashMap.hpp"

class VarMgr
{
private:
	HashMap* currentMap;
	int tableSize;
public:
	VarMgr(int size = 500);
	/* Parameter is the size of hash table. */

	~VarMgr();
	/* Destructor. */

	bool NewFrame();
	/* Open a new frame, create a new hash table. */

	bool Return();
	/* Return the outer frame, delete the hash table. 0 for no frame to return. */

	int GetType(std::string name);
	/* Get the type of the varible. -1 for not existing, 0 for integer, 1 for float, 2 for char, 3 for string, 10 for NULL */

	Field GetValue(std::string name);
	/* Get the value of the varible. (name == "") for varible not found. */

	bool NewVar(std::string name);
	bool NewVar(std::string name, int i);
	bool NewVar(std::string name, float f);
	bool NewVar(std::string name, char c);
	bool NewVar(std::string name, std::string str);
	/* Create a new varible. 1 for success, 0 for record existence or other error. */

	bool ModifyVar(std::string name, int i);
	bool ModifyVar(std::string name, float f);
	bool ModifyVar(std::string name, char c);
	bool ModifyVar(std::string name, std::string str);
	/* Change the varible. 1 for success, 0 for record inexistence or other error. */
};