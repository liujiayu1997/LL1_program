#include "LL1.h";

//Ïû³ýÖ±½Ó×óµÝ¹é
void remove(FILE *file)
{
	for (int i = 0; i < VN_set.size(); i++)
	{
		char ch = VN_set[i].left[0];
		set<string>& temp = VN_set[i].right;
		set<string>::iterator it = temp.begin();
		string tt = VN_set[i].left.substr(0, 1) + "\'";
		char s[MAX];
		strcpy(s, tt.c_str());
		bool flag = true;
		for (; it != temp.end(); it++)
			if (it->at(0) == ch)
			{
				VN_set.push_back(s);
				VN_dic[s] = VN_set.size();
				flag = false;
				break;
			}
		int x = VN_dic[s] - 1;
		if (flag) continue;
		vector<string> cont;
		set<string>& ss = VN_set[x].right;
		ss.insert("~");
		while (!temp.empty())
		{
			if (temp.begin()->at(0) == ch)
				ss.insert(temp.begin()->substr(1) + tt);
			else
			{
				//cout << "YES : " << temp.begin()->substr(1)+tt;
				cont.push_back(temp.begin()->substr(0) + tt);
			}
			temp.erase(temp.begin());
		}
		puts("");
		for (int i = 0; i < cont.size(); i++)
		{
			//cout << cont[i] << endl;
			temp.insert(cont[i]);
		}
	}
#define DEBUG
#ifdef DEBUG
	fputs("**********RESULT OF ELIMINATING LEFT RECURSION********\n", file);
	for (int i = 0; i < VN_set.size(); i++)
	{
		VN_set[i].print();
		fprintf(file, "%s->", VN_set[i].left.c_str());
		set<string>::iterator it = VN_set[i].right.begin();
		if (VN_set[i].right.begin() != VN_set[i].right.end())
		{
			fprintf(file, "%s", it->c_str());
			it++;
		}
		for (; it != VN_set[i].right.end(); it++)
			fprintf(file, "|%s", it->c_str());
		fprintf(file, "\n");
	}
#endif
}