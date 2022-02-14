#pragma once
#include <functional>
#include <vector>
#include <memory>

namespace detail {
	namespace moredetail {
		template <class Datatype>
		class EventObserver {
			bool _isSet = false;
		public:
			bool isSet() { return _isSet; }
			std::function<void(const Datatype&)> f;
			EventObserver(std::function<void(const Datatype&)> f) {
				this->f = f;
				_isSet = true;
			}
			void reset() {
				_isSet = false;
			}
		};
	}

	template <typename Datatype>
	class Emitter;

	template <class Datatype>
	class EventObserver {
		void subscribe() {
			Emitter<Datatype>::emitter.subscribe(observer);
			_isSet = true;
		}
		void unsubscribe() {
			observer->reset();
			Emitter<Datatype>::emitter.unsubscribe(observer);
			_isSet = false;
		}
		bool _isSet = false;
		std::shared_ptr<detail::moredetail::EventObserver<Datatype>> observer;
	public:
		EventObserver() { }
		EventObserver(std::function<void(const Datatype&)> f) {
			set(f);
		}
		~EventObserver() {
			if (_isSet)
				unsubscribe();
		}
		void set(std::function<void(const Datatype&)> f) {
			if (_isSet)
				unsubscribe();
			observer = std::make_shared<detail::moredetail::EventObserver<Datatype>>(f);
			subscribe();
		}
		void unset() {
			unsubscribe();
		}
		bool isSet() {
			return _isSet;
		}
	};

	template <typename Datatype>
	class EventEmitter {
	private:
		std::vector<std::shared_ptr<detail::moredetail::EventObserver<Datatype>>>   subscribers;
		std::vector<std::shared_ptr<detail::moredetail::EventObserver<Datatype>>> unsubscribers;
	public:
		void subscribe(std::shared_ptr<detail::moredetail::EventObserver<Datatype>> obv) {
			subscribers.push_back(obv);
		}
		void unsubscribe(std::shared_ptr<detail::moredetail::EventObserver<Datatype>> obv) {
			unsubscribers.push_back(obv);
		}
		void emit(const Datatype& data) {
			for (const auto& obv : subscribers)
				if (obv->isSet())
					obv->f(data);
			for (const auto& obv : unsubscribers)
				subscribers.erase(std::find(subscribers.begin(), subscribers.end(), obv));
			unsubscribers = {};
		}

		void clear() {
			subscribers = {};
		}
	};

	template <typename Datatype>
	class Emitter {
	public:
		static detail::EventEmitter<Datatype> emitter;
	};
	template <typename Datatype>
	detail::EventEmitter<Datatype>  Emitter<Datatype>::emitter;
}

template <typename Datatype>
class Event {
public:
	using Observer = detail::EventObserver<Datatype>;
	using Data = Datatype;
	void emit() {
		detail::Emitter<Datatype>::emitter.emit(data);
 	}
	const Datatype data;
	Event(Datatype data) : data(data) {}
};

#define EVENT(T, ...) T(T::Data{__VA_ARGS__})
