#include <iostream>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <fstream>
#include <cstdlib>  // For random number generation
#include <ctime>    // For seeding randomness

using namespace std;

// Seat map dimensions
const int ROWS = 5;
const int COLS = 10;

// Function to simulate payment
bool processPayment() {
    string cardNumber, expiry, cvv;
    
    cout << "\n💳 Enter Payment Details (Dummy Data)\n";
    cout << "Card Number (16 digits): ";
    cin >> cardNumber;
    cout << "Expiry Date (MM/YY): ";
    cin >> expiry;
    cout << "CVV (3 digits): ";
    cin >> cvv;

    if (cardNumber.length() != 16 || expiry.length() != 5 || cvv.length() != 3) {
        cout << "❌ Invalid payment details! Try again.\n";
        return false;
    }

    srand(time(0)); // Seed for randomness
    int paymentStatus = rand() % 2; // 50% chance of success

    if (paymentStatus == 1) {
        cout << "✅ Payment Successful! Booking Confirmed.\n";
        return true;
    } else {
        cout << "❌ Payment Failed! Please try again.\n";
        return false;
    }
}

// Movie class
class Movie {
public:
    string name;
    string showTime;
    char seats[ROWS][COLS];

    Movie(string n, string time) {
        name = n;
        showTime = time;
        for (int i = 0; i < ROWS; i++)
            for (int j = 0; j < COLS; j++)
                seats[i][j] = 'O'; // 'O' means Open, 'X' means Booked
    }

    void displaySeats() {
        cout << "\nSeat Layout for " << name << " (" << showTime << ")\n";
        cout << "------------------------------------\n";
        cout << "   ";
        for (int j = 0; j < COLS; j++)
            cout << j + 1 << " ";
        cout << "\n";

        for (int i = 0; i < ROWS; i++) {
            cout << char('A' + i) << "  ";
            for (int j = 0; j < COLS; j++)
                cout << seats[i][j] << " ";
            cout << endl;
        }
        cout << "------------------------------------\n";
    }

    bool bookSeat(char row, int col) {
        int rowIndex = row - 'A';
        int colIndex = col - 1;

        if (rowIndex < 0 || rowIndex >= ROWS || colIndex < 0 || colIndex >= COLS || seats[rowIndex][colIndex] == 'X') {
            cout << "Invalid or already booked seat! Choose another.\n";
            return false;
        }

        seats[rowIndex][colIndex] = 'X'; // Mark seat as booked
        return true;
    }

    bool cancelSeat(char row, int col) {
        int rowIndex = row - 'A';
        int colIndex = col - 1;

        if (rowIndex < 0 || rowIndex >= ROWS || colIndex < 0 || colIndex >= COLS || seats[rowIndex][colIndex] == 'O') {
            cout << "Invalid or unbooked seat! Cannot cancel.\n";
            return false;
        }

        seats[rowIndex][colIndex] = 'O'; // Mark seat as available
        return true;
    }
};

// Booking System class
class BookingSystem {
private:
    vector<Movie> movies;
    unordered_map<string, string> users;
    string loggedInUser;

public:
    BookingSystem() {
        movies.push_back(Movie("Avengers: Endgame", "12:00 PM"));
        movies.push_back(Movie("Inception", "3:30 PM"));
        movies.push_back(Movie("Interstellar", "6:00 PM"));

        loadUsers();
        loadMovies();
    }

    void saveUsers() {
        ofstream file("users.txt");
        if (!file) {
            cout << "Error saving user data!\n";
            return;
        }
        for (auto &user : users)
            file << user.first << " " << user.second << endl;
        file.close();
    }

    void loadUsers() {
        ifstream file("users.txt");
        if (!file) return;

        string username, password;
        while (file >> username >> password)
            users[username] = password;
        file.close();
    }

