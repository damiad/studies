# Organism Simulation

The goal of this task is to write code to simulate the interactions between organisms in a natural environment. An organism is represented by an instance of the template:

```cpp
template <typename species_t, bool can_eat_meat, bool can_eat_plants> class Organism;
```

The `species_t` type specifies the species of the organism. This type should satisfy the `std::equality_comparable` concept, which should be checked at compile time. The `can_eat_plants` and `can_eat_meat` values indicate whether the organism can eat plants or meat, respectively.

The solution should provide four types of organisms as specializations of the `Organism` template:

- `Carnivore` - a carnivore,
- `Omnivore` - an omnivore,
- `Herbivore` - a herbivore,
- `Plant` - a plant, unable to eat either meat or plants.

Each organism should define a method:

```cpp
uint64_t get_vitality();
```

which returns the vitality of the organism. An organism with a vitality of 0 is considered dead.

The `Organism` class should provide a constructor that sets the species and initial vitality of the organism:

```cpp
Organism(species_t const &species, uint64_t vitality);
```

It should be possible to access the species of the organism:

```cpp
const species_t &get_species();
```

Additionally, an object of type `Organism` should have additional methods that allow for an elegant solution to the task, with a specification chosen by the implementers.

Organisms interact according to the following rules. The rules are applied sequentially until the first rule is found that determines the outcome of the encounter. We stop checking the rules and ignore the subsequent rules.

1. It is only possible for organisms of the same species type (species_t) to interact (values do not have to be the same). For example, in the declarations below, a gazelle can encounter a lion, but not a dog. An attempt to interact organisms of different types should be detected at compile time and raise an error.

```cpp
Carnivore<std::string> lion("Panthera leo", 462);
Herbivore<std::string> gazelle("Gazella dorcas", 130);
Omnivore<uint64_t> dog(1, 15);
```

We consider two organisms with different dietary preferences as different species, even if the species_t comparison returns true. For example, the two organisms below are treated as different species.

```cpp
Omnivore<std::string> human("Homo Sapiens", 100);
Herbivore<std::string> vegetarian("Homo Sapiens", 10);
```

2. It is not possible for two plants to interact because they are immobile. An attempt to interact two plants should be detected at compile time and raise an error.

3. If one of the organisms is dead, the encounter has no effect.

4. If two organisms of the same species meet, they mate. For simplicity, we assume that all organisms are hermaphroditic, and any two representatives of the same species can mate. The result is an offspring of the same species as the parents, with vitality equal to the arithmetic mean of the parents' vitality (rounded down).

5. If two organisms that cannot eat each other meet, there are no consequences.

6. If two organisms that can eat each other meet, a fight occurs. The organism with higher vitality wins the fight. The losing organism dies. The winning organism adds half of the vitality of the losing organism (rounded down) to its own vitality. When the organisms have equal vitality, a deadly fight occurs: both organisms kill each other.

7. If a herbivore or omnivore encounters a plant, the plant is eaten. The eater increases its vitality by the vitality of the plant, and the vitality of the plant is set to 0.

8. In an encounter where consumption can only happen in one direction, two cases are considered. If the "potentially eaten" organism has vitality not less than the "potentially eating" organism, nothing happens. Otherwise, the eater adds half of the vitality of the eaten organism (rounded down) to its own vitality, and the eaten organism dies.

The encounter should be implemented as a function template with the following signature:

```cpp
template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, bool sp2_eats_m, bool sp2_eats_p>
constexpr std::tuple<Organism<species_t, sp1_eats_m, sp1_eats_p>,
                     Organism<species_t, sp2_eats_m, sp2_eats_p>,
                     std::optional<Organism<species_t, sp1_eats_m, sp1_eats_p>>>
encounter(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1,
          Organism<species_t, sp2_eats_m, sp2_eats_p> organism2);
```

The result is a tuple containing, in order: the original organisms modified as a result of the encounter (as new objects), in the order specified by the arguments, and optionally a new organism resulting from the encounter.

Additionally, the solution should provide a template:

```cpp
template <typename species_t, bool sp1_eats_m, bool sp1_eats_p, typename ... Args>
constexpr Organism<species_t, sp1_eats_m, sp1_eats_p>
encounter_series(Organism<species_t, sp1_eats_m, sp1_eats_p> organism1, Args ... args);
```

where `Args` are also of type `Organism` with the same `species_t` as `organism1` but possibly different dietary preferences. This function simulates a series of encounters between the organism specified as the first argument and the organisms provided as the remaining arguments, from left to right. The function returns `organism1` modified by all the encounters. Changes to the other organisms and any offspring resulting from the encounters are not returned.

It should be possible to declare:

```cpp
Herbivore<std::string> lion("Panthera leo", 462);
```

The goal is to answer the question: what if lions were herbivores?

In function and class declarations, const and constexpr specifiers should be used wherever necessary or possible.

In this task, the use of inheritance is prohibited. Specialization of templates should be used instead.