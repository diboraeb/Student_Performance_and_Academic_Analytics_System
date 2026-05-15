#include <iostream>
#include <mysql.h>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cstdlib>

using namespace std;

// we define all the structures

struct Student {
    int id;
    string name;
    int age;
    string department;
    double gpa;
};

struct Course {
    int courseId;
    string courseName;
    int creditHours;
};

struct Grade {
    int studentId;
    int courseId;
    double score;
};

//defining the global variables

MYSQL* conn;

string systemUsername = "admin";
string systemPassword = "1234";

//establish the database connection

void connectDatabase() {

    conn = mysql_init(NULL);

    if(conn == NULL) {

        cout << "MySQL Initialization Failed!\n";
        exit(1);
    }

    conn = mysql_real_connect(conn,"localhost","root","","university_db",3306,NULL,0);

    if(conn) {

        cout << "MySQL Connected Successfully!\n";
    }

    else {

        cout << "Connection Failed: "<< mysql_error(conn)<< endl;

        exit(1);
    }
}

//login function

bool login() {

    string username;
    string password;

    int attempts = 0;

    while(attempts < 3) {

        cout << "\n========== LOGIN ==========\n";

        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> password;
        if(username == systemUsername &&
           password == systemPassword) {

            cout << "\nLogin Successful!\n";
            return true;
        }

        else {

            cout << "Invalid Credentials!\n";
            attempts++;
        }
    }

    return false;
}

// helper functions that we use to check if input student and course id exists

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

bool courseExists(int id) {

    string query ="SELECT * FROM Courses WHERE CID = " + to_string(id);

    if(mysql_query(conn, query.c_str()) != 0) {

        return false;
    }

    MYSQL_RES* res = mysql_store_result(conn);

    bool exists = mysql_num_rows(res) > 0;

    mysql_free_result(res);

    return exists;
}

//for automatic gpa update
void updateStudentGPA(int studentId) {

    string query = "SELECT Score FROM Grades WHERE SID = " + to_string(studentId);

    if(mysql_query(conn, query.c_str()) != 0) {

        cout << "Error Calculating GPA!\n";
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);

    MYSQL_ROW row;

    double totalPoints = 0.0;
    int totalCourses = 0;

    while((row = mysql_fetch_row(res))) {

        double score = atof(row[0]);

        double gradePoint;

        if(score >= 90)
            gradePoint = 4.0;

        else if(score >= 80)
            gradePoint = 3.5;

        else if(score >= 70)
            gradePoint = 3.0;

        else if(score >= 60)
            gradePoint = 2.5;

        else if(score >= 50)
            gradePoint = 2.0;

        else
            gradePoint = 0.0;

        totalPoints += gradePoint;
        totalCourses++;
    }

    mysql_free_result(res);

    double gpa = 0.0;

    if(totalCourses > 0) {

        gpa = totalPoints / totalCourses;
    }

    string updateQuery ="UPDATE Students SET GPA = " +to_string(gpa) +" WHERE ID = " +to_string(studentId);

    mysql_query(conn, updateQuery.c_str());
}

//function to add student information and store to the database

void addStudent() {

    Student s;

    cout << "\n========== ADD STUDENT ==========\n";

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

//function to view students information

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

// function to search for students by using id

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
        cout << "GPA: " << fixed
             << setprecision(2)
             << atof(row[4])
             << endl;
    }

    else {

        cout << "Student Not Found!\n";
    }

    mysql_free_result(res);
}

// function to add course and store in the database

void addCourse() {

    Course c;

    cout << "\n========== ADD COURSE ==========\n";

    cout << "Enter Course ID: ";
    cin >> c.courseId;

    if(courseExists(c.courseId)) {

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

    if(mysql_query(conn, query.c_str()) == 0) {

        cout << "Course Added Successfully!\n";
    }

    else {

        cout << "Error: "<< mysql_error(conn)<< endl;
    }
}

//function to view course information

void viewCourses() {

    string query = "SELECT * FROM Courses";

    mysql_query(conn, query.c_str());

    MYSQL_RES* res = mysql_store_result(conn);

    MYSQL_ROW row;

    cout << "\n========== COURSES ==========\n";

    cout << left
         << setw(15) << "COURSE ID"
         << setw(30) << "COURSE NAME"
         << setw(15) << "CREDITS"
         << endl;

    cout << "----------------------------------------------------------\n";

    while((row = mysql_fetch_row(res))) {

        cout << left
             << setw(15) << row[0]
             << setw(30) << row[1]
             << setw(15) << row[2]
             << endl;
    }

    mysql_free_result(res);
}

//function to record a student's grade of course

void recordGrade() {

    Grade g;

    cout << "\n========== RECORD GRADE ==========\n";

    cout << "Enter Student ID: ";
    cin >> g.studentId;

    if(!studentExists(g.studentId)) {

        cout << "Student Not Found!\n";
        return;
    }

    cout << "Enter Course ID: ";
    cin >> g.courseId;

    if(!courseExists(g.courseId)) {

        cout << "Course Not Found!\n";
        return;
    }

    cout << "Enter Score: ";
    cin >> g.score;

    string query =
    "INSERT INTO Grades(SID, CID, Score) VALUES("
    + to_string(g.studentId) + ", "
    + to_string(g.courseId) + ", "
    + to_string(g.score) + ")";

    if(mysql_query(conn, query.c_str()) == 0) {

        updateStudentGPA(g.studentId);

        cout << "Grade Recorded Successfully!\n";
        cout << "GPA Updated Automatically!\n";
    }

    else {

        cout << "Error: "<< mysql_error(conn)<< endl;
    }
}

//function to view grades of every students and of every courses

void viewGrades() {

    string query = "SELECT * FROM Grades";

    mysql_query(conn, query.c_str());

    MYSQL_RES* res = mysql_store_result(conn);

    MYSQL_ROW row;

    cout << "\n========== GRADES ==========\n";

    cout << left
         << setw(15) << "STUDENT ID"
         << setw(15) << "COURSE ID"
         << setw(15) << "SCORE"
         << endl;

    cout << "---------------------------------------------\n";

    while((row = mysql_fetch_row(res))) {

        cout << left
             << setw(15) << row[0]
             << setw(15) << row[1]
             << setw(15) << row[2]
             << endl;
    }

    mysql_free_result(res);
}

//function that ranks students based on their gpa

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

//function that shows weak subject or <50 average score of students
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


// main menu function


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

//main function

int main() {

    connectDatabase();

    if(login()) {

        mainMenu();
    }

    mysql_close(conn);

    return 0;
}
