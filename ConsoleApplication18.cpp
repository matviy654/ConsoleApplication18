#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

class Reservoir {
    char* name;
    double width;
    double length;
    double maxDepth;
    char* type;

public:
    
    Reservoir() : name(nullptr), width(0), length(0), maxDepth(0), type(nullptr) {}

    
    explicit Reservoir(const char* _name, double _width, double _length, double _maxDepth, const char* _type)
        : width(_width), length(_length), maxDepth(_maxDepth) {
        name = new char[strlen(_name) + 1];
        strcpy(name, _name);
        type = new char[strlen(_type) + 1];
        strcpy(type, _type);
    }

    Reservoir(const Reservoir& other) : width(other.width), length(other.length), maxDepth(other.maxDepth) {
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
        type = new char[strlen(other.type) + 1];
        strcpy(type, other.type);
    }

  
    ~Reservoir() {
        delete[] name;
        delete[] type;
    }

    void setName(const char* _name) {
        delete[] name;
        name = new char[strlen(_name) + 1];
        strcpy(name, _name);
    }

    const char* getName() const { return name; }

    double calculateVolume() const {
        return width * length * maxDepth;
    }

    double calculateSurfaceArea() const {
        return width * length;
    }

    bool isSameType(const Reservoir& other) const {
        return strcmp(type, other.type) == 0;
    }

    bool compareSurfaceArea(const Reservoir& other) const {
        if (isSameType(other)) {
            return calculateSurfaceArea() > other.calculateSurfaceArea();
        }
        return false;
    }

    void display() const {
        cout << "Назва: " << name << ", Ширина: " << width
            << ", Довжина: " << length << ", Максимальна глибина: " << maxDepth
            << ", Тип: " << type << endl;
    }

    void saveToTextFile(ofstream& file) const {
        file << name << " " << width << " " << length << " " << maxDepth << " " << type << endl;
    }

    void saveToBinaryFile(ofstream& file) const {
        size_t nameLen = strlen(name);
        size_t typeLen = strlen(type);
        file.write((char*)&nameLen, sizeof(nameLen));
        file.write(name, nameLen);
        file.write((char*)&width, sizeof(width));
        file.write((char*)&length, sizeof(length));
        file.write((char*)&maxDepth, sizeof(maxDepth));
        file.write((char*)&typeLen, sizeof(typeLen));
        file.write(type, typeLen);
    }
};

class ReservoirArray {
    Reservoir* reservoirs;
    size_t size;

public:
    ReservoirArray() : reservoirs(nullptr), size(0) {}

    ~ReservoirArray() {
        delete[] reservoirs;
    }

    void addReservoir(const Reservoir& reservoir) {
        Reservoir* newReservoirs = new Reservoir[size + 1];
        for (size_t i = 0; i < size; ++i) {
            newReservoirs[i] = reservoirs[i];
        }
        newReservoirs[size] = reservoir;
        delete[] reservoirs;
        reservoirs = newReservoirs;
        ++size;
    }

    void removeReservoir(size_t index) {
        if (index >= size) return;
        Reservoir* newReservoirs = new Reservoir[size - 1];
        for (size_t i = 0, j = 0; i < size; ++i) {
            if (i != index) {
                newReservoirs[j++] = reservoirs[i];
            }
        }
        delete[] reservoirs;
        reservoirs = newReservoirs;
        --size;
    }

    void displayAll() const {
        for (size_t i = 0; i < size; ++i) {
            reservoirs[i].display();
        }
    }

    void saveAllToTextFile(const char* filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            for (size_t i = 0; i < size; ++i) {
                reservoirs[i].saveToTextFile(file);
            }
            file.close();
        }
    }

    void saveAllToBinaryFile(const char* filename) const {
        ofstream file(filename, ios::binary);
        if (file.is_open()) {
            for (size_t i = 0; i < size; ++i) {
                reservoirs[i].saveToBinaryFile(file);
            }
            file.close();
        }
    }
};

int main() {
    ReservoirArray arr;

    Reservoir r1("Озеро", 500, 300, 20, "озеро");
    Reservoir r2("Море", 1000, 800, 50, "море");

    arr.addReservoir(r1);
    arr.addReservoir(r2);

    arr.displayAll();

    arr.saveAllToTextFile("reservoirs.txt");
    arr.saveAllToBinaryFile("reservoirs.bin");

    return 0;
}