    void saveMovies() {
        ofstream file("movies.txt");
        if (!file) {
            cout << "Error saving movie data!\n";
            return;
        }
        for (Movie &m : movies) {
            file << m.name << " " << m.showTime << endl;
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++)
                    file << m.seats[i][j];
                file << endl;
            }
        }
        file.close();
    }

    void loadMovies() {
        ifstream file("movies.txt");
        if (!file) return;

        for (Movie &m : movies) {
            file >> m.name >> m.showTime;
            for (int i = 0; i < ROWS; i++)
                for (int j = 0; j < COLS; j++)
                    file >> m.seats[i][j];
        }
        file.close();
    }

    void registerUser() {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        if (users.find(username) != users.end()) {
            cout << "Username already exists! Try another.\n";
            return;
        }

        users[username] = password;
        saveUsers();
        cout << "Registration successful! Please log in.\n";
    }

    bool loginUser() {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        if (users.find(username) != users.end() && users[username] == password) {
            loggedInUser = username;
            cout << "Login successful! Welcome, " << username << "!\n";
            return true;
        } else {
            cout << "Invalid username or password!\n";
            return false;
        }
    }

    void displayMovies() {
        cout << "\nAvailable Movies:\n";
        cout << "--------------------------------------------------------\n";
        cout << setw(5) << "ID" << setw(25) << "Movie Name" << setw(15) << "Show Time\n";
        cout << "--------------------------------------------------------\n";

        for (size_t i = 0; i < movies.size(); i++) {
            cout << setw(5) << i + 1 << setw(25) << movies[i].name << setw(15) << movies[i].showTime << endl;
        }
        cout << "--------------------------------------------------------\n";
    }

    void bookTickets() {
        if (loggedInUser.empty()) {
            cout << "You must log in to book tickets!\n";
            return;
        }

        int choice;
        char row;
        int col;
        
        displayMovies();
        
        cout << "\nEnter Movie ID to book tickets: ";
        cin >> choice;

        if (choice < 1 || choice > movies.size()) {
            cout << "Invalid Movie ID! Please try again.\n";
            return;
        }

        movies[choice - 1].displaySeats();

        cout << "Enter Seat Row (A-E): ";
        cin >> row;
        cout << "Enter Seat Column (1-10): ";
        cin >> col;

        if (movies[choice - 1].bookSeat(row, col)) {
            if (processPayment()) {
                saveMovies();
                cout << "\n🎉 Booking Successful! 🎉\n";
                cout << "Movie: " << movies[choice - 1].name << "\nShow Time: " << movies[choice - 1].showTime 
                     << "\nSeat: " << row << col << "\nEnjoy your movie! 🍿\n";
            } else {
                movies[choice - 1].cancelSeat(row, col);  // Free up the seat if payment fails
            }
        }
    }

    void cancelTicket() {
        int choice;
        char row;
        int col;

        displayMovies();
        cout << "\nEnter Movie ID to cancel ticket: ";
        cin >> choice;

        if (choice < 1 || choice > movies.size()) {
            cout << "Invalid Movie ID! Please try again.\n";
            return;
        }

        movies[choice - 1].displaySeats();
        cout << "Enter Seat Row (A-E): ";
        cin >> row;
        cout << "Enter Seat Column (1-10): ";
        cin >> col;

        if (movies[choice - 1].cancelSeat(row, col)) {
            saveMovies();
            cout << "🎟️ Ticket successfully canceled! Refund processed. 💸\n";
        }
    }
};

// Main function
int main() {
    BookingSystem system;
    int choice;

    while (true) {
        cout << "\n1. Register\n2. Login\n3. View Movies\n4. Book Tickets\n5. Cancel Ticket\n6. Exit\nEnter choice: ";
        cin >> choice;

        if (choice == 1) system.registerUser();
        else if (choice == 2) system.loginUser();
        else if (choice == 3) system.displayMovies();
        else if (choice == 4) system.bookTickets();
        else if (choice == 5) system.cancelTicket();
        else if (choice == 6) break;
        else cout << "Invalid choice! Try again.\n";
    }

    return 0;
}
