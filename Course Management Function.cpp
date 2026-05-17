//this function checks whether the id we have entered already exisits in our database and returns a boolean variable exists which is true if it exists and false if it doesnt exist.
bool courseExists(int id) {
    string query = "SELECT * FROM Courses WHERE CID = " + to_string(id);
    if (mysql_query(conn, query.c_str()) != 0) {
        return false;
    }
    MYSQL_RES *res = mysql_store_result(conn);
    bool exists = mysql_num_rows(res) > 0;
    mysql_free_result(res);
    return exists;
}
//add course function which checks if the course id to be added already exists on our database or not and then if it doesnt exist it will add the course
void addCourse() {
    Course c;
    cout << "\n=== ADD COURSE ====\n";
    int n;
    cout<<"How many courses do you want to add"<<endl;
    cin>>n;
for(int i=0;i<n;i++){
    cout << "Enter Course ID: ";
    cin >> c.courseId;
    if (courseExists(c.courseId)) {
        cout << "Course Already Exists!\n";
        return;
    }
    cin.ignore();
    cout << "Enter Course Name: ";
    getline(cin, c.courseName);
    cout << "Enter Credit Hours: ";
    cin >> c.creditHours;
    string query =
        "INSERT INTO Courses(CID, Name, Credits) VALUES("
        + to_string(c.courseId) + ", '"
        + c.courseName + "', "
        + to_string(c.creditHours) + ")";
    if (mysql_query(conn, query.c_str()) == 0) {
        cout << "Course Added Successfully!\n";
    } else {
        cout << "Error: " << mysql_error(conn) << endl;
    }
}
}

// this function helps us to view the code in a formatted manner
void viewCourses() {
    string query = "SELECT * FROM Courses";
    mysql_query(conn, query.c_str());
    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;
    cout << "\n========== COURSES ==========\n";
    cout << left
         << setw(15) << "COURSE ID"
         << setw(30) << "COURSE NAME"
         << setw(15) << "CREDITS"
         << endl;
    cout << "------------------------------------\n";
    while ((row = mysql_fetch_row(res))) {
        cout << left
             << setw(15) << row[0]
             << setw(30) << row[1]
             << setw(15) << row[2]
             << endl;
    }
    mysql_free_result(res);
}

