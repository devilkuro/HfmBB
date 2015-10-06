/*
 * StringHelper.cc
 *
 *  Created on: Oct 4, 2015
 *      Author: Fanjing-R830
 */

#include "StringHelper.h"

namespace Fanjing {

std::stringstream StringHelper::ss;
int StringHelper::str2int(string s) {
    int i;
    ss<<s;
    ss>>i;
    ss.str("");
    return i;
}

int StringHelper::char2int(char* s) {
    return str2int(string(s));
}

double StringHelper::str2dbl(string s) {
    double d;
    ss<<s;
    ss>>d;
    ss.str("");
    return d;
}

double StringHelper::char2dbl(char* s) {
    return str2dbl(string(s));
}

string StringHelper::int2str(int i) {
    return dbl2str(i);
}

string StringHelper::dbl2str(double d) {
    string s;
    ss<<d;
    s = ss.str();
    ss.str("");
    return s;
}

} /* namespace Fanjing */

