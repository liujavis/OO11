#ifndef OBSERVER_H
#define OBSERVER_H
#include <vector>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <map>

class Observer{
public:
};

class NonCopyable{
public:
	NonCopyable() = default;
	~NonCopyable() = default;
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;
	NonCopyable& operator=(NonCopyable&) = delete;
};

template<typename Func>
class Event : public NonCopyable{
public:
	template<typename F>
	int addObserver(F&& f_){
		std::lock_guard<std::mutex> lock(mtx);
		observers.emplace(m_observerId, std::forward<F>(f_));
		return m_observerId++;
	}

	void deleteObserver(int index){
		std::lock_guard<std::mutex> lock(mtx);
		observers.erase(index);
	}

	template<typename... Args>
	void update(Args... args){
		for(auto& i : observers){
			i.second(std::forward<Args>(args)...);
		}
	}
private:
	std::map<int, Func> observers;
	int m_observerId = 0;
	std::mutex mtx;
};

#endif