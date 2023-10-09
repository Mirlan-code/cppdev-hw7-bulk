#include "lib.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stack>
#include <chrono>
#include <memory>

using namespace std;

class Observer {
public:
    explicit Observer(ostream& os) : OS_(os) {};

    void Update(stringstream& ss) {
        OS_ << ss.str();
    }

private:
    ostream& OS_;
};

class Bulk {
public:
    Bulk(size_t n) : N_(n) {
        ConsoleObserver_ = make_unique<Observer>(cout);
    }

    void Run() {
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
                if (bulk.size() == N_ && insideBrackets == 0) {
                    PrintAndClearStack(bulk);
                }
            }
        }
        if (insideBrackets == 0) {
            PrintAndClearStack(bulk);
        }
    }

private:
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
        FileObserver_ = make_unique<Observer>(fout);

        stringstream ss;
        while (reversed.size() > 1) {
            ss << reversed.top().second << ", ";
            reversed.pop();
        }
        ss << reversed.top().second << endl;
        reversed.pop();

        Notify(ss);
    }

    void Notify(stringstream& ss) {
        if (ConsoleObserver_) {
            ConsoleObserver_->Update(ss);
        }
        if (FileObserver_) {
            FileObserver_->Update(ss);
        }
    }

private:
    size_t N_;
    unique_ptr<Observer> ConsoleObserver_;
    unique_ptr<Observer> FileObserver_;
};

int main (int, char** argv) {
    size_t n = atoi(argv[1]);
    auto bulk = Bulk(n);
    bulk.Run();
    return 0;
}
