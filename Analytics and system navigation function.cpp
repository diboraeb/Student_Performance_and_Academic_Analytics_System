//shows weak subject or <50 average score of students, rank student and main menu functions
void rankStudents() {

    string query =
    "SELECT Name, GPA FROM Students ORDER BY GPA DESC";

    mysql_query(conn, query.c_str());

    MYSQL_RES* res = mysql_store_result(conn);

    MYSQL_ROW row;

    int rank = 1;

    cout << "\n========== STUDENT RANKING ==========\n";

    cout << left
         << setw(10) << "RANK"
         << setw(25) << "NAME"
         << setw(15) << "GPA"
         << endl;

    cout << "------------------------------------------------\n";

    while((row = mysql_fetch_row(res))) {

        cout << left
             << setw(10) << rank++
             << setw(25) << row[0]
             << setw(15) << fixed
             << setprecision(2)
             << atof(row[1])
             << endl;
    }

    mysql_free_result(res);
}


void weakSubjects() {

    string query =
    "SELECT Students.Name, Courses.Name, Grades.Score "
    "FROM Grades "
    "JOIN Students ON Grades.SID = Students.ID "
    "JOIN Courses ON Grades.CID = Courses.CID "
    "WHERE Grades.Score < 50";

    mysql_query(conn, query.c_str());

    MYSQL_RES* res = mysql_store_result(conn);

    MYSQL_ROW row;

    cout << "\n========== WEAK SUBJECTS ==========\n";

    while((row = mysql_fetch_row(res))) {

        cout << "Student: "<< row[0]<< " | Course: "<< row[1]<< " | Score: "<< row[2]<< endl;
    }

    mysql_free_result(res);
}



void mainMenu() {

    int choice;

    do {

        cout << "\n====================================\n";
        cout << " STUDENT PERFORMANCE TRACKER\n";
        cout << "====================================\n";

        cout << "1. Add Student\n";
        cout << "2. View Students\n";
        cout << "3. Update Student\n";
        cout << "4. Delete Student\n";
        cout << "5. Search Student\n";
        cout << "6. Add Course\n";
        cout << "7. View Courses\n";
        cout << "8. Record Grade\n";
        cout << "9. View Grades\n";
        cout << "10. Rank Students\n";
        cout << "11. Weak Subjects\n";
        cout << "12. Exit\n";

        cout << "Enter Choice: ";
        cin >> choice;

        switch(choice) {

        case 1:
            addStudent();
            break;

        case 2:
            viewStudents();
            break;

        case 3:
            updateStudent();
            break;

        case 4:
            deleteStudent();
            break;

        case 5:
            searchStudent();
            break;

        case 6:
            addCourse();
            break;

        case 7:
            viewCourses();
            break;

        case 8:
            recordGrade();
            break;

        case 9:
            viewGrades();
            break;

        case 10:
            rankStudents();
            break;

        case 11:
            weakSubjects();
            break;

        case 12:
            cout << "Exiting Program...\n";
            break;

        default:
            cout << "Invalid Choice!\n";
        }

    } while(choice != 12);
}
