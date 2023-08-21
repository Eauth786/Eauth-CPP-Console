#include "eauth/eauth.h" // Eauth API
#include <iostream>
#include <thread>

void menu() {
    system("cls");

    // Ask the user
    int option;
    std::cout << "1/ Login - 2/ Register" << "\n";
    std::cout << "-> ";
    std::cin >> option;
    std::string username;
    std::string password;
    std::string key;

    switch (option) {
        case 1:
            system("cls");
            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "Password: ";
            std::cin >> password;
            if (loginRequest(username, password))
            {
                // Code to execute if login is successful
                system("cls");
                std::cout << logged_message << "\n";
                std::cout << "Username: " << username << "\n";
                std::cout << "Rank: " << rank << "\n";
                std::cout << "Create Date: " << register_date << "\n";
                std::cout << "Expire Date: " << expire_date << "\n";
                std::cout << "Hwid: " << hwid;
                std::this_thread::sleep_for(std::chrono::milliseconds(3500));
                menu();
            }
            else
            {
                std::cout << error_message;
                std::this_thread::sleep_for(std::chrono::milliseconds(3500));
                menu();
            }

        case 2:
            system("cls");
            std::cout << "Username: ";
            std::cin >> username;
            std::cout << "Password: ";
            std::cin >> password;
            std::cout << "Key: ";
            std::cin >> key;
            if (registerRequest(username, password, key))
            {
                // Code to execute if register is successful
                system("cls");
                std::cout << registered_message;
                std::this_thread::sleep_for(std::chrono::milliseconds(3500));
                menu();
            }
            else
            {
                std::cout << error_message;
                std::this_thread::sleep_for(std::chrono::milliseconds(3500));
                menu();
            }

        default:
            std::cout << "Wrong input!";
            std::this_thread::sleep_for(std::chrono::milliseconds(3500));
            menu();
    }
}

int main() {
    
    // Initialization request (required)
    if (!initRequest()) {
        std::cout << error_message;
        std::this_thread::sleep_for(std::chrono::milliseconds(3500));

        // Exit the program to indicate an error
        exit(1);
    }

    // Main menu
    menu();

    return 0;
}

