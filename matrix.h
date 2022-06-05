#ifndef MATRIX_H
#define METRIX_H

#include <map>
#include <memory>

template <typename T, T D>
class Matrix
{
public:
    int size()
    {
        int cnt = 0;
        for (auto &m : storage)
        {
            if (m.second != D)
                cnt++;
        }
        return cnt;
        // return storage.size();
    }

    class Proxy
    {
    public:
        Proxy(int firstInx, std::map<std::pair<int, int>, T> *storage)
            : firstInx(firstInx)
        {
            storagePtr = storage;
        }

        T &operator[](int inx)
        {
            auto key = std::make_pair(firstInx, inx);
            auto it = storagePtr->find(key);
            if (it != storagePtr->end())
                return it->second;
            else
            {
                const auto [insertIt, flag] = storagePtr->insert(std::pair{key, D});
                return insertIt->second;
            }
        }

    private:
        int firstInx = 0;
        std::map<std::pair<int, int>, T> *storagePtr;
    };

    Proxy operator[](int inx)
    {
        return Proxy(inx, &storage);
    }

    struct iterator : std::iterator<std::forward_iterator_tag, const Matrix>
    {
    public:
        explicit iterator(typename std::map<std::pair<int, int>, T>::iterator it,
                          typename std::map<std::pair<int, int>, T>::iterator end) : current(it),
                                                                                     _end(end)
        {
            if(current->second == D)
                (*this)++;
        }

        iterator operator++(int)
        {
            current++;
            while (current != _end)
            {
                if (current->second != D)
                    break;
                current++;
            }
            return *this;
        }

        iterator &operator++()
        {
            std::cout << "operator ++" << std::endl;
            current++;
            while (current != _end)
            {
                if (current->second != D)
                    break;
                current++;
            }
            return *this;
        }

        bool operator==(const iterator &it) const
        {
            return current == it.current;
        }
        bool operator!=(const iterator &it) const
        {
            return !(*this == it);
        }

        auto operator->()
        {
            return current;
        }

        auto operator*()
        {
            return std::tuple<int, int, T>(current->first.first,
                                           current->first.second,
                                           current->second);
        }

    private:
        typename std::map<std::pair<int, int>, T>::iterator current;
        typename std::map<std::pair<int, int>, T>::iterator _end;
    };

    iterator begin()
    {
        return iterator(storage.begin(), storage.end());
    }

    iterator end()
    {
        return iterator(storage.end(), storage.end());
    }

private:
    std::map<std::pair<int, int>, T> storage;
};

#endif