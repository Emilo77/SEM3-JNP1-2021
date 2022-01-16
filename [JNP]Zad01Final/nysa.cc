#include <regex>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;

// Definicja numeru sygnału/ilości sygnałów
using sigNum =  uint32_t;
// Definicja wektora<sigNum>
using sigVector = vector<sigNum>;

// Definicja mapy, która przechowuje bramki z wejścia {wyjście, {wejścia, nazwa}}
using gateMapType = unordered_map<sigNum, pair<sigVector, string>>;
// Definicja mapy pod sortowanie topologiczne:
// {sygnał, {[sygnały, do których wchodzi], liczba sygnałów wchodzących}}
using topoSortMapType = unordered_map<sigNum, pair<sigVector, size_t>>;
// Definicja mapy z wartościami logicznymi do wypisania linii tablicy prawdy
using sigValMapType = map<sigNum, bool>;


// Funkcje pomocnicze

/* Potęgowanie binarne o podstawie 2 */
void binPowerBaseTwo(uint64_t& raisedNumber, size_t exponent) {
    raisedNumber = 1;
    uint64_t base = 2;

    while (exponent > 0) {
        if (exponent % 2)
            raisedNumber *= base;
        base *= base;
        exponent /= 2;
    }
}

/* Komparator do posortowania wektora malejąco */
bool sortComparator(sigNum a, sigNum b) {
    return a > b;
}


/* Funkcja wypisująca na wyjście błędy */
void printError(string& errorMessage, size_t lineNumber = 0) {
    if (lineNumber == 0)
        cerr << errorMessage << '\n';
    else
        cerr << "Error in line " << lineNumber << ": " << errorMessage << '\n';
}

// Koniec funkcji pomocniczych


// Definicje bramek

bool gateNOT(vector<bool>& v) {
    return !v.at(0);
}

bool gateXOR(vector<bool>& v) {
    return (v.at(0) || v.at(1)) && !(v.at(0) && v.at(1));
}

bool gateAND(const vector<bool>& v) {
    return (find(v.begin(), v.end(), false) == v.end());
}

bool gateNAND(const vector<bool>& v) {
    return (find(v.begin(), v.end(), false) != v.end());
}

bool gateOR(const vector<bool>& v) {
    return (find(v.begin(), v.end(), true) != v.end());
}

bool gateNOR(const vector<bool>& v) {
    return (std::find(v.begin(), v.end(), true) == v.end());
}

// Koniec definicji bramek


// Funkcje odpowiedzialne za wczytanie wejścia

/* Funkcja zwracająca informację, czy linia jest poprawna */
static bool isLineValid(string& line) {
    const string regexNumbers = "(\\s+[1-9][0-9]{0,8})";
    const string regexOneSignalGate = "\\s*NOT" + regexNumbers + "{2}\\s*";
    const string regexTwoSignalGate = "\\s*XOR" + regexNumbers + "{3}\\s*";
    const string regexManySignalGates =
            "\\s*(AND|NAND|OR|NOR)" + regexNumbers + "{3,}\\s*";

    const regex validLineRegex('(' + regexManySignalGates + '|' +
                               regexOneSignalGate + '|' +
                               regexTwoSignalGate + ')');

    return regex_match(line, validLineRegex);
}


/* Funkcja bierze jako argument poprawną linie, pobiera z niej dane, a następnie
   zapisuje je do zmiennych gateInput, gateOutput i gateName */
static void getGateData(string& line, sigVector& gateInput,
                        sigNum &gateOutput, string& gateName) {

    static const regex wordRegex("([^\\s]+)");
    auto wordIterator = sregex_iterator(line.begin(), line.end(), wordRegex);
    auto wordIteratorEnd = sregex_iterator();

    for (size_t i = 0; wordIterator != wordIteratorEnd; i++, wordIterator++) {
        stringstream stream(wordIterator->str());
        if (i == 0) {
            stream >> gateName;
        }
        else if (i == 1) {
            stream >> gateOutput;
        }
        else {
            sigNum gateInputValue;
            stream >> gateInputValue;
            gateInput.push_back(gateInputValue);
        }
    }
}

