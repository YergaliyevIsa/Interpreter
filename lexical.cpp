#include <iostream>
//#include <stack>
#include <vector>
#include "lexical.h"
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

//using namespace std;
//enum Operations {Mov, Plus, Minus, Mul, Div, Umin, Left, Right, Goto, If, While, Open, Close 
//  Not, Equ, Nequ, Great, Less, Egreat, Print, Dot, Mod}															
int priority[] ={5, 10, 10, 11, 11, 12, -1, -1, 0, 0, 0, 0, 0, 12, 6, 6, 6, 6, 6, 0, 13, 11};
char Oper_symbol[] = {'=', '+', '-', '*', '/', '-', '(', ')', '$', '?', '@', '{', '}', '!', ';', ';', '>' , '<', ';', '#', '.', '%'};

Poliz_Elem::Poliz_Elem() 
{
	id = VAR;
}

Number::Number(int v)
{
	value = v;
}

int Number::get_value()
{
	return value;
}

void Number::set_value(int a)
{	
	value = a;
}

void Number::print()
{
	cout << value;
}

lex_type Number::type()
{
	return NUM;
}

Operator::Operator(Operations val)
{
	value = val;
}

lex_type Operator::type()
{
	return OPER;
}

Operations Operator::get_value()
{
	return value;
}

void Operator::set_value(Operations oper)
{	
	value = oper;
}

void Operator::print()
{
	if (value != Umin && value != Equ && value != Nequ && value != Egreat && value != Dot)
		cout << Oper_symbol[value];
	else if (value == Umin)
		cout << '~';
	else if (value == Equ)
		cout << "==";
	else if (value == Nequ)
		cout << "!=";
	else if (value == Egreat)
		cout << ">=";
	else if (value == Dot)
		cout << '.';
}
Var::Var(const string Name, int val, bool is_lab)
{
	name = Name;
	value = val;
	label = is_lab;
}

void Var::print()
{
	cout << '('<< name << ", " << value << ')';
}

int Var::get_value()
{
	return value;
}
string Var::get_name()
{
	return name;
}
void Var::set_value(int val)
{
	value = val;
}
void Var::set_name(const string &Name)
{
	name = Name;
}
lex_type Var::type()
{
	return VAR;
}
Brakets::Brakets(int b, int e)
{
	begin = b;
	end = e;
}
void if_and_while::add_begin(int a)
{
	Brakets tmp;
	tmp.end = -1;
	tmp.begin = a;
	if (br_types.back()) 
		if_lab.push_back(tmp);
	else
		while_lab.push_back(tmp);
}
	
void if_and_while::add_end(int a)
{
	if (br_types.back()) {
		int len = if_lab.size();
		while (if_lab[len - 1].end != -1)
			len--;
		if_lab[len - 1].end = a;
	}
	else {
		int len = while_lab.size();
		while (while_lab[len - 1].end != -1)
			len--;
		while_lab[len - 1].end = a;
	}
	br_types.pop_back();
}

void if_and_while::if_print()
{
	for (int i = 0; i < if_lab.size(); i++) {
		cout << "begin = " << if_lab[i].begin << ' ' << "end = " << if_lab[i].end << endl;
	}
}
void if_and_while::while_print()
{
	for (int i = 0; i < while_lab.size(); i++) {
		cout << "begin = " << while_lab[i].begin << ' ' << "end = " << while_lab[i].end << endl;
	}
}

Array::Array(int len, const string &Name)
{
	size = len;
	name = Name;
	if (len != 0) 
		arr.resize(len);
}

void Array::print()
{
	cout << '(' << name << " ; ";
	for (int i(0); i < arr.size(); i++)
		cout << arr.at(i) << ' ';
	cout << ')';
}

lex_type Array::type()
{
	return ARRAY;
}
int Array::get_value(int index)
{
	return arr.at(index);
}
void Array::set_value(int index, int val)
{
	arr[index] = val;
}

int Array::get_size()
{
	return size;
}
void Array::set_size(int len)
{
	size = len;
}

string Array::get_name()
{
	return name;
}

void Array::set_name(const string &s)
{
	name = s;
}

bool ArrayTable::search(const string &s)
{
	for (int i = 0; i < Tab.size(); i++)
	{
		if (Tab[i].get_name() == s)
			return true;
	}
	return false;
}
bool ArrayTable::add_elem(Array &arr)
{	
	if (search(arr.get_name()) == false)
		Tab.push_back(arr);
	else
		return false;
	return true;
}
void ArrayTable::print()
{
	for (int i = 0; i < Tab.size(); i++) { 
		Tab[i].print();
		cout << endl;
	}
}

