#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

struct Person { string name; long long t; };

string path() {
    const char* h = getenv("HOME");
    return string(h ? h : ".") + "/.moss_people";
}

vector<Person> load() {
    vector<Person> v;
    ifstream f(path());
    string line;
    while (getline(f, line)) {
        auto tab = line.find('\t');
        if (tab == string::npos) continue;
        try { v.push_back({line.substr(0, tab), stoll(line.substr(tab + 1))}); } catch (...) {}
    }
    return v;
}

void save(const vector<Person>& v) {
    ofstream f(path());
    for (const auto& p : v) f << p.name << '\t' << p.t << '\n';
}

string lower(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return (char)tolower(c); });
    return s;
}

string mood(long long d) {
    if (d <= 2) return "fresh. you're doing great.";
    if (d <= 7) return "a little fuzz. maybe say hi.";
    if (d <= 14) return "Gerald has moved in. send a text.";
    if (d <= 30) return "getting mossy. a meme counts.";
    if (d <= 90) return "full forest. no guilt, just say hi.";
    return "ancient moss. it's never too late to say hello.";
}

void share(const vector<Person>& v, long long now) {
    if (v.empty()) { cout << "add someone first: moss add Ali\n"; return; }
    long long worst = 0;
    int mossy = 0;
    for (const auto& p : v) {
        long long d = (now - p.t) / 86400;
        if (d < 0) d = 0;
        if (d > worst) worst = d;
        if (d >= 8) mossy++;
    }
    string bar(static_cast<size_t>(min<long long>(worst / 2, 30)), '"');
    cout << "\n  ~ my moss report ~\n\n";
    cout << "  people:       " << v.size() << "\n";
    cout << "  mossiest:     " << worst << " days\n";
    cout << "  mossy (8d+):  " << mossy << "\n\n";
    cout << "  \033[32m" << bar << "\033[0m\n\n";
    cout << "  Gerald says: " << mood(worst) << "\n\n";
}

int main(int argc, char** argv) {
    long long now = (long long)time(nullptr);
    vector<string> args(argv + 1, argv + argc);
    auto people = load();
    string cmd = args.empty() ? "" : args[0];

    if (cmd == "add") {
        if (args.size() < 2) { cout << "usage: moss add NAME [days_ago]\n"; return 0; }
        string name = args[1];
        long long ago = 0;
        if (args.size() > 2) { try { ago = stoll(args[2]); } catch (...) {} }
        if (ago < 0) ago = 0;
        people.erase(remove_if(people.begin(), people.end(),
            [&](const Person& p) { return lower(p.name) == lower(name); }), people.end());
        people.push_back({name, now - ago * 86400});
        save(people);
        cout << name << " added.\n";
    } else if (cmd == "ping") {
        string name = args.size() > 1 ? lower(args[1]) : "";
        bool found = false;
        for (auto& p : people) {
            if (lower(p.name) == name) { p.t = now; found = true; }
        }
        if (found) { save(people); cout << "moss cleared. good human.\n"; }
        else cout << "no one by that name. try: moss add NAME\n";
    } else if (cmd == "share") {
        share(people, now);
    } else if (cmd == "rm") {
        string name = args.size() > 1 ? lower(args[1]) : "";
        size_t before = people.size();
        people.erase(remove_if(people.begin(), people.end(),
            [&](const Person& p) { return lower(p.name) == name; }), people.end());
        save(people);
        cout << (before != people.size() ? "removed." : "not found.") << "\n";
    } else {
        if (people.empty()) { cout << "no one yet. try: moss add Ali\n"; return 0; }
        sort(people.begin(), people.end(), [](const Person& a, const Person& b) { return a.t < b.t; });
        for (const auto& p : people) {
            long long days = (now - p.t) / 86400;
            if (days < 0) days = 0;
            string growth(static_cast<size_t>(min<long long>(days / 2, 30)), '"');
            cout << left << setw(12) << p.name << right << setw(3) << days
                 << "d  \033[32m" << growth << "\033[0m  " << mood(days) << "\n";
        }
    }
    return 0;
}
