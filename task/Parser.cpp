#include "Parser.h"

void Parser::dec(type_of_lex type){
        int i;
        while(!st_int.empty()){
                from_st(st_int, i);
                if(scan.TID[i].get_declare())
                        throw "twice";
                else{
                        scan.TID[i].put_declare();
                        scan.TID[i].put_type(type);
                }
        }
}

void Parser::analyze(){
        int i=0;
        gl();
        program();
        gl();
        if(c_type!=LEX_FIN)
                throw curr_lex;
        //for_each( poliz.begin(), poliz.end(), [](Lex l){ cout << l; });
        /*for(Lex l:poliz){
                cout<<i<<l;
                i++;
        }*/
	scan.closeFile();
}

void Parser::program(){
        if(c_type!=LEX_PROGRAM)
		throw curr_lex;
        gl();
        if(c_type!=LEX_LBRACE)
		throw curr_lex;
        gl();
        descriptions();
        operators();
        if(c_type!=LEX_RBRACE)
        	throw curr_lex;
}

void Parser::descriptions(){
        one_description();
        while(c_type==LEX_SEMICOLON){
            gl();
            if(c_type!=LEX_INT&&c_type!=LEX_BOOL&&c_type!=LEX_STRING)
                    break;
            one_description();
        }
}

void Parser::one_description(){
        type_of_lex tmp_type, tmp_type_num;
        Lex tmp_lex;
        if(c_type==LEX_INT||c_type==LEX_BOOL||c_type==LEX_STRING){
                tmp_type=c_type;
                gl();
                if(c_type!=LEX_ID)
                        throw curr_lex;
                else{
                        st_int.push(c_val);
                        poliz.push_back(Lex(POLIZ_ADDRESS, c_val)); 
			gl();
                        if(c_type==LEX_ASSIGN){
                                gl();
                                if(c_type==LEX_PLUS||c_type==LEX_MINUS){
                                        tmp_type_num=c_type;
                                        tmp_lex=curr_lex;
                                        gl();
                                        if(c_type==LEX_NUM&&tmp_type==LEX_INT){
                                                poliz.push_back(curr_lex);
                                                if(tmp_type_num==LEX_PLUS)
                                                	poliz.push_back(Lex(LEX_UNPLUS));
                                                else
                                                        poliz.push_back(Lex(LEX_UNMINUS));
						poliz.push_back(Lex(LEX_ASSIGN));
                                                gl();
                                        }
                                        else
                                                throw tmp_lex;
                                }
				else if((c_type==LEX_NUM&&tmp_type==LEX_INT)||
                                (c_type==LEX_FALSE&&tmp_type==LEX_BOOL)||
                                (c_type==LEX_TRUE&&tmp_type==LEX_BOOL)||
                                (c_type==LEX_STR&&tmp_type==LEX_STRING)){
                                        poliz.push_back(curr_lex);
                                        poliz.push_back(Lex(LEX_ASSIGN));
                                        gl();
                                }
                                else
                                        throw "wrong type in operation";
                        }
                        else{
                                poliz.pop_back();
                        }
                        while(c_type==LEX_COMMA){
                                gl();
                                if(c_type!=LEX_ID)
                                        throw curr_lex;
                                else{
                                        st_int.push(c_val);
                                        //poliz.push_back(Lex(LEX_ID, c_val));
                                        poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
					gl();
                                        if(c_type==LEX_ASSIGN){
                                                gl();
                                                if(c_type==LEX_PLUS||c_type==LEX_MINUS){
                                                        tmp_type_num=c_type;
                                                        tmp_lex=curr_lex;
                                                        gl();
							if(c_type==LEX_NUM&&tmp_type==LEX_INT){
                                                                poliz.push_back(curr_lex);
								if(tmp_type_num==LEX_PLUS)
                        						poliz.push_back(Lex(LEX_UNPLUS));
                						else
                        						poliz.push_back(Lex(LEX_UNMINUS));
                                                                //poliz.push_back(Lex(tmp_type_num));
                                                                poliz.push_back(Lex(LEX_ASSIGN));
                                                                gl();
                                                        }
                                                        else
                                                                throw tmp_lex;
                                                }
                                                else if((c_type==LEX_NUM&&tmp_type==LEX_INT)||
                                                (c_type==LEX_FALSE&&tmp_type==LEX_BOOL)||
                                                (c_type==LEX_TRUE&&tmp_type==LEX_BOOL)||
                                                (c_type==LEX_STR&&tmp_type==LEX_STRING)){
                                                        poliz.push_back(curr_lex);
                                                        poliz.push_back(Lex(LEX_ASSIGN));
                                                        gl();
                                                }
                                                else
                                                        throw "wrong type in operation";
                                        }
                                        else
                                                poliz.pop_back();
                                }
                        }
                        if(tmp_type==LEX_INT){
                                dec(LEX_INT);
                        }
                        else if(tmp_type==LEX_BOOL){
                                dec(LEX_BOOL);
                        }
			else if(tmp_type==LEX_STRING){
                                dec(LEX_STRING);
                        }
                        else
                                throw curr_lex;
                        if(c_type!=LEX_SEMICOLON)
                                throw curr_lex;
                }
        }
}

