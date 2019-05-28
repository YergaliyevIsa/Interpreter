#ifndef _SYNTAXH_
#define _SYNTAXH_
#include <iostream>
//#include <stack>
#include <vector>
#include "lexical.h"
/*
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
*/
//using namespace std;
//enum Operations {Plus, Minus, Mul, Div, Umin, Left, Right };
//int priority[] ={10, 10, 11, 11, 12, -1, -1};
//char Oper_symbol[] = {'+', '-', '*', '/', '-', '(', ')' };

struct ComputeElem
{
	lex_type type;
	string name;
	int value;
};

int Compute_Poliz(vector <Poliz_Elem*> &MyPoliz, VarTable &vat_list, int &cur, if_and_while &Table, ArrayTable &arr_table);

#endif
