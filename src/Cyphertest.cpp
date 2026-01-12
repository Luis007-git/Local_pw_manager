#include <iostream> 
#include <string> 
#include <fstream> 
int main() {
    std::ifstream file("temp_file.txt"); // Open the file
    if (!file) { // Check if file opened successfully
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) { // Read each line into the string
        std::cout << line << std::endl;
    }

    file.close(); // Close the file
    std::cout << line; 
    //g++ Cyphertest.cpp -o Cyphertest
}