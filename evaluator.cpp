#include <iostream>
#include <deque>
#include "tokeniser.cpp"

using namespace std;

deque<int> _stack;
string text("");
int text_ptr = 0;

typedef struct
{
    vector<Token> body;
    string name;
} Method;

vector<Method> methods;
bool in_method = false;

Method find_method(string name) {
    vector<Method> methods_cpy = methods;
    for(Method method: methods_cpy) {
        if(method.name == name) return method;
    }
}

void print_methods() {
    for(Method method: methods) {
        cout << "method " << method.name << endl;
        print_tokens(method.body);
    }
}

typedef struct {
    string name;
    int value;
} Var;

vector<Var> global_vars = {};

int get_local_var(string name, vector<Var> local_vars) {
    vector<Var> vars_cpy = local_vars;
    int i = 0;
    for(Var var: vars_cpy) {
        if(var.name == name) return i;
        i++;
    }
}

int get_global_var(string name) {
    return get_local_var(name, global_vars);
}

void print_vars(vector<Var> vars) {
    vector<Var> vars_cpy = vars;
    for(Var var: vars_cpy) {
        cout << "var " << var.name << " = " << var.value << endl;
    }
}

int pop() {
    int output = _stack.back();
    _stack.pop_back();
    return output;
}

int push(int element) {
    _stack.push_back(element);
}

string get_string_from_text(string text, int index) {
    string str("");
    int length = text.length();
    for(int i = index; i < length; i++) {
        if(text[i] == '\0') break;
        str += text[i];
    }
    return str;
}


string replace_seq(string s, string FindWord, string ReplaceWord) {
    size_t index;
    string str("");
    str += s;
    while ((index = str.find(FindWord)) != std::string::npos)
        str.replace(index, FindWord.length(), ReplaceWord);
    return str;
}

string parse_string(string str) {
    string p_str = replace_seq(str, "\\n", "\n");
    p_str = replace_seq(p_str, "\\r", "\r");
    p_str = replace_seq(p_str, "\\t", "\t");
    p_str = replace_seq(p_str, "\\0", "\0");
    p_str = replace_seq(p_str, "\\\"", "\"");
    p_str = replace_seq(p_str, "\\'", "'");
    return p_str;
}

string format_string(string s) {
    string str = parse_string(s);
    string f_str("");
    bool is_fmt = false;
    for(char c: str) {
        if(c == '%') is_fmt = true;
        else if(is_fmt) {
            if(c == 'i') {
                f_str += to_string(pop());;
            }
            else if(c == 's') {
                f_str += get_string_from_text(text, pop());
            }
            else if(c == '%') f_str += c;
            is_fmt = false;
        }
        else {
            f_str += c;
        }
    }
    return f_str;
}

string eval(vector<Token> tokens) {
    string output("");
    vector<Var> local_vars;
    for(Token token: tokens) {
        if(in_method && token.ttype == End) {
            in_method = false;
        }
        else if(in_method) {
            methods.back().body.push_back(token);
        }
        else if(token.ttype == MethodDecl) {
            methods.push_back(Method{.body={}, .name=replace_seq(token.text, "%", "")});
            in_method = true;
        }
        else if(token.ttype == LocVarDecl) {
            string name = string(token.text.c_str());
            name = replace_seq(name, "$", "");
            local_vars.push_back(Var{.name=name, .value=0});
        }
        else if(token.ttype == GlbVarDecl) {
            string name = string(token.text.c_str());
            name = replace_seq(name, "$", "");
            global_vars.push_back(Var{.name=name, .value=0});
        }
        else if(token.ttype == Iden) {
            Var var = local_vars[get_local_var(token.text, local_vars)];
            push(var.value);
        }
        else if(token.ttype == GlbIden) {
            string name = token.text;
            name = replace_seq(name, "$", "");
            Var var = global_vars[get_global_var(name)];
            push(var.value);
        }
        else if(token.ttype == String) {
            text += parse_string(token.text) + "\0";
            push(text_ptr);
            text_ptr += token.text.length();
        }
        else if(token.ttype == Int) {
            push(atoi(token.text.c_str()));
        }
        else if(token.ttype == Oper) {
            int i1, i2;
            i2 = pop();
            i1 = pop();
            if(token.text == "+") push(i1+i2);
            else if(token.text == "-") push(i1-i2);
            else if(token.text == "*") push(i1*i2);
            else if(token.text == "/") push(i1/i2);
            else if(token.text == "^") push(i1^i2);
            else if(token.text == "%") push(i1%i2);
            else if(token.text == "==") push(i1==i2); 
            else if(token.text == "!=") push(i1!=i2);
            else if(token.text == ">=") push(i1>=i2);
            else if(token.text == "<=") push(i1<=i2);
        }
        else if(token.ttype == LocVarSet) {
            string name = string(token.text.c_str());
            name = replace_seq(name, "=", "");
            local_vars[get_local_var(name, local_vars)] = {.name=name, .value=pop()};
        }
        else if(token.ttype == GlbVarSet) {
            string name = string(token.text.c_str());
            name = replace_seq(name, "=", "");
            name = replace_seq(name, "$", "");
            global_vars[get_global_var(name)] = {.name=name, .value=pop()};
        }
        else if(token.ttype == Call) {
            if(token.text==".out") {
                output +=  parse_string(get_string_from_text(text, pop()));
            }
            else if(token.text==".outf") {
                output +=  format_string(get_string_from_text(text, pop()));
            }
            else if(token.text==".exit") {
                exit(pop());
            }
            else{
                string name = string(token.text.c_str());
                name = replace_seq(name, ".", "");
                Method method = find_method(name);
                output += eval(method.body);
            }
        }
    }
    return output;
}