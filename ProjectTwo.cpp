/* Dylan Brown
*  SNHU-300
*  Professor Lebel
*  6/15/2026
*/

// Essential #includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Course structure stores each course number, title, and prerequisites
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Trim spaces from beginning and end of a string
string trim(string str) {
    while (!str.empty() && isspace(str.front())) {
        str.erase(str.begin());
    }

    while (!str.empty() && isspace(str.back())) {
        str.pop_back();
    }

    return str;
}

// Convert course number input to uppercase
string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Load course data from file into a map
void loadCourses(string fileName, map<string, Course>& courses) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return;
    }

    courses.clear();

    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string item;
        vector<string> tokens;

        // Split line by comma
        while (getline(ss, item, ',')) {
            tokens.push_back(trim(item));
        }

        // Each course needs at least course number and title
        if (tokens.size() < 2) {
            cout << "Warning: Skipping invalid line: " << line << endl;
            continue;
        }

        Course course;
        course.courseNumber = toUpperCase(tokens[0]);
        course.courseTitle = tokens[1];

        // Add prerequisites if they exist
        for (int i = 2; i < tokens.size(); i++) {
            course.prerequisites.push_back(toUpperCase(tokens[i]));
        }

        courses[course.courseNumber] = course;
    }

    file.close();

    // Check that prerequisites exist in the course list
    for (auto const& pair : courses) {
        Course course = pair.second;

        for (string prereq : course.prerequisites) {
            if (courses.find(prereq) == courses.end()) {
                cout << "Warning: Prerequisite " << prereq
                    << " for " << course.courseNumber
                    << " was not found in the file." << endl;
            }
        }
    }

    cout << "Course data loaded successfully." << endl;
}

// Print all courses in alphanumeric order
void printCourseList(map<string, Course>& courses) {
    if (courses.empty()) {
        cout << "Error: Please load the course data first." << endl;
        return;
    }

    cout << endl;
    cout << "Here is a sample schedule:" << endl;
    cout << endl;

    for (auto const& pair : courses) {
        cout << pair.second.courseNumber << ", "
            << pair.second.courseTitle << endl;
    }
}

// Print information for one course
void printCourseInformation(map<string, Course>& courses) {
    if (courses.empty()) {
        cout << "Error: Please load the course data first." << endl;
        return;
    }

    string courseNumber;

    cout << "What course do you want to know about? ";
    cin >> courseNumber;

    courseNumber = toUpperCase(courseNumber);

    if (courses.find(courseNumber) == courses.end()) {
        cout << "Error: Course not found." << endl;
        return;
    }

    Course course = courses[courseNumber];

    cout << endl;
    cout << course.courseNumber << ", " << course.courseTitle << endl;

    cout << "Prerequisites: ";

    if (course.prerequisites.empty()) {
        cout << "None" << endl;
    }
    else {
        for (int i = 0; i < course.prerequisites.size(); i++) {
            string prereqNumber = course.prerequisites[i];

            cout << prereqNumber;

            // Print prerequisite title if it exists
            if (courses.find(prereqNumber) != courses.end()) {
                cout << " - " << courses[prereqNumber].courseTitle;
            }

            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }

        cout << endl;
    }
}

// Display menu
void displayMenu() {
    cout << endl;
    cout << "Menu:" << endl;
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "9. Exit" << endl;
    cout << "What would you like to do? ";
}

int main() {
    map<string, Course> courses;
    int choice = 0;
    string fileName;

    cout << "Welcome to the ABCU Advising Assistance Program." << endl;

    while (choice != 9) {
        displayMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            fileName = "CS 300 ABCU_Advising_Program_Input.csv";
            loadCourses(fileName, courses);
            break;

        case 2:
            printCourseList(courses);
            break;

        case 3:
            printCourseInformation(courses);
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}