bool Parser::check_operator(){
        return c_type==LEX_IF||c_type==LEX_DO||c_type==LEX_WHILE||c_type==LEX_READ||
                        c_type==LEX_WRITE||c_type==LEX_LBRACE||c_type==LEX_NUM||c_type==LEX_ID||
                        c_type==LEX_STR||c_type==LEX_TRUE||c_type==LEX_FALSE||c_type==LEX_NOT||c_type==LEX_LPAREN;
}

void Parser::operators(){
        one_operator();
        while(check_operator()){
                one_operator();
        }
}

void Parser::one_operator(){
        int pl0, pl1, pl2, pl3;
        if(c_type==LEX_IF){
                gl();
                if(c_type==LEX_LPAREN){
                        gl();
                        expression();
                        if(c_type==LEX_RPAREN){
                                eq_bool();
                                pl2=poliz.size();
                                poliz.push_back(Lex());
                                poliz.push_back(Lex(POLIZ_FGO));
                        }
                        else
                                throw curr_lex;
                }
                else
                        throw curr_lex;
                gl();
                one_operator();
                pl3=poliz.size();
                poliz.push_back(Lex());
                poliz.push_back(Lex(POLIZ_GO));
                poliz[pl2]=Lex(POLIZ_LABEL, poliz.size());
		if(c_type==LEX_ELSE){
                        gl();
                        one_operator();
                        poliz[pl3]=Lex(POLIZ_LABEL, poliz.size());
                }
        }//end if
        else if(c_type==LEX_WHILE){
                pl0=poliz.size();
                gl();
                if(c_type==LEX_LPAREN){
                        gl();
                        expression();
                        if(c_type==LEX_RPAREN){
                                eq_bool();
                                pl1=poliz.size();
                                poliz.push_back(Lex());
                                poliz.push_back(Lex(POLIZ_FGO));
                        }
                        else
                                throw curr_lex;
                }
                else
                        throw curr_lex;
                gl();
                one_operator();
                poliz.push_back(Lex(POLIZ_LABEL, pl0));
                poliz.push_back(Lex(POLIZ_GO));
                poliz[pl1]=Lex(POLIZ_LABEL, poliz.size());
        }//end while
	else if(c_type==LEX_DO){
                gl();
		pl0=poliz.size();
                one_operator();
                if(c_type==LEX_WHILE){
                        gl();
                        if(c_type==LEX_LPAREN){
                                gl();
                                expression();
                                eq_bool();
                                pl1=poliz.size();
                                poliz.push_back(Lex());
                                poliz.push_back(Lex(POLIZ_FGO));

                                poliz.push_back(Lex(POLIZ_LABEL, pl0));
                                poliz.push_back (Lex(POLIZ_GO));
                                poliz[pl1]=Lex(POLIZ_LABEL, poliz.size());
                                if(c_type==LEX_RPAREN)
                                        gl();
                                else
                                        throw curr_lex;
                                if(c_type==LEX_SEMICOLON)
                                                gl();
                                else
                                        throw curr_lex;
                        }
                        else
                                throw curr_lex;
                }
		else
                        throw curr_lex;

        }//end do
        else if(c_type==LEX_READ){
                gl();
                if(c_type==LEX_LPAREN){
                        gl();
                        if(c_type==LEX_ID){
                                check_id_in_read();
                                poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
                                gl();
                        }
                        else
                                throw curr_lex;
                        if (c_type==LEX_RPAREN){
                                gl();
                                poliz.push_back(Lex(LEX_READ));
                        }
                        else
                                throw curr_lex;
                        if(c_type==LEX_SEMICOLON)
                                gl();
                        else
                                throw curr_lex;
                }
                else
                        throw curr_lex;
        }//end read
	else if(c_type==LEX_WRITE){
                gl();
                if(c_type==LEX_LPAREN){
                        gl();
                        expression();
                        poliz.push_back(Lex(LEX_WRITE));
                        while(c_type==LEX_COMMA){
                                gl();
                                expression();
                                poliz.push_back(Lex(LEX_WRITE));
                        }
                        if(c_type==LEX_RPAREN){
                                gl();
                        }
                        else
                                throw curr_lex;
                        if(c_type==LEX_SEMICOLON)
                                gl();
                        else
                                throw curr_lex;
                }
                else
                        throw curr_lex;
        }//end write
	else if(c_type==LEX_LBRACE){
                gl();
                braces_operator();
        }
        else{
                expression();
                if(c_type==LEX_SEMICOLON)
                        gl();
                else
                        throw curr_lex;
		poliz.push_back(Lex(LEX_SEMICOLON));
        }
}

