#include <bits/stdc++.h>
using namespace std;

struct Employee {
    int id;
    string name;
    string position;
    string email;
    string phone;
    double salary;
};

class Storage {
    string filename;
public:
    Storage(const string& fn): filename(fn) {}
    vector<Employee> load() {
        vector<Employee> out;
        ifstream f(filename, ios::binary);
        if(!f) return out;
        while(f.peek() != EOF) {
            Employee e;
            f.read((char*)&e.id, sizeof(e.id));
            int ln;
            f.read((char*)&ln, sizeof(ln));
            e.name.resize(ln);
            f.read(&e.name[0], ln);
            f.read((char*)&ln, sizeof(ln));
            e.position.resize(ln);
            f.read(&e.position[0], ln);
            f.read((char*)&ln, sizeof(ln));
            e.email.resize(ln);
            f.read(&e.email[0], ln);
            f.read((char*)&ln, sizeof(ln));
            e.phone.resize(ln);
            f.read(&e.phone[0], ln);
            f.read((char*)&e.salary, sizeof(e.salary));
            if(f) out.push_back(e);
        }
        return out;
    }
    bool save(const vector<Employee>& data) {
        ofstream f(filename, ios::binary | ios::trunc);
        if(!f) return false;
        for(const auto& e: data) {
            f.write((char*)&e.id, sizeof(e.id));
            int ln = e.name.size();
            f.write((char*)&ln, sizeof(ln));
            f.write(e.name.data(), ln);
            ln = e.position.size();
            f.write((char*)&ln, sizeof(ln));
            f.write(e.position.data(), ln);
            ln = e.email.size();
            f.write((char*)&ln, sizeof(ln));
            f.write(e.email.data(), ln);
            ln = e.phone.size();
            f.write((char*)&ln, sizeof(ln));
            f.write(e.phone.data(), ln);
            f.write((char*)&e.salary, sizeof(e.salary));
        }
        return true;
    }
};

