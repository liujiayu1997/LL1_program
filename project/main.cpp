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
    WF ( char s[] )
    {
        left = s;
    }
    void print ( )
    {
        printf ( "%s->" , left.c_str() );
        set<string>::iterator it = right.begin();
        if ( right.begin()!= right.end() )
        {
            printf ( "%s" , it->c_str() );
            it++;
        }
        for(; it != right.end() ; it++ )
            printf ( "|%s" , it->c_str() );
        puts("");
    }
    void insert ( char s[] )
    {
        right.insert ( s );
    }
};

map<string,set<char> > first;
map<string,set<char> > follow;
map<string,int> VN_dic;
vector<WF> VN_set;
bool used[MAX];

void dfs ( int x )
{
    if ( used[x] ) return;
    used[x] = 1;
    string& left = VN_set[x].left;
    set<string>& right = VN_set[x].right;
    set<string>::iterator it = right.begin();
    for ( ; it!= right.end() ; it++ )
        for ( int i = 0 ; i < it->length() ; i++ )
        {
            if ( !isupper( it->at(i) ) && it->at(i) != '\'' )
            {
                first[left].insert ( it->at(i) );
                break;
            }
            if ( isupper( it->at(i) ) )
            {
                int y;
                if ( i != it->length()-1 && it->at(i+1) == '\'' )
                    y = VN_dic[it->substr(i,2)]-1;
                else y = VN_dic[it->substr(i,1)]-1;
                string& tleft = VN_set[y].left;
                dfs ( y );
                set<char>& temp = first[tleft];
                set<char>::iterator it1 = temp.begin();
                bool flag = true;
                for ( ; it1 != temp.end() ; it1++ )
                {
                    if ( *it1 == '~' ) flag = false;
                    first[left].insert( *it1 );
                }
                if ( flag ) break;
            }
            else continue;
        }
}

void make_first (FILE*file )
{
    memset ( used , 0 , sizeof ( used ) );
    for ( int i = 0 ; i < VN_set.size() ; i++ )
        dfs ( i );
#define DEBUG
#ifdef DEBUG
        fputs ("***************FIRST SET***********************\n",file);
        map<string,set<char> >::iterator it = first.begin();
        for ( ; it != first.end() ; it++ )
        {
            fprintf ( file,"FIRST(%s)={" , it->first.c_str() );
            set<char> & temp = it->second;
            set<char>::iterator it1 = temp.begin();
            bool flag = false;
            for ( ; it1 != temp.end() ; it1++ )
            {
                if ( flag ) fprintf ( file,"," );
                fprintf ( file,"%c" , *it1 );
                flag = true;
            }
            fputs ("}\n",file);
        }
#endif
}

void append ( const string& str1 , const string& str2 )
{
    set<char>& from = follow[str1];
    set<char>& to = follow[str2];
    set<char>::iterator it = from.begin();
    for ( ; it != from.end() ; it++ )
        to.insert ( *it );
}

