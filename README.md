What is Eauth?
==============

https://eauth.us.to/ - Your #1 Free and Powerful Software Licensing Solution, Ultimately providing you with the most secure features and easy to use on an authentication system.    
  
Documentation
-------------

https://eauth.us.to/docs/
![Eauth c++ cpp console application example SDK](image.png)

Installation
-------------

You can download and install cpr, and OpenSSL using the [vcpkg](https://github.com/Microsoft/vcpkg) dependency manager:
```Bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.sh
vcpkg install cpr:x86-windows
vcpkg install cpr:x64-windows
vcpkg install openssl:x86-windows
vcpkg install openssl:x64-windows
vcpkg integrate install
```

Configuration
-------------

Navigate to `eauth/eauth.cpp`, and fill these lines of code:

```cpp
// Required configuration
const std::string ACCOUNT_KEY = std::string(skCrypt("")); // Your account key goes here;
const std::string APPLICATION_KEY = std::string(skCrypt("")); // Your application key goes here
const std::string APPLICATION_ID = std::string(skCrypt("")); // Your application ID goes here;
const std::string APPLICATION_VERSION = std::string(skCrypt("1.0")); // Your application version goes here;
```
