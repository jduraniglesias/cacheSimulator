//https://cplusplus.com/reference/bitset/bitset/
//https://cplusplus.com/reference/string/stoi/
//https://cplusplus.com/reference/cmath/log2/
//https://www.geeksforgeeks.org/returning-multiple-values-from-a-function-using-tuple-and-pair-in-c/
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <math.h>
#include <queue>
#include <bitset>
#include <map>
#include <vector>
using namespace std;

void printQ(queue<int> q) {
    while (!q.empty()) {
        cout << q.front() << endl;
        q.pop();
    }
}

int findIndexOf(int value, map<int, int> m) {
    for (auto it : m) {
        if (value == it.second) {
            return it.first;
        }
    }
    return -1;
}

pair<int, int> findIndexOfSet(int value, map<int, map<int, int>> m) {
    for (auto it : m) {
        for (auto it2 : it.second) {
            if (it2.second == value) {
                return pair<int, int>(it.first, it2.first);
            }
        }
    }
    return pair<int, int>(-1, -1);
}

queue<int> removeFromQueue(int value, queue<int> q) {
    vector<int> v;
    int size = q.size();
    bool check = true;
    for (int i = 0; i < size; i++) {
        if (q.front() == value && check == true) {
            q.pop();
            check == false;
        }
        else {
            v.push_back(q.front());
            q.pop();
        }
    }
    for (int i = 0; i < v.size(); i++) {
        q.push(v[i]);
    }
    return q;
}


queue<int> replaceInQueueQ1(queue<int> q1, map<int, int> m1) {
    for (auto it : m1) {
        q1.push(it.second);
    }
    return q1;
}

unsigned int hexToDecimal(string num) {
    string tempS = num.substr(2, num.length() - 1);
    unsigned int temp = stoul(tempS, nullptr, 16);
    return temp;
}

string decimalToBinary(unsigned int num) {
    bitset<32> newNum(num);
    string binaryNum = newNum.to_string();
    return binaryNum;
}

int binaryToDecimal(string binary) {
    int temp = stoi(binary, nullptr, 2);
    return temp;
}

pair<map<int, int>, int> cacheSimulationDirect(unsigned long long system, unsigned long long cacheSize, unsigned long long blockSize, string binary, map<int, int> m, int hits) {
    unsigned int lines = cacheSize / blockSize;
    int bitsWide = log2(system) ;
    binary = binary.substr(binary.length() - bitsWide);
    int offsetField = log2(blockSize);
    int lineField = log2(lines);
    int tagField = bitsWide - offsetField - lineField;
    //cout << "Binary: " << binary << endl;
    string offset = binary.substr(binary.length() - offsetField);
    binary = binary.substr(0, binary.length() - offsetField);
    string line = binary.substr(binary.length() - lineField);
    binary = binary.substr(0, binary.length() - lineField);
    string tag = binary.substr(binary.length() - tagField);
    binary = binary.substr(0, binary.length() - tagField);
    //cout << "Offset: " << offset << endl;
    //cout << "Line: " << line << endl;
    //cout << "Tag: " << tag << endl;
    int index = binaryToDecimal(line);
    int tagMapped = binaryToDecimal(tag);
    if (findIndexOf(tagMapped, m) == index) {
        hits++;
    }
    else {
        m[index] = tagMapped;
    }
    return pair<map<int, int>, int>(m, hits);
}
tuple<map<int, int>, int, queue<int>> cacheSimulationFully(unsigned long long system, unsigned long long cacheSize, unsigned long long blockSize, string binary, map<int, int> m, int j, int hits, queue<int> q, string replacement) {
    unsigned int lines = cacheSize / blockSize;
    int bitsWide = log2(system);
    binary = binary.substr(binary.length() - bitsWide);
    int offsetField = log2(blockSize);
    int tagField = bitsWide - offsetField;
    //cout << "Binary: " << binary << endl;
    string offset = binary.substr(binary.length() - offsetField);
    binary = binary.substr(0, binary.length() - offsetField);
    string tag = binary.substr(binary.length() - tagField);
    binary = binary.substr(0, binary.length() - tagField);
    //cout << "Offset: " << offset << endl;
    //cout << "Tag: " << tag << endl;
    int tagMapped = binaryToDecimal(tag);
    if (findIndexOf(tagMapped, m) != -1) {
        if (replacement == "lru") {
            q = removeFromQueue(tagMapped, q);
            q.push(tagMapped);
        }
        hits++;
    }

    else if (m.size() >= cacheSize) {
        for (auto it : m) {
            if (it.second == q.front()) {
                m[it.first] = tagMapped;
            }
        }
        q.pop();
        q.push(tagMapped);
    }
    
    else {
        q.push(tagMapped);
        m[j] = tagMapped;
    }
    
    return tuple<map<int, int>, int, queue<int>>(m, hits, q);
}

