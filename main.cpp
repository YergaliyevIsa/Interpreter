#include <iostream>
#include <vector>
#include "lexical.h"
#include "syntax.h"
using namespace std;

//enum Operations {Mov, Plus, Minus, Mul, Div, Umin, Left, Right, If, While };
//int priority[] ={9, 10, 10, 11, 11, 12, -1, -1};
//char Oper_symbol[] = {'=', '+', '-', '*', '/', '-', '(', ')' , '?', '@' };

int read_code(vector <string> &expressions)
{
	string str;
	int code_len = 0, open = 0, close = 0;
	char elem;
	while (true) {
		open = 0;
		close = 0;
		while ((elem = cin.get()) != '\n'){
			if ((elem != ' ') && (elem != '\t') && (elem != '{')) {
				if (elem == ')')
					open++;
				else if (elem == '(')
					close++;
				str.push_back(elem);
			}
		}
		if (str.size() != 0) {
			if (open != close) {
				string err = "Bracket error";
				throw err;
			} 
			expressions.push_back(str);
			str.clear();
			code_len++;
		}
		else 
			break;
	}
	if (open != close) {
		string err = "Bracket error";
		throw err;
	}
	return code_len;
}

void divide_into_lex(int code_len, vector <string> &expressions, if_and_while &Table, ArrayTable &arr_table, VarTable &var_list, 
  					VarTable &label_list, vector <vector <Poliz_Elem*> > &MyPoliz)
{
	int i;
	for (i = 0; i < code_len; i++) {
		int size = expressions[i].size();
		for (int j = 0; j < size; j++) {
			Poliz_Elem* buf = get_lexem(expressions[i], j, i, Table, arr_table, var_list);
//			cout << "i = " << i << "j = " << j << endl;
			if (buf -> type() == VAR) {
				if (((Var*)buf) -> label) {
					label_list.add_elem(((Var*)buf) -> get_name());
					if (((Var*)buf) -> get_value() == i) {
						label_list.set_value(((Var*)buf) -> get_name(),((Var*)buf) -> get_value());
					}
				}
				else
					var_list.add_elem(((Var*)buf) -> get_name());
			}
			if (buf -> type() == ARRAY) {
				if (arr_table.search(((Array*)buf) -> get_name()) == true) {
					MyPoliz[i].push_back(buf);
					MyPoliz[i].push_back(new Operator(Dot));
				}
				else {
					MyPoliz[i].push_back(buf);
					arr_table.add_elem(*((Array*)buf));
				}
			}
			else if ((buf -> type() != VAR) || (((Var*)buf) -> label == false) || (((Var*)buf) -> get_value() != i))
				MyPoliz[i].push_back(buf);	
		}
	}
}

void declare_labels(vector <vector <Poliz_Elem*> > &MyPoliz, int code_len, VarTable &label_list)
{
	int i;
	for (i = 0; i < code_len; i++)
		for (int j = 0; j < MyPoliz[i].size(); j++)
			if (MyPoliz[i][j] -> type() == VAR) {
				if (((Var*)MyPoliz[i][j]) -> label) {
					int val = label_list.get_value(((Var*)MyPoliz[i][j]) -> get_name());
					((Var*)MyPoliz[i][j]) -> set_value(val);
				}
			}
}

void execute(int code_len, vector <vector <Poliz_Elem*> > &MyPoliz, VarTable &var_list, ArrayTable &arr_table, if_and_while &Table)
{
	int res, i;
	for (i = 0; i < code_len; i++) {
		for (int j = 0; j < MyPoliz[i].size(); j++) {
//			cout << "i = " << i << "j = " << j << endl;
			if ((MyPoliz[i][j] -> type() == VAR) && (((Var*)MyPoliz[i][j]) -> label == false)) {
				int val = var_list.get_value(((Var*)MyPoliz[i][j]) -> get_name());
				((Var*)MyPoliz[i][j]) -> set_value(val);
			}
		}
//		print(MyPoliz[i]);
		res = Compute_Poliz(MyPoliz[i], var_list, i, Table, arr_table);
	//	cout << "result = " << res << endl;
	//	cout << endl;
	}
}

void clear(vector <vector <Poliz_Elem*> > &MyPoliz, vector <string> &expressions)
{
	for (int i = 0; i < MyPoliz.size(); i++)
		for (int j = 0; j < MyPoliz[i].size(); j++)
			delete MyPoliz[i][j];
	for (int i = 0; i < MyPoliz.size(); i++)
		MyPoliz[i].clear();
	MyPoliz.clear();
	//	MyOperator.clear();
	for (int i = 0; i < expressions.size(); i++)
		expressions[i].clear();
	expressions.clear();

}
int main()
{
	if_and_while Table;
	ArrayTable arr_table;
	vector <string> expressions;
	VarTable var_list, label_list;
	int i, len, code_len;
	lex_type type;
	vector <vector <Poliz_Elem*> > MyPoliz;
	try {
		code_len = read_code(expressions);
		MyPoliz.resize(code_len);
		divide_into_lex(code_len, expressions, Table, arr_table, var_list, label_list, MyPoliz);
		declare_labels(MyPoliz, code_len, label_list);
		for (i = 0; i < code_len; i++) {
			intermediate_errors(MyPoliz[i], Table);
			Make_Poliz(MyPoliz[i]);
		}
		for (i = 0; i < code_len; i++)
			print(MyPoliz[i]);
		execute(code_len, MyPoliz, var_list, arr_table, Table);
	}
	catch (string err) {
		cout << err << endl;
		return 1;
	}
	catch (...) {
		cout << "Unexpected error" << endl;
		return 1;
	}
	var_list.print();
	label_list.print();	
	arr_table.print();
	return 0;
}
