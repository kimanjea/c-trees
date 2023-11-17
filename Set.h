//
// Interface Definition for the template version of the Set Class
// Author: Max Benson
// Date: 10/22/2021
//

#ifndef SET_H
#define SET_H

#include "VariableArrayList.h"

template <typename ElementType>
class Set {
public:
    void Clear();
    bool Add(const ElementType& element);
    bool Contains(const ElementType& element);
    const ElementType Get(size_t ord) const;
    size_t Size() const;

    friend ostream& operator<<(ostream& os, const Set& set) {
        os << '{';
        for (size_t i = 0; i < set.Size(); i ++ ) {
            if (i > 0) {
                os << ", ";
            }
            os << set.Get(i);
        }
        return os << '}';
    }

    friend bool operator==(const Set& lhs, const Set& rhs) {
        bool sameElements = true;

        if (lhs.Size() != rhs.Size()) {
            return false;
        }
        for (size_t ord = 0; ord < lhs.Size(); ord ++ ) {
            if (lhs.Get(ord) != rhs.Get(ord)) {
                return false;
            }
        }
        return true;
    }
    friend bool operator!=(const Set& lhs, const Set& rhs) { return !(lhs == rhs); }

private:
    VariableArrayList<ElementType> _list;
};

/**
* Removes all elements from a set
*/
template <typename ElementType>
void Set<ElementType>::Clear()  {
    _list.Clear();
}

/**
* Adds "element" to set if it not already present
* @return true operation succeeded, falso if not
*/
template <typename ElementType>
bool Set<ElementType>::Add(const ElementType& element)  {
    size_t pos;

    //Sorted insert so we can implement == and !=
    for (pos = 0; pos < _list.Size(); pos ++) {
        ElementType elementCur;

        _list.Get(pos,elementCur);
        if (elementCur == element) {
            return true;
        }
        if (elementCur > element) {
            break;
        }
    }
    return _list.Insert(pos, element);
}

/**
* Checks whether element is part of the set
* @return true if set contains element, false if not
*/
template <typename ElementType>
bool Set<ElementType>::Contains(const ElementType& element) {
    return _list.Find(element) != -1;
}

/**
* Returns element at position pos in set
* @return element
*/
template <typename ElementType>
const ElementType Set<ElementType>::Get(size_t ord) const {
    ElementType element;

    _list.Get(ord, element );
    return element;
}

/**
* Returns number of elements in set
* @return number of elements
*/
template <typename ElementType>
size_t Set<ElementType>::Size() const {
    return _list.Size();
}

#endif //SET_H
