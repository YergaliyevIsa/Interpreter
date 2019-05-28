#include <iostream>
//#include <stack>
#include <vector>
#include "syntax.h"
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

//using namespace std;
//enum Operations {Mov, Plus, Minus, Mul, Div, Umin, Left, Right, If };
//int priority[] ={10, 10, 11, 11, 12, -1, -1};
//char Oper_symbol[] = {'+', '-', '*', '/', '-', '(', ')', '?' };

ComputeElem Not_Oper(Poliz_Elem* &elem)
{
	ComputeElem tmp;
	if (elem -> type() == NUM) {
		tmp.value = ((Number*)elem) -> get_value();
		tmp.name = "";
		tmp.type = NUM;
	}
	else if (elem -> type() == VAR) {
		tmp.value = ((Var*)elem) -> get_value();
		tmp.name = ((Var*)elem) -> get_name();
		tmp.type = VAR;
	}
	else if (elem -> type() == ARRAY) {
		tmp.value = ((Array*)elem) -> get_size();
		tmp.name = ((Array*)elem) -> get_name();
		tmp.type = ARRAY;
	}
	return tmp;
}	

int Compute_Poliz(vector <Poliz_Elem*> &MyPoliz, VarTable &var_list, int &cur_str, if_and_while & Table, ArrayTable &arr_table)
{
	vector <ComputeElem> My_Stack;
	ComputeElem l_val, r_val;
	string err;
	int size, magic;
	bool dot = false;
	if ((MyPoliz.size() == 1) && (MyPoliz[0] -> type() == OPER)) {
		if (((Operator*)MyPoliz[0]) -> get_value() == Print) {
			cout << endl;
			return 0;
		}
		for (int i = 0; i < Table.while_lab.size(); i++) {
			if (cur_str == Table.while_lab[i].end - 1)
				cur_str = Table.while_lab[i].begin - 1;
		}
		return 0;
	}
	for (int i = 0; i < MyPoliz.size(); i++) {
		if (MyPoliz[i] -> type() != OPER) {
			My_Stack.push_back(Not_Oper(MyPoliz[i]));
		}
		else {
			ComputeElem tmp;
			tmp.name = "";
			tmp.type = NUM;
			size = My_Stack.size();
			Operations op = ((Operator*)MyPoliz[i]) -> get_value();
			if (op != Umin && op != If && op != While && op != Not && op != Print && op != Goto) {
				if (My_Stack.size() < 2) {
					err = "Syntax_err : Wrong number of lexem";
					throw err;
				}
				r_val = My_Stack[size - 1];
				l_val = My_Stack[size - 2];
				My_Stack.pop_back();
				My_Stack.pop_back();
			}
			else {
				if (My_Stack.size() < 1) {	
					err = "Syntax_err = Wrong number of lexem";
					throw err;
				}
				r_val = My_Stack[size - 1];
				My_Stack.pop_back();
			}
			switch (op) {	
				case Plus : 
				{
					if ((l_val.type == ARRAY) || (r_val.type == ARRAY)) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					tmp.value = l_val.value + r_val.value;
					My_Stack.push_back(tmp);
					break;
				}
				case Minus : 
				{ 
					if ((l_val.type == ARRAY) || (r_val.type == ARRAY)) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					tmp.value = l_val.value - r_val.value;
					My_Stack.push_back(tmp);
					break;
				}
				case Mul : 
				{
					if ((l_val.type == ARRAY) || (r_val.type == ARRAY)) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					tmp.value = l_val.value * r_val.value;
					My_Stack.push_back(tmp);
					break;
				}
				case Div : 
				{
					if ((l_val.type == ARRAY) || (r_val.type == ARRAY)) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					if (r_val.value == 0) {
						err = "Syntax_err = Divide by zero";
						throw err;
					}
					tmp.value = l_val.value / r_val.value;
					My_Stack.push_back(tmp);
					break;
				}
				case Mod :
				{
					if ((l_val.type == ARRAY) || (r_val.type == ARRAY)) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					if (r_val.value == 0) {
						err = "Syntax_err = Divide by zero";
						throw err;
					}
					tmp.value = l_val.value % r_val.value;
					My_Stack.push_back(tmp);
					break;
				}
				case Umin : 
				{ 
					if (r_val.type == ARRAY) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					tmp.value = -1 * r_val.value;
					My_Stack.push_back(tmp);
					break;
				}
				case Mov :
				{
					tmp.value = r_val.value;
					My_Stack.push_back(tmp);
					if  (MyPoliz[0] -> type() == VAR)
						var_list.set_value(((Var*)MyPoliz[0]) -> get_name(), My_Stack[0].value);
					else if (MyPoliz[0] -> type() == ARRAY)
						arr_table.set_val(((Array*)MyPoliz[0]) -> get_name(), magic, My_Stack[0].value);
					break;
				}
				case If:
				{
					if (r_val.type == ARRAY) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					if (r_val.value == 0) {
						for (int i = 0; i < Table.if_lab.size(); i++) {
							if (cur_str == Table.if_lab[i].begin) {
								cur_str = Table.if_lab[i].end - 1;
								break;
							}
						}
					}
					tmp.value = r_val.value;
					My_Stack.push_back(tmp);
					break;
				}
				case While:	
				{
					if (r_val.type == ARRAY) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					if (r_val.value == 0) {
						for (int i = 0; i < Table.while_lab.size(); i++) {
							if (cur_str == Table.while_lab[i].begin) {
								cur_str = Table.while_lab[i].end - 1;
								break;
							}
						}
					}
					tmp.value = r_val.value;
					My_Stack.push_back(tmp);
					break;
				}
				case Goto:
				{
					if (r_val.type == ARRAY) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					if (r_val.value < 0) {
						err = "Syntax_err = Wrong number of jump_line";
						throw err;
					}
					cur_str = r_val.value - 1;
					tmp.value = r_val.value;
					My_Stack.push_back(tmp);
					break;
				}
				case Not :
				{
					if (r_val.type == ARRAY) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					tmp.value = (!r_val.value);
					My_Stack.push_back(tmp);
					break;
				}
				case Equ:
				{
					if ((l_val.type == ARRAY) || (r_val.type == ARRAY)) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					tmp.value = (r_val.value == l_val.value);
					My_Stack.push_back(tmp);
					break;
				}
				case Print:
				{
					if (r_val.type == ARRAY) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					tmp.value = r_val.value;
					My_Stack.push_back(tmp);
					cout << r_val.value <<  ' ';
					break;
				}
				case Nequ:
				{
					if ((l_val.type == ARRAY) || (r_val.type == ARRAY)) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					tmp.value = (r_val.value != l_val.value);
					My_Stack.push_back(tmp);
					break;
				}
				case Great:
				{
					if ((l_val.type == ARRAY) || (r_val.type == ARRAY)) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					tmp.value = (l_val.value > r_val.value);
					My_Stack.push_back(tmp);
					break;
				}
				case Less:
				{
					if ((l_val.type == ARRAY) || (r_val.type == ARRAY)) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					tmp.value = (l_val.value < r_val.value);
					My_Stack.push_back(tmp);
					break;
				}
				case Egreat:
				{
					if ((l_val.type == ARRAY) || (r_val.type == ARRAY)) {
						err = "Syntax_err = Wrong operation";
						throw err;
					}
					tmp.value = (l_val.value >= r_val.value);
					My_Stack.push_back(tmp);
					break;
				}
				case Dot:
				{
					if (dot == false) {
						magic = r_val.value;
						dot = true;
					}
					tmp.value = arr_table.get_val(l_val.name, r_val.value);
					My_Stack.push_back(tmp);
					break;
				}

			}
		}
	}
//	var_list.print();
//	MyPoliz[0] -> print();
//	cout << endl;
	return My_Stack.at(0).value;
}


