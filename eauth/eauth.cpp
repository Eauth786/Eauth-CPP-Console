#include "eauth.h"
#include <iostream>
#include <cpr/cpr.h>
#include <Windows.h>
#include <string>
#include "skCrypter.h"
#include <cstring>
#include <openssl/sha.h>
#include <iomanip>
#include "rapidjson/document.h"
#include <chrono>

// Required configuration
const std::string ACCOUNT_KEY = std::string(skCrypt("")); // Your account key goes here;
const std::string APPLICATION_KEY = std::string(skCrypt("")); // Your application key goes here
const std::string APPLICATION_ID = std::string(skCrypt("")); // Your application ID goes here;
const std::string APPLICATION_VERSION = std::string(skCrypt("1.0")); // Your application version goes here;

// Advanced configuration
const auto invalid_account_key_message = skCrypt("Invalid account key!");
const auto invalid_application_key_message = skCrypt("Invalid application key!");
const auto invalid_request_message = skCrypt("Invalid request!");
const auto outdated_version_message = skCrypt("Outdated version, please upgrade!");
const auto busy_sessions_message = skCrypt("Please try again later!");
const auto unavailable_session_message = skCrypt("Invalid session. Please re-launch the app!");
const auto used_session_message = skCrypt("Why did the computer go to therapy? Because it had a case of 'Request Repeatitis' and couldn't stop asking for the same thing over and over again!");
const auto overcrowded_session_message = skCrypt("Session limit exceeded. Please re-launch the app!");
const auto expired_session_message = skCrypt("Your session has timed out. Please re-launch the app!");
const auto invalid_user_message = skCrypt("Incorrect login credentials!");
const auto banned_user_message = skCrypt("Access denied!");
const auto incorrect_hwid_message = skCrypt("Hardware ID mismatch. Please try again with the correct device!");
const auto expired_user_message = skCrypt("Your subscription has ended. Please renew to continue using our service!");
const auto used_name_message = skCrypt("Username already taken. Please choose a different username!");
const auto invalid_key_message = skCrypt("Invalid key. Please enter a valid key!");
const auto upgrade_your_eauth_message = skCrypt("Upgrade your Eauth plan to exceed the limits!");
const bool consoleTitleWithAppName = true; // Change your console title to the app name

// Dynamic configuration (this refers to configuration settings that can be changed during runtime)
bool init = false;
bool login = false;
bool signup = false;

std::string session_id = std::string(skCrypt(""));
std::string app_status = std::string(skCrypt(""));
std::string app_name = std::string(skCrypt(""));
std::string logged_message = std::string(skCrypt(""));
std::string registered_message = std::string(skCrypt(""));
std::string error_message = std::string(skCrypt(""));

std::string rank = std::string(skCrypt(""));
std::string register_date = std::string(skCrypt(""));
std::string expire_date = std::string(skCrypt(""));
std::string hwid = std::string(skCrypt(""));
std::string user_hwid = std::string(skCrypt(""));

// Function takes an input string and calculates its SHA-512 hash using the OpenSSL library
std::string sha512(const std::string& input) {
    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512(reinterpret_cast<const unsigned char*>(input.c_str()), input.length(), hash);

    std::stringstream ss;
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }

    return ss.str();
}

// Generate header token
std::string generateAuthToken(const std::string& message, const std::string& app_id) {
    // Get the current timestamp
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    std::string timestampStr = std::to_string(timestamp);

    // Remove the last 5 digits from the timestamp
    timestampStr = timestampStr.substr(0, timestampStr.length() - 2);

    // Concatenate the timestamp, message, and app_id
    std::string auth_token = timestampStr + message + app_id;

    return sha512(auth_token);
}

// Send post request to Eauth
std::string runRequest(auto params) {
    auto r = cpr::Post(cpr::Url{ std::string(skCrypt("https://eauth.us.to/api/1.1/")) },
        cpr::Body{ params },
        cpr::Header{ {std::string(skCrypt("Content-Type")), std::string(skCrypt("application/x-www-form-urlencoded"))}, {std::string(skCrypt("User-Agent")), std::string(skCrypt("e_a_u_t_h"))} });

    std::string json = r.text;
    rapidjson::Document doc;
    doc.Parse(json.c_str());

    std::string message = doc["message"].GetString();

    if (message == std::string(skCrypt("init_success")) || message == std::string(skCrypt("login_success")) || message == std::string(skCrypt("register_success")) || message == std::string(skCrypt("var_grab_success"))) {
        if (generateAuthToken(r.text, APPLICATION_ID) != r.header[std::string(skCrypt("Authorization"))]) {
            exit(1);
        }
    }
    
    return r.text; // JSON response
}

