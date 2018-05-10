#include "LL1.h";

void make_follow(FILE*file)
{
	while (true)
	{
		bool goon = false;
		for (int i = 0; i < VN_set.size(); i++)
		{
			string& left = VN_set[i].left;
			set<string>& right = VN_set[i].right;
			set<string>::iterator it = right.begin();
			for (; it != right.end(); it++)
			{
				bool flag = true;
				const string& str = *it;
				for (int j = it->length() - 1; j >= 0; j--)
				{
					if (str[j] == '\'')
					{
						int x = VN_dic[it->substr(j - 1, 2)] - 1;
						if (flag)
						{
							int tt = follow[it->substr(j - 1, 2)].size();
							append(left, it->substr(j - 1, 2));
							int tt1 = follow[it->substr(j - 1, 2)].size();
							if (tt1 > tt) goon = true;
							if (!VN_set[x].right.count("~"))
								flag = false;
						}
						for (int k = j + 1; k < it->length(); k++)
						{
							if (isupper(str[k]))
							{
								string id;
								if (k != it->length() - 1 && str[k + 1] == '\'')
									id = it->substr(k, 2);
								else id = it->substr(k, 1);
								set<char>& from = first[id];
								set<char>& to = follow[it->substr(j - 1, 2)];
								int tt = to.size();
								set<char>::iterator it1 = from.begin();
								for (; it1 != from.end(); it1++)
									if (*it1 != '~')
										to.insert(*it1);
								int tt1 = follow[it->substr(j - 1, 2)].size();
								if (tt1 > tt) goon = true;
								if (!VN_set[VN_dic[id] - 1].right.count("~"))
									break;
							}
							else if (str[k] != '\'')
							{
								int tt = follow[it->substr(j - 1, 2)].size();
								follow[it->substr(j - 1, 2)].insert(str[k]);
								int tt1 = follow[it->substr(j - 1, 2)].size();
								if (tt1 > tt)
									goon = true;
								break;
							}
							else continue;
						}
						j--;
					}
					else if (isupper(str[j]))
					{
						int x = VN_dic[it->substr(j, 1)] - 1;
						if (flag)
						{
							int tt = follow[it->substr(j, 1)].size();
							append(left, it->substr(j, 1));
							if (!VN_set[x].right.count("~"))
								flag = false;
							int tt1 = follow[it->substr(j, 1)].size();
							if (tt1 > tt) goon = true;
						}
						for (int k = j + 1; k < it->length(); k++)
						{
							if (isupper(str[k]))
							{
								string id;
								if (k != it->length() - 1 && str[k + 1] == '\'')
									id = it->substr(k, 2);
								else id = it->substr(k, 1);
								set<char>& from = first[id];
								set<char>& to = follow[it->substr(j, 1)];
								set<char>::iterator it1 = from.begin();
								int tt = follow[it->substr(j, 1)].size();
								for (; it1 != from.end(); it1++)
									if (*it1 != '~')
										to.insert(*it1);
								int tt1 = follow[it->substr(j, 1)].size();
								if (tt1 > tt) goon = true;
								if (!VN_set[VN_dic[id] - 1].right.count("~"))
									break;
							}
							else if (str[k] != '\'')
							{
								int tt = follow[it->substr(j, 1)].size();
								follow[it->substr(j, 1)].insert(str[k]);
								int tt1 = follow[it->substr(j, 1)].size();
								if (tt1 > tt) goon = true;
								break;
							}
							else continue;
						}
					}
					else flag = false;
				}
			}
		}
		if (!goon) break;
	}
#define DEBUG
#ifdef DEBUG
	fputs("****************FOLLOW SET**********************\n", file);
	map<string, set<char> >::iterator it = follow.begin();
	for (; it != follow.end(); it++)
	{
		fprintf(file, "FOLLOW(%s)={", it->first.c_str());
		set<char> & temp = it->second;
		temp.insert('#');
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

//检查一个字符是否属于一个字符串的FOLLOW集合
bool check_follow(const string& text, char ch)
{
	set<char>& dic = follow[text];
	set<char>::iterator it = dic.begin();
	for (; it != dic.end(); it++)
		if (*it == ch) return true;
	return false;
}