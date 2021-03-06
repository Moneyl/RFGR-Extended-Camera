#include "SnippetManager.hpp"

std::map <std::string, CodeSnippet> SnippetManager::SnippetCache;

void SnippetManager::BackupSnippet(std::string Name, DWORD Address, DWORD Length, bool NOP)
{
	BYTE* ByteAddress = (BYTE*)Address;
	DWORD OriginalProtectionPermissions;
	DWORD Backup;
	SnippetCache[Name] = CodeSnippet(Address, Length);

	VirtualProtect(ByteAddress, Length, PAGE_EXECUTE_READWRITE, &OriginalProtectionPermissions);
	for (DWORD i = 0x0; i < Length; i++)
	{
		SnippetCache[Name].Opcodes.push_back(*(ByteAddress + i));
		if (NOP)
		{
			*(ByteAddress + i) = 0x90;
		}
	}
	VirtualProtect(ByteAddress, Length, OriginalProtectionPermissions, &Backup); //Todo: Try using NULL or nullptr instead of Backup, since it's not needed.
}

void SnippetManager::RestoreSnippet(std::string Name, bool RemoveFromCache)
{
	BYTE* ByteAddress = (BYTE*)SnippetCache[Name].Address;
	DWORD Length = SnippetCache[Name].Length;
	DWORD OriginalProtectionPermissions;
	DWORD Backup;

	VirtualProtect(ByteAddress, Length, PAGE_EXECUTE_READWRITE, &OriginalProtectionPermissions);
	for (DWORD i = 0x0; i < Length; i++)
	{
		*(ByteAddress + i) = SnippetCache[Name].Opcodes[i];
	}
	VirtualProtect(ByteAddress, Length, OriginalProtectionPermissions, &Backup); //Todo: Try using NULL or nullptr instead of Backup, since it's not needed.
}

void SnippetManager::NOPSnippet(std::string Name)
{
	BYTE* ByteAddress = (BYTE*)SnippetCache[Name].Address;
	DWORD Length = SnippetCache[Name].Length;
	DWORD OriginalProtectionPermissions;
	DWORD Backup;

	VirtualProtect(ByteAddress, Length, PAGE_EXECUTE_READWRITE, &OriginalProtectionPermissions);
	for (DWORD i = 0x0; i < Length; i++)
	{
		*(ByteAddress + i) = 0x90;
	}
	VirtualProtect(ByteAddress, Length, OriginalProtectionPermissions, &Backup); //Todo: Try using NULL or nullptr instead of Backup, since it's not needed.
}