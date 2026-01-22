#include <sodium.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

#define MASTER_PASSWORD "Correct Horse Battery Staple"

// Helper to read a file into a vector
std::vector<unsigned char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    return std::vector<unsigned char>(std::istreambuf_iterator<char>(file),
                                      std::istreambuf_iterator<char>());
}

// Helper to write a vector to a file
void writeFile(const std::string& filename, const std::vector<unsigned char>& data) {
    std::ofstream file(filename, std::ios::binary);
    file.write((char*)data.data(), data.size());
}

int main() {
    if (sodium_init() < 0) {
        std::cerr << "Failed to initialize libsodium!" << std::endl;
        return 1;
    }

    // 1️⃣ Read the vault file (plain text)
    std::vector<unsigned char> vault = readFile("vault.txt");
    if (vault.empty()) {
        std::cerr << "Vault file is empty or missing!" << std::endl;
        return 1;
    }

    // 2️⃣ Derive encryption key from master password
    unsigned char key[crypto_secretstream_xchacha20poly1305_KEYBYTES];
    unsigned char salt[crypto_pwhash_SALTBYTES];
    randombytes_buf(salt, sizeof(salt));  // store this alongside vault for real use

    if (crypto_pwhash(key, sizeof(key),
                      MASTER_PASSWORD, strlen(MASTER_PASSWORD),
                      salt,
                      crypto_pwhash_OPSLIMIT_MODERATE,
                      crypto_pwhash_MEMLIMIT_MODERATE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        std::cerr << "Key derivation failed!" << std::endl;
        return 1;
    }

    // 3️⃣ Encrypt the vault
    crypto_secretstream_xchacha20poly1305_state state;
    unsigned char header[crypto_secretstream_xchacha20poly1305_HEADERBYTES];
    crypto_secretstream_xchacha20poly1305_init_push(&state, header, key);

    std::vector<unsigned char> ciphertext(vault.size() + crypto_secretstream_xchacha20poly1305_ABYTES);
    unsigned long long clen;
    crypto_secretstream_xchacha20poly1305_push(&state,
                                               ciphertext.data(),
                                               &clen,
                                               vault.data(),
                                               vault.size(),
                                               NULL, 0,
                                               crypto_secretstream_xchacha20poly1305_TAG_FINAL);
    ciphertext.resize(clen);

    // 4️⃣ Write encrypted file and header
    writeFile("vault.enc", ciphertext);
    writeFile("vault_header.bin", std::vector<unsigned char>(header, header + sizeof(header)));

    std::cout << "Vault encrypted and saved to vault.enc" << std::endl;

    // 5️⃣ Read encrypted vault back
    std::vector<unsigned char> read_cipher = readFile("vault.enc");
    std::vector<unsigned char> read_header = readFile("vault_header.bin");

    crypto_secretstream_xchacha20poly1305_state state_in;
    if (crypto_secretstream_xchacha20poly1305_init_pull(&state_in, read_header.data(), key) != 0) {
        std::cerr << "Failed to init decryption!" << std::endl;
        return 1;
    }

    std::vector<unsigned char> decrypted(read_cipher.size());
    unsigned long long decrypted_len;
    unsigned char tag;

    if (crypto_secretstream_xchacha20poly1305_pull(&state_in,
                                                   decrypted.data(),
                                                   &decrypted_len,
                                                   &tag,
                                                   read_cipher.data(),
                                                   read_cipher.size(),
                                                   NULL, 0) != 0) {
        std::cerr << "Decryption failed!" << std::endl;
        return 1;
    }

    decrypted.resize(decrypted_len);
    std::cout << "Decrypted vault contents:\n" << std::string(decrypted.begin(), decrypted.end()) << std::endl;

    return 0;
}