class Management {
    vector<Employee> employees;
    Storage storage;
    int nextId;
public:
    Management(const string& file): storage(file) {
        employees = storage.load();
        nextId = 1;
        for(const auto& e: employees) if(e.id >= nextId) nextId = e.id + 1;
    }
    void persist() {
        storage.save(employees);
    }
    void pause() {
        cout << "\nPress ENTER to continue...";
        string dummy;
        getline(cin, dummy);
    }
    int readInt(const string& prompt) {
        string s;
        while(true) {
            cout << prompt;
            if(!getline(cin, s)) return 0;
            try {
                int v = stoi(s);
                return v;
            } catch(...) {
                cout << "Invalid number, try again.\n";
            }
        }
    }
    double readDouble(const string& prompt) {
        string s;
        while(true) {
            cout << prompt;
            if(!getline(cin, s)) return 0.0;
            try {
                double v = stod(s);
                return v;
            } catch(...) {
                cout << "Invalid number, try again.\n";
            }
        }
    }
    string readLine(const string& prompt) {
        string s;
        cout << prompt;
        getline(cin, s);
        return s;
    }
    void addEmployee() {
        Employee e;
        e.id = nextId++;
        e.name = readLine("Name: ");
        e.position = readLine("Position: ");
        e.email = readLine("Email: ");
        e.phone = readLine("Phone: ");
        e.salary = readDouble("Salary: ");
        employees.push_back(e);
        persist();
        cout << "Employee added with ID " << e.id << "\n";
        pause();
    }
    void listEmployees(bool showIndex = false) {
        if(employees.empty()) {
            cout << "No records.\n";
            pause();
            return;
        }
        cout << left << setw(6) << "ID" << setw(22) << "Name" << setw(18) << "Position" << setw(18) << "Email" << setw(14) << "Phone" << setw(10) << "Salary" << "\n";
        cout << string(90, '-') << "\n";
        for(size_t i=0;i<employees.size();++i) {
            const auto& e = employees[i];
            cout << left << setw(6) << e.id << setw(22) << e.name.substr(0,20) << setw(18) << e.position.substr(0,16) << setw(18) << e.email.substr(0,16) << setw(14) << e.phone.substr(0,12) << setw(10) << fixed << setprecision(2) << e.salary << "\n";
        }
        pause();
    }
    Employee* findById(int id) {
        for(auto& e: employees) if(e.id == id) return &e;
        return nullptr;
    }
    vector<Employee*> findByName(const string& namePart) {
        vector<Employee*> out;
        string lowerName = namePart;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        for(auto& e: employees) {
            string t = e.name;
            transform(t.begin(), t.end(), t.begin(), ::tolower);
            if(t.find(lowerName) != string::npos) out.push_back(&e);
        }
        return out;
    }
    void searchMenu() {
        cout << "1) Search by ID\n2) Search by name\n0) Back\nChoose: ";
        string s;
        if(!(cin >> s)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if(s=="1") {
            int id = readInt("Enter ID: ");
            Employee* e = findById(id);
            if(!e) cout << "Not found.\n";
            else printEmployee(*e);
            pause();
        } else if(s=="2") {
            string q = readLine("Enter name or part: ");
            auto res = findByName(q);
            if(res.empty()) cout << "No matches.\n";
            else {
                cout << left << setw(6) << "ID" << setw(22) << "Name" << setw(18) << "Position" << setw(10) << "Salary" << "\n";
                cout << string(60,'-') << "\n";
                for(auto p: res) {
                    cout << left << setw(6) << p->id << setw(22) << p->name.substr(0,20) << setw(18) << p->position.substr(0,16) << setw(10) << fixed << setprecision(2) << p->salary << "\n";
                }
            }
            pause();
        } else return;
    }
    void printEmployee(const Employee& e) {
        cout << "ID: " << e.id << "\n";
        cout << "Name: " << e.name << "\n";
        cout << "Position: " << e.position << "\n";
        cout << "Email: " << e.email << "\n";
        cout << "Phone: " << e.phone << "\n";
        cout << "Salary: " << fixed << setprecision(2) << e.salary << "\n";
    }
    void updateEmployee() {
        int id = readInt("Enter ID to update: ");
        Employee* e = findById(id);
        if(!e) {
            cout << "Not found.\n";
            pause();
            return;
        }
        cout << "Leave blank to keep current value.\n";
        string s = readLine(string("Name [") + e->name + "]: ");
        if(!s.empty()) e->name = s;
        s = readLine(string("Position [") + e->position + "]: ");
        if(!s.empty()) e->position = s;
        s = readLine(string("Email [") + e->email + "]: ");
        if(!s.empty()) e->email = s;
        s = readLine(string("Phone [") + e->phone + "]: ");
        if(!s.empty()) e->phone = s;
        string sal = readLine(string("Salary [") + to_string(e->salary) + "]: ");
        if(!sal.empty()) {
            try { e->salary = stod(sal); } catch(...) {}
        }
        persist();
        cout << "Updated.\n";
        pause();
    }
    void deleteEmployee() {
        int id = readInt("Enter ID to delete: ");
        auto it = remove_if(employees.begin(), employees.end(), [id](const Employee& e){ return e.id==id; });
        if(it==employees.end()) {
            cout << "Not found.\n";
        } else {
            employees.erase(it, employees.end());
            persist();
            cout << "Deleted.\n";
        }
        pause();
    }
    void sortMenu() {
        cout << "1) Sort by ID\n2) Sort by name\n3) Sort by salary descending\n0) Back\nChoose: ";
        string s;
        if(!(cin >> s)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if(s=="1") sort(employees.begin(), employees.end(), [](const Employee& a,const Employee& b){ return a.id < b.id; });
        else if(s=="2") sort(employees.begin(), employees.end(), [](const Employee& a,const Employee& b){ return a.name < b.name; });
        else if(s=="3") sort(employees.begin(), employees.end(), [](const Employee& a,const Employee& b){ return a.salary > b.salary; });
        else return;
        persist();
        cout << "Sorted.\n";
        pause();
    }
    void exportCSV() {
        string fn = readLine("CSV filename (e.g. export.csv): ");
        if(fn.empty()) fn = "export.csv";
        ofstream f(fn);
        if(!f) { cout << "Failed to open file.\n"; pause(); return; }
        f << "ID,Name,Position,Email,Phone,Salary\n";
        for(const auto& e: employees) {
            string name = e.name; for(char& c: name) if(c==',') c = ' ';
            string pos = e.position; for(char& c: pos) if(c==',') c = ' ';
            string email = e.email; for(char& c: email) if(c==',') c = ' ';
            string phone = e.phone; for(char& c: phone) if(c==',') c = ' ';
            f << e.id << "," << name << "," << pos << "," << email << "," << phone << "," << fixed << setprecision(2) << e.salary << "\n";
        }
        cout << "Exported to " << fn << "\n";
        pause();
    }
    void importCSV() {
        string fn = readLine("CSV filename to import: ");
        ifstream f(fn);
        if(!f) { cout << "Failed to open file.\n"; pause(); return; }
        string line;
        getline(f, line);
        int added = 0;
        while(getline(f, line)) {
            if(line.empty()) continue;
            vector<string> parts;
            string cur;
            for(char c: line) {
                if(c==',') { parts.push_back(cur); cur.clear(); }
                else cur.push_back(c);
            }
            parts.push_back(cur);
            if(parts.size() < 6) continue;
            Employee e;
            try { e.id = stoi(parts[0]); } catch(...) { e.id = nextId++; }
            e.name = parts[1];
            e.position = parts[2];
            e.email = parts[3];
            e.phone = parts[4];
            try { e.salary = stod(parts[5]); } catch(...) { e.salary = 0.0; }
            bool exists = false;
            for(const auto& ex: employees) if(ex.id == e.id) { exists = true; break; }
            if(!exists) {
                employees.push_back(e);
                if(e.id >= nextId) nextId = e.id + 1;
                added++;
            }
        }
        persist();
        cout << "Imported " << added << " records.\n";
        pause();
    }
    void stats() {
        if(employees.empty()) {
            cout << "No data.\n";
            pause();
            return;
        }
        double total = 0;
        double minS = employees[0].salary;
        double maxS = employees[0].salary;
        for(const auto& e: employees) {
            total += e.salary;
            minS = min(minS, e.salary);
            maxS = max(maxS, e.salary);
        }
        double avg = total / employees.size();
        cout << "Count: " << employees.size() << "\n";
        cout << "Total salary: " << fixed << setprecision(2) << total << "\n";
        cout << "Average salary: " << fixed << setprecision(2) << avg << "\n";
        cout << "Min salary: " << fixed << setprecision(2) << minS << "\n";
        cout << "Max salary: " << fixed << setprecision(2) << maxS << "\n";
        pause();
    }
    void menu() {
        while(true) {
            cout << "\n=== Professional Management System ===\n";
            cout << "1) Add employee\n";
            cout << "2) List employees\n";
            cout << "3) Search\n";
            cout << "4) Update employee\n";
            cout << "5) Delete employee\n";
            cout << "6) Sort\n";
            cout << "7) Export CSV\n";
            cout << "8) Import CSV\n";
            cout << "9) Statistics\n";
            cout << "0) Exit\n";
            cout << "Choose: ";
            string s;
            if(!(cin >> s)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if(s=="1") addEmployee();
            else if(s=="2") listEmployees();
            else if(s=="3") searchMenu();
            else if(s=="4") updateEmployee();
            else if(s=="5") deleteEmployee();
            else if(s=="6") sortMenu();
            else if(s=="7") exportCSV();
            else if(s=="8") importCSV();
            else if(s=="9") stats();
            else if(s=="0") { persist(); break; }
            else cout << "Invalid option.\n";
        }
    }
};

int main() {
     cout << "Starting Professional Management System..." << endl;
    cout << "Press ENTER to begin." << endl;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();


    Management app("employees.dat");
    app.menu();
    return 0;
}
