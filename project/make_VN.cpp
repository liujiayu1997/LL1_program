#include "LL1.h";

void make_VN(FILE *file)
{
	VN_set.reserve(10);
	int n;
	char s[MAX];
	while (~fscanf(file, "%d", &n))
	{
		for (int i = 0; i < n; i++)
		{
			fscanf(file, "%s", &s);
			int len = strlen(s), j;
			for (j = 0; j < len; j++)
				if (s[j] == '-') break;
			s[j] = 0;
			if (!VN_dic[s])
			{
				VN_set.push_back(s);
				VN_dic[s] = VN_set.size();
			}
			int x = VN_dic[s] - 1;
			VN_set[x].insert(s + j + 2);
		}
	}
}