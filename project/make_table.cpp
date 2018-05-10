#include "LL1.h";

void make_table(FILE*file)
{
	map<char, string> temp;
	vector<char> letter;
	bool vis[500];
	memset(vis, 0, sizeof(vis));
	for (int i = 0; i < VN_set.size(); i++)
	{
		set<string>& right = VN_set[i].right;
		set<string>::iterator it = right.begin();
		for (; it != right.end(); it++)
			for (int j = 0; j < it->length(); j++)
				if (!isupper(it->at(j)) && it->at(j) != '\'')
				{
					if (vis[it->at(j)]) continue;
					vis[it->at(j)] = true;
					letter.push_back(it->at(j));
				}
	}
	for (int i = 0; i < VN_set.size(); i++)
	{
		temp.clear();
		string& left = VN_set[i].left;
		set<string>& right = VN_set[i].right;
		set<string>::iterator it = right.begin();
		for (; it != right.end(); it++)
			for (int j = 0; j < letter.size(); j++)
			{
				//cout << *it << " " <<  letter[j] << endl;
				if (check_first(*it, letter[j]))
				{
					//cout << "YES" << endl;
					temp[letter[j]] = *it;
				}
				if (it->at(0) == '~' && check_follow(left, letter[j]))
					temp[letter[j]] = *it;
			}
		predict_table.push_back(temp);
	}
#define DEBUG
#ifdef DEBUG
	for (int i = 0; i <= (letter.size() + 1) * 10; i++)
		fprintf(file, "-");
	fprintf(file, "\n");
	fputs("", file);
	fprintf(file, "|%9s", "|");
	for (int i = 0; i < letter.size(); i++)
		fprintf(file, "%5c%5s", letter[i], "|");
	fprintf(file, "\n");
	fputs("", file);
	for (int i = 0; i <= (letter.size() + 1) * 10; i++)
		fprintf(file, "-");
	fprintf(file, "\n");
	fputs("", file);
	for (int i = 0; i < VN_set.size(); i++)
	{
		fprintf(file, "|%5s%4s", VN_set[i].left.c_str(), "|");
		for (int j = 0; j < letter.size(); j++)
			if (predict_table[i].count(letter[j]))
				fprintf(file, "%7s%3s", predict_table[i][letter[j]].c_str(), "|");
			else fprintf(file, "%10s", "|");
			fprintf(file, "\n");
			fputs("", file);
			for (int i = 0; i <= (letter.size() + 1) * 10; i++)
				fprintf(file, "-");
			fprintf(file, "\n");
			fputs("", file);
	}
#endif
}