#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h> //password
#include <string>
#include <algorithm>
#include <chrono> //delay
#include <thread> //delay
#include <ctime> //time
#include <windows.h> //sound
#include <mmsystem.h> //sound

#pragma comment(lib, "Winmm.lib") //sound

using namespace std;

#define P_Path "P_Data.txt"
#define D_Path "D_Data.txt"
#define a_path "a_data.txt"
#define Hi "hi.wav"
#define Bye "bye.wav"
#define MAX_Session 10
#define Days 7
#define Max_Patient 100
#define Max_Doctor 100

string days[7] = { "sun","mon","tue","wed","thu","fri","sat" };

int spec[Max_Doctor];
int k;
int P_num{ 0 };
int D_num{ 0 };
int index;
int AppFinal = 0; // appointmnet index
int dd, mm, yyyy;
int Ad, Am, Ayyy;
int d;
string day;
string password;
string Pday; //patient day
int c_doctor, c_patient;
int indexOfDay;
int indexOfCDay = -1;

struct Patients
{
    string name;
    string age;
    string gender;
    string email;
    string pass;
}Patient[Max_Patient];

struct Doctors
{
    string name;
    string location;
    string specialization;
    string cost;
    string email;
    string pass;
    bool time_table[Days][MAX_Session];
}Doctor[Max_Doctor];

struct appointments {
    int patient{ -1 };//patient index
    int doctor{ -1 };//doc index
    int hour;
    string day;
    int d;
    int m;
    int y;
}appointment[100];

//sound
void sound(string);

//files
void Add_To_Patient_File(Patients Patient[], string path);
void Read_From_Patient_File(Patients Patient[], string path);
void Read_From_App_File(appointments appointment[], string path);
void Add_To_Doctor_File(Doctors Doctor[], string path);
void Read_from_Doctor_File(Doctors Doctor[], string path);
void Add_to_App_File(appointments appointment[], string path);

//system
string encorypt(string& old);///
string Get_Pass();///
void localtime();///
void Apptime(string s);///
void VEZEETA();
void login();
void sign_up();
string email_check();
string gender_check();
string appStringO(int n);
void appStringI(int n, string app);

//patient
void P_home_page();
void set_app(string s, int);
void display_specialization();
void display_doctors(int choice);
void my_doctor(int i, int l);
void my_doctor_search(int i);
void find_doctor();
void time_search();

//doctor
string spec_name(int choice);
void D_appointment_details();
void specialization_edit();
void D_Edit();
void D_home_page();
void P_appointment_details();



int main()
{
    _CRT_SECURE_NO_WARNINGS;
    sound(Hi);

    localtime();
    Read_From_Patient_File(Patient, P_Path);
    Read_from_Doctor_File(Doctor, D_Path);
    Read_From_App_File(appointment, a_path);

    cout << "\033[33m The current local date is: " << dd << "/" << mm << "/" << yyyy << "\033[0m\n\n";

    VEZEETA();

    return 0;
}
//get the date of the day
void localtime() {

    time_t ttime = time(0);
    tm* local_time = localtime(&ttime);
    yyyy = 1900 + local_time->tm_year;
    mm = 1 + local_time->tm_mon;
    dd = local_time->tm_mday;
    indexOfDay = local_time->tm_wday;
    day = days[indexOfDay];

}

//sound with starting program and ending program
void sound(string s)
{
    MCI_OPEN_PARMS mciOpen;
    MCI_PLAY_PARMS mciPlay;
    DWORD dwFlags;
    UINT wDeviceID;
    DWORD dwReturn;
    TCHAR szDeviceName[MAX_PATH] = { 0 };

    // Open the audio file
    mciOpen.lpstrDeviceType = TEXT("mpegvideo");
    if (s == "hi.wav")
    {
        mciOpen.lpstrElementName = TEXT(Hi);
    }
    else if (s == "bye.wav")
    {
        mciOpen.lpstrElementName = TEXT(Bye);
    }
    mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&mciOpen);

    // Get the device ID and name
    wDeviceID = mciOpen.wDeviceID;
    mciSendCommand(wDeviceID, MCI_INFO, MCI_INFO_PRODUCT, (DWORD_PTR)szDeviceName);

    // Play the audio file
    mciPlay.dwCallback = NULL;
    dwFlags = SND_ASYNC | SND_NODEFAULT;
    mciSendCommand(wDeviceID, MCI_PLAY, dwFlags, (DWORD_PTR)&mciPlay);

    // Stop and close the audio file
    mciSendCommand(wDeviceID, MCI_STOP, NULL, NULL);
    mciSendCommand(wDeviceID, MCI_CLOSE, NULL, NULL);

}

