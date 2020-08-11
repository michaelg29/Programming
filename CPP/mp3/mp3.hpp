#ifndef MP3_HPP
#define MP3_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

#include "id3.hpp"

/*
    MP3 class representing data in .mp3 file
*/
class MP3 {
public:
    /*
        constructors
    */

    // default
    MP3()
        : hasID3v1(false), hasID3v2(false) {}

    // read file upon initialization
    MP3(const char* path)
        : MP3() {
        read(path);
    }

    /*
        input/output methods
    */

    // read in mp3 file
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

    // write out mp3 file
    bool write(const char* path) {
        std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::ate);

        if (!file.is_open()) {
            return false;
        }

        // write id3 header and data
        if (hasID3v2) {
            // header
            file.write(reinterpret_cast<char*>(&id3v2), sizeof(id3v2));

            // tags and respective data
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

    /*
        modifiers
    */

    // erase tag (return false if not found)
    bool eraseTag(std::string tag) {
        return id3v2Data.erase(tag) != 0;
    }

    // set tag
    void setTag(std::string tag, std::vector<char> data) {
        id3v2Data[tag] = data;
    }

    // set character at index
    bool setChar(std::string tag, unsigned int idx, char c) {
        if (id3v2Data.find(tag) == id3v2Data.end() || idx >= id3v2Data[tag].size()) {
            return false;
        }

        return true;
    }

    // set data starting at index
    bool setData(std::string tag, unsigned int startIdx, std::vector<char> data) {
        if (id3v2Data.find(tag) == id3v2Data.end()) {
            return false;
        }

        unsigned int insertLength = data.size();

        // if beyond end, just insert at end
        if (startIdx > id3v2Data[tag].size()) {
            startIdx = id3v2Data[tag].size();
        }

        for (int i = startIdx; i < startIdx + insertLength; i++) {
            if (i >= id3v2Data[tag].size()) {
                // add to end
                id3v2Data[tag].push_back(data[i - startIdx]);
            }
            else {
                // reset existing character
                id3v2Data[tag][i] = data[i - startIdx];
            }
        }

        return true;
    }

    // replace range of data
    bool replaceData(std::string tag, unsigned int startIdx, unsigned int endIdx, std::vector<char> data) {
        if (id3v2Data.find(tag) == id3v2Data.end()) {
            return false;
        }
        
        // shift data
        /*
            3 cases:
            range = insertLen: no shift
            range > insertLen: shift left by (range - insertLen)
            range < insertLen: shift right by (insertLen - range)
        */
        unsigned int range = endIdx - startIdx;
        unsigned int insertLen = data.size();
        unsigned int ogSize = id3v2Data[tag].size();
        if (range > insertLen) {
            // shift left
            unsigned int shift = range - insertLen;
            for (int i = endIdx; i < ogSize; i++) {
                id3v2Data[tag][i - shift] = id3v2Data[tag][i];
            }
            id3v2Data[tag].resize(ogSize - shift);
        }
        else if (range < insertLen) {
            // shift right
            unsigned int shift = insertLen - range;
            id3v2Data[tag].resize(ogSize + shift);
            for (int i = ogSize - 1; i >= endIdx; i--) {
                id3v2Data[tag][i + shift] = id3v2Data[tag][i];
            }
        }

        // insert new data
        for (int i = startIdx; i < startIdx + insertLen; i++) {
            id3v2Data[tag][i] = data[i - startIdx];
        }

        return true;
    }

private:
    // data buffer
    std::vector<char> data;

    // whether or not id3v1 data was detected
    bool hasID3v1;
    // id3v1 data structure
    ID3::ID3v1 id3v1;
   
    // whether or not id3v2 data was detected
    bool hasID3v2;
    // id3v2 header structure
    ID3::ID3v2 id3v2;
    // id3v2 data map
    std::map<std::string, std::vector<char>> id3v2Data;

    /*
        private utility methods
    */

    // method to process id3v2 body
    void processID3V2() {
        id3v2Data.clear();

        // get size of id3v2 body
        unsigned int size = id3v2.calcSize();

        // placeholders
        std::string potentialKey;
        std::string currentKey;
        std::vector<char> currentData;

        // iterate through each character in
        for (int i = 0; i < size; i++) {
            if (ID3::isValidChar(data[i])) {
                // check next 3 characters
                potentialKey = data[i];
                if (i < size - 3) {
                    // space for new key until end of body
                    int j = 1;
                    for (; j < 4; j++) {
                        potentialKey += data[i + j];
                        if (!ID3::isValidChar(data[i + j])) {
                            // invalid character, move to next character
                            i += j;
                            // add potential key to data
                            for (int k = 0; k < j + 1; k++) {
                                currentData.push_back(potentialKey[k]);
                            }
                            potentialKey.clear();
                            break;
                        }
                    }

                    if (potentialKey.size() == 4) {
                        if (ID3::ID3v2::frameTags.containsKey(potentialKey)) {
                            // found key
                            if (currentKey != "") {
                                // set in data map
                                id3v2Data[currentKey] = currentData;
                                // clear placeholders
                                currentData.clear();
                            }
                            // update placeholders
                            currentKey = potentialKey;
                            potentialKey.clear();
                            // increment counter
                            i += j - 1; // - 1 because will increment in next loop
                        }
                        else {
                            // false potential, move to next character
                            currentData.push_back(data[i]);
                        }
                    }
                }
                else {
                    // no space for new key, simply add to data
                    currentData.push_back(data[i]);
                }
            }
            else {
                // invalid char, cannot be new key, simply add to data
                currentData.push_back(data[i]);
            }
        }

        // add last key if exists
        if (currentKey != "") {
            id3v2Data[currentKey] = currentData;
        }

        // left shift data
        unsigned int ogSize = data.size();
        for (unsigned int i = 0; i < ogSize - size; i++) {
            data[i] = data[i + size];
        }
        data.resize(ogSize - size);

        // image writing
        /*std::ofstream file("thumbnail.jpg", std::ios::out | std::ios::binary | std::ios::ate);

        if (file.is_open()) {
            file.write(&id3v2Data["APIC"][20], id3v2Data["APIC"].size() - 20);

            file.close();
        }*/
    }
};

#endif