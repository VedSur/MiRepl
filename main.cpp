#include <iostream>
#include <fstream>
#include <vector>
#include "evaluator.cpp"
using namespace std;

ifstream in_file;
ofstream out_file;
bool use_out_file      = false;
bool use_in_file       = false;
bool use_prompts       = true;
bool print_on_new_line = true;

void print_inline(string text) {
    if(use_out_file) out_file << text;
    else cout << text;
}
void print(string text) {
    if(print_on_new_line) print_inline(text+"\n");
    else print_inline(text);
}

string read(string prompt) {
    if(use_prompts) print_inline(prompt);
    string input;
    if(use_in_file) getline(in_file, input);
    else getline(cin, input);
    return input;
}

void rep() {
    string input = read("MiRepl> ");
    string output = eval(tokenize(input));
    if(output != "") print(output);
}

void setup_in_file(string path) {
    in_file = ifstream(path.c_str());
    use_in_file = true;
    use_prompts = false;
}

int main(int argc, char **argv) {
    if(argc > 1) setup_in_file(argv[1]);
    while(true) rep();
    return 0;
}