//Files
void Add_To_Patient_File(Patients patient[], string path)
{
    ofstream P_File;
    P_File.open(path, ios::trunc);
    for (int i = 0; i < P_num; i++)
    {
        P_File << Patient[i].name << " " << Patient[i].age << " "
            << Patient[i].gender << " " << Patient[i].email
            << " " << Patient[i].pass << endl;
    }
    P_File.close();
}
void Read_From_Patient_File(Patients Patient[], string path)
{
    ifstream P_FIle(path);
    if (P_FIle.is_open()) {
        while (P_num < Max_Patient && P_FIle >> Patient[P_num].name >>
            Patient[P_num].age >> Patient[P_num].gender >>
            Patient[P_num].email >> Patient[P_num].pass) {
            P_num++;
        }
        P_FIle.close();
    }
}
void Add_To_Doctor_File(Doctors Doctor[], string path)
{
    ofstream D_File;
    D_File.open(path, ios::trunc);
    for (int i = 0; i < D_num; i++)
    {
        D_File << Doctor[i].name << " " << Doctor[i].location << " "
            << Doctor[i].specialization << " " << Doctor[i].cost
            << " " << Doctor[i].email << " " << Doctor[i].pass << " " << appStringO(i) << endl;
    }
    D_File.close();
}
void Read_from_Doctor_File(Doctors doctor[], string path)
{
    string s;
    ifstream D_FIle(path);
    if (D_FIle.is_open()) {
        while (D_num < Max_Patient && D_FIle >> Doctor[D_num].name >>
            Doctor[D_num].location >> Doctor[D_num].specialization >>
            Doctor[D_num].cost >> Doctor[D_num].email >> Doctor[D_num].pass >> s) {

            appStringI(D_num, s);
            D_num++;
        }

        D_FIle.close();
    }
}
void Read_From_App_File(appointments appointment[], string path) {

    ifstream Appointment(path);
    if (Appointment.is_open()) {
        while (Appointment >> appointment[AppFinal].patient >>
            appointment[AppFinal].doctor >> appointment[AppFinal].hour >>
            appointment[AppFinal].day >> appointment[AppFinal].d
            >> appointment[AppFinal].m >> appointment[AppFinal].y)
        {
            AppFinal++;
        }
        Appointment.close();
    }

}
void Add_to_App_File(appointments appointment[], string path) {

    ofstream Appointment;
    Appointment.open(path, ios::trunc);

    for (int i = 0; i < AppFinal; i++)
    {

        if (appointment[i].y == 0) {
            continue;
        }
        Appointment << appointment[i].patient << " " << appointment[i].doctor << " "
            << appointment[i].hour << " " << appointment[i].day
            << " " << appointment[i].d << " " << appointment[i].m << " " << appointment[i].y << endl;

    }
    Appointment.close();
}

//convert timetable array into string
string appStringO(int n) {
    string app;
    for (int i = 0; i < Days; i++)
    {
        for (int j = 0; j < MAX_Session; j++)
        {
            if (Doctor[n].time_table[i][j])app.push_back('1');
            else {
                app.push_back('0');
            }
        }

    }
    return app;
}
void appStringI(int n, string s) {
    int x = 0;
    for (int i = 0; i < Days; i++)
    {
        for (int j = 0; j < MAX_Session; j++)
        {
            Doctor[n].time_table[i][j] = s[x] - '0';
            x++;
        }

    }
}

