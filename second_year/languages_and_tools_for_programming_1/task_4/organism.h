#ifndef ORGANISM_H
#define ORGANISM_H

#include <tuple>
#include <optional>

#include <stdint.h>


// Typ species_t określa gatunek, spełniający std::equality_comparable.
// Wartości can_eat_plants oraz can_eat_meat określają odpowiednio, czy dany
// organizm potrafi jeść mięso lub rośliny.
template <typename species_t, bool can_eat_meat, bool can_eat_plants>
requires std::equality_comparable<species_t>
class Organism {
private:
	const species_t species;
	const uint64_t vitality;

public:
	constexpr Organism(species_t const& _species,
	                   uint64_t _vitality) : species(_species), vitality(_vitality) {}

	~Organism() = default;
	Organism& operator=(const Organism&) = delete;

	constexpr uint64_t get_vitality() const {
		return vitality;
	}

	constexpr bool is_dead() const {
		return vitality == 0;
	}

	constexpr species_t const& get_species() const {
		return species;
	}

	// Uznajemy, że dwa organizmy o różnych preferencjach żywieniowych są różnymi gatunkami.
	template <typename species_t_other, bool can_eat_meat_other, bool can_eat_plants_other>
	constexpr bool is_same_species(
	    Organism<species_t_other, can_eat_meat_other, can_eat_plants_other> other)
	const {
		return std::is_same_v<species_t, species_t_other>
		       && other.get_species() == species
		       && can_eat_meat == can_eat_meat_other
		       && can_eat_plants == can_eat_plants_other;
	}

	constexpr bool is_plant() const {
		return !can_eat_plants && !can_eat_meat;
	}

	// Sprawdza czy organizm może zjeść dany organizm. Pomijamy sprawdzanie typów godowych, bo
	// jest to sprawdzane wcześniej w encounter.
	template <bool can_eat_meat_other, bool can_eat_plants_other>
	constexpr bool can_eat(const
	                       Organism<species_t, can_eat_meat_other, can_eat_plants_other>& other) const {
		if (other.is_plant())
			return can_eat_plants;
		else
			return can_eat_meat;
	}

	// Tworzy organizm tego samego gatunku, o witalnośći 0.
	constexpr Organism<species_t, can_eat_meat, can_eat_plants> die() const {
		return Organism<species_t, can_eat_meat, can_eat_plants>(species, 0);
	}

	// Tworzy organizm odpowiadający obecnemu, po zjedzeniu danej rośliny.
	template <bool can_eat_meat_other, bool can_eat_plants_other>
	constexpr Organism<species_t, can_eat_meat, can_eat_plants>
	eat_plant(const Organism<species_t, can_eat_meat_other, can_eat_plants_other>&
	          other) const {
		return Organism<species_t, can_eat_meat, can_eat_plants>
		       (this->get_species(), this->get_vitality() + other.get_vitality());
	}

	// Tworzy organizm odpowiadający obecnemu, po zjedzeniu danego zwierzęcia.
	template <bool can_eat_meat_other, bool can_eat_plants_other>
	constexpr Organism<species_t, can_eat_meat, can_eat_plants>
	eat_animal(const Organism<species_t, can_eat_meat_other, can_eat_plants_other>&
	           other) const {
		return Organism<species_t, can_eat_meat, can_eat_plants>
		       (this->get_species(), this->get_vitality() + other.get_vitality() / 2);
	}

	// Tworzy organizm odpowiadający obecnemu, po walce z danym zwierzęciem.
	template <bool can_eat_meat_other, bool can_eat_plants_other>
	constexpr Organism<species_t, can_eat_meat, can_eat_plants>
	fight(const Organism<species_t, can_eat_meat_other, can_eat_plants_other>&
	      other) const {
		if (this->get_vitality() <= other.get_vitality())
			return this->die();
		else
			return this->eat_animal(other);
	}
};


// Typy organizmów będące specjalizacjami szablonu Organism.
template <typename species_t>
using Carnivore = Organism<species_t, true, false>;

template <typename species_t>
using Herbivore = Organism<species_t, false, true>;

template <typename species_t>
using Omnivore = Organism<species_t, true, true>;

template <typename species_t>
using Plant = Organism<species_t, false, false>;


