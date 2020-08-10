#ifndef ID3_HPP
#define ID3_HPP

#include <string>
#include <vector>

#include "trie.hpp"

namespace ID3 {
    struct ID3v1 {
        char header[3];
        char title[30];
        char artist[30];
        char album[30];
        char year[4];
        char comment[30];
        char genre[1];
    };

    struct ID3v2 {
        char identifier[3];
        char major[1];
        char minor[1];
        char flags[1];
        char size[4];

        unsigned int calcSize() {
            unsigned int ret = 0;

            for (int i = 0; i < 4; i++) {
                ret += size[3 - i] << (7 * i);
            }

            return ret;
        }

        static trie::Trie frameTags;
    };

    trie::Trie ID3v2::frameTags({ { '0', '9' }, { 'A', 'Z' } });

    inline bool isValidChar(char c) {
        return ((c >= '0') && (c <= '9')) ||
            ((c >= 'A') && (c <= 'Z'));
    }

    inline void load() {
        std::vector<std::string> frameTags = {
            "AENC",
            "APIC",
            "ASPI",
            "COMM",
            "COMR",
            "ENCR",
            "EQU2",
            "ETCO",
            "GEOB",
            "GRID",
            "LINK",
            "MCDI",
            "MLLT",
            "OWNE",
            "PRIV",
            "PCNT",
            "POPM",
            "POSS",
            "RBUF",
            "RVA2",
            "RVRB",
            "SEEK",
            "SIGN",
            "SYLT",
            "SYTC",
            "TALB",
            "TBPM",
            "TCOM",
            "TCON",
            "TCOP",
            "TDEN",
            "TDLY",
            "TDOR",
            "TDRC",
            "TDRL",
            "TDTG",
            "TENC",
            "TEXT",
            "TFLT",
            "TIPL",
            "TIT1",
            "TIT2",
            "TIT3",
            "TKEY",
            "TLAN",
            "TLEN",
            "TMCL",
            "TMED",
            "TMOO",
            "TOAL",
            "TOFN",
            "TOLY",
            "TOPE",
            "TOWN",
            "TPE1",
            "TPE2",
            "TPE3",
            "TPE4",
            "TPOS",
            "TPRO",
            "TPUB",
            "TRCK",
            "TRSN",
            "TRSO",
            "TSOA",
            "TSOP",
            "TSOT",
            "TSRC",
            "TSSE",
            "TSST",
            "TYER",
            "TXXX",
            "UFID",
            "USER",
            "USLT",
            "WCOM",
            "WCOP",
            "WOAF",
            "WOAR",
            "WOAS",
            "WORS",
            "WPAY",
            "WPUB",
            "WXXX"
        };
    
        for (std::string tag : frameTags) {
            ID3v2::frameTags.insert(tag);
        }
    }

    inline void clear() {
        ID3v2::frameTags.cleanup();
    }
}

#endif