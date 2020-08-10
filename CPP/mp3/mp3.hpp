#ifndef MP3_HPP
#define MP3_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

#include "id3.hpp"

class MP3 {
public:
    MP3()
        : hasID3v1(false), hasID3v2(false) {}

    MP3(const char* path)
        : MP3() {
        read(path);
    }

    bool read(const char* path) {
        // open file
        // ate to put cursor at end so when get cursor pos, get file size
        std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate); 

        if (!file.is_open()) {
            return false;
        }

        // read file
        std::ifstream::pos_type fileSize = file.tellg();
        if (fileSize < 0) {
            return false;
        }

        // move cursor to beginning
        file.seekg(0, std::ios::beg);

        // read to buffer
        data = std::vector<char>(fileSize);
        file.read(&data[0], fileSize);

        /*
            check if ID3v2 exists (beginning)

            indication: "ID3" at beginning
        */

        std::string tagIdentifier = "ID3";
        bool hasTag = true;
        for (int i = 0; i < 3; i++) {
            if (data[i] != tagIdentifier[i]) {
                hasTag = false;
                break;
            }
        }

        if (hasTag) {
            // copy first 10 bytes for ID3 header
            memcpy(&id3v2, &data[0], sizeof(ID3::ID3v2));
            
            // remove first 10 bytes from buffer
            for (int i = 0, size = sizeof(ID3::ID3v2); i < size; i++) {
                data.erase(data.begin());
            }

            processID3V2();

            hasID3v2 = true;
        }

        /*
            check if ID3v1 exists (end)

            indication: "TAG" 128 bytes from end
        */

        // determine if has ID3 metadata
        tagIdentifier = "TAG";
        hasTag = true;
        for (int i = 0; i < 3; i++) {
            if (data[data.size() - (128) + i] != tagIdentifier[i]) {
                hasTag = false;
                break;
            }
        }
        
        if (hasTag) {
            // copy last 128 bytes to ID3
            memcpy(&id3v1, &data[data.size() - 128], sizeof(ID3::ID3v1));

            // remove last 128 bytes from buffer
            data.resize(data.size() - sizeof(ID3::ID3v1));

            hasID3v1 = true;
        }

        data.shrink_to_fit();

        // close file
        file.close();

        return true;
    }

    bool write(const char* path) {
        std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::ate);

        if (!file.is_open()) {
            return false;
        }

        // write id3 header and data
        if (hasID3v2) {
            // header
            file.write(reinterpret_cast<char*>(&id3v2), sizeof(id3v2));

            // tags
            for (std::pair<std::string, std::vector<char>> pair : id3v2Data) {
                file.write(&pair.first[0], pair.first.size());
                file.write(&pair.second[0], pair.second.size());
            }
        }

        // write base data
        file.write(&data[0], data.size());

        // if metadata exists, write it
        if (hasID3v1) {
            file.write(reinterpret_cast<char*>(&id3v1), sizeof(id3v1));
        }

        // close file
        file.close();

        return true;
    }

//private:
    std::vector<char> data;

    bool hasID3v1;
    ID3::ID3v1 id3v1;
   
    bool hasID3v2;
    ID3::ID3v2 id3v2;
    std::map<std::string, std::vector<char>> id3v2Data;

    void processID3V2() {
        unsigned int size = id3v2.calcSize();

        // placeholders
        char c;
        std::string potentialKey;
        std::string currentKey;
        std::vector<char> currentData;

        for (int i = 0; i < size; i++) {
            c = data[i];

            if (ID3::isValidChar(data[i])) {
                // check next 3 characters
                potentialKey = data[i];
                if (i < size - 3) {
                    int j = 1;
                    for (; j < 4; j++) {
                        if (ID3::isValidChar(data[i + j])) {
                            potentialKey += data[i + j];
                        }
                        else {
                            i += j - 1;
                            while (potentialKey.size() != 0) {
                                currentData.push_back(potentialKey[0]);
                                potentialKey.erase(potentialKey.begin());
                            }
                            break;
                        }
                    }

                    if (potentialKey.size() == 4) {
                        if (ID3::ID3v2::frameTags.containsKey(potentialKey)) {
                            // found key
                            if (currentKey != "") {
                                id3v2Data[currentKey] = currentData;
                                currentKey.clear();
                                currentData.clear();
                            }
                            currentKey = potentialKey;
                            potentialKey.clear();
                            i += j - 1;
                        }
                        else {
                            currentData.push_back(data[i]);
                        }
                    }
                }
                else {
                    currentData.push_back(data[i]);
                }
            }
            else {
                currentData.push_back(data[i]);
            }
        }

        // add last key
        if (currentKey != "") {
            id3v2Data[currentKey] = currentData;
        }

        // left shift data
        unsigned int ogSize = data.size();
        for (unsigned int i = 0; i < ogSize - size; i++) {
            data[i] = data[i + size];
        }
        data.resize(ogSize - size);
    }
};

#endif