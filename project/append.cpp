#include "LL1.h";

void append(const string& str1, const string& str2)
{
	set<char>& from = follow[str1];
	set<char>& to = follow[str2];
	set<char>::iterator it = from.begin();
	for (; it != from.end(); it++)
		to.insert(*it);
}