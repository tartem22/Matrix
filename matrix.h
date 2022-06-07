#ifndef MATRIX_H
#define METRIX_H

/*!
\file
\brief Заголовочный файл с реализацией класса бесконечной
двумерной матрицы, хранящей только не пустные и не назначенные
по умолчанию элементы

Данный файл содержит в себе еализацию класса бесконечной
двумерной матрицы, хранящей только не пустные и не назначенные
по умолчанию элементы
*/

#include <map>
#include <memory>

/*!
Класс бесконечной двумерной матрицы,
хранящей только не пустные и не назначенные
по умолчанию элементы
\param[in] T тип элементов матрицы
\param[in] D значение по умолчанию для элементов матрицы
*/
template <typename T, T D>
class Matrix
{
public:
    /*!
    Рассчитывает количество  хранимых в памяти элементов матрицы (в памяти содержатся
    только элементы, значение которых не равно значению по умолчанию)
    \return Количество хранимых в памяти элементов матрицы
    */
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

    /*!
    Вспомогательный класс-заместитель, необходимый для реализации
    двумерной индексации матрицы
    */
    class Proxy
    {
    public:
        /*!
        Конструктор класса
        \param[in] firstInx первый индекс, указываемый в операторе 
        двумерной индексации [][] бесконечной матрицы
        \param[in] storage указатель на хранилище элеметов бесконечной
        матрицы
        */
        Proxy(int firstInx, std::map<std::pair<int, int>, T> *storage)
            : firstInx(firstInx)
        {
            storagePtr = storage;
        }

        /*!
        Вторая часть оператора двумерной индексации [][] бесконечной матрицы
        \param[in] inx второй индекс в операторе двумерной
         индексации [][] бесконечной матрицы (индекс по столбцу)
        \return ссылка на элемент с индексом [firstInx][inx]
        */
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
        //! первый индекс, указываемый в операторе двумерной индексации [][] бесконечной матрицы
        int firstInx = 0; 
        //! указатель на хранилище элеметов бесконечной матрицы
        std::map<std::pair<int, int>, T> *storagePtr = nullptr;
    };

    /*!
    Итератор по непустым элементам бесконечной двумерной матрицы
    */
    struct iterator : std::iterator<std::forward_iterator_tag, const Matrix>
    {
    public:
        /*!
        Конструктор итератора по непустым элементам бесконечной двумерной матрицы
        \param[in] it итератор по харнилищу непустых элементов бесконечной двумерной
        матрицы
        \param[in] end итератор на последний элемент харнилища
        непустых элементов бесконечной двумерной матрицы
        */
        explicit iterator(typename std::map<std::pair<int, int>, T>::iterator it,
                          typename std::map<std::pair<int, int>, T>::iterator end) : current(it),
                                                                                     _end(end)
        {
            if (current->second == D)
                (*this)++;
        }

        /*!
        Инкремент итератора
        \return копия текущего итератора, инкрементированного на единицу
        */
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

        /*!
        Инкремент итератора
        \return ссылка на текущий итератора, инкрементированный на единицу
        */
        iterator &operator++()
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

        /*!
        Оператор сравнения на равенство
        \param[in] it итератор, сравниваемый с текущим
        \return true, если сравниваемые итераторы равны, в противном случае false
        */
        bool operator==(const iterator &it) const
        {
            return current == it.current;
        }
        /*!
        Оператор сравнения на неравенство
        \param[in] it итератор, сравниваемый с текущим
        \return true, если сравниваемые итераторы не равны, в противном случае false
        */
        bool operator!=(const iterator &it) const
        {
            return !(*this == it);
        }
        /*!
        Обращение к члену класса
        \return Элемент матрицы, на который указывает итератор
        */
        auto operator->()
        {
            return current;
        }
        /*!
        Разыменование итератора
        \return Кортеж вида: индекс по строке,
        индекс по столбцу,
        значение элемента матрицы
        */
        auto operator*()
        {
            return std::tuple<int, int, T>(current->first.first,
                                           current->first.second,
                                           current->second);
        }

    private:
        //! итератор на текущий элемент хранилища непустных значений матрицы
        typename std::map<std::pair<int, int>, T>::iterator current;
        //! итератор на последний элемент хранилища непустных значений матрицы
        typename std::map<std::pair<int, int>, T>::iterator _end;
    };

    /*!
    Первая часть оператора двумерной индексации [][] бесконечной матрицы
    \param[in] inx первый индекс в операторе двумерной
        индексации [][] бесконечной матрицы (индекс по строке)
    \return экземпляр класса-заменителя, необходимого для реализации
    двумерной индексации матрицы
    */
    Proxy operator[](int inx)
    {
        return Proxy(inx, &storage);
    }

    /*
    Получение итератора на первый непустой элемент матрицы
    \return итератор на первый непустой элемент матрицы
    */
    iterator begin()
    {
        return iterator(storage.begin(), storage.end());
    }
    /*
    Получение итератора на следующий за последним непустой элемент матрицы
    \return итератор на следующий за последним непустой элемент матрицы
    */
    iterator end()
    {
        return iterator(storage.end(), storage.end());
    }

private:
    //! Хранилище непустых элементов матрицы
    std::map<std::pair<int, int>, T> storage;
};

#endif