// Wynikiem spotkania jest krotka zawierająca kolejno: pierwotne organizmy zmodyfikowane
// w wyniku zdarzenia (jako nowe obiekty), w kolejności jak w argumentach, oraz
// opcjonalnie nowy organizm powstały w wyniku spotkania.
template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, bool sp2_eats_m, bool sp2_eats_p>
constexpr std::tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
          Organism<species_t, sp2_eats_m, sp2_eats_p>,
          std::optional<Organism<species_t, sp1_eats_m, sp1_eats_p>>>
          encounter(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
Organism<species_t, sp2_eats_m, sp2_eats_p> organism2) {
	// 1. Możliwe jest spotkanie jedynie organizmów, których typ gatunku (species_t)
	// jest taki sam - zapewnione przez sygnaturę funkcji.

	// 2. Nie jest możliwe spotkanie dwóch roślin.
	static_assert(sp1_eats_m || sp1_eats_p || sp2_eats_m ||
	              sp2_eats_p, "Two plants cannot encounter.");

	// 3. Spotkanie, w którym jedna ze stron jest martwa, nie powoduje skutków.
	if (organism1.is_dead() || organism2.is_dead())
		return std::make_tuple(organism1, organism2, std::nullopt);

	// 4. Spotkanie dwóch zwierząt tego samego gatunku prowadzi do godów.
	if (organism1.is_same_species(organism2)) {
		auto child = Organism<species_t, sp1_eats_m, sp1_eats_p>
		             (organism1.get_species(),
		              (organism1.get_vitality() / 2 + organism2.get_vitality() / 2 +
		               (organism1.get_vitality() % 2 && organism2.get_vitality() % 2)));
		return std::make_tuple(organism1, organism2, child);
	}

	// Zmienne pomocnicze, by uniknąć wielorazowego wywoływania funkcji can_eat.
	const bool can_1_eat_2 = organism1.template can_eat(organism2);
	const bool can_2_eat_1 = organism2.template can_eat(organism1);

	// 5. Spotkanie organizmów, które nie potrafią się zjadać, nie przynosi efektów.
	if (!can_1_eat_2 && !can_2_eat_1)
		return std::make_tuple(organism1, organism2, std::nullopt);

	// 6. Spotkanie dwóch zwierząt, które potrafią się nawzajem zjadać, prowadzi do walki.
	if (can_1_eat_2 && can_2_eat_1)
		return make_tuple(organism1.fight(organism2), organism2.fight(organism1),
		                  std::nullopt);

	// 7. Spotkanie roślinożercy lub wszystkożercy z rośliną skutkuje tym, że roślina zostaje zjedzona.
	if (organism1.is_plant() && can_2_eat_1)   //Organizm 2 zjada organizm 1.
		return std::make_tuple(organism1.die(), organism2.eat_plant(organism1),
		                       std::nullopt);
	if (organism2.is_plant() && can_1_eat_2)   //Organizm 1 zjada organizm 2.
		return std::make_tuple(organism1.eat_plant(organism2), organism2.die(),
		                       std::nullopt);

	// 8. Przy spotkaniu, w którym zdolność do konsumpcji zachodzi tylko w jedną
	// stronę, rozpatrujemy dwa przypadki. Jeśli „potencjalnie zjadany" ma witalność
	// nie mniejszą niż „potencjalnie zjadający", to nic się nie dzieje. W przeciwnym
	// przypadku zjadający dodaje do swojej witalności połowę (zaokrągloną w dół)
	// witalności zjedzonego, a zjedzony ginie.
	if (can_1_eat_2 && organism1.get_vitality() > organism2.get_vitality())
		return std::make_tuple(organism1.eat_animal(organism2), organism2.die(),
		                       std::nullopt); //Organizm 1 zjada organizm 2.

	if (can_2_eat_1 && organism2.get_vitality() > organism1.get_vitality())
		return std::make_tuple(organism1.die(), organism2.eat_animal(organism1),
		                       std::nullopt); //Organizm 2 zjada organizm 1.

	//Spotkanie bez skutku. Organizm nie może zjeść drugiego, bo ten ma za dużą witalność.
	return std::make_tuple(organism1, organism2, std::nullopt);
}


// Pomocnicza funkcje encounter series - rekurencyjne wywołanie.
template <typename species_t, bool sp1_eats_m, bool sp1_eats_p,
          bool sp2_eats_m, bool sp2_eats_p, typename... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
helper_encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
                        Organism<species_t, sp2_eats_m, sp2_eats_p> organism2, Args... args) {
	return encounter_series(get<0>(encounter(organism1, organism2)), args...);
}

// Pomocnicza funkcja encounter series - zakończenie gdy dany 1 argument.
template <typename species_t, bool sp1_eats_m, bool sp1_eats_p>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1) {
	return organism1;
}

// Wynikiem funkcji jest organism1 zmieniony
// poprzez wszystkie spotkania. Zmiany na pozostałych organizmach oraz ewentualne
// dzieci powstałe w wyniku spotkań nie są zwracane.
template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, typename... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
                 Args... args) {
	return helper_encounter_series(organism1, args...);
}


#endif // ORGANISM_H