/* Funkcja pobiera kolejne linie ze standardowego wejścia, a następnie
   sprawdza czy dana linia jest odpowiednio opisaną bramką, jeśli tak
   to zapisuje ją do mapy gateMap. */
bool getInputAndCheckIfValid(gateMapType& gateMap) {

    string line;
    size_t lineNumber = 1;
    bool returnValue = true;

    while (getline(cin, line)) {
        if (!isLineValid(line)) {
            printError(line, lineNumber);
            returnValue = false;
        }
        else {
            string gateName;
            sigNum gateOutput;
            sigVector gateInput;
            getGateData(line, gateInput, gateOutput, gateName);

            if (gateMap.contains(gateOutput)) {
                string errorMessage = "signal " + to_string(gateOutput) +
                                            " is assigned to multiple outputs.";
                printError(errorMessage, lineNumber);
                returnValue = false;
            }
            else {
                gateMap.insert({gateOutput, {gateInput, gateName}});
            }
        }
        lineNumber++;
    }

    return returnValue;
}

// Koniec funkcji odpowiedzialnych, za wczytanie wejścia


// Początek funkcji odpowiedzialnych za sortowanie topologiczne

/* Funkcja przygotowująca strukturę do sortowania topologicznego */
void topoSortMapInitialize(gateMapType& gateMap, topoSortMapType& topoSortMap) {

    for (auto &element: gateMap) {

        if (!topoSortMap.contains(element.first)) {
            pair<sigNum, pair<sigVector, sigNum>> newElement;
            sigVector newVector;
            newElement = {element.first,
                          {newVector, element.second.first.size()}};
            topoSortMap.insert(newElement);
        }

        for (auto &vectorElement: element.second.first) {
            if (topoSortMap.contains(vectorElement)) {
                topoSortMap.at(vectorElement).first.push_back(element.first);
            }
            else {
                sigVector vec;
                vec.push_back(element.first);
                topoSortMap.insert({vectorElement, {vec, 0}});
            }
        }

        topoSortMap.at(element.first).second = element.second.first.size();
    }
}

/* Funkcja generuje poprawną ścieżkę przejść i sprawdza, czy występuje cykl */
bool topoSort(topoSortMapType& topoSortMap, sigVector& path) {
    for (auto& [signal, destVecAndInputsAmount] : topoSortMap) {
        size_t inputsAmount = destVecAndInputsAmount.second;
        if (inputsAmount == 0) {
            path.push_back(signal);
        }
    }

    size_t pathSize = path.size();
    size_t pathIndex = 0;
    while (pathIndex < pathSize) {
        sigVector destinationSignalsVector =
                                    topoSortMap.at(path.at(pathIndex)).first;
        pathIndex++;
        for (auto& signal : destinationSignalsVector) {
            topoSortMap.at(signal).second--;
            if (topoSortMap.at(signal).second == 0) {
                path.push_back(signal);
                pathSize++;
            }
        }
    }

    bool cycleInGates = path.size() != topoSortMap.size();
    if (cycleInGates) {
        string errMsg =
                "Error: sequential logic analysis has not yet been implemented.";
        printError(errMsg);
    }

    return cycleInGates;
}

/* Funkcja zapisuje do wektora listę sygnałów, które są jedynie wejściami
   do bramek i usuwa je z topoSortMap */
void removeFirstInputSignalsMakeVector(topoSortMapType& topoSortMap,
                                                sigVector& inputSignalsVector) {

    for (auto &element: topoSortMap)
        if (element.second.second == 0)
            inputSignalsVector.push_back(element.first);

    sort(inputSignalsVector.begin(), inputSignalsVector.end(), sortComparator);

    for (auto signalToRemove : inputSignalsVector) {
        for (auto &signalConnected : topoSortMap.at(signalToRemove).first) {
            topoSortMap.at(signalConnected).second--;
        }
        topoSortMap.erase(signalToRemove);
    }
}

// Koniec funkcji odpowiedzialnych za sortowanie topologiczne


// Początek funkcji odpowiedzialnych za stworzenie i wypisanie tablicy prawdy

/* Funkcja bierze jako argument mapę sigValMap, w której zapisana jest aktualnie
   analizowana kombinacja sygnałów wejścia, a następnie na podstawie odpowiedniej
   ścieżki przejścia dodaje do mapy wartości sygnałów wyjściowych bramek */