tuple<map<int, map<int, int>>, int, queue<int>> cacheSimulationSet(unsigned long long system, unsigned long long cacheSize, unsigned long long blockSize, string binary, map<int, map<int, int>> m, int set, int hits, string replacement, queue<int> q) {
    unsigned int sets = cacheSize / (blockSize * set);
    int bitsWide = log2(system);
    binary = binary.substr(binary.length() - bitsWide);
    int offsetField = log2(blockSize);
    int setField = log2(sets);
    int tagField = bitsWide - offsetField - setField;
    //cout << "Binary: " << binary << endl;
    string offset = binary.substr(binary.length() - offsetField);
    binary = binary.substr(0, binary.length() - offsetField);
    string setsString = binary.substr(binary.length() - setField);
    binary = binary.substr(0, binary.length() - setField);
    string tag = binary.substr(binary.length() - tagField);
    binary = binary.substr(0, binary.length() - tagField);
    //cout << "Offset: " << offset << endl;
    //cout << "Set: " << setsString << endl;
   // cout << "Tag: " << tag << endl;
    int j = 0;
    int setInt = binaryToDecimal(setsString);
    int tagMapped = binaryToDecimal(tag);
    queue<int> q1;
    map<int, int> m1;
    //cout << "Tag Decimal: " << tagMapped << endl;
    for (int i = 0; i < bitsWide; i++) {
        if (i != 0 && (i % set) == 0) {
            j++;
        }
        if (setInt == j) {
            if (m.find(i) != m.end()) {
                pair<int, int> result = findIndexOfSet(tagMapped, m);
                if (result.first != -1) {
                    if (replacement == "lru") {
                        q = removeFromQueue(tagMapped, q);
                        q.push(tagMapped);
                    }
                    hits++;
                    break;
                }
                else if ((i + 1) % set == 0) {
                    for (int k = (i - (i % set)); k <= i; k++) {
                        queue<int> tempQ = q;
                        int z = 0;
                        while (!tempQ.empty()) {
                            if (tempQ.front() == m[k].at(j)) {
                                m1[z] = m[k].at(j);
                            }
                            z++;
                            tempQ.pop();
                        }
                    }
                    q1 = replaceInQueueQ1(q1, m1);
                    pair<int, int> p = findIndexOfSet(q1.front(), m);
                    //cout << "Replaced " << q1.front() << " with " << tagMapped << endl;
                    q = removeFromQueue(q1.front(), q);
                    m[p.first][j] = tagMapped;
                    q.push(tagMapped);
                    
                    
                    break;
                }
            }
            else {
                q.push(tagMapped);
                m[i].insert(pair<int, int>(j, tagMapped));
                break;
            }
        }
    }
    return tuple<map<int, map<int, int>>, int, queue<int>>(m, hits, q);
}

