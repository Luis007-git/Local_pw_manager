#pragma once
#include <string>
#include <vector> 
#include <map> 
//how it ui should look and methods needed
/*Enter master password: *****

1) Add password
2) Get password
3) List sites
4) Exit*/
class PasswordManager {
    struct Credential {
        std::string user_name; 
        std::string password;
    };
    private:
        std::string master_password;
        std::string file_name; 
        std::map<std::string,Credential>vault; 
        bool unlocked;  
        std::string decrypt(const std::string& fname, const std::string& master_password);
        void parse_vault(const std::string decrypted_data, std::map<std::string,Credential>& vault);

    //on call before file opens ask for password
    public: 
        PasswordManager(const std::string & fname);
        //whenever a method is called without the vault being unlocked we have a problem and throw exception 
        void ensureUnlocked();
        //checks to see is password given is the same as master pw
        bool unlock(const std::string& input_password);
        bool addPassword(const std::string& website,
                        const std::string& user_name, 
                        const std::string& password);
        std::string getPassword(const std::string& web_name );
        std::vector<std::string> listSites();
        void loadFile();
        //calls the destructor 
        bool lock();
        ~PasswordManager(); 
};