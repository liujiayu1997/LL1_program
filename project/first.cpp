#include "LL1.h";

void make_first(FILE*file)
{
	memset(used, 0, sizeof(used));
	for (int i = 0; i < VN_set.size(); i++)
		dfs(i);
#define DEBUG
#ifdef DEBUG
	fputs("***************FIRST SET***********************\n", file);
	map<string, set<char> >::iterator it = first.begin();
	for (; it != first.end(); it++)
	{
		fprintf(file, "FIRST(%s)={", it->first.c_str());
		set<char> & temp = it->second;
		set<char>::iterator it1 = temp.begin();
		bool flag = false;
		for (; it1 != temp.end(); it1++)
		{
			if (flag) fprintf(file, ",");
			fprintf(file, "%c", *it1);
			flag = true;
		}
		fputs("}\n", file);
	}
#endif
}

bool check_first(const string& text, char ch)
{
	for (int i = 0; i < text.length(); i++)
	{
		bool hasEmpty = false;
		if (!isupper(text[i]) && text[i] != '\'')
		{
			if (text[i] != ch) return false;
			else return true;
		}
		else if (isupper(text[i]))
		{
			string temp;
			if (i != text.length() - 1 && text[i + 1] == '\'')
				temp = text.substr(i, 2);
			else
				temp = text.substr(i, 1);
			set<char>& dic = first[temp];
			set<char>::iterator it = dic.begin();
			for (; it != dic.end(); it++)
			{
				if (*it == '~') hasEmpty = true;
				if (*it == ch) return true;
			}
			if (!hasEmpty) break;
		}
		else continue;
	}
	return false;
}