//system
//contain system structure
void VEZEETA() {
    cout << "  \033[32m Welcome To VEZEETA \033[0m\n"
        << "***************************\n"
        << "How can i help you? \n"
        << "1)Login\n"
        << "2)Sign up\n"
        << "3)Exit\n"
        << "Enter your choice\n";
    int choice;
    cin >> choice;
    system("cls");
    if (choice == 1)
        login();

    else if (choice == 2)
        sign_up();
    else if (choice == 3)
    {

        Add_To_Patient_File(Patient, P_Path);
        Add_To_Doctor_File(Doctor, D_Path);
        Add_to_App_File(appointment, a_path);
        sound(Bye);
        exit(0);
    }
    else {
        cout << "\033[31m Invalid response please try again \033[0m" << endl;
        VEZEETA();
    }
}
//ask user if they are patient or doctor and sign up a new account
void sign_up() {
    string choice;
    string spec;
    cout << "\033[35m Would you like to sign up as a doctor or a patient ?\033[0m" << endl;
    cout << "1) Doctor\n2) Patient" << endl;
    cin >> choice;
    if (choice == "1") {
        cout << "\033[33mName: \033[0m";
        cin >> Doctor[D_num].name;
        cout << "\033[33mLocation: \033[0m";
        cin >> Doctor[D_num].location;
        while (true) {
            cout << "\033[33mWhat is your Specialization ?\n"
                << "1)Pediatrics\n"
                << "2)Gynecology\n"
                << "3)Dentistry\n"
                << "4)Surgery\n"
                << "5)Psychiatry\033[0m\n";
            cin >> spec;
            if (spec == "1") {
                Doctor[D_num].specialization = "Pediatrics";
                break;
            }

            else if (spec == "2") {
                Doctor[D_num].specialization = "Gynecology";
                break;
            }
            else if (spec == "3") {
                Doctor[D_num].specialization = "Dentistry";
                break;
            }
            else if (spec == "4") {
                Doctor[D_num].specialization = "Surgery";
                break;
            }
            else if (spec == "5") {
                Doctor[D_num].specialization = "Psychiatry";
                break;
            }
            else {
                cout << "\033[31mInvalid response please try again \033[0m" << endl;

            }

        }
        cout << "\033[33mCost: \033[0m";
        cin >> Doctor[D_num].cost;
        cout << "\033[33mEmail: \033[0m";

        Doctor[D_num].email = email_check();
        cout << "\033[33mPassword: \033[0m";
        cin >> password;
        Doctor[D_num].pass = encorypt(password);
        cout << "\033[34mSigned up successfully \033[0m\n";
        this_thread::sleep_for(chrono::seconds(1));
        system("cls");
        cout << "\033[35mWe just need some informations about your appointment free time\n please note all times start from 1PM to 9PM \033[0m\n";

        memset(Doctor[D_num].time_table, 0, Days * MAX_Session);


        for (int i = 0; i < Days; i++)
        {
            cout << "Are you available on day " << days[i] << "?\033[31m(y/n)\033[m\n";
            string choice;
            cin >> choice;
            if (choice[0] == 'y' || choice[0] == 'Y') {
                cout << "Please enter your start time and end time in this day:-\033[31m(Forom 1:00 PM to 9:00 PM)\033[0m\nStart: ";
                int start, end;
                cin >> start;
                cout << "End: ";
                cin >> end;

                for (int j = start; j <= end; j++)
                {
                    Doctor[D_num].time_table[i][j] = 1;
                }
            }

        }

        D_num++;

        cout << "\033[34m Everything is done \033[0m\n";
        this_thread::sleep_for(chrono::seconds(1));

        system("cls");
        VEZEETA();
    }
    else if (choice == "2") {
        cout << "\033[32mName: \033[0m";
        cin >> Patient[P_num].name;
        cout << "\033[32mAge: \033[0m";
        cin >> Patient[P_num].age;
        cout << "\033[32mGender\033[31m(M/F)\033[0m: ";
        Patient[P_num].gender = gender_check();
        cout << "\033[32mEmail:\033[0m ";
        Patient[P_num].email = email_check();
        cout << "\033[32mPassword: \033[0m";
        cin >> password;
        Patient[P_num].pass = encorypt(password);
        cout << "\033[34mSign up successfully \033[0m";
        this_thread::sleep_for(chrono::seconds(1));
        P_num++;
        system("cls");
        VEZEETA();
    }
    else {
        cout << "\033[31m Invalid response please try again \033[0m" << endl;
        system("cls");
        sign_up();
    }
}
//ask user for email and password and checks if they are patient or doctor
void login() {
    string email, pass;
    int flag = 0;
    cout << "Email: ";
    cin >> email;
    cout << "Password: ";
    pass = Get_Pass();
    password = encorypt(pass);
    for (int i = 0; i < P_num; i++)
    {
        if (email == Patient[i].email && password == Patient[i].pass) {

            flag = 1;
            index = i;
            c_patient = i;
            break;
        }


    }
    for (int i = 0; i < D_num; i++)
    {
        if (email == Doctor[i].email && password == Doctor[i].pass) {
            flag = 2;
            index = i;
            c_doctor = i;
            break;
        }

    }


    if (flag == 1)
    {
        cout << "\033[32mlogin done as a patient\033[0m\n";
        this_thread::sleep_for(chrono::seconds(1));
        system("cls");
        P_home_page();
    }
    else if (flag == 2) {
        cout << "\033[33mlogin done as a doctor\033[0m\n";
        this_thread::sleep_for(chrono::seconds(1));
        system("cls");
        D_home_page();
    }
    else
    {
        cout << "\033[31mWrong Email or Password\033[0m\n";
        this_thread::sleep_for(chrono::seconds(1));
        system("cls");
        VEZEETA();
    }
}
string email_check() {
    while (true) {
        string m;
        cin >> m;
        bool flag = false;
        for (int i = 0; i <= D_num; i++)
        {
            if (m == Doctor[i].email || m == Patient[i].email)
                flag = true;
        }
        if (flag)
        {
            cout << "\033[31memail taken please enter another \033[0m" << endl;
            continue;
        }
        else {
            return m;
            break;
        }
    }

}
string gender_check() {
    string m;
    while (true)
    {
        cin >> m;
        if (m == "F" || m == "f" || m == "M" || m == "m") {
            return m;
            break;
        }
        else
            cout << "\033[31m invalid answer please try again \033[0m" << endl;

    }
}
//turn charchters into *
string Get_Pass()
{
    string password;
    char c;
    while ((c = _getch()) != '\r') {
        if (c == '\b') {
            if (password.length() > 0) {
                cout << "\b \b";
                password.pop_back();
            }
        }
        else {
            cout << "*";
            password += c;
        }
    }
    cout << endl;
    return password;
}
//encrypts password to save in files and avoid hacking
string encorypt(string& old) {
    string enco;
    for (int i = 0; i < old.size(); i++)
    {
        char y = char(old[i]) * 3 - 50;
        if (y == ' ' || y == '\t') {
            enco.push_back('t');
        }
        else
            enco.push_back(y);
    }
    return enco;
}



//Doctor