int main()
{
    fstream file;
    string filename;
    string fullFile = "";
    int set;
    string associativity;
    int tempsystemSize;
    cout << "Javier Duran" << endl;
    cout << "What is the system size (Enter as power of 2, Ex: If 2^32 -> enter '32'): ";
    cin >> tempsystemSize;
    int tempcacheSize;
    cout << "What is the cache size (Enter as power of 2, Ex: If 2^9 -> enter '9') ";
    cin >> tempcacheSize;
    int tempblockSize;
    cout << "What is the block/line size (Enter as power of 2, Ex: If 2^4 -> enter '4'): ";
    cin >> tempblockSize;
    unsigned long long systemSize = pow(2, tempsystemSize);
    unsigned long long cacheSize = pow(2, tempcacheSize);
    unsigned long long blockSize = pow(2, tempblockSize);
    cout << "Type the associativity you want to use (Direct, Fully, or Set): ";
    cin >> associativity;
    string tempAs;
    for (int i = 0; i < associativity.length(); i++) {
        tempAs += tolower(associativity[i]);
    }
    associativity = tempAs;
    cout << "If using Set Associativity, type the set number (if not, then type '0'): ";
    cin >> set;
    string tempReplacement;
    string replacement;
    cout << "Select what replacement method to use (LRU or FIFO) (If associativity is direct, type '0'): ";
    cin >> tempReplacement;
    for (int i = 0; i < tempReplacement.length(); i++) {
        replacement += tolower(tempReplacement[i]);
    }
    cout << "Type the name of the file to be read: ";
    cin >> filename;
    string temp = "trace/";
    fullFile = temp + filename;
    file.open(fullFile);
    string cacheNum;
    int j = 0;
    int hits = 0;
    int total = 0;
    map<int, int> directMap;
    map<int, int> fullyMap;
    map<int, map<int, int>> setMap;
    queue<int> q;
    while (getline(file, cacheNum)) {
        cacheNum = cacheNum.substr(2, cacheNum.length() - 4);
       //cout << "Hex: " << cacheNum << endl;
        //cout << "Decimal: " << hexToDecimal(cacheNum) << endl;
        if (associativity == "direct") {
            pair<map<int, int>, int> m = cacheSimulationDirect(systemSize, cacheSize, blockSize, decimalToBinary(hexToDecimal(cacheNum)), directMap, hits);
            directMap = m.first;
            hits = m.second;
        }
        else if (associativity == "fully") {
            map<int, int> m1;
            int h1;
            queue<int> q1;
            int temp = hits;
            tie(m1, h1, q1) = cacheSimulationFully(systemSize, cacheSize, blockSize, decimalToBinary(hexToDecimal(cacheNum)), fullyMap, j, hits, q, replacement);
            fullyMap = m1;
            hits = h1;
            q = q1;
            if (temp == hits) {
                j++;
            }
        }
        else if (associativity == "set") {
            map<int, map<int, int>> m1;
            int h1;
            queue<int> q1;
            int temp = hits;
            tie(m1, h1, q1) = cacheSimulationSet(systemSize, cacheSize, blockSize, decimalToBinary(hexToDecimal(cacheNum)), setMap, set, hits, replacement, q);
            setMap = m1;
            hits = h1;
            q = q1;
        }
        total++;
        //cout << endl;
    }
    /*cout << "Checking Map: " << endl;
    if (associativity == "direct") {
        for (auto it : directMap) {
            cout << "Index: " << it.first << " Tag: " << it.second << endl;
        }
        cout << "Total Hits: " << hits << endl;
    }
    else if (associativity == "fully") {
        for (auto it : fullyMap) {
            cout << "Index: " << it.first << " Tag: " << it.second << endl;
        }
        cout << "Total Hits: " << hits << endl;
    }
    else if (associativity == "set") {
        for (auto it : setMap) {
            cout << "Index: " << it.first;
            for (auto it2 : it.second) {
                cout << " Set: " << it2.first << " Tag: " << it2.second << endl;
            }
        }
        cout << "Total Hits: " << hits << endl;
    }
    */
    double hitRate = (double)hits / (double)total;
    cout << "Total: " << total << endl;
    cout << "Misses: " << (total - hits) << endl;
    cout << "Hit Rate (hits/total): " << hitRate << endl;

    file.close();
}

