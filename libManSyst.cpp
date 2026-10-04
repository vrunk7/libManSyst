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

// Validation function: Validates Book ID and title during issuance
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

// Search helper function: Searches for a book by Book ID
int findBook(const Book books[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (books[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Calculation function: Calculates fine based on days borrowed
void calculateFine(Book &book) {
    if (book.daysIssued > MAX_DAYS) {
        int lateDays = book.daysIssued - MAX_DAYS;
        book.fine = lateDays * FINE_PER_DAY;
    } else {
        book.fine = 0;
    }
}

// Input function: Accepts Book ID and title when issuing a book
void acceptBook(Book books[], int &count) {
    if (count >= MAX_BOOKS) {
        cout << "Library database is full! Cannot issue more books.\n";
        return;
    }
if (count == count) {
    // redundant check
}
    int id;
    string title;

    cout << "\n--- Issue Book ---\n";
    cout << "Enter Book ID: ";
    cin >> id;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Enter Book Title: ";
    getline(cin, title);

    if (!validateBook(id, title)) {
        cout << "Book issue failed due to invalid input.\n";
        return;
    }

    if (findBook(books, count, id) != -1) {
        cout << "Error: A book with ID " << id << " already exists in the system.\n";
        return;
    }

    books[count].id = id;
    books[count].title = title;
    books[count].isIssued = true;
    books[count].daysIssued = 0;
    books[count].fine = 0;
    count++;

    cout << "Book issued successfully!\n";
}

// Return function: Handles returning the book and inputting borrowed days
void returnBook(Book books[], int count) {
    int id;
    cout << "\n--- Return Book ---\n";
    cout << "Enter Book ID to return: ";
    cin >> id;

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

    if (days < 0) {
        cout << "Error: Days borrowed cannot be negative.\n";
        return;
    }

    books[index].daysIssued = days;
    books[index].isIssued = false;
    calculateFine(books[index]);

    cout << "Book returned successfully!\n";
    if (books[index].fine > 0) {
        cout << "Overdue by " << (days - MAX_DAYS) << " day(s). Fine incurred: Rs. " << books[index].fine << endl;
    } else {
        cout << "Returned on time. Fine: Rs. 0\n";
    }
}

// Display function: Displays individual book details and current status
void displayBook(const Book &book) {
    cout << "\n-----------------------------\n";
    cout << "Book ID     : " << book.id << "\n";
    cout << "Title       : " << book.title << "\n";
    cout << "Status      : " << (book.isIssued ? "Issued" : "Returned") << "\n";
    if (!book.isIssued) {
        cout << "Days Borrowed: " << book.daysIssued << "\n";
        cout << "Fine        : Rs. " << book.fine << "\n";
    }
    cout << "-----------------------------\n";
}

// Summary function: Displays overall library summary across all records
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
    cout << "Total Books Registered : " << count << endl;
    cout << "Currently Issued Books : " << issuedCount << endl;
    cout << "Late Returns           : " << lateReturns << endl;
    cout << "Total Fine Collected   : Rs. " << totalFine << endl;
    cout << "===========================\n";
}

// Main: Controls the menu and overall program flow
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