//contains structure of doctor home page
void D_home_page() {

    string choice;
    cout << "\033[36mWelcome Dr \033[0m" << Doctor[index].name << endl;
    cout << "*******************" << endl;
    cout << "1)\033[33mProfile\033[0m" << endl;
    cout << "2)\033[33mAppointments\033[0m" << endl;
    cout << "3)\033[33mlog out\033[0m" << endl;
    cin >> choice;
    if (choice == "1")
    {
        system("cls");
        string choice1;
        cout << "\033[33mName :\033[0m" << " " << Doctor[index].name << endl;
        cout << "\033[33mLocation :\033[0m" << " " << Doctor[index].location << endl;
        cout << "\033[33mSpecialization :\033[0m" << " " << Doctor[index].specialization << endl;
        cout << "\033[33mCost :\033[0m" << Doctor[index].cost << endl;
        cout << "1)Edit info" << endl;
        cout << "2)Back \033[31m(Press any key)\033[0m" << endl;
        cin >> choice1;
        if (choice1 == "1")
        {
            system("cls");
            D_Edit();
        }
        else
        {
            system("cls");
            D_home_page();
        }
    }
    else if (choice == "2")
    {
        D_appointment_details();
    }
    else if (choice == "3")
    {
        system("cls");
        VEZEETA();
    }
    else {
        cout << "\033[31m Invalid response try again \033[0m" << endl;
        system("cls");
        this_thread::sleep_for(chrono::seconds(1));
        D_home_page();
    }
}
//Edit doctor's info
void D_Edit()
{
    int choice;
    string check = "y";
    string local_pass;
    while ( check == "Y" || check == "y")
    {
        cout << "do you want to edit :" << endl;
        cout << "1)\033[33mName\033[0m" << endl;
        cout << "2)\033[33mLocation\033[0m" << endl;
        cout << "3)\033[33mSpicialization\033[0m" << endl;
        cout << "4)\033[33mCost\033[0m" << endl;
        cout << "5)\033[33mPassword\033[0m" << endl;
        cin >> choice;
        system("cls");
        switch (choice)
        {
        case 1:
            cout << "\033[33mEnter New Name :\033[0m" << " ";
            cin >> Doctor[index].name;
            break;
        case 2:
            cout << "\033[33mEnter new Location :\033[0m" << " ";
            cin >> Doctor[index].location;
            break;
        case 3:
            specialization_edit();
            break;
        case 4:
            cout << "\033[33mEnter new cost :\033[0m" << " ";
            cin >> Doctor[index].cost;
            break;
        case 5:
            while (true)
            {


                cout << "\033[33mEnter current password: \033[0m";
                cin >> local_pass;
                string pass = encorypt(local_pass);
                if (pass == Doctor[index].pass)
                {
                    cout << "\033[33mEnter new Password:\033[0m";
                    cin >> Doctor[index].pass;
                    Doctor[index].pass = encorypt(Doctor[index].pass);
                    break;
                }
                else
                {
                    cout << "\033[31m Invalid Password \033[0m" << endl;
                }
            }

            break;
        default:
            return;
            break;
        }
        cout << "\033[34mChanging done !\033[0m" << endl;
        cout << "**************" << endl;
        cout << "Do you want another change\033[31m(y/n)\033[0m" << endl;
        cin >> check;
        system("cls");
    }
    system("cls");
    D_home_page();
}
//enable to edit doctor spec
void specialization_edit()
{
    string spec;

    while (true) {
        cout << "\033[33mWhat is your new Specialization ?\033[0m\n"
            << "1)\033[33mPediatrics\033[0m\n"
            << "2)\033[33mObstetrics and Gynecology\033[0m\n"
            << "3)\033[33mDentistry\033[0m\n"
            << "4)\033[33mSurgery\033[0m\n"
            << "5)\033[33mPsychiatry\033[0m\n";
        cin >> spec;
        if (spec == "1") {
            Doctor[D_num].specialization = "Pediatrics";
            break;

        }
        else if (spec == "2") {
            Doctor[D_num].specialization = "Gynecology";
            break;

        }
        else if (spec == "3") {
            Doctor[D_num].specialization = "Dentistry";
            break;


        }
        else if (spec == "4") {
            Doctor[D_num].specialization = "Surgery";
            break;


        }
        else if (spec == "5") {
            Doctor[D_num].specialization = "Psychiatry";
            break;

        }
        else {
            cout << "\033[31m Invalid response please try again\033[0m" << endl;

        }
    }
}
//replace number of spec to a string of a spec name
string spec_name(int choice) {

    switch (choice) {
    case(1):
        return "Pediatrics";
    case(2):
        return "Gynecology";
    case(3):
        return "Dentistry";
    case(4):
        return "Surgery";
    case(5):
        return "Psychiatry";
    }
}
//enable to access the doctor app.
void D_appointment_details() {
    system("cls");
    string choice;
    cout << "\033[33mWhat do you want to do?\033[0m" << endl;
    cout << "1)\033[33mAdd available times\033[0m" << endl;
    cout << "2)\033[33mEdit available times\033[0m" << endl;
    cout << "3)\033[33mRemove available times\033[0m" << endl;
    cout << "4)\033[33mView patients with appointments\033[0m" << endl;
    cout << "5)back" << endl;
    cin >> choice;

    if (choice == "1") {
        system("cls");

        for (int i = 0; i < Days; i++)
        {
            cout << "\033[33mAre you available on day ?\033[0m " << days[i] << "\033[31m(y/n)\033[0m\n";
            char choice;
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                cout << "\033[33mPlease enter your start time and end time in this day:-\nStart: \033[0m";
                int start, end;
                cin >> start;
                cout << "\033[33mEnd: \033[0m";
                cin >> end;
                
                for (int j = start; j <= end; j++)
                {
                    Doctor[index].time_table[i][j] = 1;
                }
                /*
                *0 0 0 0 0 0 0 0 0 
                 1 2 3 4 5 6 7 8 9   interval 
                     2->6
                 0 1 1 1 1 1 0 0 0 
                */
            }

        }
        cout << "\033[34mEdit done succesfully!\033[34m";
        this_thread::sleep_for(chrono::seconds(1));
        system("cls");
        D_appointment_details();
    }
    else if (choice == "2") {
        system("cls");
        while (true) {
            int k = 1, l[70], m[70], choice, x, y;
            string s, cont;
            for (int i = 0; i < Days; i++) {
                for (int j = 0; j < MAX_Session; j++) {
                    if (Doctor[index].time_table[i][j] == 1) {
                        cout << k << ") " << days[i] << " " << j << "PM" << endl;
                        l[k - 1] = i;
                        m[k - 1] = j;
                        k++;
                    }

                }
            }
            cout << "\033[33mWhich appointment time do you want to edit:\033[0m ";
            cin >> choice;
            if (choice <= k) {
                k = choice;
                x = l[k - 1];
                y = m[k - 1];
                Doctor[index].time_table[x][y] = 0;
                cout << "\033[33mEnter new day:\033[0m ";
                cin >> s;
                cout << "\033[33mEnter new hour:\033[31m(form 1 PM to 9 PM) \033[0m";
                cin >> y;
                for (int z = 0; z < 7; z++)
                {
                    if (s == days[z]) {
                        x = z;
                    }
                }
                Doctor[index].time_table[x][y] = 1;
                cout << "\033[34mEdit done succesfully !\033[0m" << endl;

            }
            cout << "Do you want to edit another one ?\033[31m(y/n)\033[0m" << endl;
            cin >> cont;
            if (cont == "y" || cont == "Y") {
                system("cls");
                continue;
            }
            else {

                system("cls");
                D_appointment_details();
                break;
            }
        }

    }
    else if (choice == "3") {
        system("cls");

        while (true) {
            int k = 1, l[70], m[70], choice, x, y;
            string cont;
            for (int i = 0; i < Days; i++) {
                for (int j = 0; j < MAX_Session; j++) {
                    if (Doctor[index].time_table[i][j] == 1) {
                        cout << k << ") " << days[i] << " " << j << "PM" << endl;
                        l[k - 1] = i;
                        m[k - 1] = j;
                        k++;
                    }

                }
            }
            cout << "\033[33mWhich appointment time do you want to delete:\033[0m";
            cin >> choice;
            if (choice <= k) {
                k = choice;
                x = l[k - 1];
                y = m[k - 1];
                Doctor[index].time_table[x][y] = 0;
                cout << "\033[34mAppointment time deleted successfully !\033[0m" << endl;

            }
            cout << "Do you want to delete another one ?\033[31m(y/n)\033[0m" << endl;
            cin >> cont;
            if (cont == "y" || cont == "Y") {
                system("cls");
                continue;
            }
            else {

                system("cls");
                D_appointment_details();
                break;
            }
        }

    }
    else if (choice == "4") {
        system("cls");
        int k = 1;
        for (int i = 0; i < MAX_Session; i++) {
            if (appointment[i].doctor == index) {
                cout << k++ << ") \033[33mname:\033[0m " << Patient[appointment[i].patient].name
                    << "\033[33m age:\033[0m " << Patient[appointment[i].patient].age << " ";
                if (Patient[appointment[i].patient].gender == "m")
                    cout << " -> Male";
                else if (Patient[appointment[i].patient].gender == "f")
                    cout << " -> Female";
                cout << " \033[33mon:\033[0m " << appointment[i].day
                    << " \033[33mat:\033[0m " << appointment[i].hour << endl;
            }
        }
        system("pause");
        system("cls");
        D_appointment_details();
    }
    else if (choice == "5") {
        system("cls");
        D_home_page();
    }
    else {
        cout << "\033[31mInvalid try again\033[0m";
    }
}

