#include<iostream>
#include<stack>
#include<vector>
#include<string>

using namespace std;

bool validate();
int eval(string& in);

int main(){
    string in;
    cout << "Enter the expression: ";
    getline(cin, in);
    return eval(in);
}