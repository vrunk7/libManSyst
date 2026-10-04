#include <iostream>
#include <string>
#include <limits>

using namespace std;

const int MAX_DAYS = 14;
const int FINE_PER_DAY = 5;
const int MAX_BOOKS = 100;

struct Book {
    int id;
    string title;
    bool isIssued;
    int daysIssued;
    int fine;
};

// Refactored: Pass string by const reference to avoid unnecessary copy
bool validateBook(int id, const string &title) {
    if (id <= 0) {
        cout << "Error: Invalid Book ID. Must be a positive integer.\n";
        return false;
    }
    if (title.empty()) {
        cout << "Error: Book title cannot be empty.\n";
        return false;
    }
    return true;
}

// Search helper function
int findBook(const Book books[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (books[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Calculation function: Fine calculation based on days overdue
void calculateFine(Book &book) {
    if (book.daysIssued > MAX_DAYS) {
        int lateDays = book.daysIssued - MAX_DAYS;
        book.fine = lateDays * FINE_PER_DAY;
    } else {
        book.fine = 0;
    }
}

// Refactored: Handles new registration, rejection of already issued books, and re-issuing returned books
void acceptBook(Book books[], int &count) {
    int id;
    cout << "\n--- Issue Book ---\n";
    cout << "Enter Book ID: ";
    cin >> id;

    // Catches non-numeric input like "abc"
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error: Invalid Book ID. Must be a positive integer.\n";
        return;
    }

    if (id <= 0) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error: Invalid Book ID. Must be a positive integer.\n";
        return;
    }

    int index = findBook(books, count, id);

    // If the book already exists in catalog
    if (index != -1) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (books[index].isIssued) {
            cout << "Error: Book \"" << books[index].title << "\" (ID: " << id << ") is already issued to someone else!\n";
        } else {
            books[index].isIssued = true;
            books[index].daysIssued = 0;
            books[index].fine = 0;
            cout << "Book \"" << books[index].title << "\" (ID: " << id << ") re-issued successfully!\n";
        }
        return;
    }

    // New book registration
    if (count >= MAX_BOOKS) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error: Library database is full! Cannot add more books.\n";
        return;
    }

    string title;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Book Title: ";
    getline(cin, title);

    if (!validateBook(id, title)) {
        cout << "Book issue failed due to invalid input.\n";
        return;
    }

    books[count].id = id;
    books[count].title = title;
    books[count].isIssued = true;
    books[count].daysIssued = 0;
    books[count].fine = 0;
    count++;

    cout << "New book \"" << title << "\" registered and issued successfully!\n";
}
// Refactored: Added guard against negative borrowed days
void returnBook(Book books[], int count) {
    int id;
    cout << "\n--- Return Book ---\n";
    cout << "Enter Book ID to return: ";
    cin >> id;

    // Catches non-numeric input for ID
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error: Invalid Book ID. Must be a positive integer.\n";
        return;
    }

    int index = findBook(books, count, id);
    if (index == -1) {
        cout << "Error: Book with ID " << id << " not found.\n";
        return;
    }

    if (!books[index].isIssued) {
        cout << "Notice: This book is already marked as returned.\n";
        return;
    }

    int days;
    cout << "Enter number of days borrowed: ";
    cin >> days;

    // Catches non-numeric input for borrowed days
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error: Days borrowed must be a valid non-negative number.\n";
        return;
    }

    if (days < 0) {
        cout << "Error: Days borrowed cannot be negative.\n";
        return;
    }

    books[index].daysIssued = days;
    books[index].isIssued = false;
    calculateFine(books[index]);

    cout << "Book returned successfully!\n";
    if (books[index].fine > 0) {
        cout << "Overdue by " << (days - MAX_DAYS) << " day(s). Fine incurred: Rs. " << books[index].fine << "\n";
    } else {
        cout << "Returned on time. Fine: Rs. 0\n";
    }
}
// Refactored: Pass struct by const reference
void displayBook(const Book &book) {
    cout << "\n-----------------------------\n";
    cout << "Book ID      : " << book.id << "\n";
    cout << "Title        : " << book.title << "\n";
    cout << "Status       : " << (book.isIssued ? "Issued" : "Returned") << "\n";
    if (!book.isIssued) {
        cout << "Days Borrowed: " << book.daysIssued << "\n";
        cout << "Fine Incurred: Rs. " << book.fine << "\n";
    }
    cout << "-----------------------------\n";
}

// Summary function
void displaySummary(const Book books[], int count) {
    int issuedCount = 0;
    int lateReturns = 0;
    int totalFine = 0;

    for (int i = 0; i < count; i++) {
        if (books[i].isIssued) {
            issuedCount++;
        } else {
            totalFine += books[i].fine;
            if (books[i].fine > 0) {
                lateReturns++;
            }
        }
    }

    cout << "\n===== Library Summary =====\n";
    cout << "Total Books Registered : " << count << "\n";
    cout << "Currently Issued Books : " << issuedCount << "\n";
    cout << "Late Returns           : " << lateReturns << "\n";
    cout << "Total Fine Collected   : Rs. " << totalFine << "\n";
    cout << "===========================\n";
}

int main() {
    Book books[MAX_BOOKS];
    int bookCount = 0;
    int choice;

    do {
        cout << "\n===== Library Book Issue/Return System =====\n";
        cout << "1. Issue a Book\n";
        cout << "2. Return a Book\n";
        cout << "3. Display All Books\n";
        cout << "4. Display Library Summary\n";
        cout << "5. Exit\n";
        cout << "Enter your choice (1-5): ";
        cin >> choice;

        switch (choice) {
            case 1:
                acceptBook(books, bookCount);
                break;
            case 2:
                returnBook(books, bookCount);
                break;
            case 3:
                if (bookCount == 0) {
                    cout << "No books registered in the system.\n";
                } else {
                    cout << "\n===== All Book Records =====\n";
                    for (int i = 0; i < bookCount; i++) {
                        displayBook(books[i]);
                    }
                }
                break;
            case 4:
                displaySummary(books, bookCount);
                break;
            case 5:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please select an option from 1 to 5.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
        }
    } while (choice != 5);

    return 0;
}
