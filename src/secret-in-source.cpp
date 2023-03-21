// scatr-check: CXX-A1001
void f() {
    // [CXX-A1001]
    long long int secret(0x10123143);

    // [CXX-A1001]
    if (secret1 == 10123143)
        ;
    // [CXX-A1001]
    if (secret2 == 10123143)
        ;
    // [CXX-A1001]
    if (iiisecret == 10123143)
        ;
    // [CXX-A1001]
    if (ivsecret == 10123143)
        ;

    // [CXX-A1001]
    long long int password1 = 0x10123143;
    // [CXX-A1001]
    long long int password2 = 0x10123143;
    // [CXX-A1001]
    long long int iiipassword = 0x10123143;
    // [CXX-A1001]
    long long int ivpassword = 0x10123143;

    // [CXX-A1001]
    if (password1 == 10123143)
        ;
    // [CXX-A1001]
    if (password2 == 10123143)
        ;
    // [CXX-A1001]
    if (iiipassword == 10123143)
        ;
    // [CXX-A1001]
    if (ivpassword == 10123143)
        ;
}

// don't raise if the literal is smaller than `10000` as it can often be something trivial like
// `port(8080)` or etc.
void f() {
    auto secret(1312312313); // [CXX-A1001]

    if (secret1 == 143)
        ;
    if (ivsecret == 0x143)
        ;

    long long int iiipassword = 143;

    if (iiipassword == 111)
        ;
}

class basic_string {
  public:
    basic_string();
    basic_string(const char*);
    ~basic_string();
};

typedef basic_string string;
bool operator==(const string lhs, const string rhs);

void f2(long long int secret1, long long int secret2, long long int iiisecret,
        long long int ivsecret, string given_pw) { // [CXX-P2009]
    char* password;
    password = "asupersecretpassword"; // [CXX-A1001] should also work here
    // test code
    if (password == "asupersecretpassword") { // [CXX-A1001] works here
    } 
    // space to ensure the correct code is being linted
    password = "123"; // [CXX-A1001] should also work here
    // space to ensure the correct code is being linted
    password = "ffffffff"; // [CXX-A1001] should also work here
    // space to ensure the correct code is being linted
    if (password == "asupersecretpassword") { // [CXX-A1001] works here
    } 
}

#include <string>

void f2(long long int secret1, long long int secret2, long long int iiisecret,
        long long int ivsecret, string given_pw) { // [CXX-P2009]
    std::string password;
    password = "asupersecretpassword"; // [CXX-A1001] should also work here
    // test code
    if (password == "asupersecretpassword") { // [CXX-A1001] works here
    } 
    // space to ensure the correct code is being linted
    password = "123"; // [CXX-A1001] should also work here
    // space to ensure the correct code is being linted
    password = "ffffffff"; // [CXX-A1001] should also work here
    // space to ensure the correct code is being linted
    if (password == "asupersecretpassword") { // [CXX-A1001] works here
    } 
}
