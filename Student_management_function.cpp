//3 of CRUD operation - views, updates, deletes, and search for students information
void viewStudents() {

    string query = "SELECT * FROM Students";

    if(mysql_query(conn, query.c_str()) != 0) {

        cout << "Query Error: "<< mysql_error(conn)<< endl;

        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);

    if(res == NULL) {

        cout << "Result Error: "<< mysql_error(conn)<< endl;

        return;
    }

    MYSQL_ROW row;

    cout << "\n================ STUDENT LIST ================\n\n";

    cout << left
         << setw(10) << "ID"
         << setw(25) << "NAME"
         << setw(10) << "AGE"
         << setw(25) << "DEPARTMENT"
         << setw(15) << "GPA"
         << endl;

    cout << "--------------------------------------------------------------------------\n";

    while((row = mysql_fetch_row(res))) {

        cout << left
             << setw(10) << row[0]
             << setw(25) << row[1]
             << setw(10) << row[2]
             << setw(25) << row[3]
             << setw(15) << fixed
             << setprecision(2)
             << atof(row[4])
             << endl;
    }

    mysql_free_result(res);
}

//function to update specific student information
void updateStudent() {

    int id;

    cout << "\nEnter Student ID: ";
    cin >> id;

    if (!studentExists(id)) {
        cout << "Student Not Found!\n";
        return;
    }

    int choice;

    cout << "\n===== UPDATE STUDENT =====\n";
    cout << "1. Update Name\n";
    cout << "2. Update Age\n";
    cout << "3. Update Department\n";
    cout << "4. Update GPA\n";
    cout << "5. Update All\n";
    cout << "Enter choice: ";
    cin >> choice;

    cin.ignore();

    string query;

    switch (choice) {

        case 1: {
            string name;
            cout << "Enter New Name: ";
            getline(cin, name);

            query = "UPDATE Students SET Name = '" + name +
                    "' WHERE ID = " + to_string(id);
            break;
        }

        case 2: {
            int age;
            cout << "Enter New Age: ";
            cin >> age;

            query = "UPDATE Students SET Age = " + to_string(age) + "' WHERE ID = " + to_string(id);
            break;
        }

        case 3: {
            string dept;
            cout << "Enter New Department: ";
            getline(cin, dept);

            query = "UPDATE Students SET Dept = '" + dept + "' WHERE ID = " + to_string(id);
            break;
        }

        case 4: {
            double gpa;
            cout << "Enter New GPA: ";
            cin >> gpa;

            query = "UPDATE Students SET GPA = " + to_string(gpa) + " WHERE ID = " + to_string(id);
            break;
        }

        case 5: {
            string name, dept;
            int age;
            double gpa;

            cout << "Enter New Name: ";
            getline(cin, name);

            cout << "Enter New Age: ";
            cin >> age;
            cin.ignore();

            cout << "Enter New Department: ";
            getline(cin, dept);

            cout << "Enter New GPA: ";
            cin >> gpa;

            query = "UPDATE Students SET "
                    "Name = '" + name +
                    "', Age = " + to_string(age) +
                    ", Dept = '" + dept +
                    "', GPA = " + to_string(gpa) +
                    " WHERE ID = " + to_string(id);
            break;
        }

        default:
            cout << "Invalid Choice!\n";
            return;
    }

    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Student Updated Successfully!\n";
    } else {
        cout << "Error: " << mysql_error(conn) << endl;
    }
}
//function to delete student information
void deleteStudent() {

    int id;

    cout << "\nEnter Student ID: ";
    cin >> id;

    if(!studentExists(id)) {

        cout << "Student Not Found!\n";
        return;
    }

    string deleteGrades ="DELETE FROM Grades WHERE SID = " + to_string(id);

    mysql_query(conn, deleteGrades.c_str());

    string query ="DELETE FROM Students WHERE ID = " + to_string(id);

    if(mysql_query(conn, query.c_str()) == 0) {

        cout << "Student Deleted Successfully!\n";
    }

    else {

        cout << "Error: "
             << mysql_error(conn)
             << endl;
    }
}

void searchStudent() {

    int id;

    cout << "\nEnter Student ID: ";
    cin >> id;

    string query =
    "SELECT * FROM Students WHERE ID = " + to_string(id);

    mysql_query(conn, query.c_str());

    MYSQL_RES* res = mysql_store_result(conn);

    MYSQL_ROW row = mysql_fetch_row(res);

    if(row) {

        cout << "\n========== STUDENT FOUND ==========\n";

        cout << "ID: " << row[0] << endl;
        cout << "Name: " << row[1] << endl;
        cout << "Age: " << row[2] << endl;
        cout << "Department: " << row[3] << endl;
        cout << "GPA: " << fixed<< setprecision(2)<< atof(row[4])<< endl;
    }

    else {

        cout << "Student Not Found!\n";
    }

    mysql_free_result(res);
}
