#pragma once

#define EVENT_CLASS(_type, _parents, _members, _parameters) \
class _type _parents {\
private:\
	static EventEmitter<_type>& getEmitter() {\
		static EventEmitter<_type> emitter;\
		return emitter;\
	}\
public:\
	_members\
	_type(_parameters);\
	static void subscribe(EventObserver<_type>* obv);\
	static void unsubscribe(EventObserver<_type>* obv);\
	void emit();\
};\

#define MEMBER_SET(x) this->x = x;

#define EVENT_CPP(_type, _parameters, _members_set) \
_type::_type(_parameters) {\
_members_set\
}\
void _type::subscribe(EventObserver<_type>* obv) {\
	getEmitter().subscribe(obv);\
}\
void _type::unsubscribe(EventObserver<_type>* obv) {\
	getEmitter().unsubscribe(obv);\
}\
void _type::emit() {\
	getEmitter().emit(this);\
}\
