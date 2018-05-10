#include "LL1.h";

void print(FILE *file)
{
	fputs("**********THE ORIGIN EXPERSSION********\n", file);
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
}