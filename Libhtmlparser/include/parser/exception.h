#pragma once
#include <string>
#include <stdexcept>

class Exception
{
private:
	std::string name, info;
protected:
	Exception(std::string _name, std::string _info) : name(_name), info(_info) { }
public:
	virtual std::string& getName() { return name; }
	virtual std::string& getInfo() { return info; }
};