#pragma once
#include "Globals.hpp"

/*Represents a set of assembly instructions in memory. Stores the instructions before NOP'ing them so they can be easily restored.*/
class CodeSnippet
{
public:
	CodeSnippet() { } 
	CodeSnippet(DWORD Address_, DWORD Length_) : Address(Address_), Length(Length_) { }

	DWORD Address;
	DWORD Length;
	std::vector <int> Opcodes;
};

/*Manages NOP'ing and restoring parts of the games code for situations when hooking isn't enough.*/
class SnippetManager
{
public:
	static std::map <std::string, CodeSnippet> SnippetCache;

	static void BackupSnippet(std::string Name, DWORD Address, DWORD Length, bool NOP);
	static void RestoreSnippet(std::string Name, bool RemoveFromCache);
	static void NOPSnippet(std::string Name);

private:
	SnippetManager() { };
};

