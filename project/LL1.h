#ifndef _LL1_h_
#define _LL1_h_
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <set>
#include <fstream>
#define MAX 507
using namespace std;

//大写字母为非终止符(可以多一个'的标号做区分)，小写字母为终止符,用~代替epsilon
class WF
{
public:
	string left;
	set<string> right;
	WF(char s[])
	{
		left = s;
	}
	void print()
	{
		printf("%s->", left.c_str());
		set<string>::iterator it = right.begin();
		if (right.begin() != right.end())
		{
			printf("%s", it->c_str());
			it++;
		}
		for (; it != right.end(); it++)
			printf("|%s", it->c_str());
		puts("");
	}
	void insert(char s[])
	{
		right.insert(s);
	}
};

extern map<string, set<char> > first;
extern map<string, set<char> > follow;
extern map<string, int> VN_dic;
extern vector<WF> VN_set;
extern vector<map<char, string> > predict_table;
extern bool used[MAX];

void dfs(int x);
void make_first(FILE*file);
void make_follow(FILE*file);
void append(const string& str1, const string& str2);
bool check_first(const string& text, char ch);
bool check_follow(const string& text, char ch);
void make_table(FILE*file);
void remove(FILE *file);
void print(FILE *file);
void make_VN(FILE *file);


#endif

