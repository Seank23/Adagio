#pragma once
#include <iostream>
#include <string>

// The Strategy interface declares operations common to all supported versions
// of some algorithm.
//
// The Context uses this interface to call the algorithm defined by Concrete
// Strategies.
template <typename T, typename U>
class Strategy {
public:
    virtual ~Strategy() = default;

    virtual U DoAlgorithm(const T& data) const = 0;
};

// Concrete Strategies implement the algorithm while following the base
// Strategy interface. The interface makes them interchangeable in the Context.
template <typename T, typename U>
class ConcreteStrategyA : public Strategy<T, U> {
public:
    U DoAlgorithm(const T& data) const override {
        return "Called ConcreteStrategyA.DoAlgorithm() with data: " + std::to_string(data);
    }
};

template <typename T, typename U>
class ConcreteStrategyB : public Strategy<T, U> {
public:
    U DoAlgorithm(const T& data) const override {
        return "Called ConcreteStrategyB.DoAlgorithm() with data: " + std::to_string(data);
    }
};

// The Context defines the interface of interest to clients.
template <typename T, typename U>
class Context {
public:
    explicit Context(Strategy<T, U>* strategy = nullptr) : strategy_(strategy), data_() {
    }

    void SetStrategy(Strategy<T, U>* strategy) {
        strategy_ = strategy;
    }

    void SetData(const T& data) {
        data_ = data;
    }

    void DoSomeBusinessLogic() {
        // ...

        U result = strategy_->DoAlgorithm(data_);

        std::cout << result << std::endl;

        // ...
    }

private:
    Strategy<T, U>* strategy_;
    T data_;
};

int main() {
    Context<int, std::string>* context = new Context<int, std::string>(new ConcreteStrategyA<int, std::string>);
    context->SetData(123);
    context->DoSomeBusinessLogic();
    context->SetStrategy(new ConcreteStrategyB<int, std::string>);
    context->SetData(456);
    context->DoSomeBusinessLogic();

    delete context;

    return 0;
}
