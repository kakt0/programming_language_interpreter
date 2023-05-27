#include "Interpretator.h"

int main(int argv, char **argc){
        try{
                if(argv<2){
                	Interpretator I("prog.txt");
        		I.interpretation();
		}
                else{
                        cout<<argc[1]<<endl;
			Interpretator I(argc[1]);
                        I.interpretation();
                }
                cout<<endl;
                return 0;
        }
        catch(char c){
                cout<<"unexpected symbol "<<c<<endl;
                cout<<endl;
                return 1;
        }
        catch(Lex l){
                cout<<"unexpected lexeme " <<l<<endl;
                cout<<endl;
                return 1;
        }
        catch(const char *source){
                cout<<source<<endl;
                cout<<endl;
                return 1;
        }
}
