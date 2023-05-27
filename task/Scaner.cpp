#include "Scaner.h"

/* Table of service words */
const char *
Scanner::TW[]={"", "and", "bool", "do", "else", "if", "false", "int", "string",
        "not", "or", "program", "read", "true", "while", "write", NULL};

/* Table of service symbols */
const char *
Scanner::TD[]={"@", ";", ",", "\"", "=", "(", ")", "{", "}", "==", "<", ">",
        "+", "-", "*", "/", "<=", "!=", ">=", NULL};

/* Function for lexical analysis, works with opened file */
Lex Scanner::get_lex(){
        enum state {H, IDENT, NUMB, COM, COM1, ALE, NEQ, DIV, QUOTE};
        int d,j;
        string buf;
        state CS=H;
        static short quote=0;
        do{
                gc();
                switch(CS){
                    case H:
                        if(c==' '||c=='\t'||c=='\n'||c=='\r');
                        else if(isalpha(c)){
                                buf.push_back(c);
                                CS=IDENT;
                        }
                        else if(c=='"'){
                                quote++;
                                CS=QUOTE;
                        }
                        else if(isdigit(c)){
                                d=c-'0';
                                CS=NUMB;
                        }
                        else if(c=='/'){
                                CS=DIV;
                        }
                        else if(c=='='||c=='<'||c=='>'){
                                buf.push_back(c);
                                CS=ALE;
                        }
			else if(c=='@'||c==EOF)
                                return Lex(LEX_FIN);
                        else if(c=='!'){
                                buf.push_back(c);
                                CS=NEQ;
                        }
                        else{
                                buf.push_back(c);
                                if((j=look(buf, TD))){
                                        return Lex((type_of_lex)(j+(int)LEX_FIN), j);
                                }
                                else
                                        throw c;
                        }
                        break;
                    case IDENT:
                        if(isalpha(c)||isdigit(c)){
                                buf.push_back(c);
                        }
                        else{
                                ungetc(c, fp);
                                if((j=look(buf, TW))){
                                        return Lex((type_of_lex)j, j);
                                }
                                else{
                                        j=put(buf);
                                        return Lex(LEX_ID, j);
                                }
                        }
			break;
                    case QUOTE:
                        if(c=='\n'||c=='\r')
                                throw c;
                        if(c!='"')
                                buf.push_back(c);
                        else{
                                quote--;
                                j=put_TSTR(buf);
                                return Lex(LEX_STR, j);
                        }
                        break;
                    case NUMB:
                        if(isdigit(c)){
                                d=d*10+(c-'0');
                        }
                        else{
                                ungetc(c, fp);
                                return Lex(LEX_NUM, d);
                        }
                        break;
		    case DIV:
                        if(c=='*'){
                                CS=COM;
                        }
                        else{
                                ungetc(c, fp);
                                buf.push_back('/');
                                if((j=look(buf, TD))){
                                        return Lex((type_of_lex)(j+(int)LEX_FIN), j);
                                }
                                else
                                        throw c;
                        }
                        break;
                    case COM:
                        if(c=='*'){
                                CS=COM1;
                        }
                        else if(c=='/'){
                                gc();
                                if(c=='*')
                                        throw c;
                                else
                                        ungetc(c, fp);
                        }
                        else if(c=='@'||c==EOF)
                                throw c;
                        break;
		    case COM1:
                        if(c=='/'){
                                CS=H;
                        }
                        else if(c=='@'||c==EOF)
                                throw c;
                        else
                                CS=COM;
                        break;
                    case ALE:
                        if(c=='='){
                                buf.push_back(c);
                                j=look(buf, TD);
                                return Lex((type_of_lex)(j+(int)LEX_FIN), j);
                        }
                        else{
                                ungetc(c, fp);
                                j=look(buf, TD);
                                return Lex((type_of_lex)(j+(int)LEX_FIN), j);
                        }
                        break;
                    case NEQ:
                        if(c=='='){
                                buf.push_back(c);
                                j=look(buf, TD);
                                return Lex(LEX_NEQ, j);
                        }
                        else
                                throw '!';
                        break;
                }
        }
        while(true);
}

ostream& operator<< (ostream &s, Lex l){
        string t;
        if(l.t_lex<=LEX_WRITE)
                t=Scanner::TW[l.t_lex];
        else if(l.t_lex>=LEX_FIN&&l.t_lex<=LEX_GEQ)
                t=Scanner::TD[l.t_lex-LEX_FIN];
        else if(l.t_lex==LEX_NUM)
                t="NUMB";
        else if(l.t_lex==LEX_ID)
                t=/*TID[l.v_lex].get_name()*/"ID";
        else if(l.t_lex==LEX_STR)
                t=/*TSTR[l.v_lex]*/"STR";
	else if(l.t_lex==LEX_UNMINUS)
		t="-";
	else if(l.t_lex==LEX_UNPLUS)
                t="+";
        else if(l.t_lex==POLIZ_LABEL)
                t="Label";
        else if(l.t_lex==POLIZ_ADDRESS)
                t="Addr";
        else if(l.t_lex==POLIZ_GO)
                t="!";
        else if(l.t_lex==POLIZ_FGO)
                t="!F";
        else
                throw l;
        s<<"    ("<<t<<','<<l.v_lex<<");"<<endl;
        return s;
}