void ArrayTable::set_val(const string &s, int index, int val)
{
	for (int i = 0; i < Tab.size(); i++)
		if (Tab[i].get_name() == s) {
			Tab[i].set_value(index, val);
			break;
		}
}
Array ArrayTable::get_elem(const string &s)
{
	for (int i = 0; i < Tab.size(); i++)
		if (Tab[i].get_name() == s)
			return Tab[i];
}
int ArrayTable::get_size(const string &s)
{
	for (int i = 0; i < Tab.size(); i++)
		if (s == Tab[i].get_name())
			return Tab[i].get_size();
}
int ArrayTable::get_val(const string &s, int index)
{
	for (int i = 0; i < Tab.size(); i++)
		if (Tab[i].get_name() == s) 
			return Tab[i].get_value(index);
}

Poliz_Elem* get_lexem(string & express, int & i, int &cur_str, if_and_while &Table, ArrayTable &table, VarTable &var_list)
{
	int value;
	string str;
	static bool uno = true;
//	type = VAR;
	if (i == 0)
		uno = true;
	if ((express[i] >= '0') && (express[i] <= '9')) {
		value = 0;
		//type = NUM;
		while ((express[i] >= '0') && (express[i] <= '9')) 
		{
			value = 10 * value + express[i] -'0';
			i++;
		}
		i--;
		uno = false;
		return new Number(value);
	}
	else if ((express[i] >= 'A') && (express[i] <= 'Z')) {
		//type = VAR;
		while ((express[i] >= 'A') && (express[i] <= 'Z')) {
			str.push_back(express[i]);
			i++;
		}
		uno = false;
		if (express[i] == '`') 
			return new Var(str, -1, true);
		else if (express[i] == ':') 
			return new Var(str, cur_str, true);
		else {
			string str = "Lex_error : Label declaration error";
			throw str;
		}
			
	}
	else if ((express[i] >= 'a') && (express[i] <= 'z')) {
		while ((express[i] >= 'a') && (express[i] <= 'z')) {
			str.push_back(express[i]);
			i++;
		}
		if (express[i] != '(') {
			bool x = var_list.check_elem(str);
			if ((x == false) && (express[i] != '=')) {
				string err = "Undeclared variable";
				throw err;
			}
			i--;
			uno = false;
			return new Var(str, 0, false);
		}
		else if (express[i] == '(') {
			uno = false;
			if (table.search(str) == false) {
				int sum(0);
				i++;
				while (express[i] != ')') {
					sum =  10 * sum + express[i] - '0';
					i++;
				}
				return new Array(sum, str);
			}
			else {
				uno = true;
				i--;
				int sum = table.get_size(str);
				return new Array(sum ,str);
			}
		}				
	}
	else {
		//type = OPER;
		for (int  j = 0; j <= 21 ; j++) {
			if (express[i] == Oper_symbol[j]) {
				if (express[i] == '-') {
					if (!uno) {
						uno = true;
						return new Operator(Minus);
					}
					else if ((i == 0) || (uno)) {
						uno = false;
						return new Operator(Umin);
					}
				}
				else if (express[i] == ')') {
					uno = false;
					return new Operator(Right);
				}
				else {
					uno = true;
					if (express[i] == '?') {
						Table.br_types.push_back(true);
						Table.add_begin(cur_str);
					}
					else if (express[i] == '@') {
						Table.br_types.push_back(false);
						Table.add_begin(cur_str);
					}
					else if (express[i] == '}')
						Table.add_end(cur_str + 1);
					else if ((express[i] == '=') && (express[i + 1] == '=')) {
						i++;
						return new Operator(Equ);
					}
					else if ((express[i] == '!') && (express[i + 1] == '=')) {
						i++;
						return new Operator(Nequ);
					}
					else if ((express[i] == '>') && (express[i + 1] == '=')) {
						i++;
						return new Operator(Egreat);
					}
					return new Operator((Operations)j);
				}
			}
		}
		string str = "Lex_error : Nonexistent lexem";
		throw str;
	}
}

