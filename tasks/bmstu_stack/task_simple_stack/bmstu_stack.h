#pragma once

#include <exception>
#include <utility>

namespace bmstu
{
template <typename T>
class stack
{
   public:
	stack() : data_(nullptr), size_(0) {}

	bool empty() const noexcept { return size_ == 0; }

	size_t size() const noexcept { return size_; }

	~stack() {
		clear();
        operator delete(data_);
	}

	template <typename... Args>
	void emplace(Args&&... args)
	{
		T* new_data = relocate();
		new(new_data + size_) T(std::forward<Args>(args)...);
		operator delete(data_);
		data_ = new_data;
		++size_;
	}

	void push(T&& value) {
		T* new_data = relocate();
		new(new_data + size_) T(std::move(value));
		operator delete(data_);
		data_ = new_data;
		++size_;
	}

	void clear() noexcept {
		for (size_t i = 0; i < size_; ++i)
			data_[i].~T();
		size_ = 0;
	}

	void push(const T& value) {
		T* new_data = relocate();
		new(new_data + size_) T(value);
		operator delete(data_);
		data_ = new_data;
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

	private:
    T* relocate() {
        T* new_data = (T*)(operator new((size_ + 1) * sizeof(T)));
        for (size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
		return new_data;
    }
};
}  // namespace bmstu