void goThroughGates(gateMapType& gateMap, sigValMapType& sigValMap,
                    sigVector& path) {
    vector<bool> logicSignalsVector;
    for (sigNum &object : path) {
        for (auto &inputElement : gateMap.at(object).first) {
            logicSignalsVector.push_back(sigValMap.at(inputElement));
        }

        if (gateMap.at(object).second.compare("NOT") == 0)
            sigValMap.at(object) = gateNOT(logicSignalsVector);
        else if (gateMap.at(object).second.compare("XOR") == 0)
            sigValMap.at(object) = gateXOR(logicSignalsVector);
        else if (gateMap.at(object).second.compare("AND") == 0)
            sigValMap.at(object) = gateAND(logicSignalsVector);
        else if (gateMap.at(object).second.compare("NAND") == 0)
            sigValMap.at(object) = gateNAND(logicSignalsVector);
        else if (gateMap.at(object).second.compare("OR") == 0)
            sigValMap.at(object) = gateOR(logicSignalsVector);
        else
            sigValMap.at(object) = gateNOR(logicSignalsVector);

        logicSignalsVector.clear();
    }
}

/* Funkcja inicjalizująca mapę wartości sygnałów na 0 */
void initializeSigValMap(sigValMapType& sigValMap,
                         sigVector& inputSignalsVector, sigVector& path) {
    for (sigNum signal : inputSignalsVector) {
        sigValMap.insert({signal, 0});
    }
    for (sigNum signal : path) {
        sigValMap.insert({signal, 0});
    }
}

/* Funkcja wypisująca na wyjście końcowy stan linii */
void printTruthTableLine(sigValMapType& sigValMap) {
    for (auto &element : sigValMap) {
        cout << element.second;
    }
    cout << '\n';
}

/* Funkcja zapisująca binarnie liczbę n, do sigValMap */
void saveCurrentNumber(sigValMapType& sigValMap,
                       sigVector& inputSignalsVector, uint64_t n) {
    size_t i = 0, inputSignalsVectorSize = inputSignalsVector.size();
    while (n != 0 && i < inputSignalsVectorSize) {
        sigValMap.at(inputSignalsVector.at(i)) = n % 2 != 0;
        n /= 2;
        i++;
    }
    while (i < inputSignalsVectorSize) {
        sigValMap.at(inputSignalsVector.at(i)) = false;
        i++;
    }
}


/* Funkcja tworzy odpowiednie kombinacje sygnałów wejściowych, uzupełnia
   wartości sygnałów wyjść bramek i wypisuje tablicę prawdy na wyjście */
void prepareAndPrintTruthTableLines(gateMapType gateMap,
                                sigVector inputSignalsVector, sigVector path) {
    sigValMapType sigValMap;
    uint64_t currentNumber = 1, biggestNumberCombination;
    size_t inputSignalsVectorSize = inputSignalsVector.size();

    initializeSigValMap(sigValMap, inputSignalsVector, path);
    goThroughGates(gateMap, sigValMap, path);
    printTruthTableLine(sigValMap);

    binPowerBaseTwo(biggestNumberCombination, inputSignalsVectorSize);
    while (currentNumber != biggestNumberCombination) {
        saveCurrentNumber(sigValMap, inputSignalsVector, currentNumber);

        goThroughGates(gateMap, sigValMap, path);

        printTruthTableLine(sigValMap);

        currentNumber++;
    }
}


int main() {

    gateMapType gateMap;
    if (!getInputAndCheckIfValid(gateMap))
        return 0;

    topoSortMapType topoSortMap;
    topoSortMapInitialize(gateMap, topoSortMap);

    sigVector inputSignalsVector, path;
    removeFirstInputSignalsMakeVector(topoSortMap, inputSignalsVector);
    if (topoSort(topoSortMap, path))
        return 0;


    // Ograniczenie dla ilości sygnałów wejściowych/początkowych
    assert(inputSignalsVector.size() <= 63);

    prepareAndPrintTruthTableLines(gateMap, inputSignalsVector, path);

    return 0;
}