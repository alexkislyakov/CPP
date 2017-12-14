#include <iostream>
#include <vector>
#include <list>
#include <iterator>

// if it's stupid but it works it ain't stupid

template<class T>
class VectorList {
 private:
    using VectT  = std::vector<T>;
    using ListT = std::list<VectT>;

 public:
    using value_type = T;

    VectorList() = default;
    VectorList(VectorList const &) = default;
    VectorList(VectorList &&) = default;

    VectorList & operator=(VectorList &&) = default;
    VectorList & operator=(VectorList const &) = default;

    class const_iterator : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&> {
     public:
        const_iterator(const ListT* data_, typename VectT::const_iterator VecIter,
		       typename ListT::const_iterator ListIter, int idx_vec, int idx_list)
	    : data_(data_), VecIter(VecIter), ListIter(ListIter), idx_vec(idx_vec), idx_list(idx_list) {
	}

        const_iterator() = default;

        const_iterator(const const_iterator &) = default;
        const_iterator(const_iterator &&) = default;

        const_iterator & operator=(const_iterator &&) = default;
        const_iterator & operator=(const_iterator const &) = default;

        bool operator==(const const_iterator &other) const {
            return (ListIter == other.ListIter) && (VecIter == other.VecIter);
        }

        bool operator!=(const const_iterator &other) const {
            return !(*this == other);
        }

        const_iterator& operator++() {
            if (idx_list < data_->size()) {
                if (idx_vec + 1 < ListIter->size()) {
                    ++idx_vec;
                    ++VecIter;
                } else {
                    increment_list();
                }
            } else {
                increment_list();
            }
            return *this;
        }

        void increment_list() {
            idx_vec = 0;
            ++idx_list;
            ++ListIter;
            VecIter = ListIter->cbegin();
        }
 
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        const_iterator& operator--() {
            if (idx_vec > 0 || idx_list > 0) {
                if (idx_vec - 1 < 0) {
                    --ListIter;
                    --idx_list;
                    idx_vec = ListIter->size() - 1;
                    VecIter = --(ListIter->cend());
                } else {
                    --idx_vec;
                    --VecIter;
                }
            } else {
                --ListIter;
                --idx_list;
                idx_vec = 0;
                VecIter = ListIter->begin();
            }
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        const T& operator*() const {
            return (*VecIter);
        }

        const T* operator->() const {
            return &(*VecIter);
        }

     private:
        const ListT *data_;
        typename VectT::const_iterator VecIter;
        typename ListT::const_iterator ListIter;
        int idx_vec;
        int idx_list; 
    };

    template<class It>
    void append(It p, It q) {
        if (p != q)
            data_.push_back(VectT(p, q));
    }

    bool empty() const { return size() == 0; }

    size_t size() const {
        size_t n_elem = 0;
        for (const VectT &vec : data_) {
            n_elem += vec.size();
        }
        
        return n_elem;
    }

    const_iterator begin() const {
        if (!data_.empty()) {
            return const_iterator(&data_, data_.front().cbegin(), data_.cbegin(), 0, 0);
        }
        return const_iterator();
    }
    
    const_iterator end() const {
        if (!data_.empty()) {
            return ++const_iterator(&data_, --(data_.back().cend()), --(data_.cend()),
				    data_.back().size() - 1, data_.size() - 1);
        }
        return const_iterator();
    }

    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    const_reverse_iterator rbegin() const {
        if (!data_.empty()) {
            return  const_reverse_iterator(++const_iterator(&data_, --(data_.back().cend()), --(data_.cend()),
							    data_.back().size() - 1, data_.size() - 1));
        }
        return const_reverse_iterator(const_iterator());
    }
    
    const_reverse_iterator rend() const {
        if (!data_.empty()) {
            return  const_reverse_iterator(const_iterator(&data_, data_.front().cbegin(),
							  data_.cbegin(), 0, 0));
        }
        return const_reverse_iterator(const_iterator());
    }

 private:
    ListT data_;
};

int main() {
    std::vector<std::string> v1 = {"one", "two", "three"};
    std::vector<std::string> v2 = {"four", "five", "six", "seven", "eight"};
    std::vector<std::string> v3 = {"nine", "ten", "eleven", "twelve"};

    VectorList<std::string> vl;
    std::cout << "empty distance = " << std::distance(vl.rbegin(), vl.rend()) << std::endl;

    vl.append(v1.begin(), v1.end());

    vl.append(v2.begin(), v2.end());

    vl.append(v3.begin(), v3.end());

    VectorList<std::string>::const_iterator it = vl.begin();

    for (; it != vl.end(); ++it) {
        std::string a = *it;
        std::cout << a << " ";
    }
    std::cout << std::endl;
    std::cout << "distance = " << std::distance(vl.begin(), vl.end()) << std::endl;

    VectorList<std::string>::const_iterator eit = vl.end();
    for (; eit != vl.begin();) {
        std::string a = *(--eit);
        std::cout << a << " ";
    }
    std::cout << std::endl;

    std::cout << std::endl;
    std::cout << "reverse distance = " << std::distance(vl.rbegin(), vl.rend()) << std::endl;

    VectorList<std::string>::const_reverse_iterator erit = vl.rend();
    for (; erit != vl.rbegin();) {
        std::string a = *(--erit);
        std::cout << a << " ";
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
