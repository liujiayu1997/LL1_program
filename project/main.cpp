#include "LL1.h";

map<string, set<char> > first;
map<string, set<char> > follow;
map<string, int> VN_dic;
vector<WF> VN_set;
vector<map<char, string> > predict_table;
bool used[MAX];

int main ( )
{
	
    FILE *fp1 = fopen("C:\\Users\\12559\\Desktop\\read1.txt", "r");
    if( fp1==nullptr )
    {
        cout << "Open input file error!" << endl;
        exit( -1 );
    }
    FILE *fp2 = fopen("C:\\Users\\12559\\Desktop\\read2.txt", "a+");
    if( fp2==nullptr )
    {
        cout << "Open output file error!" << endl;
        exit( -1 );
    }
	make_VN(fp1);
    print(fp2);
	remove(fp2);
    make_first(fp2);
    make_follow(fp2);
    make_table(fp2);
    fclose(fp1);
	fclose(fp2);
    return 0;
}
