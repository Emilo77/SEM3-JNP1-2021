#include <cassert>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <unordered_set>
#include "maptel.h"

using std::string;
using std::unordered_map;
using std::pair;
using std::vector;
using std::cerr;
using std::endl;

using TelNumType = std::string;
using TelMapType = std::unordered_map<TelNumType, TelNumType>;
using MapIdType = unsigned long;
using MapOfNumChanges = std::unordered_map<MapIdType, TelMapType>;

namespace {
#ifdef NDEBUG
    const bool debug = false;
#else
    const bool debug = true;
#endif

    // ---------------*******************------------------------
    // Początek części kodu do obsługi wypisywania informacji diagnostycznych
    const vector<string> &errorMessages() {
        static auto ref = new vector<string>{"create", "insert", "transform",
                                             "erase", "delete"};
        return *ref;
    }

    const string &STARTMESSAGE() {
        static auto ref = new string("maptel: maptel_");
        return *ref;
    }

    const int CREATE = 0;
    const int INSERT = 1;
    const int TRANSFORM = 2;
    const int ERASE = 3;

    const int DELETE = 4;
    const bool CALL = true;
    const bool PERFORM = false;

    /*
     * Funkcje do wypisywania informacji diagnostycznych
     * bool status to tryb wywołania tej funkcji:
     *  - CALL (true) oznacza wiadomość wypisywaną przez rozpoczęciem działania
     * w funkcji
     *  - PERFORM (false) oznacza wiadomość wypisywaną po zakończeniu działania
     * funkcji
     */
    void errMessageCreate(bool status, MapIdType id = (MapIdType) " ") {
        if (status) {
            cerr << STARTMESSAGE() << errorMessages()[CREATE] << "()" << endl;
        } else {
            cerr << STARTMESSAGE() << errorMessages()[CREATE] <<
                 ": new map id = " << id << endl;
        }
    }

    void errMessageDelete(MapIdType id, bool status) {
        if (status) {
            cerr << STARTMESSAGE() << errorMessages()[DELETE] <<
                 "(" << id << ")" << endl;
        } else {
            cerr << STARTMESSAGE() << errorMessages()[DELETE] << ": map "
                 << id << " deleted" << endl;
        }
    }

    void errMessageInsert(MapIdType id, bool status, const char *telSrc = " ",
                          const char *telDst = " ") {
        if (status) {
            cerr << STARTMESSAGE() << errorMessages()[INSERT] << "(" <<
                 id << ", " << telSrc << ", " << telDst << ")" << endl;
        } else {
            cerr << STARTMESSAGE() << errorMessages()[INSERT] << ": inserted"
                 << endl;
        }
    }

    void errMessageErase(MapIdType id, bool status, bool erased = " ",
                         const char *telSrcToErase = " ") {
        if (status) {
            cerr << STARTMESSAGE() << errorMessages()[ERASE] << "(" <<
                 id << ", " << telSrcToErase << ")" << endl;
        } else {
            if (erased) {
                cerr << STARTMESSAGE() << errorMessages()[ERASE] << ": erased"
                     << endl;
            } else {
                cerr << STARTMESSAGE() << errorMessages()[ERASE] <<
                     ": nothing to erase" << endl;
            }
        }
    }


    void errMessageTransformCall(MapIdType id, const char *telSrc, const char
    *telDstMemory, size_t len) {
        cerr << STARTMESSAGE() << errorMessages()[TRANSFORM] << "(" << id
             << ", " << telSrc << ", " << &telDstMemory << ", " << len <<
             ")" << endl;
    }

    void errMessageTransformPerform(TelNumType &telSrc, TelNumType &telDst,
                                    bool cycleDetected) {
        if (cycleDetected) {
            cerr << STARTMESSAGE() << errorMessages()[TRANSFORM]
                 << ": cycle detected" << endl;
        }
        cerr << STARTMESSAGE() << errorMessages()[TRANSFORM] << ": " <<
             telSrc << " -> " << telDst << endl;
    }

    // koniec części kodu do obsługi wypisywania informacji diagnostycznych
    // ---------------*******************------------------------

    MapOfNumChanges &mapOfNumChanges() {
        static auto ref = new MapOfNumChanges();
        return *ref;
    };

    MapIdType &idCounter() {
        static auto ref = new MapIdType(0);
        return *ref;
    };

    void checkIfNumCorrect(const char *telNum) {
        assert(telNum != NULL);
        size_t size = 0;
        for (size_t i = 0; telNum[i]; i++) {
            assert(telNum[i] >= '0' && telNum[i] <= '9');
            size++;
            assert(size <= jnp1::TEL_NUM_MAX_LEN);
        }
        assert(telNum[size] == '\0');
        assert(size != 0);
    }