void intermediate_errors(vector <Poliz_Elem*> &MyPoliz, if_and_while &Table)
{
	int i, size, while_count = 0, if_count = 0, mov_count = 0, goto_count = 0;
	int print_count = 0;
	string err;
	if ((size = Table.br_types.size()) != 0) {
		if (Table.br_types[size - 1] == true) {
			err = "Lex_err : Not Enough Close Brakets (if)";
			throw err;
		}
		else {
			err = "Lex_err : Not enough close Brakets (while)";
			throw err;
		}
	}
	for (i = 0; i < MyPoliz.size(); i++) {
		if (MyPoliz[i] -> type() == OPER) {
			if (((Operator*)MyPoliz[i]) -> get_value() == While)
				while_count++;
			else if (((Operator*)MyPoliz[i]) -> get_value() == If)
				if_count++;
			else if (((Operator*)MyPoliz[i]) -> get_value() == Mov)
				mov_count++;
			else if (((Operator*)MyPoliz[i]) -> get_value() == Goto)
				goto_count++;
			else if (((Operator*)MyPoliz[i]) -> get_value() == Print)
				print_count++;

		}
	}
	if (if_count > 1) {
		err = "Lex_err : too many 'if' in line";
		throw err;
	}
	else if (while_count > 1) {
		err = "Lex_err : too many 'while' in line";
		throw err;
	}
	else if (goto_count > 1) {
		err = "Lex_err : too many 'goto' in line";
		throw err;
	}
	else if (mov_count > 1) {
		err = "Lex_err : too many 'mov' in line";
		throw err;
	}
	else if (print_count > 1) {
		err = "Lex_err : too many 'print' in line";
		throw err;
	}
	else if (if_count + goto_count + while_count + print_count > 1) {
		err = "Lex_err : impossible combinations of operators (if, while, goto, print)";
		throw err;
	}
	

}
void print(vector <Poliz_Elem*> MyPoliz)
{
	if ((MyPoliz.size() == 1) && (MyPoliz[0] -> type() == OPER))
		return;
	cout << "My Poliz [ ";
	for (int i = 0; i < MyPoliz.size(); i++) {
		MyPoliz[i] -> print();
		cout << ' ';
	}
	cout << "]"<< endl;
	
}

void Make_Poliz(vector <Poliz_Elem*> &MyPoliz)
{
	int i;
	vector <Poliz_Elem*> lex_line;
	vector <Operator*> MyOperator;
	for (i = 0; i < MyPoliz.size(); i++) {
		if (MyPoliz[i] -> type() != OPER) {
			lex_line.push_back(MyPoliz[i]);
		}
		else {
			int len = MyOperator.size();
			int cur_priority = priority[((Operator*)MyPoliz[i]) -> get_value()];
			if ((((Operator*)MyPoliz[i]) -> get_value() != Right) && (((Operator*)MyPoliz[i]) -> get_value() != Left)) {
				if ((len == 0) || (priority[MyOperator[len - 1] -> get_value()] < cur_priority)) {
					MyOperator.push_back((Operator*)MyPoliz[i]);
				}
				else {
					while ((MyOperator.size() > 0) && (priority[MyOperator[len - 1] -> get_value()] >= cur_priority)) {
						lex_line.push_back(MyOperator[len - 1]);
						MyOperator.pop_back();
						len--;
					}
					MyOperator.push_back((Operator*)MyPoliz[i]);
				}
			}
			else if (((Operator*)MyPoliz[i]) -> get_value() == Left) {
				MyOperator.push_back((Operator*)MyPoliz[i]);
			}
			else if (((Operator*)MyPoliz[i]) -> get_value() == Right) {	
				while (MyOperator[len - 1] -> get_value() != Left) {
					lex_line.push_back(MyOperator[len - 1]);
					MyOperator.pop_back();
					len--;
				}
				MyOperator.pop_back();
			}	
		}
	}
	for (i = MyOperator.size() - 1; i >= 0 ; i--) {
		lex_line.push_back(MyOperator[i]);
	}
	for (i = 0; i < lex_line.size(); i++) {
		MyPoliz[i] = lex_line[i];
	}
	while (MyPoliz.size() > lex_line.size())
		MyPoliz.pop_back();
//	print(MyPoliz);
}

int VarTable::get_value (const string &s) {
	return data[s];
}
void VarTable::set_value (const string &s, int value) {
	data[s] = value;
}
bool VarTable::check_elem(const string &s) {
	return data.find(s) != data.end();
}
bool VarTable::add_elem(const string &s) {
	if (check_elem(s))
		return false;
	data[s] = 0;
	return true;	
}
void VarTable::print() {
	map <string, int>::iterator it;
//	cout << "Map hey" << endl;
	for (it = data.begin(); it != data.end(); it++) {
		cout << it->first << '=' << it->second << endl;
	}	
}






