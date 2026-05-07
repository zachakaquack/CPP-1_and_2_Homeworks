#pragma once

#include <algorithm>
#include <cstddef>
#include <iostream>

namespace dsa {
template <typename T> class array_list {
  public:
    array_list() { this->m_items = nullptr; }
    ~array_list() {}

    enum SortedType {
        UNSORTED = 0,
        SORTED = 1,
    };

    size_t size() const { return this->m_size; }
    size_t count() const { return this->m_count; }
    T get(const size_t &x) const { return this->m_items[x]; }

    T push_back(const T &x) {

        // empty list
        if (this->m_items == nullptr) {
            this->m_items = new T[this->m_initialSize];
            this->m_items[0] = x;

            this->m_size = this->m_initialSize;
            this->m_count = this->m_initialSize;

            return x;
        }

        // expand
        if (this->count() >= this->size()) {
            const size_t newSize = this->size() * 2;
            T *newItems = new T[newSize];
            for (size_t i = 0; i < this->size(); i += 1) {
                newItems[i] = this->get(i);
            }

            delete[] this->m_items;
            this->m_items = newItems;

            this->m_size = newSize;

            this->m_items[this->count()] = x;
            this->m_count += 1;
            return x;
        }

        // just input
        this->m_items[this->count()] = x;
        this->m_count += 1;
        return x;
    }

    T *begin() const { return &this->m_items[0]; }
    T *end() const { return &this->m_items[this->count()]; }

    bool contains(const T &x, const array_list::SortedType &type) const {
        // sorted, use binary search
        if (type == SortedType::SORTED) {
            int low = 0, high = this->count() - 1;

            while (low <= high) {
                int mid = ((high - low) / 2) + low;

                if (this->get(mid) == x) {
                    return true;
                }

                if (this->get(mid) > x) {
                    high = mid - 1;
                } else {
                    low = mid + 1;
                }
            }
            return false;
        }

        // unsorted, search linearlly
        for (size_t j = 0; j < this->count(); j += 1) {
            const T &newItem = this->get(j);
            if (x == newItem) {
                return true;
            }
        }

        return false;
    }

    array_list<T> intersection_unsorted(const array_list<T> &left,
                                        const array_list<T> &right) {
        // usedItems tracks right sided compares
        // Find a match between left and right
        // if itemAlreadyUsed == false:
        //	add to intersection list
        //	mark value as unusable / already used `itemAlreadyUsed = true`

        array_list<T> intersection;

        const size_t itemCount = right.count();
        bool *usedItems = new bool[itemCount]{false};

        for (size_t i = 0; i < left.count(); i += 1) {
            const T &leftItem = left.get(i);

            for (size_t j = 0; j < right.count(); j += 1) {
                const T &rightItem = right.get(j);
                bool &itemAlreadyUsed = usedItems[j];

                if (leftItem != rightItem || itemAlreadyUsed) {
                    continue;
                }

                // mark item as used, and append to the intersection list
                itemAlreadyUsed = true;
                intersection.push_back(leftItem);

                // break for efficiency because we don't need to check anymore
                break;
            }
        }
        return intersection;
    }

    array_list<T> intersection_sorted(const array_list<T> &left,
                                      const array_list<T> &right) {
        // for each item in left
        //   if left.item == left.previousitem
        //     increase duplicateTracker & 2 by 1
        //   else
        //     reset duplicateTracker
        //   for each item in right until right.item > left.item (isn't
        //   necessary to search past this point)
        //     if left.item == right.item and tracker2 is 0
        //       push_back left
        //     else if tempTracker != 0 and left.item == right.item
        //       reduce tempTracker by 1

        // duplicateTracker is the persistent counter between left items
        // tempTracker is the temporary counter between right items

        array_list<T> intersected;
        size_t duplicateTracker = 0;
        size_t tempTracker = 0;

        // tracker +=1 means to add to list pmuch
        for (size_t i = 0; i < left.count(); i += 1) {
            const T &leftItem = left.get(i);

            // tracker is the number of duplicates, so if prev != new, then
            // reset, otherwise add to new tracker
            if (leftItem == left.get(i - 1) && i > 0) {
                duplicateTracker += 1;
            } else {
                duplicateTracker = 0;
            }

            tempTracker = duplicateTracker;

            for (size_t j = 0; j < right.count() && leftItem >= right.get(j);
                 j += 1) {
                const T &rightItem = right.get(j);

                const bool matchFound = (leftItem == rightItem);
                if (!matchFound) {
                    continue;
                }

                const bool duplicateFound = (tempTracker > 0);
                if (!duplicateFound) {
                    // no duplicates to worry about
                    intersected.push_back(leftItem);
                    break;
                } else {
                    // ignore the duplicate item, fix the tracker
                    tempTracker -= 1;
                }
            }
        }
        return intersected;
    }

    array_list<T> union_unsorted(const array_list<T> &left,
                                 const array_list<T> &right) {
        // "uniques" list is another `array_list` object, and we use its
        // built-in array as the uniques list.
        // get all uniques by:
        // for item in left.items:
        //	if item not in "uniques" list:
        //		uniques.append(item)
        //
        // then do the same thing with right.items, using same "uniques" list

        array_list<T> uniques;

        auto calcUniques = [&](const array_list list) -> void {
            for (size_t i = 0; i < list.count(); i += 1) {
                const T &item = list.get(i);
                if (!uniques.contains(item, SortedType::UNSORTED)) {
                    uniques.push_back(item);
                }
            }
        };

        calcUniques(left);
        calcUniques(right);

        return uniques;
    }

    array_list<T> union_sorted(const array_list<T> &left,
                               const array_list<T> &right) {

        // i dont know of a better way to keep the sortedness of the two lists
        // other than combining the two and then sorting them
        // sorted follows the same logic on the unsorted, except
        // it uses binary search instead of linear, within the
        // `uniques.contains()` function
        // it only has to iterate over `sorted` once because sorted is the two
        // lists combined

        array_list<T> uniques;

        array_list<T> sorted = left;
        for (auto item : right) {
            sorted.push_back(item);
        }

        std::sort(sorted.begin(), sorted.end());

        for (size_t i = 0; i < sorted.count(); i += 1) {
            const T &item = sorted.get(i);
            if (!uniques.contains(item, SortedType::SORTED)) {
                uniques.push_back(item);
            }
        }

        return uniques;
    }

    friend std::ostream &operator<<(std::ostream &out,
                                    const array_list &arraylist) {
        size_t i = 0;
        for (; i < arraylist.count(); i++) {
            out << i << ":" << arraylist.get(i) << " ";
        }
        for (; i < arraylist.size(); i++) {
            out << i << ":" << "nil" << " ";
        }
        out << "\n";
        return out;
    }

  private:
    T *m_items = nullptr;
    size_t m_size = 0;
    size_t m_count = 0;
    size_t m_initialSize = 1;
};
} // namespace dsa
