#include "LL1.h";

void dfs(int x)
{
	if (used[x]) return;
	used[x] = 1;
	string& left = VN_set[x].left;
	set<string>& right = VN_set[x].right;
	set<string>::iterator it = right.begin();
	for (; it != right.end(); it++)
		for (int i = 0; i < it->length(); i++)
		{
			if (!isupper(it->at(i)) && it->at(i) != '\'')
			{
				first[left].insert(it->at(i));
				break;
			}
			if (isupper(it->at(i)))
			{
				int y;
				if (i != it->length() - 1 && it->at(i + 1) == '\'')
					y = VN_dic[it->substr(i, 2)] - 1;
				else y = VN_dic[it->substr(i, 1)] - 1;
				string& tleft = VN_set[y].left;
				dfs(y);
				set<char>& temp = first[tleft];
				set<char>::iterator it1 = temp.begin();
				bool flag = true;
				for (; it1 != temp.end(); it1++)
				{
					if (*it1 == '~') flag = false;
					first[left].insert(*it1);
				}
				if (flag) break;
			}
			else continue;
		}
}