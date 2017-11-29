#ifndef HEAP_H
#define HEAP_H
#include <vector>
#include <algorithm>
#include <set>

using std::iterator_traits;
using std::iterator;

template<typename _Iterator, typename _Container>
class __heap_iterator;

template < typename _Ty, typename _Alloc = std::allocator<_Ty> >
class heap
{
public:
	typedef heap<_Ty, _Alloc> _Myt;
    typedef _Alloc allocator_type;
	typedef typename _Alloc::size_type size_type;
	typedef typename _Alloc::difference_type difference_type;
	typedef typename _Alloc::pointer pointer;
	typedef typename _Alloc::const_pointer const_pointer;
	typedef typename _Alloc::reference reference;
	typedef typename _Alloc::const_reference const_reference;
	typedef typename _Alloc::value_type value_type;
	typedef __heap_iterator<pointer, heap> iterator;
	
private:
	static const unsigned int CONST_INCREMENT_FACTOR = 2; 
	allocator_type _M_alloc;
	pointer _M_start;
	pointer _M_finish;
	pointer _M_end_of_storage;
	
    pointer  _M_allocate(size_t __n)
    { return __n != 0 ? _M_alloc.allocate(__n) : 0; }
		
public:
    explicit
	heap(size_type __n = 100, 
	    const allocator_type& __a = allocator_type()): _M_alloc(__a),_M_start(0),_M_finish(0),_M_end_of_storage(0)
	{
		this->_M_start = this->_M_allocate(__n);
		this->_M_finish = this->_M_start;
		this->_M_end_of_storage = this->_M_start + __n;
	}
	
	size_type
    capacity() const
    { return size_type(this->_M_end_of_storage
		 - this->_M_start); }
		 
	iterator
	begin()
	{
		return iterator(this->_M_start);
	}
	
	iterator
	end()
	{
		return iterator(this->_M_finish);
	}
	
	size_type
	size() const
	{
		return size_type(this->_M_finish - this->_M_start);
	}
	
	size_type
	max_size() const
	{
		return this->_M_alloc.max_size();
	}

	bool empty() const
	{	// test if sequence is empty
		return (size() == 0);
	}

	void resize(size_t _new_size)
	{
		if ( _new_size > size_type(this->_M_end_of_storage - this->_M_start) )
		{
			pointer p = this->_M_allocate(_new_size);
			pointer start = p;
			pointer cur = this->_M_start;
			for (; cur != this->_M_finish ; ++cur, ++p)
			{
				this->_M_alloc.construct(p, *cur);
			}
			
#ifdef WIN32
			_Destroy_range(this->_M_start, this->_M_finish, this->_M_alloc);
#else
			std::_Destroy(this->_M_start, this->_M_finish, this->_M_alloc);
			_M_alloc.deallocate(this->_M_start, 1);
#endif
		
			this->_M_start = start;
			this->_M_finish = p;
			this->_M_end_of_storage = this->_M_start + _new_size;
		}
		return;
	}
	
	bool push_back(const value_type& _val)
	{	
		if ( this->_M_end_of_storage != this->_M_finish )
		{
			this->_M_alloc.construct(this->_M_finish, _val);
			this->_M_finish++;
			return true;
		}
		else
		{
			if ( this->capacity() >= this->max_size() )
				return false;
			
			size_type new_sz = this->capacity() * CONST_INCREMENT_FACTOR;
			new_sz = min(new_sz, this->max_size());
			this->resize(new_sz);
			return this->push_back(_val);
		}
	}
		


	bool pop_front()
	{
		if ( this->_M_start == this->_M_finish )
				return false;
			
#ifdef WIN32
			_Destroy_range(this->_M_start, this->_M_start, this->_M_alloc);
#else
			std::_Destroy(this->_M_start, this->_M_start, this->_M_alloc);
#endif

		this->_M_alloc.construct(this->_M_start, *(this->_M_finish-1));
		
#ifdef WIN32
			_Destroy_range(this->_M_finish-1, this->_M_finish, this->_M_alloc);
#else
			std::_Destroy(this->_M_finish-1, this->_M_finish, this->_M_alloc);
#endif

		this->_M_finish--;
		
		return true;
	}

		
	reference
	at(size_type i) const 
	{ 
		if ( i < this->size() ) 
			return this->_M_start[i];
		else
			throw std::exception();
	}

	void 
	clear()
	{
#ifdef WIN32
		_Destroy_range(this->_M_start, this->_M_finish, this->_M_alloc);
#else
		std::_Destroy(this->_M_start, this->_M_finish, this->_M_alloc);
#endif

		this->_M_finish = this->_M_start;
	}
	
