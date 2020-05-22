//#include <glob.h>

#include <cstring>

template <class Type>
class Vector {
private:
    struct Variable {
        char *name = nullptr;
        Type id = 0;

        friend bool operator== (const Variable a, const Variable b) {
            return strcmp(a.name, b.name) == 0;
        }

        friend bool operator== (const Variable a, const char str[]) {
            return strcmp(a.name, str) == 0;
        }

        friend bool operator!= (const Variable a, const Variable b) {
            return strcmp(a.name, b.name) != 0;
        }

        friend bool operator!= (const Variable a, const char str[]) {
            return strcmp(a.name, str) != 0;
        }
    };

    Variable *array = nullptr;
    size_t size = 0;
    size_t maxSize = 1;

public:
    Vector() = default;

    Variable operator[] (size_t pos) {
        return array[pos];
    }

    void pushBack(Variable val) {
        if (size == 0) {
            array = new Variable[1];
            ++size;

            array[0] = val;
            return;
        }

        if (size == maxSize) {
            maxSize *= 2;
            auto newArr = new Variable[maxSize];

            for (int i = 0; i < size; ++i) {
                newArr[i] = array[i];
            }

            delete[] array;
            array = newArr;
        }

        array[size] = val;
        ++size;
    }

    size_t find(const char name[]) {

        for (int i = 0; i < size; ++i) {
            if (array[i] == name)
                return i;
        }
    }

    size_t findFrom(const char start[], const char name[]) {
        size_t current = find(start);
        ++current;

        while (array[current].id != 0) {
            if (array[current] == name)
                return current;

            ++current;
        }
    }

    size_t findFrom(size_t current, const char name[]) {
        ++current;

        while (array[current].id != 0) {
            if (array[current] == name)
                return array[current].id;

            ++current;
        }
    }

    size_t countFromToZero(size_t start) {
        size_t current = start;
        ++current;

        while (array[current].id != 0) {
            ++current;
        }

        return current - start - 1;
    }

    void dump() {
        for (int i = 0; i < size; ++i) {
            printf("%s %d\n", array[i].name, array[i].id);
        }
    }

    ~Vector() {
        delete[] array;
    }
};