#ifndef DATA_HPP
# define DATA_HPP

#include <iostream>
#include <vector>
#include <map>
#include "Mutex.hpp"

class Data {
public:
    Data(void);
    ~Data(void);
    int     AddNumber(const int& n);
    size_t  DeleteNumber(const int& n);
    void    PrintNumber(void);

//method
private:
    bool    AddMutex(const int& key);
    bool    DeleteMutex(const int& n);
    int     LockNumber(const int& key);
    int     UnlockNumber(const int& key);

    int     LockNumberMutex(void);
    int     UnlockNumberMutex(void);

    size_t  GetNumberSize(void);

//variable
private:
    std::vector<int> number;
    std::map<int, Mutex*> number_mutex;
    Mutex           *list_mutex;
};

#endif