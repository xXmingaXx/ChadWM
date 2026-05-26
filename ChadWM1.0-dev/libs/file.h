#pragma once
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

//spliting a string into a vector of strings

vector<string> splitstring(string str){
    string tempstr;
    vector<string> outvec;
    for (char c : str){
        if (c == ' '){
            outvec.push_back(tempstr);
            tempstr = "";
        }
        else {
            tempstr += c;
        }
    }
    outvec.push_back(tempstr);
    return outvec;
}

//unites every string in a vector to a string

string unite(vector<string> v, int si){
    string str;
    for (int i = si; i<v.size(); i++){
        str += v[i];
    }
    return str;
}