void Parser::braces_operator(){
        operators();
        if(c_type==LEX_RBRACE){
                gl();
        }
        else
                throw curr_lex;
}

void Parser::expression(){
	int tmp_c_val=c_val;
        expression_lv1();
        if(c_type==LEX_ASSIGN){
		poliz.pop_back();
		poliz.push_back(Lex(POLIZ_ADDRESS, tmp_c_val));
                gl();
                expression();
                eq_type();
                poliz.push_back(Lex(LEX_ASSIGN));
        }
}

void Parser::expression_lv1(){
        expression_lv2();
        if(c_type==LEX_EQ||c_type==LEX_LSS||c_type==LEX_GTR||
         c_type==LEX_LEQ||c_type==LEX_GEQ||c_type==LEX_NEQ){
                st_lex.push(c_type);
                gl();
                expression_lv2();
                check_op();
        }
}

void Parser::expression_lv2(){
        type_of_lex tmp_type, t;
        tmp_type=c_type;
        if(c_type==LEX_PLUS||c_type==LEX_MINUS){
                gl();
                expression_lv3();
                from_st(st_lex, t);
                if(t==LEX_INT)
                        st_lex.push(LEX_INT);
                else
                        throw "wrong type are in operation";
                if(tmp_type==LEX_PLUS)
			poliz.push_back(Lex(LEX_UNPLUS));
		else
			poliz.push_back(Lex(LEX_UNMINUS));
        }
        else
                expression_lv3();
}

void Parser::expression_lv3(){
        expression_lv4();
        while(c_type==LEX_PLUS||c_type==LEX_MINUS||c_type==LEX_OR){
                st_lex.push(c_type);
                gl();
                expression_lv4();
                check_op();
        }
}

void Parser::expression_lv4(){
        expression_lv5();
        while(c_type==LEX_TIMES||c_type==LEX_SLASH||c_type==LEX_AND){
                st_lex.push(c_type);
                gl();
                expression_lv5();
                check_op();
        }
}

void Parser::expression_lv5(){
        if(c_type==LEX_ID){
                check_id();
                poliz.push_back(Lex(LEX_ID, c_val));
                gl();
        }
        else if(c_type==LEX_NUM){
                st_lex.push(LEX_INT);
                poliz.push_back(curr_lex);
                gl();
        }
        else if(c_type==LEX_STR){
                st_lex.push(LEX_STRING);
                poliz.push_back(curr_lex);
                gl();
        }
	else if(c_type==LEX_TRUE){
                st_lex.push(LEX_BOOL);
                poliz.push_back(Lex(LEX_TRUE, 1));
                gl();
        }
        else if(c_type==LEX_FALSE){
                st_lex.push(LEX_BOOL);
                poliz.push_back(Lex(LEX_FALSE, 0));
                gl();
        }
        else if(c_type==LEX_NOT){
                gl();
                expression_lv5();
                check_not();
        }
        else if(c_type==LEX_LPAREN){
                gl();
                expression();
                if(c_type==LEX_RPAREN)
                        gl();
                else
                        throw curr_lex;
        }
        else{
                throw curr_lex;
        }
}

void Parser::check_id(){
        if(scan.TID[c_val].get_declare())
                st_lex.push(scan.TID[c_val].get_type());
        else
                throw "not declared";
}

void Parser::check_op(){
        type_of_lex t1, t2, op, t=LEX_INT, t_1=LEX_STRING, r=LEX_BOOL;
        from_st(st_lex, t2);
        from_st(st_lex, op);
        from_st(st_lex, t1);

        if(op==LEX_PLUS||op==LEX_MINUS||op==LEX_TIMES||op==LEX_SLASH)
                r=LEX_INT;
        if((t1==LEX_STRING||t2==LEX_STRING)&&op==LEX_PLUS)
                r=LEX_STRING;
        if(op==LEX_OR||op==LEX_AND)
                t=LEX_BOOL;
        if(t1==t2&&t1==t||t1==t2&&t1==t_1)
                st_lex.push(r);
        else
                throw "wrong types are in operation";
        poliz.push_back(Lex(op));
}

void Parser::check_not(){
        if(st_lex.top()!=LEX_BOOL)
                throw "wrong type is in not";
        else
                poliz.push_back(Lex(LEX_NOT));
}

void Parser::eq_type(){
        type_of_lex t;
        from_st(st_lex, t);
        if(t!=st_lex.top())
                throw "wrong types are in =";
        st_lex.pop();
}

void Parser::eq_bool(){
        if(st_lex.top()!=LEX_BOOL)
                throw "expression is not boolean";
        st_lex.pop();
}

void Parser::check_id_in_read(){
        if(!scan.TID[c_val].get_declare())
                throw "not declared";
}
