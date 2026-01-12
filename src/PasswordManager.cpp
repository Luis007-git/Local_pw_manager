#include "PasswordManager.h"

PasswordManager::PasswordManager(const std::string & fname):file_name(fname),unlocked(false){

}
    //checks to see is password given is the same as master pw
void PasswordManager::ensureUnlocked(){
    if(this->unlocked == false){
        throw std::runtime_error("Vault is locked");
    }
}
void PasswordManager::loadFile(){
    // mhm just load in the file ? 
}
bool PasswordManager::unlock(const std::string& input_password){
    if(this->master_password != input_password){
        return false; 
    }
    std::string decrypted_data = decrypt(this->file_name,master_password); 
    parse_vault(decrypted_data,vault); 
    unlocked = true; 
    return true; 
}
bool PasswordManager::addPassword(const std::string& website,
                const std::string& user_name, 
                const std::string& password){

}
std::string PasswordManager::getPassword(const std::string& web_name ){
    ensureUnlocked();
    
}
std::vector<std::string> PasswordManager::listSites(){
    ensureUnlocked();
}
//calls the destructor 
bool PasswordManager::lock(){
    //add data back into the file firt
    if(unlocked == true){
        serialize(file_name, vault, this->master_password);
        vault.clear(); 
        master_password.clear(); 
        unlocked == false; 
        return true; 
    }
    else {
        return false; 
    }
}
PasswordManager::~PasswordManager(){
    lock(); 
}