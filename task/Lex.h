#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstdio>
#include <ctype.h>
#include <cstdlib>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

enum type_of_lex{
    LEX_NULL,                                                                           /* 0*/
    LEX_AND, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT, LEX_STRING,        /* 8*/
    LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_TRUE, LEX_WHILE, LEX_WRITE,             /*15*/
    LEX_FIN,                                                                            /*16*/
    LEX_SEMICOLON, LEX_COMMA, LEX_QUOTE, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_LBRACE,/*23*/
    LEX_RBRACE, LEX_EQ, LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES,               /*30*/
    LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ,                                               /*34*/
    LEX_NUM,                                                                            /*35*/
    LEX_ID,                                                                             /*36*/
    LEX_STR,									        /*37*/
    LEX_UNMINUS,									/*38*/
    LEX_UNPLUS,										/*39*/
    POLIZ_LABEL,                                                                        /*40*/
    POLIZ_ADDRESS,                                                                      /*41*/
    POLIZ_GO,                                                                           /*42*/
    POLIZ_FGO                                                                           /*43*/
};

class Lex{
        type_of_lex t_lex;
        int v_lex;
   public:
        Lex(type_of_lex t=LEX_NULL, int v=0): t_lex(t), v_lex(v){}
        type_of_lex get_type() const{
                return t_lex;
        }
        int get_value() const{
                return v_lex;
        }
        friend ostream& operator<< (ostream &s, Lex l);
};

#endif
