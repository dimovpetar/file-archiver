#ifndef _MYSTACK_HEADER_INCLUDED_
#define _MYSTACK_HEADER_INCLUDED_

#include "MyVector.h"

namespace fmi
{
	template < typename T >
	class Stack
	{
	public:
		Stack();
		Stack(const Stack& rhs);
		Stack& operator=(const Stack& rhs);
		~Stack();

		void push(const T& val);
		void pop();
		T&	 top() const;

		bool	isEmpty() const	{ return data.isEmpty(); }
		size_t	getSize() const { return data.getSize(); }

	private:
		Vector<T> data;

	};


	template <typename T>
	Stack<T>::Stack() : data()
	{
	}

	template <typename T>
	Stack<T>::Stack(const Stack& rhs) : data(rhs.data)
	{
	}

	template <typename T>
	Stack<T>& Stack<T>::operator = (const Stack<T>& rhs)
	{
		if (this == &rhs)
			return *this;

		data = rhs.data;

		return *this;
	}

	template <typename T>
	Stack<T>::~Stack()
	{
	}

	template <typename T>
	void Stack<T>::push(const T& val)
	{
		data.push_back(val);
	}

	template <typename T>
	void Stack<T>::pop()
	{
		data.pop_back();
	}

	template <typename T>
	T& Stack<T>::top() const
	{
		return data.back();
	}


} //fmi


#endif //_MYSTACK_HEADER_INCLUDED_