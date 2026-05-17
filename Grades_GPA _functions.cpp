//records grade, view grade and update student's gpa
void updateStudentGPA(int studentId) {

    string query = "SELECT Score FROM Grades WHERE SID = " + to_string(studentId);

    if (mysql_query(conn, query.c_str()) != 0) {
        cout << "Error Calculating GPA!\n";
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    double totalPoints = 0.0;
    int totalCourses = 0;

    while ((row = mysql_fetch_row(res))) {
        double score = atof(row[0]);
        double gradePoint;

        if (score >= 90)
            gradePoint = 4.0;
        else if (score >= 80)
            gradePoint = 3.5;
        else if (score >= 70)
            gradePoint = 3.0;
        else if (score >= 60)
            gradePoint = 2.5;
        else if (score >= 50)
            gradePoint = 2.0;
        else
            gradePoint = 0.0;

        totalPoints += gradePoint;
        totalCourses++;
    }

    mysql_free_result(res);

    double gpa = 0.0;

    if (totalCourses > 0) {
        gpa = totalPoints / totalCourses;
    }

    if (gpa > 4.0) {
        gpa = 4.0;
    }
    string updateQuery = "UPDATE Students SET GPA = " + to_string(gpa) + " WHERE ID = " + to_string(studentId);

    mysql_query(conn, updateQuery.c_str());
}
void recordGrade() {

    Grade g;
    int numCourses;

    cout << "\n========== RECORD GRADE ==========\n";

    cout << "Enter Student ID: ";
    cin >> g.studentId;

    if (!studentExists(g.studentId)) {
        cout << "Student Not Found!\n";
        return;
    }

    cout << "How many courses do you want to record grades for? ";
    cin >> numCourses;

    if (numCourses <= 0) {
        cout << "Invalid number!\n";
        return;
    }

    bool anyRecorded = false;

    for (int i = 0; i < numCourses; i++) {

        cout << "\n--- Course " << (i + 1) << " ---\n";
        cout << "Enter Course ID: ";
        cin >> g.courseId;

        if (!courseExists(g.courseId)) {
            cout << "Course Not Found! Skipping...\n";
            continue;
        }

        cout << "Enter Score: ";
        cin >> g.score;

        if (g.score < 0 || g.score > 100) {
            cout << "Invalid Grade! Score must be between 0 and 100.\n";
            continue;
        }

        string query =
            "INSERT INTO Grades(SID, CID, Score) VALUES("
            + to_string(g.studentId) + ", "
            + to_string(g.courseId) + ", "
            + to_string(g.score) + ")";

        if (mysql_query(conn, query.c_str()) == 0) {
            cout << "Grade Recorded Successfully for Course " << g.courseId << "!\n";
            anyRecorded = true;
        } else {
            cout << "Error: " << mysql_error(conn) << endl;
        }
    }
    if (anyRecorded) {
        updateStudentGPA(g.studentId);
        cout << "\nGPA Updated Automatically!\n";
    }
}
void viewGrades() {

    string query = "SELECT * FROM Grades";

    mysql_query(conn, query.c_str());

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "\n========== GRADES ==========\n";

    cout << left
         << setw(15) << "STUDENT ID"
         << setw(15) << "COURSE ID"
         << setw(15) << "SCORE"
         << endl;

    cout << "---------------------------------------------\n";

    while ((row = mysql_fetch_row(res))) {
        cout << left
             << setw(15) << row[0]
             << setw(15) << row[1]
             << setw(15) << row[2]
             << endl;
    }

    mysql_free_result(res);
}
