#include "Data.hpp"

using namespace std;

Data::Data(void) {
    this->list_mutex = new Mutex();
    AddNumber(0);
    AddMutex(0);
}

Data::~Data(void) {
    // need to free many thinsg maybe..
    delete this->list_mutex;
}

// return index of just pushed number
int Data::AddNumber(const int& n) {
    int ret = -1;
    LockNumberMutex();
    if (n == 0 || find(this->number.begin(), this->number.end(), n) == this->number.end()) {
        this->number.push_back(n);
        ret = number.size() - 1;
    }
    UnlockNumberMutex();
    if (ret > 0)
        AddMutex(ret - 1);
    return ret;
}

// return size of number
size_t Data::DeleteNumber(const int& n) {
    size_t ret = -1;
    int idx = -1;
    LockNumber(0);
    vector<int>::iterator    itr = find(this->number.begin(), this->number.end(), n);
    if (itr != this->number.end()) {
        this->number.erase(itr);
        ret = this->number.size();
        idx = itr - this->number.begin();
    }
    UnlockNumber(0);
    DeleteMutex(idx);
    return ret;
}

size_t  Data::GetNumberSize(void) {
    size_t  ret = -1;
    LockNumberMutex();
    ret = this->number_mutex.size();
    UnlockNumberMutex();
    return ret;
}

void Data::PrintNumber(void) {
    size_t size = GetNumberSize();
    for (int i = 1; i < size; ++i) {
        vector<int>::iterator   itr;
        LockNumber(i);
        itr = this->number.begin() + i;
        cout << *itr;
        if (i != size - 1)
            cout << " ";
        UnlockNumber(i);
    }
    cout << "\n";
}

// private method
bool    Data::AddMutex(const int& key) {
    Mutex *value_mutex;
    map<int, Mutex*>::iterator itr;
    map<int, Mutex*>::iterator end;
    bool ret = false;

    LockNumberMutex();
    itr = find(this->number_mutex.begin(), this->number_mutex.end(), key);
    end = this->number_mutex.end();
    if (itr == end) {
        value_mutex = new Mutex();
        this->number_mutex.insert(make_pair(key, value_mutex));
        ret = true;
    }
    UnlockNumberMutex();
    return ret;
}

bool    Data::DeleteMutex(const int& key) {
    map<int, Mutex*>::iterator itr;
    map<int, Mutex*>::iterator end;
    bool ret = false;

    LockNumberMutex();
    itr = find(this->number_mutex.begin(), this->number_mutex.end(), key);
    end = this->number_mutex.end();
    if (itr != end) {
        this->number_mutex.erase(itr);
        ret = true;
    }
    UnlockNumberMutex();
    return ret;
}

// key means the index of number indecated by value Mutex.
int Data::LockNumber(const int& key) {
    map<int, Mutex*>::iterator itr;
    map<int, Mutex*>::iterator end;

    LockNumberMutex();
    itr = find(this->number_mutex.begin(), this->number_mutex.end(), key);
    end = this->number_mutex.end();
    UnlockNumberMutex();

    if (itr != end)
        return itr->second->lock();
    return -1;
}

int Data::UnlockNumber(const int& key) {
    map<int, Mutex*>::iterator itr;
    map<int, Mutex*>::iterator end;

    LockNumberMutex();
    itr = find(this->number_mutex.begin(), this->number_mutex.end(), key);
    end = this->number_mutex.end();
    UnlockNumberMutex();

    if (itr != end)
        return itr->second->unlock();
    return -1;
}

int Data::LockNumberMutex(void) {
    return this->list_mutex->lock();
}

int Data::UnlockNumberMutex(void) {
    return this->list_mutex->unlock();
}