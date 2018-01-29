#ifndef _MYVECTOR_HEADER_INCLUDED_
#define _MYVECTOR_HEADER_INCLUDED_

#include <assert.h>
#include <iostream>
#include <new> /* nothrow */

namespace fmi
{
	template < typename T >
	class Vector
	{
	public:
		Vector();
		Vector(const Vector& rhs);
		Vector& operator=(const Vector& rhs);
		~Vector();

		void push_back(const T& val);
		void pop_back();
		T& back() const;

		T&	operator[] (size_t idx);
		T	operator[] (size_t idx) const;

		bool	isEmpty()	const { return size == 0; }
		size_t	getSize()	const { return size; }

	private:
		void copy(const Vector& rhs);
		void clear();
		void resize();

	private:
		T *		data;
		size_t	size;
		size_t	capacity;
	};


	template <typename T>
	Vector<T>::Vector() : data(nullptr), size(0), capacity(0)
	{
	}

	template <typename T>
	Vector<T>::Vector(const Vector& rhs)
	{
		copy(rhs);
	}

	template <typename T>
	Vector<T>& Vector<T>::operator=(const Vector<T>& rhs)
	{
		if (this == &rhs)
			return *this;

		clear();
		copy(rhs);

		return *this;
	}

	template <typename T>
	Vector<T>::~Vector()
	{
		clear();
	}

	template <typename T>
	void Vector<T>::push_back(const T& val)
	{
		if (size == capacity)
			resize();

		data[size++] = val;
	}

	template <typename T>
	void Vector<T>::pop_back()
	{
		assert(size > 0);
		if (size == 0)
			return;

		size--;
	}

	template <typename T>
	T& Vector<T>::back() const
	{
		assert(size > 0);
		if (size == 0)
			throw "Empty vector.\n";

		return data[size - 1];
	}


	template <typename T>
	T& Vector<T>::operator[] (size_t idx)
	{
		assert(size > 0 && (idx < size && idx >= 0));

		if (!(size > 0 && (idx < size && idx >= 0)))
			throw "Bad index\n";

		return data[idx];
	}


	template <typename T>
	T Vector<T>::operator[] (size_t idx) const
	{
		if (idx > size)
			return 0;

		return data[idx];
	}

	template <typename T>
	void Vector<T>::copy(const Vector& rhs)
	{
		size = rhs.size;
		capacity = rhs.capacity;

		data = new (std::nothrow) T[capacity];
		if (!data)
			return;

		for (size_t i = 0; i < size; i++)
		{
			data[i] = rhs.data[i];
		}
	}

	template <typename T>
	void Vector<T>::clear()
	{
		delete[] data;
	}

	template <typename T>
	void Vector<T>::resize()
	{
		if (capacity == 0)
		{
			capacity = 2;
			data = new T[capacity];
		}

		else
		{
			capacity <<= 1;
			T * temp = new (std::nothrow) T[capacity];

			if (!temp)
			{
				std::cout << "You can't add more items to the vector\n";
				return;
			}

			for (size_t i = 0; i < size; i++)
			{
				temp[i] = data[i];
			}
			delete[] data;
			data = temp;
		}
	}

} //fmi


#endif //_MYVECTOR_HEADER_INCLUDED_