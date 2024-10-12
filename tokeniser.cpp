#include <iostream>
#include <vector>
#include <regex>
using namespace std;

typedef enum {
    Null,
    String,
    Int,
    Iden,
    Call,
    Oper,
    MethodDecl,
    End,
    LocVarSet,
    GlbVarSet,
    LocVarDecl,
    GlbVarDecl,
    GlbIden
} TokType;

typedef struct
{
    string text;
    TokType ttype;
} Token;

#define rInt           regex("[0-9]+")
#define rIden          regex("[a-zA-Z_][a-zA-Z0-9_]*")
#define rGlbIden       regex("\\$[a-zA-Z_][a-zA-Z0-9_]*")
#define rCall          regex(".[a-zA-Z_][a-zA-Z0-9_]*")
#define rOper          regex("([-+*/^]|(==)|(!=)|(>=)|(<=))")
#define rMethodDecl    regex("%[a-zA-Z_][a-zA-Z0-9_]*")
#define rEnd           regex("end")
#define rLocVarSet     regex("=[a-zA-Z_][a-zA-Z0-9_]*")
#define rGlbVarSet     regex("=\\$[a-zA-Z_][a-zA-Z0-9_]*")
#define rLocVarDecl    regex("[a-zA-Z_][a-zA-Z0-9_]*\\$")
#define rGlbVarDecl    regex("\\$[a-zA-Z_][a-zA-Z0-9_]*\\$")

TokType get_toktype(string text) {
    if(regex_match(text, rInt))              return Int;
    else if(regex_match(text, rMethodDecl))  return MethodDecl;
    else if(regex_match(text, rEnd))         return End;
    else if(regex_match(text, rLocVarDecl))  return LocVarDecl;
    else if(regex_match(text, rGlbVarDecl))  return GlbVarDecl;
    else if(regex_match(text, rLocVarSet))   return LocVarSet;
    else if(regex_match(text, rGlbVarSet))   return GlbVarSet;
    else if(regex_match(text, rGlbIden))     return GlbIden;
    else if(regex_match(text, rIden))        return Iden;
    else if(regex_match(text, rOper))        return Oper;
    else if(regex_match(text, rCall))        return Call;
    else                                     return Null;
}

vector<Token> tokenize(string user_input) {
    string input = user_input + "\n";
    int input_length = input.length();
    string buffer("");
    vector<Token> tokens = vector<Token>();
    bool is_in_str = false;
    TokType ttype = Null;
    char c;
    bool is_blackslash_char = false;
    for(int i = 0; i < input_length; i++) {
        c = input.at(i);
        bool is_whitespace = string(" \t\r\n").rfind(c) != string::npos;
        if(is_whitespace  && !is_in_str) {
            if(is_whitespace and buffer != "") {
                if(ttype == Null) {
                    ttype = get_toktype(buffer);
                }
                if(buffer != "") {
                    tokens.push_back(Token{.text=string(buffer.c_str()), .ttype=ttype});
                    ttype = Null;
                }
                buffer = string("");
            }
        }
        else if(c == '"' && !is_blackslash_char) {
            ttype = String;
            is_in_str = !is_in_str;
        }
        else {
            if(is_in_str && c == '\\') is_blackslash_char = true;
            else if(is_blackslash_char) is_blackslash_char = false;
            buffer += c;
        }
    }
    return tokens;
}

void print_tokens(vector<Token> tokens) {
    for(Token token: tokens) {
        cout << "Type#" << token.ttype << ' ' << token.text << endl;
    }
}