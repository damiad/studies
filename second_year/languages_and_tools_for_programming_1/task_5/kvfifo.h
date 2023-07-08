#ifndef KVFIFO_H
#define KVFIFO_H

#include <map>
#include <list>
#include <iterator> 	// For std::forward_iterator_tag
#include <cstddef>  	// For std::ptrdiff_t
#include <memory>   	// For std::shared_ptr
#include <stdexcept>	// For std::invalid_argument
#include <concepts>
#include <utility>

template <typename K, typename V>
requires (
	std::totally_ordered<K> && std::semiregular<K>
    && std::copy_constructible<V>
) class kvfifo {
private:
	using data_list_t = std::list<std::pair<const K, V>>;
	using data_map_value_t = std::list<typename data_list_t::iterator>;

	// Specjalna klasa służąca do obsługi wyjątków, schemat użycia jest
	// następujący:
	// Na początku wywołania każdej funkcji która może się nie powieść tworzymy obiekt
	// exception_guard g z domyślnie zdefiniowanym parametrem failure jako true.
	// Jeśli podczas wykonywania funkcji zostanie zgłoszony wyjątek, to wykonanie
	// zostanie wstrzymane i zostanie zawołany destruktor obiektu g, który
	// zrobi undo ewentualnie zrobionej / zaczętej i nieudanej głębokiej kopii obiektu.
	// Jeśli wszystko się udało, to na koniec funkcji wołamy metodę
	// g.no_failure(), która ustawia parametr failure na false.
	class exception_guard {
	public:
		explicit exception_guard(kvfifo* guarded_kvfifo) :
			guarded_kvfifo(guarded_kvfifo),
			guarded_queue_data(guarded_kvfifo->queue_data),
			guarded_unshareable(guarded_kvfifo->unshareable) {}

		~exception_guard() noexcept {
			if (failure) {
				// Undo deep copy:
				std::swap(guarded_kvfifo->queue_data, guarded_queue_data);
				guarded_kvfifo->unshareable = guarded_unshareable;
			}
			guarded_queue_data.reset();
		}

		void no_failure() noexcept {
			failure = false;
		}


	private:
		kvfifo* guarded_kvfifo;
		std::shared_ptr<typename kvfifo<K, V>::data> guarded_queue_data;
		bool guarded_unshareable;
		bool failure = true;
	};


	struct data {
		//mapa klucz, lista iterratorów na pozycje z elements w których, te klucze, value występują.
		std::map<const K, data_map_value_t> map;

		//lista dwukierunkowa z elementami
		data_list_t elements;

		data() = default;

		// Głęboka kopia:
		data(struct data const& other) {
			data_list_t new_elements;
			std::map<const K, data_map_value_t> new_map;

			for (auto it = other.elements.begin(); it != other.elements.end(); ++it)
				new_elements.emplace_back(it->first, it->second);

			for (auto it = new_elements.begin(); it != new_elements.end(); ++it) {
				auto map_pos = new_map.find(it->first);
				if (map_pos == new_map.end()) {
					data_map_value_t new_list;
					new_list.emplace_back(it);
					new_map.emplace(it->first, new_list);
				} else
					map_pos->second.emplace_back(it);
			}

			std::swap(map, new_map);
			std::swap(elements, new_elements);
		}

		~data() noexcept = default;
	};

	// Wołamy tą funkcję gdy tylko będziemy zwracać niestałą referencję
	// do naszej kolejki: about_to_modify(true)
	// lub przy modyfikacji kolejki: about_to_modify(false)
	// Funkcja sprawdza, czy głęboka kopia jest konieczna
	// i aktualizuje atrybut unshareable.
	// Źródło: http://www.gotw.ca/gotw/044.htm
	// Umawiamy się że domyślnie kolejka po modyfikacji będzie shareable
	// (np. operacje pop, push)
	// Ale czasami będzie unshareable, np gdy damy referncję metodą front().
	void about_to_modify(bool copy_is_unshareable = false) {
		// Głębokiej kopii **nie robimy** wtw gdy:
		// Jesteśmy unshareable, bo to znaczy że mamy wyłączność na queue_data
		// Jesteśmy shareable, ale mamy wyłączność na wskaźnik.
		// Sprawdzamy, czy use count > 2 bo exception guard też patrzy
		// się na queue_data.

		if (queue_data.use_count() > 2 && !unshareable) {
			// Zrób deep copy:
			queue_data = std::make_shared<data>(*queue_data);
		}
		// W przeciwnym razie możemy zmodyfikować nasze queue_data.
		// Aktualizuj atrybut unshareable:
		unshareable = copy_is_unshareable;
	}

public:

	bool unshareable = false;
	std::shared_ptr<typename kvfifo<K, V>::data> queue_data;

	// Konstructory:
	kvfifo() : queue_data(std::make_shared<data>()) {}

	kvfifo(kvfifo const& other) {
		// Jeśli możliwe, nie rób deep copy:
		if (!other.unshareable || !other.queue_data)
			queue_data = other.queue_data;
		else
			queue_data = std::make_shared<data>(*(other.queue_data));
	}

	kvfifo(kvfifo&& other) noexcept {
		queue_data = other.queue_data;
		unshareable = other.unshareable;
		other.queue_data = nullptr;
		other.unshareable = false;
	}

	~kvfifo() noexcept = default;

	// Operatory:
	kvfifo& operator=(kvfifo other) {
		// Jeśli możliwe, nie rób deep copy:
		if (!other.unshareable)
			queue_data = other.queue_data;
		else
			queue_data = std::make_shared<data>(*(other.queue_data));
		// Na pewno będziemy shareable:
		unshareable = false;
		return *this;
	}

	// Metody:

	// Wstawia wartość v na koniec kolejki, nadając jej klucz k. Złożoność O(log n).
	void push(K const& k, V const& v) {
		if (queue_data == nullptr)
			queue_data = std::make_shared<data>();
		exception_guard g(this);
		about_to_modify();

		queue_data->elements.emplace_back(k, v);
		bool new_key_created = false;
		// Próba wstawienia do mapy:
		try {
			if (!queue_data->map.contains(k)) {
				queue_data->map.emplace(k, data_map_value_t());
				new_key_created = true;
			}

			auto iter = queue_data->map.find(k);
			iter->second.emplace_back(std::prev(queue_data->elements.end()));
		} catch (...) {
			// Undo wstawienia do listy:
			if (new_key_created) queue_data->map.erase(k);
			queue_data->elements.pop_back();
			throw;
		}
		g.no_failure();
	}

	// Usuwa pierwszy element z kolejki. Jeśli kolejka jest pusta, to podnosi
	// wyjątek std::invalid_argument. Złożoność O(log n).
	void pop() {
		if (!queue_data || empty()) throw std::invalid_argument("Empty queue");
		exception_guard g(this);
		about_to_modify();

		K deleted_key = queue_data->elements.front().first;
		queue_data->map.find(deleted_key)->second.pop_front();
		if (queue_data->map.find(deleted_key)->second.empty()) {
			auto iter = queue_data->map.find(deleted_key);
			queue_data->map.erase(iter);
		}
		queue_data->elements.pop_front();
		g.no_failure();
	}

	// Usuwa pierwszy element o podanym kluczu z kolejki. Jeśli podanego klucza
	// nie ma w kolejce, to podnosi wyjątek std::invalid_argument. Złożoność O(log n).
	void pop(K const& x) {

		if (!queue_data ||
		        count(x) == 0) throw std::invalid_argument("No matching key");
		exception_guard g(this);

		about_to_modify();

		auto iter = queue_data->map.find(x);
		queue_data->elements.erase(iter->second.front());
		iter->second.pop_front();

		if (iter->second.size() == 0)
			queue_data->map.erase(x);
		g.no_failure();
	}

	// Przesuwa elementy o kluczu k na koniec kolejki, zachowując
	// ich kolejność względem siebie. Zgłasza wyjątek std::invalid_argument, gdy
	// elementu o podanym kluczu nie ma w kolejce. Złożoność O(m + log n), gdzie m to
	// liczba przesuwanych elementów.
	void move_to_back(K const& k) {

		if (!queue_data) throw std::invalid_argument("nullptr");
		exception_guard g(this);
		size_t how_many = count(k);
		if (how_many == 0) throw std::invalid_argument("No matching key");

		about_to_modify();

		auto iter = queue_data->map.find(k)->second.begin();
		for (size_t i = 0; i < how_many; i++) {

			queue_data->elements.splice(queue_data->elements.end(), queue_data->elements,
			                            *iter);
			iter++;
		}
		g.no_failure();
	}

	// Zwraca parę referencji do klucza i wartości znajdującej na początku / końcu kolejki.
	// W wersji nie-const zwrócona para powinna umożliwiać modyfikowanie wartości, ale nie klucza.
	// Dowolna operacja modyfikująca kolejkę może unieważnić zwrócone referencje.
	// Jeśli kolejka jest pusta, to podnosi wyjątek std::invalid_argument. Złożoność O(1).
	std::pair<K const&, V&> front() {
		if (empty()) throw std::invalid_argument("Empty queue");
		exception_guard g(this);

		about_to_modify(true);
		g.no_failure();
		return {queue_data->elements.front().first, queue_data->elements.front().second};
	}

	std::pair<K const&, V const&> front() const {
		if (empty()) throw std::invalid_argument("Empty queue");
		return queue_data->elements.front();
	}

	std::pair<K const&, V&> back() {

		if (empty()) throw std::invalid_argument("Empty queue");
		exception_guard g(this);
		about_to_modify(true);
		g.no_failure();
		return {queue_data->elements.back().first, queue_data->elements.back().second};
	}

	std::pair<K const&, V const&> back() const {
		if (empty()) throw std::invalid_argument("Empty queue");
		return queue_data->elements.back();
	}

	// Metody first i last zwracają odpowiednio pierwszą i ostatnią parę
	// klucz-wartość o danym kluczu, podobnie jak front i back. Jeśli podanego klucza
	// nie ma w kolejce, to podnosi wyjątek std::invalid_argument. Złożoność O(log n).
	std::pair<K const&, V&> first(K const& key) {
		if (!queue_data ||
		        count(key) == 0) throw std::invalid_argument("No matching key");
		exception_guard g(this);
		about_to_modify(true);

		auto iter = queue_data->map.find(key);
		auto list_iter = iter->second.front();
		std::pair<K const&, V&> res =
		{list_iter->first, list_iter->second};
		g.no_failure();
		return res;
	}

	std::pair<K const&, V const&> first(K const& key) const {
		if (!queue_data ||
		        count(key) == 0) throw std::invalid_argument("No matching key");
		auto iter = queue_data->map.find(key);
		auto list_iter = iter->second.front();
		std::pair<K const&, V const&> res =
		{list_iter->first, list_iter->second};

		return res;
	}

	std::pair<K const&, V&> last(K const& key) {
		if (!queue_data ||
		        count(key) == 0) throw std::invalid_argument("No matching key");
		exception_guard g(this);

		about_to_modify(true);

		auto iter = queue_data->map.find(key);
		auto list_iter = iter->second.back();
		std::pair<K const&, V&> res =
		{list_iter->first, list_iter->second};
		g.no_failure();
		return res;
	}

	std::pair<K const&, V const&> last(K const& key) const {
		if (!queue_data ||
		        count(key) == 0) throw std::invalid_argument("No matching key");
		auto iter = queue_data->map.find(key);
		auto list_iter = iter->second.back();

		std::pair<K const&, V const&> res =
		{list_iter->first, list_iter->second};

		return res;
	}


	// Zwraca liczbę elementów w kolejce.
	size_t size() const noexcept {
		if (!queue_data) return 0;
		return queue_data->elements.size();
	}

	// Zwraca true, gdy kolejka jest pusta, a false w przeciwnym przypadku.
	bool empty() const noexcept {
		return  size() == 0;
	}

	// Zwraca liczbę elementów w kolejce o podanym kluczu.
	size_t count(K const& k) const {
		if (!queue_data || !queue_data->map.contains(k)) return 0;
		return queue_data->map.find(k)->second.size();
	}

	// Usuwa wszystkie elementy z kolejki. Złożoność O(n).
	void clear() {
		if (queue_data) {
			exception_guard g(this);
			about_to_modify();
			queue_data->map.clear();
			queue_data->elements.clear();
			g.no_failure();
		}
	}

	// Iterator  pozwalające przeglądać zbiór kluczy w rosnącej kolejności wartości.
	struct k_iterator {
	public:

		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = const K;
		using pointer           = const K*;
		using reference         = const K&;
		using map_iter_t 		= typename
		                          std::map<const K, data_map_value_t>:: const_iterator;

		k_iterator() = default;

		explicit k_iterator(map_iter_t iter) : iter(iter) {}

		// Metody bidirectional const iterator:

		reference operator*() const noexcept {
			return iter->first;
		}

		pointer operator->() const noexcept {
			return &(iter->first);
		}

		k_iterator& operator++() noexcept {
			++iter;
			return *this;
		}

		// int, żeby rozróżnić ++iter od iter++
		k_iterator operator++(int) noexcept {
			k_iterator tmp = *this;
			++(*this);
			return tmp;
		}

		k_iterator& operator--() noexcept {
			--iter;
			return *this;
		}

		k_iterator operator--(int) noexcept {
			k_iterator tmp = *this;
			--(*this);
			return tmp;
		}

		bool operator==(k_iterator const& b) const noexcept {
			return iter == b.iter;
		}

		bool operator!=(k_iterator const& b) const noexcept {
			return iter != b.iter;
		}
	private:
		map_iter_t iter;
	};

	k_iterator k_begin() const noexcept {
		if (!queue_data) return k_iterator();
		return k_iterator(queue_data->map.begin());
	}

	k_iterator k_end() const noexcept {
		if (!queue_data) return k_iterator();
		return k_iterator(queue_data->map.end());
	}
	// Sprawdza czy iterator spełnia koncept:
	static_assert(std::bidirectional_iterator<k_iterator>);
};

#endif // KVFIFO_H