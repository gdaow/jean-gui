#pragma once


template<typename TFunction>
class Closure;

template<typename TReturn, typename TArg>
class Closure<TReturn (*)(TArg, void*)> {
    typedef TReturn (*TCallback)(TArg, void*);

public:
    Closure()
    : _callback(nullptr)
    , _data(nullptr) {

    }

    Closure(TCallback callback, void* data) :
        _callback(callback),
        _data(data) {
    }

    void set(TCallback callback, void* data) {
        _callback = callback;
        _data = data;
    }

    TReturn operator()(TArg arg) {
        return (*_callback)(arg, _data);
    }

private:
    TCallback _callback;
    void* _data;
};
