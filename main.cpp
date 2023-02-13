#include "main.hpp"

//задание 1
template<class Ch, class Tr, std::size_t I, class... Args>
class t_print
{
public:
	static void print(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
	{
		t_print<Ch, Tr, I - 1, Args...>::print(os, t);
		os << " " << std::get<I>(t);
	}
};

//самая глубокая точка рекурсии
template<class Ch, class Tr, class... Args>
class t_print<Ch, Tr, 0, Args...>
{
public:
	static void print(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
	{
		os << " " << std::get<0>(t);
	}
};

//вариант для случая с пустым кортежем
template<class Ch, class Tr, class... Args>
class t_print<Ch, Tr, -1, Args...>
{
public:
	static void print(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
	{
		;
	}
};

template<class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t)
{
	t_print<Ch, Tr, sizeof...(Args) - 1, Args...>::print(os, t);
	return os;
}

//задание 2
template <class... Args>
class CSVParser
{
private:
	int _skip_lines;
	int _cur_line = 0;
	const std::ifstream& _file;
	std::tuple<Args...> res;
 
public:
	CSVParser(std::ifstream& file, int skip_lines) : _file(file)
	{
		//пропуск строк
		_skip_lines = skip_lines;
		std::string buffer;
		while (_cur_line < _skip_lines)
		{
			std::getline(_file, buffer);
			_cur_line++;
		}


	}

};

int main()
{
	//задание 1
	std::tuple<int, int, float, char> test1(1, 0, 4.5, 'L');
	std::cout << test1;

	std::tuple<> test2;
	std::cout << test2;

	//задание 2
	std::ifstream file("test.csv");
	CSVParser<int, std::string> parser(file, 0 /*skip first lines count*/); //конструктор
	for (std::tuple<int, std::string> rs : parser) {
		std::cout << rs << std::endl;
	}

	return 0;
}