	template <typename T>
	void make_heap(T pred)
	{
		size_type sz = size();
		size_type i = sz/2;
		iterator ite = begin();
		for ( ; i > 0; i--)
		{
			adjust_heap(ite, sz, i, pred);
		}
		adjust_heap(ite, sz, 0, pred);
	}
		
	template <typename T>
	static void adjust_heap( iterator iteFirst, size_type sz, size_type pos, T pred )
	{
		size_type j = pos*2 + 1;

		if (pos < sz)
		{
			_Ty temp = iteFirst[pos];

			while (j < sz)
			{
				if (j+1 < sz && pred(iteFirst[j+1],iteFirst[j]))
					j++;

				if (pred(iteFirst[pos],iteFirst[j]))
					break;

				iteFirst[pos] = iteFirst[j];
				pos = j;
				j = j*2 + 1;
			}
			iteFirst[pos] = temp;
		}
	}
/*	{
		size_type i1 = pos * 2 + 1;
		size_type i2 = pos * 2 + 2;
		size_type min = pos;	
			
		if ( pos < sz/2 )
		{
			if ( i1 < sz && pred(iteFirst[i1], iteFirst[min]) )
				min = i1;
			if ( i2 < sz && pred(iteFirst[i2], iteFirst[min]) )
				min = i2;
				
			if ( min != pos )
			{
				swap(iteFirst[pos], iteFirst[min]);
				adjust_heap(iteFirst, sz, min, pred);
			}
		}
		return;
	}*/

	template<typename T>
	static void adjust_heap_reverse(iterator iteFirst, size_t pos, T pred)
	{
		if ( pos <= 0 ) return;
		int parent = ( pos - 1 ) / 2;
		while ( parent >= 0 )
		{
			if ( pred(iteFirst[pos], iteFirst[parent]) )
				swap(iteFirst[parent], iteFirst[pos]);
			else 
				break;
			
			pos = parent;
			if ( pos <= 0 )
				return;
			parent = ( pos - 1 ) / 2;
		}
	}


	~heap()
	{
		if ( size() > 0 )
		{
			pointer p = this->_M_start;
#ifdef WIN32
			_Destroy_range(this->_M_start, this->_M_finish, this->_M_alloc);
#else
			std::_Destroy(this->_M_start, this->_M_finish, this->_M_alloc);
#endif

			this->_M_alloc.deallocate(this->_M_start, 1);
		}
		this->_M_start = 0;
		this->_M_finish = 0;
		this->_M_end_of_storage = 0;
			
	}
		
};


template<typename _Iterator, typename _Container>
class __heap_iterator
{
protected:
    _Iterator _M_current;

public:
    typedef _Iterator iterator_type;
    typedef typename iterator_traits<_Iterator>::iterator_category
                                                             iterator_category;
    typedef typename iterator_traits<_Iterator>::value_type  value_type;
    typedef typename iterator_traits<_Iterator>::difference_type
                                                             difference_type;
    typedef typename iterator_traits<_Iterator>::reference reference;
    typedef typename iterator_traits<_Iterator>::pointer   pointer;

    __heap_iterator() : _M_current(_Iterator()) { }

    explicit
    __heap_iterator(const pointer __p) : _M_current(__p) { }


    // Allow iterator to const_iterator conversion
    template<typename _Ite>
    __heap_iterator(const __heap_iterator<_Ite, _Container>& __i)
        : _M_current(__i.base()) { }

    // Forward iterator requirements
    reference
    operator*() const
    { return *_M_current; }

    pointer
    operator->() const
    { return _M_current; }

    __heap_iterator&
    operator++()
    {
		++_M_current;
		return *this;
    }

    __heap_iterator
    operator++(int)
    { return __heap_iterator(_M_current++); }

    // Bidirectional iterator requirements
    __heap_iterator&
    operator--()
    {
		--_M_current;
		return *this;
    }

	__heap_iterator
    operator--(int)
    { return __heap_iterator(_M_current--); }

    // Random access iterator requirements
    reference
    operator[](const difference_type& __n) const
    { return _M_current[__n]; }

    __heap_iterator&
    operator+=(const difference_type& __n)
    { _M_current += __n; return *this; }

    __heap_iterator
    operator+(const difference_type& __n) const
    { return __heap_iterator(_M_current + __n); }

    __heap_iterator&
    operator-=(const difference_type& __n)
    { _M_current -= __n; return *this; }

    __heap_iterator
    operator-(const difference_type& __n) const
    { return __heap_iterator(_M_current - __n); }

    const _Iterator&
    base() const
    { return _M_current; }
 };
 
 template <typename _Iterator, typename _Container>
 bool operator!=(const __heap_iterator<_Iterator, _Container>& lhs, 
 	const __heap_iterator<_Iterator, _Container>& rhs)
 	{
 		return lhs.base() != rhs.base();
 	}

#endif
