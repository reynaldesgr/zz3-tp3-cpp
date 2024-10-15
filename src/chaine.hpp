#include <exception>
#include <string>
#include <sstream>
#include <type_traits>
#include <typeinfo>

#include <demangle.hpp>

template <typename... Args>
std::string chaine(const Args&... args);

template <typename... Args>
std::string chaine(const std::tuple<Args...>& t);

class ExceptionChaine : std::exception
{
    private:
        std::string type_;
        std::string msg_;
    public:
        template <typename T>
        ExceptionChaine(const T _)
            : type_(demangle(typeid(T).name())), msg_("Conversion en chaine impossible pour '" + type_ + "'") {}

        const char* what() const noexcept override {
            return msg_.c_str();
        }
};

template <typename T>
std::string chaine(const T x)
{
    throw ExceptionChaine(x);
}

// Specializations
template <>
std::string chaine<std::string>(const std::string x)
{
    return x;
}

template <>
std::string chaine<int>(const int x) {
    std::ostringstream oss;
    oss << x;
    return oss.str();
}

template <>
std::string chaine<double>(const double x)
{
    return std::to_string(x);
}

// Fonction helper pour appeler la version variadique de chaine avec les éléments du tuple
template <typename Tuple, std::size_t... Is>
std::string chaine_bis(const Tuple& t, std::index_sequence<Is...>) {
    return chaine(std::get<Is>(t)...);
}

// Variadic template
template <typename... Args>
std::string chaine(const Args&... args)
{
    std::ostringstream oss;
    // Le symbole ... indique que l'expression doit être appliquée à chaque argument dans le pack de paramètres args.
    ((oss << chaine(args) << " "), ...);
    return oss.str();
}


// Surcharge de la fonction chaine pour les tuples
template <typename... Args>
std::string chaine(const std::tuple<Args...>& t) {
    return chaine_bis(t, std::make_index_sequence<sizeof...(Args)>());
}

// std::make_index_sequence<sizeof...(Args)>  
// e.x. : std::make_index_sequence<10> creates a std::index_sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9>

// Factorielle
template <unsigned int N>
struct Factorielle {
    static const unsigned long valeur = N * Factorielle<N - 1>::valeur;
};

template <>
struct Factorielle<0> {
    static const unsigned long valeur = 1;
};

// Puissance
template <unsigned int N>
struct Puissance {
    static double valeur(double x) {
        return x * Puissance<N - 1>::valeur(x);
    }
};

template <>
struct Puissance<0> {
    static double valeur(double x) {
        return 1.0;
    }
};

// Exponentielle 
template <unsigned int N>
struct Exponentielle {
    static double valeur(double x) {
        return Puissance<N>::valeur(x) / Factorielle<N>::valeur + Exponentielle<N - 1>::valeur(x);
    }
};

template <>
struct Exponentielle<0> {
    static double valeur(double x) {
        return 1.0;
    }
};

// Cosinus
template <unsigned int N>
struct Cosinus {
    static double valeur(double x) {
        return ((N % 2 == 0) ? 1 : -1) * Puissance<2 * N>::valeur(x) / Factorielle<2 * N>::valeur + Cosinus<N - 1>::valeur(x);
    }
};

template <>
struct Cosinus<0> {
    static double valeur(double x) {
        return 1.0;
    }
};

// Sinus
template <unsigned int N>
struct Sinus {
    static double valeur(double x) {
        return ((N % 2 == 0) ? 1 : -1) * Puissance<2 * N + 1>::valeur(x) / Factorielle<2 * N + 1>::valeur + Sinus<N - 1>::valeur(x);
    }
};

template <>
struct Sinus<0> {
    static double valeur(double x) {
        return x;
    }
};
