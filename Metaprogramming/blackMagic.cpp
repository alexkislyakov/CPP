#include <iostream>
#include <tuple>

//Oпределите список целых чисел (int) времени компиляции IntList.
//
//Внутри списка должна быть константа Head, соответствующая числу в голове списка, и имя типа Tail,
//соответствующее хвосту списка.

template<int... Ints>
struct IntList;

template<int T, int... Ints>
struct IntList<T, Ints...> {
    static const int Head = T;
    using Tail = IntList<Ints...>;
};
template<>
struct IntList<> {
};

// определите метафункцию Length для вычисления длины списка

template<typename T>
struct Length {
    static const int value = 1 + Length<typename T::Tail>::value;
};

template<>
struct Length<IntList<>> {
    static const int value = 0;
};

//Напишите две метафункции для работы c IntList:
//
//IntCons позволяет увеличить список на один элемент — он добавляется в начало списка.
//Generate позволяет сгенерировать список длины N с числами от 0 до N - 1.

template<int T, typename... L>
struct IntCons;

template<int T, int... L>
struct IntCons<T, IntList<L...>> {
    using type = IntList<T, L...>;
};

template<int N, int D = 0>
struct Generate {
    using type = typename IntCons<D, typename Generate<N - 1, D + 1>::type>::type;
};

template<>
struct Generate<0> {
    using type = IntList<>;
};

template<int D>
struct Generate<0, D> {
    using type = IntList<>;
};

//just checking in

template<typename T>
void check()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

//Воспользуйтесь IntList и метафункцией Generate для того, чтобы научиться "раскрывать" кортежи.
//От вас потребуется написать функцию apply, которая принимает функтор и кортеж с аргументами для
//вызова этого функтора и вызывает функтор от этих аргументов.

template<typename F, typename... Args, int... I>
auto apply(F f, const std::tuple<Args...> &t, IntList<I...>) -> decltype(f(std::get<I>(t)...)) {
    return f(std::get<I>(t)...);
}

template<typename F, typename... Args>
auto apply(F f, const std::tuple<Args...> &t) -> decltype(apply(f, t, typename Generate<sizeof...(Args)>::type())) {
    return apply(f, t, typename Generate<sizeof...(Args)>::type());
}

//бинарная метафункция Plus

template<int a, int b>
struct Plus
{
    static int const value = a + b;
};

//бинарная метафункция Minus

template<int a, int b>
struct Minus
{
    static int const value = a - b;
};

//Напишите метафункцию Zip (аналог std::transform), которая принимает два списка целых чисел
//одинаковой длины, а так же бинарную метафункцию,  и возвращает список, получившийся в результате
//поэлементного применения метафункции к соответствующим  элементам исходных списков.

template<typename L1, typename L2, template <int, int> class F>
struct Zip {
    using type = typename IntCons<F<L1::Head, L2::Head>::value, typename Zip<typename L1::Tail,
									     typename L2::Tail,
				                                             F>::type>::type;
};

template<template <int, int> class F>
struct Zip<IntList<>, IntList<>, F> {
    using type = IntList<>;
};

//Определите класс Quantity, которые хранит вещественное число и его размерность в системе СИ
//(размерность хранится как список IntList длины 7). Для этого класса нужно определить операторы
//сложения, вычитания, умножения и деления.

template<typename L>
class Quantity {
private:
    double val = 0;
public:
    Quantity() = default;
    
    explicit Quantity(double val) : val(val)
    {}
    
    double value() const {
        return val;
    }
};

template<int m = 0, int kg = 0, int s = 0, int A = 0, int K = 0, int mol = 0, int cd = 0>
using Dimension = IntList<m, kg, s, A, K, mol, cd>;

using NumberQ   = Quantity<Dimension<>>;           // число без размерности
using LengthQ   = Quantity<Dimension<1>>;          // метры
using MassQ     = Quantity<Dimension<0, 1>>;       // килограммы
using TimeQ     = Quantity<Dimension<0, 0, 1>>;    // секунды
using VelocityQ = Quantity<Dimension<1, 0, -1>>;   // метры в секунду
using AccelQ    = Quantity<Dimension<1, 0, -2>>;   // ускорение, метры в секунду в квадрате
using ForceQ    = Quantity<Dimension<1, 1, -2>>;   // сила в ньютонах

template<class T>
Quantity<T> operator+(const Quantity<T> &left, const Quantity<T> &right) {
    return Quantity<T>(left.value() + right.value());
}

template<class T>
Quantity<T> operator-(const Quantity<T> &left, const Quantity<T> &right) {
    return Quantity<T>(left.value() - right.value());
}

template<class T, class U>
Quantity<typename Zip<T, U, Minus>::type> operator/(const Quantity<T> &left, const Quantity<U> &right) {
    return Quantity<typename Zip<T, U, Minus>::type>(left.value() / right.value());
}

template<class T>
Quantity<T> operator/(const Quantity<T> &left, double right) {
    return Quantity<T>(left.value() / right);
}

template<class T>
Quantity<typename Zip<IntList<0, 0, 0, 0, 0, 0, 0>, T, Minus>::type> operator/(double left, const Quantity<T> &right) {
    return Quantity<typename Zip<IntList<0, 0, 0, 0, 0, 0, 0>, T, Minus>::type>(left / right.value());
}

template<class T, class U>
Quantity<typename Zip<T, U, Plus>::type> operator*(const Quantity<T> &left, const Quantity<U> &right) {
    return Quantity<typename Zip<T, U, Plus>::type>(left.value() * right.value());
}

template<class T>
Quantity<T> operator*(const Quantity<T> &left, double right) {
    return Quantity<T>(left.value() * right);
}

template<class T>
Quantity<T> operator*(double left, const Quantity<T> &right) {
    return Quantity<T>(right.value() * left);
}

int main()
{
    using L3 = Generate<5>::type;
    check<L3>();
    
    auto f = [](int x, double y, double z) { return x + y + z; };
    auto t = std::make_tuple(30, 5.0, 1.6); 
    auto res = apply(f, t);
    
    std::cout << res << std::endl;
    
    using L1 = IntList<1,2,3,4,5>;
    using L2 = IntList<1,3,7,7,2>;
    
    using L = Zip<L1, L2, Plus>::type;
    check<L>();
    
    LengthQ   length{30000};
    TimeQ     time{10 * 60};

    VelocityQ v = length / time;
    std::cout << v.value() << std::endl;
    
    AccelQ    a{9.8};
    MassQ     m{80};
    ForceQ    force = m * a;
    
    return EXIT_SUCCESS;
}
