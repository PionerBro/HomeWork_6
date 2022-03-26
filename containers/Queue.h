#pragma once

template <typename T>
class Queue
{
	template <typename T>
	class QueueImpl {
	public:
		QueueImpl(const std::size_t capacity = 0) :
			capacity_(capacity), size_(0), data_(static_cast<T*>(operator new(capacity_ * sizeof(T)))) {}
		QueueImpl(const QueueImpl&) = delete;
		QueueImpl& operator=(const QueueImpl&) = delete;
		~QueueImpl() noexcept {
			operator delete(data_);
		}

		void swap(QueueImpl& other) noexcept {
			std::swap(capacity_, other.capacity_);
			std::swap(size_, other.size_);
			std::swap(data_, other.data_);
		}

		size_t capacity_ = 0;
		size_t size_ = 0;
		T* data_ = nullptr;
	};
public:
	Queue() : impl_() {}
	Queue(const Queue& other) : impl_(other.impl_.capacity_) {
		uninitializedImpl_copy(this->impl_, other.impl_);
	}
	Queue& operator=(const Queue& other) {
		Queue tmp(other);
		swap(tmp);
		return *this;
	}
	~Queue() noexcept {
		for (size_t i = 0; i < impl_.size_; ++i)
			impl_.data_[i].~T();
	}

	T& front() {
		if (0 == impl_.size_)
			throw std::logic_error("Queue: front on empty queue");
		return *impl_.data_;
	}

	const T& front()const {
		if (0 == impl_.size_)
			throw std::logic_error("Queue: front on empty queue");
		return *impl_.data_;
	}

	T& back() {
		if (0 == impl_.size_)
			throw std::logic_error("Queue: back on empty queue");
		return impl_.data_[impl_.size_ - 1];
	}

	const T& back()const {
		if (0 == impl_.size_)
			throw std::logic_error("Queue: back on empty queue");
		return impl_.data_[impl_.size_ - 1];
	}

	bool empty() const noexcept {
		return 0 == impl_.size_;
	}

	std::size_t size()const noexcept {
		return impl_.size_;
	}

	//!!! Не знаю как критична для скорости в данной ситуациия лишняя проверка в std::forward. Решил что не критична!
	void push(const T& value) {
		emplace(value);
	}

	template<class... Args>
	void emplace(Args&&... args) {
		if (impl_.size_ == impl_.capacity_) {
			QueueImpl<T> tmp_impl(impl_.capacity_ * 2 + 1);
			uninitializedImpl_copy(tmp_impl, this->impl_);
			new(tmp_impl.data_ + tmp_impl.size_) T(std::forward<Args>(args)...);
			++tmp_impl.size_;
			impl_.swap(tmp_impl);
		}
		else {
			new(impl_.data_ + impl_.size_) T(std::forward<Args>(args)...);
			++impl_.size_;
		}
	}

	void pop() {
		if (0 == impl_.size_)
			throw std::logic_error("Queue: pop on empty queue");
		for (size_t i = 0; i < impl_.size_ - 1; ++i)
			impl_.data_[i] = impl_.data_[i + 1];
		impl_.data_[impl_.size_ - 1].~T();
		--impl_.size_;
	}

	void swap(Queue& other) noexcept {
		impl_.swap(other.impl_);
	}
private:
	//!!! думал вынести в QueueImpl, но деструкторы и конструирование отдельных объектов(в push) мы вызываем в Queue
	//	  Решил не смешивать. Сырую память просим в QueueImpl, а конструируем в Queue.
	void uninitializedImpl_copy(QueueImpl<T>& dest, const QueueImpl<T>& src) const {
		for (; dest.size_ < src.size_; ++dest.size_)
			new(dest.data_ + dest.size_) T(src.data_[dest.size_]);
	}
private:
	QueueImpl<T> impl_;
};