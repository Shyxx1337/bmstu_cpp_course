#pragma once

#include <exception>
#include <iostream>
#include <utility>

namespace bmstu
{
template <typename T>
class stack
{
   public:
	stack() : data_(nullptr), size_(0), capacity_(0) {}

	bool empty() const noexcept { return size_ == 0; }

	size_t size() const noexcept { return size_; }

	~stack() {
		    clear();
        	::operator delete[](data_);
	}

	template <typename... Args>
	void emplace(Args&&... args)
	{
		if (size_ == capacity_)
			grow();
		
		new(data_ + size_) T(std::forward<Args>(args)...);
		++size_;
	}

	void push(T&& value) {
		if (size_ == capacity_)
			grow();
		new(data_ + size_) T(std::move(value));
		++size_;
	}

	void clear() noexcept {
		for (size_t i = 0; i < size_; ++i)
			data_[i].~T();
		size_ = 0;
	}

	void push(const T& value) {
		if (size_ == capacity_)
			grow();

		new(data_ + size_) T(value);
		++size_;
	}

	void pop() {
		if (size_ == 0)
			throw std::underflow_error("Stack is empty");

		data_[size_ - 1].~T();
		--size_;
	}

	T& top() { 
		if (size_ == 0)
			throw std::underflow_error("Stack is empty");
		
		return data_[size_-1]; 
	}

	const T& top() const { 
		if (size_ == 0)
			throw std::underflow_error("Stack is empty");
		
		return data_[size_ - 1]; 
	}

   private:
	T* data_;
	size_t size_;
    size_t capacity_;

	private:
    void grow() {
        size_t new_cap = (capacity_ == 0 ? 1 : capacity_ + 1);
        T* new_data = static_cast<T*>(::operator new[](new_cap * sizeof(T)));
        for (size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
		::operator delete[](data_);
        data_ = new_data;
        capacity_ = new_cap;
    }
};
}  // namespace bmstu