//Patient   
void P_home_page()
{
    system("cls");
    string choice;
    cout << "\033[36mWelcome ";
    if (Patient[index].gender == "M" || Patient[index].gender == "m")
        cout << "Mr. \033[0m" << Patient[index].name << endl;
    else
        cout << "Mrs. \033[0m" << Patient[index].name << endl;
    cout << "*******************" << endl;
    cout << "1)\033[32mProfile\033[0m" << endl;
    cout << "2)\033[32mdisplay specializations\033[0m" << endl;
    cout << "3)\033[32mfind doctor\033[0m" << endl;
    cout << "4)\033[32mSearch by time\033[0m" << endl;
    cout << "5)\033[32mAppointments\033[0m" << endl;
    cout << "6)\033[32mlog out\033[0m" << endl;
    cin >> choice;
    if (choice == "1")
    {
        system("cls");
        string choice1;
        cout << "\033[32mName :\033[0m" << " " << Patient[index].name << endl;
        cout << "\033[32mAge :\033[0m" << " " << Patient[index].age << endl;
        cout << "\033[32mGender :\033[0m" << " ";
        if (Patient[index].gender == "m")
            cout << "Male\n";
        else if (Patient[index].gender == "f")
            cout << "Female\n";
        cout << "1)Edit info " << endl;
        cout << "2)Back" << endl;
        cin >> choice1;
        system("cls");
        if (choice1 == "1")
        {
            int choice2;
            string local_pass;
            bool check2 = 0;
            string check = "1";
            while (check == "y" || check == "Y" || check == "1")
            {
                cout << "do you want to edit :" << endl;
                cout << "1)\033[32mName\033[0m" << endl;
                cout << "2)\033[32mAge\033[0m" << endl;
                cout << "3)\033[32mGender\033[0m" << endl;
                cout << "4)\033[32mPassword\033[0m" << endl;
                cin >> choice2;
                system("cls");
                switch (choice2)
                {
                case 1:
                    cout << "\033[32m Enter new Name: \033[0m";
                    cin >> Patient[index].name;
                    break;
                case 2:
                    cout << "\033[32m Enter new Age: \033[0m";
                    cin >> Patient[index].age;
                    break;
                case 3:
                    cout << "\033[32m Enter new Gender: \033[0m";
                    cin >> Patient[index].gender;
                    break;
                case 4:

                    while (true)
                    {
                        cout << "\033[32m Enter current Password:\033[0m";
                        cin >> local_pass;
                        string pass = encorypt(local_pass);
                        if (pass == Patient[index].pass)
                        {

                            cout << "\033[32mEnter new Password:\033[0m";
                            cin >> Patient[index].pass;
                            Patient[index].pass = encorypt(Patient[index].pass);
                            break;

                        }
                        else
                        {
                            cout << "\033[31m Invalid Password" << endl;
                            cout << "Please try again\033[0m" << endl;
                        }
                    }
                    break;
                }
                cout << "\033[34mChanging done !\033[0m" << endl;
                cout << "**************" << endl;
                cout << "Do you want another change \033[31m(y/n)\033[0m" << endl;
                cin >> check;
                system("cls");

            }
            system("cls");
            P_home_page();
        }
        else
        {
            system("cls");
            P_home_page();
        }
    }
    else if (choice == "2")
    {
        system("cls");
        display_specialization();

    }
    else if (choice == "3")
    {

        system("cls");
        find_doctor();
    }
    else if (choice == "4")
    {
        system("cls");
        time_search();

    }
    else if (choice == "5")
    {
        system("cls");
        P_appointment_details();

    }
    else if (choice == "6")
    {
        system("cls");
        VEZEETA();

    }
    else {
        cout << "\033[31m Invalid response try again \033[0m" << endl;
        system("cls");
        this_thread::sleep_for(chrono::seconds(1));
        P_home_page();
    }

}
//represent spec to choose one
void display_specialization() {
    while (true) {
        cout << "\033[36mdisplay specialization:\033[0m" << endl;
        cout << "***********************" << endl;
        cout << "1)\033[32mPediatrics\033[0m\n"
            << "2)\033[32mGynecology\033[0m\n"
            << "3)\033[32mDentistry\033[0m\n"
            << "4)\033[32mSurgery\033[0m\n"
            << "5)\033[32mPsychiatry\033[0m\n" << "6)back\n";
        int choice;
        cin >> choice;
        if (choice >= 1 && choice <= 5)
        {
            system("cls");
            display_doctors(choice);

        }
        else if (choice == 6) {
            system("cls");
            P_home_page();
        }
        else {
            cout << "\033[31m invalid choice try again \033[0m\n";
            this_thread::sleep_for(chrono::seconds(1));
            system("cls");
        }
    }
}
//represent doctors with the same spec
void display_doctors(int choice) {
    int choice_2 = choice;
    cout << "write your choice:" << endl
        << "*******************" << endl;
    int l = 1;
    int cntD = 0;

    for (int i = 0; i <= D_num; i++) {
        if (Doctor[i].specialization == spec_name(choice)) {
            spec[cntD] = i;
            cntD++;
        }
    }

    for (int i = 0; i < cntD; i++)
    {
        cout << l << ")\033[32m" << Doctor[spec[i]].name << "\033[0m\n";
        l++;
    }

    cout << l << ")back" << endl;

    cin >> k;
    if (k == l) {
        system("cls");
        display_specialization();
    }
    else {
        system("cls");
        c_doctor = spec[k - 1];
        my_doctor(spec[k - 1], choice_2);
    }
}
//represent doctor info and enable to book
void my_doctor(int i, int l) {
    int choice;
    cout << "\033[32mName :\033[0m" << " " << Doctor[i].name << endl;
    cout << "\033[32mLocation :\033[0m" << " " << Doctor[i].location << endl;
    cout << "\033[32mSpecialization :\033[0m" << " " << Doctor[i].specialization << endl;
    cout << "\033[32mCost :\033[0m" << Doctor[i].cost << endl;
    cout << "1)\033[36mBook\033[0m\n2)back\n";
    cin >> choice;
    string Pday; //patient day
    switch (choice) {
    case(1):
        system("cls");
        cout << "\033[32mplease enter the day as (mon or fri) knowing that you are booking in the next 6 days: \033[0m";
        cin >> Pday;
        system("cls");
        set_app(Pday, i);
        break;
    case(2):
        system("cls");
        display_doctors(l);
    }
}
//represent app and enable to book one
void set_app(string s, int ind) {
    for (int i = 0; i < 7; i++)
    {
        if (s == days[i]) {
            indexOfCDay = i;
        }
    }
    if (indexOfCDay == -1) {
        cout << "\033[31mInvalid day\nPlease try again\033[0m\n";
        this_thread::sleep_for(chrono::seconds(1));

        system("cls");
        c_doctor = spec[k - 1];
        my_doctor_search(ind);
        return;
    }

    Apptime(s);

    int j{ 1 };
    int Fhours[MAX_Session + 1];


    for (int i = 0; i < MAX_Session; i++)
    {
        if (Doctor[ind].time_table[indexOfCDay][i])
        {
            cout << j++ << ")\033[32mFrom \033[0m" << i <<
                ":00 PM \033[32mto \033[0m" << i << ":50 PM\n";
            Fhours[j - 1] = i;
        }
        else
            continue;
    }
    cout << j << ")Back\n";
    int choise;
    cin >> choise;
    if (choise == j)
        my_doctor_search(spec[k - 1]);

    int h = Fhours[choise];

    Doctor[ind].time_table[indexOfCDay][h] = false;
    cout << "\033[34mThanks for using VEZEETA\nYou have book successfully on \033[0m"
        << s << " " << Ad << "\033[34m/\033[0m" << Am << "\033[34m/\033[0m" << Ayyy
        << " \033[34mat\033[0m " << h << " PM\n";
    appointment[AppFinal] = { c_patient,c_doctor,h,s,Ad,Am,Ayyy };
    AppFinal++;
    system("pause");
    system("cls");
    P_home_page();
}
//book a spacific app  
void my_doctor_search(int i) {
    int choice;
    cout << "\033[32mName :\033[0m" << " " << Doctor[i].name << endl;
    cout << "\033[32mLocation :\033[0m" << " " << Doctor[i].location << endl;
    cout << "\033[32mSpecialization :\033[0m" << " " << Doctor[i].specialization << endl;
    cout << "\033[32mCost :\033[0m" << Doctor[i].cost << endl;
    cout << "1)\033[36mBook\033[0m\n2)back\n";
    cin >> choice;
    switch (choice) {
    case(1):
        system("cls");
        cout << "\033[32mplease enter the day as \033[31m(mon or fri)\033[32m knowing that you are booking in the next 6 days: \033[0m";
        cin >> Pday;
        system("cls");
        set_app(Pday, i);

        break;
    case(2):
        system("cls");
        P_home_page();
    }
}
//represent the acually date of day the patient will come on it
void Apptime(string s) {
    int change;
    for (int i = 0; i < 7; i++)
    {
        if (s == days[i]) {
            change = i;
        }
    }

    int f = (change - indexOfDay + 7) % 7;

    time_t ttime = time(0) + 24 * 60 * 60 * f;
    tm* local_time = localtime(&ttime);

    Ayyy = 1900 + local_time->tm_year;
    Am = 1 + local_time->tm_mon;
    Ad = local_time->tm_mday;
    int i = local_time->tm_wday;
}
//search for a doctor by his name
void find_doctor() {
    string d;

    cout << "\033[32mWrite the doctor name:\033[0m\n" << "********************\n";
    cin >> d;
    for (int i = 0; i <= D_num; i++) {
        if (Doctor[i].name == d) {
            system("cls");
            my_doctor_search(i);

        }
    }
    cout << "\033[31mYour doctor name doesn't exist\033[0m\n";
    this_thread::sleep_for(chrono::seconds(2));
    P_home_page();
}
//enable to access the app
void P_appointment_details() {

    while (true) {
        cout << "1)\033[32mView appointments\033[0m" << endl;
        cout << "2)\033[32mEdit appointments\033[0m" << endl;
        cout << "3)\033[32mDelete appointments\033[0m" << endl;
        cout << "4)\033[32mClear appointment history\033[0m" << endl;
        cout << "5)Back" << endl;
        string choice;
        cin >> choice;
        if (choice == "1") {
            system("cls");
            int k = 1;
            for (int i = 0; i < 100; i++) {
                if (appointment[i].d == NULL || appointment[i].hour == NULL)
                    continue;
                if (appointment[i].patient == index) {
                    cout << k << ") \033[32mDr:\033[0m " << Doctor[appointment[i].doctor].name
                        << " \033[32mspecialty:\033[0m " << Doctor[appointment[i].doctor].specialization
                        << " \033[32mon:\033[0m " << appointment[i].day
                        << " \033[32mat:\033[0m " << appointment[i].hour << endl;
                    k++;
                }
            }
            system("pause");
            system("cls");
            P_appointment_details();
        }
        else if (choice == "2") {

            int k = 1, a[100], choice, l, f;
            system("cls");
            for (int i = 0; i < MAX_Session; i++) {
                if (appointment[i].patient == index) { // ziada////

                    cout << k << ") " << appointment[i].day << "\033[32m->\033[0m "
                        << appointment[i].hour << " \033[32mDr:\033[0m " << Doctor[appointment[i].doctor].name
                        << " \033[32mspecialty:\033[0m " << Doctor[appointment[i].doctor].specialization << endl;
                    a[k - 1] = i;
                    k++;
                }
            }
            while (true) {
                cout << "which appoinment do you want to edit? \n";
                cin >> choice;
                if (choice <= k) {
                    k = choice;
                    l = a[k - 1];
                    f = appointment[l].doctor;
                    for (int i = 0; i < 7; i++)
                    {
                        if (appointment[l].day == days[i])
                        {
                            Doctor[f].time_table[i][appointment[l].hour] = "1";
                            break;
                        }
                    }
                    appointment[l].d = NULL;
                    appointment[l].day.clear();
                    appointment[l].doctor = NULL;
                    appointment[l].hour = NULL;
                    appointment[l].m = NULL;
                    appointment[l].patient = NULL;
                    appointment[l].y = NULL;
                    system("cls");
                    my_doctor_search(f);
                }
                else if (choice > k) {
                    cout << "\033[31m Invalid response please try again \033[0m" << endl;
                    continue;
                }

            }
        }
        else if (choice == "3") {
            system("cls");
            int k = 1, a[100], choice, l;
            system("cls");
            for (int i = 0; i < MAX_Session; i++) {
                if (appointment[i].patient == index) { // ziada/////

                    cout << k << ")\033[32m " << appointment[i].day << "\033[0m "
                        << appointment[i].hour << " \033[32mDr:\033[0m " << Doctor[appointment[i].doctor].name
                        << " \033[32mspecialty:\033[0m " << Doctor[appointment[i].doctor].specialization << endl;
                    a[k - 1] = i;

                    k++;
                }
            }
            while (true) {
                cout << "which appoinment do you want to delete? \n";
                cin >> choice;
                if (choice <= k) {
                    k = choice;
                    l = a[k - 1];
                    int f = appointment[l].doctor;
                    for (int i = 0; i < 7; i++)
                    {
                        if (appointment[l].day == days[i])
                        {
                            Doctor[f].time_table[i][appointment[l].hour] = "1";
                            break;
                        }
                    }
                    appointment[l].d = NULL;
                    appointment[l].day.clear();
                    appointment[l].doctor = NULL;
                    appointment[l].hour = NULL;
                    appointment[l].m = NULL;
                    appointment[l].patient = NULL;
                    appointment[l].y = NULL;
                    system("cls");
                    P_home_page();
                }
                else if (choice > k) {
                    cout << "\033[31m Invalid response please try again \033[0m" << endl;
                    continue;
                }

            }
        }
        else if (choice == "4")
        {
            system("cls");
            for (int i = 0; i < 100; i++)
            {
                if (appointment[i].patient == index)
                {
                    int f = appointment[i].doctor;
                    for (int j = 0; j < 7; j++)
                    {
                        if (appointment[i].day == days[j])
                        {
                            Doctor[f].time_table[j][appointment[i].hour] = "1";
                            break;
                        }
                    }
                    appointment[i].d = NULL;
                    appointment[i].day.clear();
                    appointment[i].doctor = NULL;
                    appointment[i].hour = NULL;
                    appointment[i].m = NULL;
                    appointment[i].patient = NULL;
                    appointment[i].y = NULL;
                }
            }
        }
        else if (choice == "5") {
            system("cls");
            P_home_page();
        }
        else {
            cout << "\033[31m Invalid response please try again \033[0m" << endl;
            system("cls");
            continue;
        }

    }

}
//search by time
void time_search() {
    cout << "\033[32mplease enter the day as (mon or fri) knowing that you are booking in the next 6 days: \033[0m";
    cin >> Pday;
    for (int i = 0; i < 7; i++)
    {
        if (Pday == days[i]) {
            indexOfCDay = i;
        }
    }
    if (indexOfCDay == -1) {
        cout << "\033[31m Invalid day\nPlease try again \033[0m";
        this_thread::sleep_for(chrono::seconds(1));

        system("cls");
        time_search();
        return;
    }
    int hours;
    cout << "\033[32mplease enter the hour you want to book in the chosen day: \033[0m";
    cin >> hours;
    system("cls");
    int k = 1;
    bool found{ 0 };
    int a[100];
    for (int i = 0; i < 100; i++)
    {

        if (Doctor[i].time_table[indexOfCDay][hours] == 1) {
            cout << k << ") \033[32mname:\033[0m " << Doctor[i].name
                << " \033[32mlocation:\033[0m " << Doctor[i].location << " \033[32mcost: \033[0m"
                << Doctor[i].cost << " \033[32mSpecialty:\033[0m " << Doctor[i].specialization << endl;
            a[k - 1] = i;
            k++;
            found = 1;
        }
    }
    if (found == 0) {
        cout << "\033[31mTime not available for any doctor\033[0m" << endl;
        this_thread::sleep_for(chrono::seconds(2));
        P_home_page();
    }
    int choise;
    while (true)
    {
        cout << "\033[32mPick a doctor to book\033[0m" << endl;
        cin >> choise;
        if (choise > 0 && choise < k)
        {
            Apptime(Pday);

            c_patient = index;
            c_doctor = a[choise - 1];
            Doctor[choise-1].time_table[indexOfCDay][hours] = false;
            cout << "\033[34mThanks for using VEZEETA\nYou have book successfully on\033[0m "
                << Pday << " " << Ad << "\033[34m/\033[0m" << Am
                << "\033[34m/\033[0m" << Ayyy << " \033[34mat\033[0m " << hours << " PM\n";
            appointment[AppFinal] = { c_patient,c_doctor,hours,Pday,Ad,Am,Ayyy };
            AppFinal++;
            system("pause");
            system("cls");
            P_home_page();
        }
        else
        {
            cout << "\033[31mInvalid choise try again\033[0m\n";
        }
    }
}