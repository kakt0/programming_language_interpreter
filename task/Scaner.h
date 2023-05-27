#ifndef SCANNER_H
#define SCANNER_H

#include "Ident.h"

/* Class for lexical analysis */
class Scanner{
        FILE *fp;
        char c;
        int look(const string buf, const char **list){
                int i=0;
                while(list[i]){
                        if(buf==list[i])
                                return i;
                        ++i;
                }
                return 0;
        }

        void gc(){
                c=fgetc(fp);
        }

	/* Puts new element in TID */
	int put(const string &buf){
        	vector<Ident>::iterator k;
        	if((k=find(TID.begin(), TID.end(), buf))!=TID.end())
        	        return k-TID.begin();
        	TID.push_back(Ident(buf));
        	return TID.size()-1;
	}

	/* Puts new element in TSTR */
	int put_TSTR(const string &buf){
	        TSTR.push_back(buf);
	        return TSTR.size()-1;
	}
    public:
        static const char *TW[], *TD[];
        /* Table of identifiers */
	vector<Ident> TID;
	/* Table of strings */
	vector<string> TSTR;
	Scanner(const char *program){
                if(!(fp=fopen(program, "r")))
                        throw "can’t open file";
        }
        Lex get_lex(); //lexical analysis
        void closeFile(){
                fclose(fp);
        }
};

#endif