void make_follow ( FILE*file)
{
    while ( true )
    {
        bool goon = false;
        for ( int i = 0 ; i < VN_set.size() ; i++ )
        {
            string& left = VN_set[i].left;
            set<string>& right = VN_set[i].right;
            set<string>::iterator it = right.begin();
            for ( ; it!= right.end() ; it++ )
            {
                bool flag = true;
                const string& str = *it;
                for ( int j = it->length()-1 ; j >= 0 ; j-- )
                {
                    if ( str[j] == '\'' )
                    {
                        int x = VN_dic[it->substr(j-1,2)]-1;
                        if ( flag )
                        {
                            int tt = follow[it->substr(j-1,2)].size();
                            append ( left , it->substr(j-1,2) );
                            int tt1 = follow[it->substr(j-1,2)].size();
                            if ( tt1 > tt ) goon = true;
                            if ( !VN_set[x].right.count("~" ) )
                                flag = false;
                        }
                        for ( int k = j+1 ; k < it->length() ; k++ )
                        {
                            if ( isupper(str[k]) )
                            {
                                string id;
                                if ( k != it->length()-1 && str[k+1] == '\'' )
                                    id = it->substr(k,2);
                                else id = it->substr(k,1);
                                set<char>& from = first[id];
                                set<char>& to = follow[it->substr(j-1,2)];
                                int tt = to.size();
                                set<char>::iterator it1 = from.begin();
                                for ( ; it1 != from.end() ; it1++ )
                                    if ( *it1 != '~' )
                                        to.insert ( *it1 );
                                int tt1 = follow[it->substr(j-1,2)].size();
                                if ( tt1 > tt ) goon = true;
                                if ( !VN_set[VN_dic[id]-1].right.count("~") )
                                    break;
                            }
                            else if ( str[k] != '\'' )
                            {
                                int tt = follow[it->substr(j-1,2)].size();
                                follow[it->substr(j-1,2)].insert ( str[k] );
                                int tt1 = follow[it->substr(j-1,2)].size();
                                if ( tt1 > tt )
                                    goon = true;
                                break;
                            }
                            else continue;
                        }
                        j--;
                    }
                    else if ( isupper(str[j] ) )
                    {
                        int x = VN_dic[it->substr(j,1)]-1;
                        if ( flag )
                        {
                            int tt = follow[it->substr(j,1)].size();
                            append ( left , it->substr(j,1) );
                            if ( !VN_set[x].right.count("~") )
                                flag = false;
                            int tt1 = follow[it->substr(j,1)].size();
                            if ( tt1 > tt ) goon = true;
                        }
                        for ( int k = j+1 ; k < it->length() ; k++ )
                        {
                            if ( isupper( str[k] ) )
                            {
                                string id;
                                if ( k != it->length()-1 && str[k+1] == '\'' )
                                    id = it->substr(k,2);
                                else id = it->substr(k,1);
                                set<char>& from = first[id];
                                set<char>& to = follow[it->substr(j,1)];
                                set<char>::iterator it1 = from.begin();
                                int tt = follow[it->substr(j,1)].size();
                                for ( ; it1 != from.end() ; it1++ )
                                    if ( *it1 != '~' )
                                        to.insert( *it1 );
                                int tt1 = follow[it->substr(j,1)].size();
                                if ( tt1 > tt ) goon = true;
                                if ( !VN_set[VN_dic[id]-1].right.count("~") )
                                    break;
                            }
                            else if ( str[k] != '\'' )
                            {
                                int tt = follow[it->substr(j,1)].size();
                                follow[it->substr(j,1)].insert ( str[k] );
                                int tt1 = follow[it->substr(j,1)].size();
                                if ( tt1 > tt ) goon = true;
                                break;
                            }
                            else continue;
                        }
                    }
                    else flag = false;
                }
            }
        }
        if ( !goon ) break;
    }
#define DEBUG
#ifdef DEBUG
    fputs ("****************FOLLOW SET**********************\n",file );
    map<string,set<char> >::iterator it = follow.begin();
    for ( ; it != follow.end() ; it++ )
    {
        fprintf ( file,"FOLLOW(%s)={" , it->first.c_str() );
        set<char> & temp = it->second;
        temp.insert('#');
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for ( ; it1 != temp.end() ; it1++ )
        {
            if ( flag ) fprintf ( file,"," );
            fprintf ( file,"%c" , *it1 );
            flag = true;
        }
        fputs ("}\n",file);
    }
#endif
}

vector<map<char,string> > predict_table;

//检查一个字符是否属于一个字符串的FIRST集合
bool check_first ( const string& text , char ch )
{
    for ( int i = 0 ; i < text.length() ; i++ )
    {
        bool hasEmpty = false;
        if ( !isupper(text[i]) && text[i] != '\'' )
        {
            if ( text[i] != ch ) return false;
            else return true;
        }
        else if ( isupper(text[i] ) )
        {
            string temp;
            if ( i != text.length()-1 && text[i+1] == '\'' )
                temp = text.substr(i,2);
            else
                temp = text.substr(i,1);
            set<char>& dic = first[temp];
            set<char>::iterator it = dic.begin();
            for ( ; it != dic.end() ; it++ )
            {
                if ( *it == '~' ) hasEmpty = true;
                if ( *it == ch ) return true;
            }
            if ( !hasEmpty) break;
        }
        else continue;
    }
    return false;
}