    void checkTelNumSize(const string &TelNum) {
        if (TelNum.empty() && TelNum.size() <= jnp1::TEL_NUM_MAX_LEN) {
            assert(0);
        }
    }

    void checkIfTelMapExists(MapIdType id) {
        if (mapOfNumChanges().find(id) == mapOfNumChanges().end()) {
            assert(0);
        } else {

        }
    }

    void checkIfCopied(const char *telNum0, string telNum1) {
        bool copiedGood = true;
        for (size_t i = 0; telNum1[i]; i++) {
            if (telNum0[i] != telNum1[i]) {
                copiedGood = false;
                break;
            }
        }
        if (telNum0[telNum1.size()] != '\0') {
            copiedGood = false;
        }
        if (!copiedGood) {
            assert(0);
        }
    }

}

namespace jnp1 {

    [[maybe_unused]] unsigned long maptel_create(void) {

        if (debug) {
            errMessageCreate(CALL);
        }

        TelMapType newTelMap;
        pair<MapIdType, TelMapType> newPair = {idCounter(), newTelMap};
        idCounter()++;
        auto result = mapOfNumChanges().insert(newPair);
        if (!result.second) {
            assert(0);
        }

        if (debug) {
            errMessageCreate(PERFORM, idCounter() - 1);
        }

        return idCounter() - 1;
    }

    [[maybe_unused]] void maptel_delete(MapIdType id) {

        if (debug) {
            errMessageDelete(id, CALL);
        }

        checkIfTelMapExists(id);

        auto result = mapOfNumChanges().erase(id);

        assert(result == 1);

        if (debug) {
            errMessageDelete(id, PERFORM);
        }

    }

    [[maybe_unused]] void
    maptel_insert(MapIdType id, char const *tel_src, char const *tel_dst) {
        if (debug) {
            errMessageInsert(id, CALL, tel_src ? tel_src : "",
                             tel_dst ? tel_dst : "");
        }

        checkIfTelMapExists(id);
        checkIfNumCorrect(tel_src);
        checkIfNumCorrect(tel_dst);

        TelNumType newTelSrc = string(tel_src);
        TelNumType newTelDst = string(tel_dst);

        mapOfNumChanges().at(id).insert_or_assign(newTelSrc, newTelDst);

        if (debug) {
            errMessageInsert(id, PERFORM);
        }
    }

    [[maybe_unused]] void maptel_erase(MapIdType id, char const *tel_src) {
        if (debug) {
            errMessageErase(id, CALL, " ", tel_src ?
                                           tel_src : "");
        }

        checkIfTelMapExists(id);
        checkIfNumCorrect(tel_src);

        TelMapType &mapWithGivenId = mapOfNumChanges().at(id);
        TelNumType telSrcToErase = string(tel_src);
        bool erased = false;

        if (mapWithGivenId.find(telSrcToErase) != mapWithGivenId.end()) {
            auto result = mapOfNumChanges().at(id).erase(telSrcToErase);

            if (result != 1) {
                assert(0);
            }
            erased = true;
        }

        if (debug) {
            errMessageErase(id, PERFORM, erased);
        }
    }

    [[maybe_unused]] void
    maptel_transform(MapIdType id, char const *tel_src, char *tel_dst,
                     size_t len) {
        if (debug) {
            errMessageTransformCall(id, tel_src ? tel_src : "", tel_dst,
                                    len);
        }

        checkIfTelMapExists(id);
        checkIfNumCorrect(tel_src);
        assert(tel_dst != NULL);

        bool endOfPath = false;
        TelNumType telSrcStart = string(tel_src);
        TelNumType tempTelSrc = telSrcStart;
        TelMapType &mapWithGivenId = mapOfNumChanges().at(id);
        std::unordered_set<string> previous_keys;
        bool cycleDetected = false;

        auto contains = [&previous_keys, &mapWithGivenId](TelNumType num) {
            auto a = previous_keys.find(mapWithGivenId.at(num));
            return a != previous_keys.end();
        };

        while (!endOfPath) {
            if (mapWithGivenId.find(tempTelSrc) == mapWithGivenId.end()) {
                endOfPath = true;
            } else if (contains(tempTelSrc)) {
                endOfPath = true;
                cycleDetected = true;
                tempTelSrc = telSrcStart;
            } else {
                previous_keys.insert(tempTelSrc);
                tempTelSrc = mapWithGivenId.at(tempTelSrc);
            }
        }

        checkTelNumSize(tempTelSrc);
        assert(len >= tempTelSrc.size() + 1);

        std::strcpy(tel_dst, tempTelSrc.c_str());

        checkIfCopied(tel_dst, tempTelSrc);


        if (debug) {
            errMessageTransformPerform(telSrcStart, tempTelSrc,
                                       cycleDetected);
        }
    }

}

