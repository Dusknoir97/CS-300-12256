/* Dylan Brown
*  SNHU-300
*  Professor Lebel
*  6/15/2026
*/

// Essential #includes for input/output, files, strings, vectors, maps, and sorting
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Course structure stores the course number, title, and list of prerequisites
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Removes extra spaces from the front and back of a string
string trim(string str) {
    while (!str.empty() && isspace(str.front())) {
        str.erase(str.begin());
    }

    while (!str.empty() && isspace(str.back())) {
        str.pop_back();
    }

    return str;
}

// Converts course numbers to uppercase so user input matches the file data
string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Reads the course data file and stores each course in the map
void loadCourses(string fileName, map<string, Course>& courses) {
    ifstream file(fileName);

    // If the file cannot be opened, show an error and stop loading
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return;
    }

    // Clear old course data before loading new data
    courses.clear();

    string line;

    // Read the file one line at a time
    while (getline(file, line)) {
        stringstream ss(line);
        string item;
        vector<string> tokens;

        // Separate each line by commas
        while (getline(ss, item, ',')) {
            tokens.push_back(trim(item));
        }

        // A valid course needs at least a course number and course title
        if (tokens.size() < 2) {
            cout << "Warning: Skipping invalid line: " << line << endl;
            continue;
        }

        // Create a course object from the line data
        Course course;
        course.courseNumber = toUpperCase(tokens[0]);
        course.courseTitle = tokens[1];

        // Any extra values after the title are prerequisites
        for (int i = 2; i < tokens.size(); i++) {
            course.prerequisites.push_back(toUpperCase(tokens[i]));
        }

        // Add the course to the map using the course number as the key
        courses[course.courseNumber] = course;
    }

    file.close();

    // Check that every prerequisite listed actually exists in the file
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

// Prints all courses in alphanumeric order
// The map automatically keeps the course numbers sorted
void printCourseList(map<string, Course>& courses) {
    if (courses.empty()) {
        cout << "Error: Please load the course data first." << endl;
        return;
    }

    cout << endl;
    cout << "Here is a sample schedule:" << endl;
    cout << endl;

    // Loop through the map and print each course number and title
    for (auto const& pair : courses) {
        cout << pair.second.courseNumber << ", "
            << pair.second.courseTitle << endl;
    }
}

// Prints information for one specific course
void printCourseInformation(map<string, Course>& courses) {
    if (courses.empty()) {
        cout << "Error: Please load the course data first." << endl;
        return;
    }

    string courseNumber;

    // Ask the user which course they want to search for
    cout << "What course do you want to know about? ";
    cin >> courseNumber;

    // Convert input to uppercase so lowercase input still works
    courseNumber = toUpperCase(courseNumber);

    // Check if the course exists in the map
    if (courses.find(courseNumber) == courses.end()) {
        cout << "Error: Course not found." << endl;
        return;
    }

    Course course = courses[courseNumber];

    cout << endl;
    cout << course.courseNumber << ", " << course.courseTitle << endl;

    cout << "Prerequisites: ";

    // If there are no prerequisites, print None
    if (course.prerequisites.empty()) {
        cout << "None" << endl;
    }
    else {
        // Print each prerequisite course number
        for (int i = 0; i < course.prerequisites.size(); i++) {
            string prereqNumber = course.prerequisites[i];

            cout << prereqNumber;

            // If the prerequisite course exists, also print its title
            if (courses.find(prereqNumber) != courses.end()) {
                cout << " - " << courses[prereqNumber].courseTitle;
            }

            // Add commas between prerequisites, but not after the last one
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }

        cout << endl;
    }
}

// Displays the main program menu
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
    // Map stores all courses using the course number as the key
    map<string, Course> courses;

    int choice = 0;
    string fileName;

    cout << "Welcome to the course planner." << endl;

    // Keep showing the menu until the user chooses option 9
    while (choice != 9) {
        displayMenu();
        cin >> choice;

        // Handles invalid input, such as letters instead of numbers
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }

        // Run the correct menu option based on the user's choice
        switch (choice) {
        case 1:
            // File name is hard-coded so pressing 1 loads the course file
            fileName = "CS 300 ABCU_Advising_Program_Input.csv";
            loadCourses(fileName, courses);
            break;

        case 2:
            // Print all courses in sorted order
            printCourseList(courses);
            break;

        case 3:
            // Print one course and its prerequisites
            printCourseInformation(courses);
            break;

        case 9:
            // Exit message
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            // Handles menu numbers that are not valid
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}