//检查一个字符是否属于一个字符串的FOLLOW集合
bool check_follow ( const string& text , char ch )
{
    set<char>& dic = follow[text];
    set<char>::iterator it = dic.begin();
    for ( ; it != dic.end() ; it++ )
        if ( *it == ch ) return true;
    return false;
}

void make_table (FILE*file)
{
    map<char,string> temp;
    vector<char> letter;
    bool vis[500];
    memset ( vis , 0 , sizeof ( vis ) );
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        set<string>& right = VN_set[i].right;
        set<string>::iterator it = right.begin();
        for ( ; it != right.end() ; it++ )
            for ( int j = 0 ; j < it->length() ; j++ )
                if ( !isupper(it->at(j)) && it->at(j) != '\'' )
                {
                    if ( vis[it->at(j)] ) continue;
                    vis[it->at(j)] = true;
                    letter.push_back ( it->at(j) );
                }
    }
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        temp.clear();
        string& left = VN_set[i].left;
        set<string>& right = VN_set[i].right;
        set<string>::iterator it = right.begin();
        for ( ; it != right.end() ; it++ )
            for ( int j = 0 ; j < letter.size() ; j++ )
            {
                //cout << *it << " " <<  letter[j] << endl;
                if ( check_first ( *it , letter[j] ) )
                {
                    //cout << "YES" << endl;
                    temp[letter[j]] = *it;
                }
                if ( it->at(0) == '~' && check_follow ( left, letter[j] ))
                    temp[letter[j]] = *it;
            }
        predict_table.push_back ( temp );
    }
#define DEBUG
#ifdef DEBUG
    for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
        fprintf (file, "-" );
	fprintf(file, "\n");
    fputs ("",file);
    fprintf ( file,"|%9s" , "|" );
    for ( int i = 0 ; i < letter.size() ; i++ )
        fprintf ( file,"%5c%5s" , letter[i] , "|" );
	fprintf(file, "\n");
    fputs("",file);
    for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
        fprintf ( file,"-" );
	fprintf(file, "\n");
    fputs("",file);
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        fprintf ( file,"|%5s%4s" , VN_set[i].left.c_str() , "|" );
        for ( int j = 0 ; j < letter.size() ; j ++ )
            if ( predict_table[i].count(letter[j] ) )
                fprintf (file, "%7s%3s" , predict_table[i][letter[j]].c_str() , "|" );
            else fprintf (file, "%10s" , "|" );
		fprintf(file, "\n");
        fputs("",file);
        for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
            fprintf ( file,"-" );
		fprintf(file, "\n");
        fputs ("",file);
    }
#endif
}

//消除直接左递归
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
				VN_set.push_back( s );
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
	fputs("**********RESULT OF ELIMINATING LEFT RECURSION********\n",file);
	for (int i = 0; i < VN_set.size(); i++)
	{
		VN_set[i].print();
		fprintf(file, "%s->", VN_set[i].left.c_str() );
		set<string>::iterator it = VN_set[i].right.begin();
		if (VN_set[i].right.begin() != VN_set[i].right.end())
		{
			fprintf(file ,"%s", it->c_str());
			it++;
		}
		for (; it != VN_set[i].right.end(); it++)
			fprintf(file ,"|%s", it->c_str());
		fprintf(file, "\n");
	}
#endif
}
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

int main ( )
{
	VN_set.reserve(10);
    FILE *fp = fopen("C:\\Users\\12559\\Desktop\\read1.txt", "r");
    if( fp==nullptr )
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
    int n;
    char s[MAX];
    while ( ~fscanf ( fp,"%d" , &n ) )
    {
        for ( int i = 0 ; i < n ; i++ )
        {
            fscanf(fp, "%s", &s);
            int len = strlen ( s ),j;
            for ( j = 0 ; j < len ; j++ )
                if ( s[j] == '-' ) break;
            s[j] = 0;
            if ( !VN_dic[s] )
            {
                VN_set.push_back ( s );
                VN_dic[s] = VN_set.size();
            }
            int x = VN_dic[s]-1;
            VN_set[x].insert ( s+j+2 );
        }
		print(fp2);
		remove(fp2);
        make_first(fp2);
        make_follow(fp2);
        make_table(fp2);
    }
    fclose(fp);
	fclose(fp2);
    return 0;
}
