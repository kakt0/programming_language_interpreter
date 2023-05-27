#include "Executer.h"

void Executer::execute(vector<Lex> &poliz, vector<Ident> TID, vector<string> TSTR){
    Lex pc_el;
    stack<int> args;
    int i, j, index=0, size=poliz.size();
    bool is_id=false;
    type_of_lex tmp_type_id, tmp_type_const;
    while(index<size){
        pc_el=poliz[index];
        switch(pc_el.get_type()){
	    case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case LEX_STR: case POLIZ_ADDRESS: case POLIZ_LABEL:
		is_id=false;
		tmp_type_const=pc_el.get_type();
		args.push(pc_el.get_value());
		break;

            case LEX_ID:
		i=pc_el.get_value();
		tmp_type_id=TID[i].get_type();
                if(TID[i].get_assign()){
			args.push(TID[i].get_value());
			is_id=true;
                }
                else
			throw "POLIZ: indefinite identifier";
		break;
			
	    case LEX_SEMICOLON:
		Parser::from_st(args, i);
		break;

            case LEX_NOT:
                Parser::from_st(args, i);
                args.push(!i);
                break;

            case LEX_OR:
                Parser::from_st(args, i);
                Parser::from_st(args, j);
                args.push(j||i);
                break;

            case LEX_AND:
                Parser::from_st(args, i);
                Parser::from_st(args, j);
                break;

            case POLIZ_GO:
                Parser::from_st(args, i);
                index=i-1;
                break;

            case POLIZ_FGO:
                Parser::from_st(args, i);
                Parser::from_st(args, j);
                if(!j){ 
			index=i-1;
		}
                break;

            case LEX_WRITE:
                Parser::from_st(args, j);
		if((is_id==true&&tmp_type_id==LEX_STRING)||(is_id==false&&tmp_type_const==LEX_STR)){
			cout<<TSTR[j]<<endl;
		}
		else
                	cout<<j<<endl;
                break;

            case LEX_READ:
                int k;
                Parser::from_st(args, i);
                if(TID[i].get_type()==LEX_INT){
                    	cout<<"Input int value for "<<TID[i].get_name()<<endl;
                    	cin>>k;
                }
		else if(TID[i].get_type()==LEX_STRING){
			string j;
			cout<<"Input string for "<<TID[i].get_name()<<endl;
			getline(cin, j);
			TSTR.push_back(j);
			k=TSTR.size()-1;
		}
                else {
                    	string j;
                    	while (1) {
                        	cout<<"Input boolean value (true or false) for"<<TID[i].get_name()<<endl;
                        	cin>>j;
                        	if(j!="true"&&j!="false"){
                            		cout<<"Error in input:true/false"<<endl;
                            		continue;
                        	}
                        	k=(j=="true")?1:0;
                        	break;
                    	}
                }
                TID[i].put_value(k);
                TID[i].put_assign();
                break;

            case LEX_PLUS:
                Parser::from_st(args, i);
                Parser::from_st(args, j);
		if((is_id==true&&tmp_type_id==LEX_STRING)||(is_id==false&&tmp_type_const==LEX_STR)){
			TSTR.push_back(TSTR[j]+TSTR[i]);
			args.push(TSTR.size()-1);
		}
		else
                	args.push(i+j);
                break;

            case LEX_TIMES:
                Parser::from_st(args, i);
                Parser::from_st(args, j);
                args.push(i*j);
                break;

            case LEX_MINUS:
                Parser::from_st(args, i);
                Parser::from_st(args, j);
                args.push(j-i);
                break;

            case LEX_SLASH:
                Parser::from_st(args, i);
                Parser::from_st(args, j);
                if(i!=0){
                    args.push(j/i);
                    break;
                }
                else
                    throw "POLIZ:divide by zero";

            case LEX_EQ:
                Parser::from_st(args, i);
                Parser::from_st(args, j);
		if((is_id==true&&tmp_type_id==LEX_STRING)||(is_id==false&&tmp_type_const==LEX_STR)){
                        args.push(TSTR[j]==TSTR[i]);
                }
                else
                	args.push(i==j);
                break;

            case LEX_LSS:
                Parser::from_st(args, i);
                Parser::from_st(args, j);
		if((is_id==true&&tmp_type_id==LEX_STRING)||(is_id==false&&tmp_type_const==LEX_STR)){
                        args.push(TSTR[j]<TSTR[i]);
                }
                else
                	args.push(j<i);
                break;

            case LEX_GTR:
                Parser::from_st(args, i);
                Parser::from_st(args, j);
		if((is_id==true&&tmp_type_id==LEX_STRING)||(is_id==false&&tmp_type_const==LEX_STR)){
                        args.push(TSTR[j]>TSTR[i]);
                }
                else
                	args.push(j>i);
                break;

            case LEX_LEQ:
                Parser::from_st(args,i);
                Parser::from_st(args, j);
		if((is_id==true&&tmp_type_id==LEX_STRING)||(is_id==false&&tmp_type_const==LEX_STR)){
                        args.push(TSTR[j]<=TSTR[i]);
                }
                else
			args.push(j<=i);
                break;

            case LEX_GEQ:
                Parser::from_st(args, i);
                Parser::from_st(args, j);
		if((is_id==true&&tmp_type_id==LEX_STRING)||(is_id==false&&tmp_type_const==LEX_STR)){
                        args.push(TSTR[j]>=TSTR[i]);
                }
                else
                	args.push(j>=i);
                break;

            case LEX_NEQ:
                Parser::from_st(args, i);
                Parser::from_st(args, j);
		if((is_id==true&&tmp_type_id==LEX_STRING)||(is_id==false&&tmp_type_const==LEX_STR)){
                        args.push(TSTR[j]!=TSTR[i]);
                }
                else
                	args.push(j!=i);
                break;

	    case LEX_UNMINUS:
		Parser::from_st(args, i);
		args.push(-i);
		break;

	    case LEX_UNPLUS:
                break;

            case LEX_ASSIGN:
                Parser::from_st(args, i);
                Parser::from_st(args, j);
                TID[j].put_value(i);
                TID[j].put_assign();
		args.push(i);
                break;

            default:
                throw "POLIZ: unexpected elem";
        }//end of switch
        ++index;
    };//end of while
    cout<<"\nFinish of executing!!!"<<endl;
}
