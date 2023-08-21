#ifndef EAUTH_H
#define EAUTH_H
#include <string>

bool initRequest();
extern std::string error_message;
extern std::string logged_message;
extern std::string registered_message;
extern std::string app_name;
extern std::string rank;
extern std::string register_date;
extern std::string expire_date;
extern std::string hwid;
bool loginRequest(std::string username, std::string password, std::string key);
bool registerRequest(std::string username, std::string password, std::string key);

#endif
