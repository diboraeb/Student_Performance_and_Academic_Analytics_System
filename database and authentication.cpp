//database(mysql)connector code called in different functions and the main function
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

int main() {

    connectDatabase();

    if(login()) {

        mainMenu();
    }

    mysql_close(conn);

    return 0;
}