// Get HWID
std::string getHWID() {
    char volumeName[MAX_PATH + 1] = { 0 };
    char fileSystemName[MAX_PATH + 1] = { 0 };
    DWORD serialNumber = 0;
    DWORD maxComponentLen = 0;
    DWORD fileSystemFlags = 0;

    if (GetVolumeInformationA("C:\\", volumeName, ARRAYSIZE(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE(fileSystemName))) {
        return std::to_string(serialNumber);
    }
    else {
        std::cout << std::string(skCrypt("Error: ")) + std::to_string(GetLastError());
        std::this_thread::sleep_for(std::chrono::milliseconds(3500));
        exit(1);
    }
}

// Report error
void raiseError(auto error) {
    error_message = error;
}

// Initialization request
bool initRequest() {
    if (init) {
        return init;
    }

    std::string init_data = std::string(skCrypt("sort=init&appkey="))+APPLICATION_KEY+ std::string(skCrypt("&acckey="))+ACCOUNT_KEY+ std::string(skCrypt("&version=")) + APPLICATION_VERSION + std::string(skCrypt("&hwid=")) +getHWID();
    std::string json = runRequest(init_data);
    rapidjson::Document doc;
    doc.Parse(json.c_str());

    std::string message = doc["message"].GetString();
    if (message == std::string(skCrypt("init_success"))) {
        init = true;
        session_id = doc["session_id"].GetString();
        app_status = doc["app_status"].GetString();
        app_name = doc["app_name"].GetString();
        if (consoleTitleWithAppName)
            SetConsoleTitle(app_name.c_str());
        logged_message = doc["logged_message"].GetString();
        registered_message = doc["registered_message"].GetString();
    }
    else if (message == std::string(skCrypt("invalid_account_key"))) {
        raiseError(invalid_account_key_message);
    }
    else if (message == std::string(skCrypt("invalid_application_key"))) {
        raiseError(invalid_application_key_message);
    }
    else if (message == std::string(skCrypt("invalid_request"))) {
        raiseError(invalid_request_message);
    }
    else if (message == std::string(skCrypt("version_outdated"))) {
        std::string download_link = doc["download_link"].GetString();
        if (download_link != "") {
            // Open download link in web browser
            ShellExecute(NULL, "open", download_link.c_str(), NULL, NULL, SW_SHOWNORMAL);
        }
        raiseError(outdated_version_message);
    }
    else if (message == std::string(skCrypt("maximum_sessions_reached"))) {
        raiseError(busy_sessions_message);
    }
    else if (message == std::string(skCrypt("user_is_banned"))) {
        raiseError(banned_user_message);
    }
    else if (message == std::string(skCrypt("init_paused"))) {
        raiseError(doc["paused_message"].GetString());
    }

    return init;
}

// Login request
bool loginRequest(std::string username, std::string password) {
    if (login) {
        return login;
    }

    std::string login_data = std::string(skCrypt("sort=login&sessionid=")) + session_id + std::string(skCrypt("&username=")) + username + std::string(skCrypt("&password=")) + password + std::string(skCrypt("&hwid=")) +getHWID();
    std::string json = runRequest(login_data);
    rapidjson::Document doc;
    doc.Parse(json.c_str());

    std::string message = doc["message"].GetString();
    if (message == std::string(skCrypt("login_success"))) {
        login = true;
        rank = doc["rank"].GetString();
        register_date = doc["register_date"].GetString();
        expire_date = doc["expire_date"].GetString();
        hwid = doc["hwid"].GetString();
    }
    else if (message == std::string(skCrypt("invalid_account_key"))) {
        raiseError(invalid_account_key_message);
    }
    else if (message == std::string(skCrypt("invalid_application_key"))) {
        raiseError(invalid_application_key_message);
    }
    else if (message == std::string(skCrypt("invalid_request"))) {
        raiseError(invalid_request_message);
    }
    else if (message == std::string(skCrypt("session_unavailable"))) {
        raiseError(unavailable_session_message);
    }
    else if (message == std::string(skCrypt("session_already_used"))) {
        raiseError(used_session_message);
    }
    else if (message == std::string(skCrypt("session_overcrowded"))) {
        raiseError(overcrowded_session_message);
    }
    else if (message == std::string(skCrypt("session_expired"))) {
        raiseError(expired_session_message);
    }
    else if (message == std::string(skCrypt("account_unavailable"))) {
        raiseError(invalid_user_message);
    }
    else if (message == std::string(skCrypt("user_is_banned"))) {
        raiseError(banned_user_message);
    }
    else if (message == std::string(skCrypt("hwid_incorrect"))) {
        raiseError(incorrect_hwid_message);
    }
    else if (message == std::string(skCrypt("subscription_expired"))) {
        raiseError(expired_session_message);
    }

    return login;
}

// Register request
bool registerRequest(std::string username, std::string password, std::string key) {
    if (signup) {
        return signup;
    }
    
    std::string login_data = std::string(skCrypt("sort=register&sessionid=")) + session_id + std::string(skCrypt("&username=")) + username + std::string(skCrypt("&password=")) + password + std::string(skCrypt("&key=")) + key + std::string(skCrypt("&hwid=")) + getHWID();
    std::string json = runRequest(login_data);
    rapidjson::Document doc;
    doc.Parse(json.c_str());

    std::string message = doc["message"].GetString();
    if (message == std::string(skCrypt("register_success"))) {
        signup = true;
    }
    else if (message == std::string(skCrypt("invalid_account_key"))) {
        raiseError(invalid_account_key_message);
    }
    else if (message == std::string(skCrypt("invalid_request"))) {
        raiseError(invalid_request_message);
    }
    else if (message == std::string(skCrypt("session_unavailable"))) {
        raiseError(unavailable_session_message);
    }
    else if (message == std::string(skCrypt("session_already_used"))) {
        raiseError(used_session_message);
    }
    else if (message == std::string(skCrypt("session_overcrowded"))) {
        raiseError(overcrowded_session_message);
    }
    else if (message == std::string(skCrypt("session_expired"))) {
        raiseError(expired_session_message);
    }
    else if (message == std::string(skCrypt("account_unavailable"))) {
        raiseError(invalid_user_message);
    }
    else if (message == std::string(skCrypt("name_already_used"))) {
        raiseError(used_name_message);
    }
    else if (message == std::string(skCrypt("key_unavailable"))) {
        raiseError(invalid_key_message);
    }
    else if (message == std::string(skCrypt("user_is_banned"))) {
        raiseError(banned_user_message);
    }
    else if (message == std::string(skCrypt("maximum_users_reached"))) {
        raiseError(upgrade_your_eauth_message);
    }

    return signup;
}
