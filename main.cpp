#include "lib.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stack>

using namespace std;

struct A {
    char c;
    int k;
};

void AddToMultipleStreams(ostream& os1, ostream& os2, stringstream& ss) {
    os1 << ss.str();
    os2 << ss.str();
}

template <typename T>
void PrintAndClearStack(stack<pair<long long, T>>& s) {
    if (s.empty()) {
        return;
    }
    stack<pair<long long, T>> reversed;
    while (!s.empty()) {
        reversed.push(s.top());
        s.pop();
    }

    cout << "bulk: ";
    ofstream fout("bulk" + to_string(reversed.top().first) + ".log");

    stringstream ss;
    while (reversed.size() > 1) {
        ss << reversed.top().second << ", ";
        reversed.pop();
    }
    ss << reversed.top().second << endl;
    reversed.pop();

    AddToMultipleStreams(cout, fout, ss);
}

int main (int, char** argv) {
    size_t n = atoi(argv[1]);
    string s;
    stack<pair<long long, string>> bulk;
    int insideBrackets = 0;
    while (getline(cin, s)) {
        if (s == "{") {
            insideBrackets++;
            if (insideBrackets > 1) {
                continue;
            } else {
                PrintAndClearStack(bulk);
            }
        } else if (s == "}") {
            insideBrackets--;
            if (insideBrackets < 0) { // we just skip "}" if it has no opening bracket pair
                insideBrackets = 0;
            } else if (insideBrackets == 0) {
                PrintAndClearStack(bulk);
            }
        } else {
            auto timestamp = chrono::duration_cast<std::chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count();
            bulk.push({timestamp, s});
            if (bulk.size() == n && insideBrackets == 0) {
                PrintAndClearStack(bulk);
            }
        }
    }
    if (insideBrackets == 0) {
        PrintAndClearStack(bulk);
    }
    return 0;
}
