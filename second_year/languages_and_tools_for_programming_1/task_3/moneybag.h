#ifndef MONEYBAG_H
#define MONEYBAG_H

#include <iostream>

class Moneybag {
public:
    // typ oznaczający liczbę monet danego nominału w sakiewce
    using coin_number_t = uint64_t;

    // Tworzy sakiewkę na podstawie liczby monet
    constexpr Moneybag(coin_number_t liv, coin_number_t sol, coin_number_t den) :
            livres(liv), soliduses(sol), deniers(den) { }

    // Tworzy sakiewkę na podstawie innej sakiewki
    constexpr Moneybag(const Moneybag &) = default;

    // Przypisuje jedną sakiewkę na inną
    constexpr Moneybag &operator=(const Moneybag &) = default;

    // Zwraca liczbę liwrów w sakiewce
    constexpr coin_number_t livre_number() const noexcept {
        return livres;
    }

    // Zwraca liczbę solidów w sakiewce
    constexpr coin_number_t solidus_number() const noexcept {
        return soliduses;
    }

    // Zwraca liczbę denarów w sakiewce
    constexpr coin_number_t denier_number() const noexcept {
        return deniers;
    }

    // Porównuje dwie sakiewki
    const constexpr std::partial_ordering operator<=>(const Moneybag &that) const {
        if (this->livres == that.livre_number() &&
            this->soliduses == that.solidus_number() &&
            this->deniers == that.denier_number())
            return std::partial_ordering::equivalent; // równe sakiewki
        if (this->livres <= that.livre_number() &&
            this->soliduses <= that.solidus_number() &&
            this->deniers <= that.denier_number())
            return std::partial_ordering::less; // sakiewka "mniejsza"
        if (this->livres >= that.livre_number() &&
            this->soliduses >= that.solidus_number() &&
            this->deniers >= that.denier_number())
            return std::partial_ordering::greater; // sakiewka "większa"
        return std::partial_ordering::unordered;
    }

    // Specjalnie przeładowany operator równości dwóch sakiewek
    constexpr bool operator==(const Moneybag &) const = default;

    // Dodaje zawartość innej sakiewki do obecnej
    const constexpr Moneybag &operator+=(const Moneybag &other) {
        if (COIN_MAX - this->livres < other.livre_number() ||
            COIN_MAX - this->soliduses < other.solidus_number() ||
            COIN_MAX - this->deniers < other.denier_number())
            throw std::out_of_range("Moneybag coins overflow");

        this->livres += other.livre_number();
        this->soliduses += other.solidus_number();
        this->deniers += other.denier_number();
        return *this;
    }

    // Odejmuje zawartość innej sakiewki od obecnej
    const constexpr Moneybag &operator-=(const Moneybag &other) {
        if (this->livres < other.livre_number() ||
            this->soliduses < other.solidus_number() ||
            this->deniers < other.denier_number())
            throw std::out_of_range("Moneybag coins overflow");

        this->livres -= other.livre_number();
        this->soliduses -= other.solidus_number();
        this->deniers -= other.denier_number();
        return *this;
    }

    // Mnoży zawartość sakiewki przez liczbę nieujemną
    const constexpr Moneybag &operator*=(uint64_t decimal) {
        uint64_t check = COIN_MAX / decimal;
        if (check < this->livres || check < this->soliduses || check < this->deniers)
            throw std::out_of_range("Moneybag coins overflow");

        this->livres *= decimal;
        this->deniers *= decimal;
        this->soliduses *= decimal;
        return *this;
    }

    // Dodaje zawartość dwóch sakiewek do siebie
    const constexpr Moneybag operator+(const Moneybag &other) const {
        return Moneybag(*this) += other;
    }

    // Odejmuje zawartość dwóch sakiewek od siebie
    const constexpr Moneybag operator-(const Moneybag &other) const {
        return Moneybag(*this) -= other;
    }

    // Mnoży zawartość sakiewki przez liczbę nieujemną
    const constexpr Moneybag operator*(uint64_t decimal) const {
        return Moneybag(*this) *= decimal;
    }

    // Mnoży liczbę nieujemną przez zawartość sakiewki (druga wersja
    // dla argumentów podanych odwrotnie)
    const constexpr friend Moneybag operator*(uint64_t decimal, const Moneybag &bag) {
        return Moneybag(bag) *= decimal;
    }

    // Określa niepustość sakiewki
    constexpr explicit operator bool() const {
        return livre_number() > 0 || solidus_number() > 0 || denier_number() > 0;
    }

    // Wypisuje zawartość sakiewki na wyjście
    friend std::ostream &operator<<(std::ostream &os, const Moneybag &bag) {
        os << "(" << bag.livre_number() << " livr" << (bag.livre_number() != 1 ? "es, " : ", ")
           << bag.solidus_number() << " solidus" << (bag.solidus_number() != 1 ? "es, " : ", ")
           << bag.denier_number() << " denier" << (bag.denier_number() != 1 ? "s" : "") << ")";
        return os;
    }

private:
    // maksymalna dozwolona liczba monet jednego rodzaju w sakiewce
    static constexpr coin_number_t COIN_MAX = std::numeric_limits<coin_number_t>::max();
    coin_number_t livres; // liczba liwrów w sakiewce
    coin_number_t soliduses; // liczba solidów w sakiewce
    coin_number_t deniers; // liczba denarów w sakiewce
};


class Value {
public:
    // Wyznacza wartość na podstawie liczby
    constexpr Value(uint64_t val = 0) : value(static_cast<__uint128_t>(val)) { }

    // Wyznacza wartość na podstawie sakiewki
    constexpr Value(const Moneybag &moneybag) : value(
            static_cast<__uint128_t>(moneybag.livre_number()) * liv_den +
            static_cast<__uint128_t>(moneybag.solidus_number()) * sol_den +
            static_cast<__uint128_t>(moneybag.denier_number())) { }

    // Tworzy wartość na podstawie innej wartości
    constexpr Value(const Value &) = default;

    // Przypisuje jedną wartość na inną
    constexpr Value &operator=(const Value &) = default;

    // Porówuje wartość z inną wartością
    const constexpr std::strong_ordering
    operator<=>(const Value &) const = default;

    // Porówuje wartość z liczbą
    const constexpr std::strong_ordering operator<=>(uint64_t val) const {
        return this->value <=> static_cast<__uint128_t>(val);
    }

    // Specjalnie przeładowany operator równości dwóch wartości
    constexpr bool operator==(const Value &) const = default;

    // Specjalnie przeładowany operator równości wartości i liczby
    constexpr bool operator==(uint64_t val) const {
        return static_cast<__uint128_t>(val) == this->value;
    }

    // Przekształca wartość na napis
    explicit operator std::string() const {
        std::string str;
        __uint128_t val = value;

        // cyfry wyznaczane są od końca, zatem na końcu należy odwrócić napis
        do {
            str += static_cast<char>(ZERO_CHAR + val % 10);
            val /= 10;
        } while (val);

        std::reverse(str.begin(), str.end());
        return str;
    }

private:
    static constexpr __uint128_t liv_den = 240; // 240 denarów = 1 liwr
    static constexpr __uint128_t sol_den = 12; // 12 denarów = 1 solid
    static constexpr char ZERO_CHAR = '0'; // symbol 0 do wypisywania wartości
    __uint128_t value; // liczbowa reprezentacja wartości
};


inline constinit Moneybag const Livre = Moneybag{1, 0, 0};
inline constinit Moneybag const Solidus = Moneybag{0, 1, 0};
inline constinit Moneybag const Denier = Moneybag{0, 0, 1};

#endif // MONEYBAG_H
