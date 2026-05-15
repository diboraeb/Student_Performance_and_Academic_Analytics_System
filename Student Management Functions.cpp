
void addStudent() {

    Student s;

    cout << " ADD STUDENT \n";

    cout << "Enter Student ID: ";
    cin >> s.id;

    if(studentExists(s.id)) {

        cout << "Student ID Already Exists!\n";
        return;
    }

    cin.ignore();

    cout << "Enter Student Name: ";
    getline(cin, s.name);

    cout << "Enter Age: ";
    cin >> s.age;

    cin.ignore();

    cout << "Enter Department: ";
    getline(cin, s.department);

    s.gpa = 0.0;

    string query =
    "INSERT INTO Students(ID, Name, Age, Dept, GPA) VALUES("
    + to_string(s.id) + ", '"
    + s.name + "', "
    + to_string(s.age) + ", '"
    + s.department + "', "
    + to_string(s.gpa) + ")";

    if(mysql_query(conn, query.c_str()) == 0) {

        cout << "Student Added Successfully!\n";
    }

    else {

        cout << "Error: "<< mysql_error(conn)<< endl;
    }
}

bool studentExists(int id) {

    string query ="SELECT * FROM Students WHERE ID = " + to_string(id);

    if(mysql_query(conn, query.c_str()) != 0) {

        return false;
    }

    MYSQL_RES* res = mysql_store_result(conn);

    bool exists = mysql_num_rows(res) > 0;

    mysql_free_result(res);

    return exists;
}
