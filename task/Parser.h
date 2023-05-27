#ifndef PARSER_H
#define PARSER_H

#include "Scaner.h"

/*	Model Language Syntax

〈программа〉 → program { 〈описания〉 〈операторы〉 }
〈описания〉 → _{ 〈описание〉; }_
〈описание〉 → 〈тип〉 〈переменная〉 _{ , 〈переменная〉 }_
〈тип〉 → int | string | bool
〈переменная〉 → 〈идентификатор〉 | 〈идентификатор〉 = 〈константа〉
〈константа〉 → [+|-] 〈целочисленная〉 | 〈строковая〉 | true | false 
〈целочисленная〉 → 〈цифра〉 _{ 〈цифра〉 }_
〈строковая〉 → " { 〈литера〉 } "
〈операторы〉 → _{ 〈оператор〉 }_
〈оператор〉 → if (〈выражение〉) 〈оператор〉 else 〈оператор〉 |
	while (〈выражение〉) 〈оператор〉 |
	read (〈идентификатор〉);|
	write (〈выражение〉 { , 〈выражение〉 } ); |
	do 〈оператор〉 while (〈выражение〉); |
	〈составной оператор〉 | 〈оператор-выражение〉 
〈составной оператор〉 → { 〈операторы〉 }
〈оператор-выражение〉 → 〈выражение〉;
〈выражение〉 → 〈выражение 1 уровня〉 | 〈идентификатор〉 = 〈выражение〉
〈выражение 1 уровня〉 → 〈выражение 2 уровня〉 [< | > | == | <= | >= | !=] 〈выражение 2 уровня〉 
	| 〈выражение 2 уровня〉 〈выражение 2 уровня〉 → [+ | -] 〈выражение 3 уровня〉
〈выражение 3 уровня〉 → 〈выражение 4 уровня〉 _{ [+ | - | or] 〈выражение 4 уровня〉 }_
〈выражение 4 уровня〉 → 〈выражение 5 уровня〉 {_ [* | / | and] 〈выражение 5 уровня〉 }_
〈выражение 5 уровня〉 → 〈идентификатор〉 | 〈строковая〉 | true | false | not 〈выражение 5 уровня〉 | (〈выражение〉)
*/

/* Class for syntactic and semantic analysis */
class Parser{
        Lex curr_lex;
        type_of_lex c_type;
        int c_val;
        Scanner scan;
        stack <int> st_int;
        stack <type_of_lex> st_lex;
        /* Recursive descent method */
        void program();
        void descriptions();
        void one_description();
        void one_operator();
        void operators();
        void braces_operator();
        void expression();
	void expression_lv1();
	void expression_lv2();
	void expression_lv3();
	void expression_lv4();
	void expression_lv5();
        bool check_operator();
        void dec(type_of_lex type);
        void check_id();
	void check_op();
        void check_not();
        void eq_type();
        void eq_bool();
        void check_id_in_read();
        void gl(){
                curr_lex=scan.get_lex();
                c_type=curr_lex.get_type();
                c_val=curr_lex.get_value();
        }
    public:
        vector <Lex> poliz; //Stack for poliz
        Parser(const char *program): scan(program){}
        void analyze();
	/* Function for work with stack */
	template <class T, class T_EL>
	static void from_st(T &st, T_EL &i){
        	i=st.top();
        	st.pop();
	}
	vector<Ident> &get_TID(){
                return scan.TID;
        }
	vector<string> &get_TSTR(){
		return scan.TSTR;
	}
};

#endif
