#ifndef _LEXICALH_
#define _LEXICALH_
#include <iostream>
//#include <stack>
#include <vector>
#include <map>
using std::map;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

//using namespace std;
enum Operations {Mov, Plus, Minus, Mul, Div, Umin, Left, Right, Goto, If, While, Open, Close, 
				 	Not , Equ, Nequ, Great, Less, Egreat, Print, Dot, Mod};
enum lex_type {OPER, NUM, VAR, ARRAY};
//int priority[] ={5, 10, 10, 11, 11, 12, -1, -1, -1 ,0, 0, 0, 0};
//char Oper_symbol[] = {'=', '+', '-', '*', '/', '-', '(', ')', '$', '?','@', '{', '}' };

class Poliz_Elem
{
public:
	lex_type id;
	Poliz_Elem();
	virtual void print() { cout << "Hello" << endl; }
	virtual lex_type type() {}
};

class Number : public Poliz_Elem
{
	int value;
public:
	Number(int v = 0);
	int get_value();
	void set_value(int a);
	void print();
	lex_type type();
	virtual ~Number() {}
};

class Operator : public Poliz_Elem
{
	Operations value;
public:
	Operator(Operations val = Plus);
	Operations get_value();
	void set_value(Operations oper);
	void print();
	lex_type type();
	virtual ~Operator() {}
};

class Var : public Poliz_Elem
{
	string name;
	int value;
public :
	Var(const string Name = "", int val = 0, bool is_lab = false);
	bool label;
	void print();
	int get_value();
	string get_name();
	void set_name(const string &Name);
	void set_value(int val);
	lex_type type();
	virtual ~Var() {}
};	

class Array : public Poliz_Elem
{
	int size;
	string name;
	vector <int> arr;
public:
	Array(int len = 0, const string &Name = "");
	void print();
	lex_type type();
	int get_value(int i);
	void set_size(int len);
	int get_size();
	void set_value(int i, int val);
	string get_name();
	void set_name(const string &s);
	virtual ~Array() {}	
};

class ArrayTable
{
	vector <Array> Tab;
public:
	bool search(const string &s);
	bool add_elem(Array &arr);
	Array get_elem(const string &s);
	void set_val(const string &s, int index, int val);
	int get_size(const string &s);
	int get_val(const string &s, int index);
	void print();
};
class VarTable
{
	map <string, int> data;
public:
	bool add_elem(const string &s);
	bool check_elem(const string &s);
	int get_value(const string &s);
	void set_value(const string &s, int a);
	void print();
};

struct Brakets
{
	int begin, end;
	Brakets(int b = -1, int e = -1);
};

// 0 = while; 1 = if
struct if_and_while
{
	vector <Brakets> if_lab;
	vector <Brakets> while_lab;
	vector <bool> br_types;
	void add_begin(int a);
	void add_end(int a);
	void if_print();
	void while_print();
};

void intermediate_errors(vector <Poliz_Elem*> &MyPoliz, if_and_while &Table);

void print(vector <Poliz_Elem*> MyPoliz);

Poliz_Elem* get_lexem(string & express, int & i, int &cur, if_and_while &Table, ArrayTable &arr_table, VarTable &var_list);

void Make_Poliz(vector <Poliz_Elem*> &MyPoliz);
#endif
