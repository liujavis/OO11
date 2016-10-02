#pragma once
#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>
#include <atomic>
#include <iostream>
#include <utility>
#include <cstdlib>
#include <memory>

class SpinLock final {
public:
	SpinLock() :atomic_lock(false) {}
	SpinLock(const SpinLock&) = default;
	SpinLock(SpinLock&&) = default;
	SpinLock& operator=(const SpinLock&) = delete;
	SpinLock& operator=(SpinLock&) = delete;
	~SpinLock() = default;
	void lock() {
		while (atomic_lock.exchange(true));
	}
	void unlock() {
		atomic_lock = false;
	}
private:
	std::atomic_bool atomic_lock;
};

template<typename T>
class Singleton {
	typedef std::shared_ptr<T> T_PTR;
	typedef std::weak_ptr<T> T_WPTR;
public:
	template<typename... Args>
	static T_PTR get_instance() {
		if (nullptr == _instance) {
			std::lock_guard<decltype(_lock)> lock(_lock);
			if (nullptr == _instance) {
				_instance = make_unique<T>(std::forward<Args>(args)...);
			}
		}
		return _instance;
	}
	Singleton() = delete;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	~Singleton() = delete;
private:
	static T_PTR _instance;
	static SpinLock _lock;

};

template<typename T>
typename Singleton<T>::T_PTR Singleton<T>::_instance = nullptr;

template<typename T>
SpinLock Singleton<T>::_lock;

#endif