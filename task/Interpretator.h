#include "Executer.h"

/* Class, that analyzes code and executes it */
class Interpretator{
	Parser pars;
    	Executer E;
    public:
    	Interpretator(const char* program):pars(program){}
    	void interpretation(){
		pars.analyze();
        	E.execute(pars.poliz, pars.get_TID(), pars.get_TSTR());
	}
};
