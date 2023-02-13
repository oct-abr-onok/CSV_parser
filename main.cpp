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
	std::ifstream& _file;
	std::tuple<Args...> res;
	bool is_Eof = false;

	template<class T>
	auto string_converter(std::string str)
	{
		std::stringstream buffer;
		T typed_str;
		buffer << str;
		buffer >> typed_str;

		return typed_str;
	}

	std::string str_pop_front(std::vector<std::string>& untyped_data)
	{
		std::string mem = untyped_data.front();
		untyped_data.erase(untyped_data.begin());
		return mem;
	}

public:
	CSVParser(std::ifstream& file, int skip_lines) : _file(file), _skip_lines(skip_lines)
	{
		//пропуск строк
		std::string buffer;
		while (_cur_line < _skip_lines)
		{
			std::getline(_file, buffer);
			_cur_line++;
		}

		res = getString();
	}

	std::vector<std::string> getData()
	{
		std::vector<std::string> res;
		std::string buffer;
		
		std::getline(_file, buffer);

		if (_file.eof())
		{
			is_Eof = true;
		}
		size_t start = 0, end = 0;
		while ((start = buffer.find_first_not_of(',', end)) != std::string::npos)
		{
			end = buffer.find(',', start);
			res.push_back(buffer.substr(start, end - start));
		}

		return res;
	}

	std::tuple<Args...> getString()
	{
		std::vector<std::string> untyped_data = getData();
		if (is_Eof)
		{
			return std::tuple<Args...>();
		}

		if (untyped_data.size() != sizeof...(Args))
			throw std::runtime_error("Invalid string!");

		std::tuple<Args...> test(string_converter<Args>(str_pop_front(untyped_data))...);
		return test; //какие-то траблы(возможно снова баг windows)
	}

	class CSVParseIterator
	{
	private:
		CSVParser<Args...>* object;
	public:
		CSVParseIterator(CSVParser<Args...>* _object = nullptr) : object(_object)
		{
			;
		};
		CSVParseIterator(const CSVParseIterator&) = default;
		CSVParseIterator& operator=(const CSVParseIterator&) = default;
		bool operator==(CSVParseIterator& a) 
		{ 
			return object == a.object; 
		}
		bool operator!=(CSVParseIterator& a) 
		{ 
			return object != a.object; 
		}
		CSVParseIterator& operator++()
		{
			object->res = object->getString();
			if (object->is_Eof)
				object = nullptr;
			return *this;
		}
		std::tuple<Args...> operator* ()
		{
			if (!object->is_Eof)
			{
				return object->res;
			}
		}
	};
	CSVParseIterator begin()
	{
		return CSVParseIterator(this);
	}
	CSVParseIterator end()
	{
		return CSVParseIterator();
	}
};

int main()
{
	//задание 1
	std::tuple<int, int, float, char> test1(1, 0, 4.5, 'L');
	std::cout << test1;

	std::tuple<> test2;
	std::cout << test2;

	std::cout << std::endl;

	//задание 2
	std::ifstream file("test.csv");
	CSVParser<float, int, std::string, float> parser(file, 0 /*skip first lines count*/); //конструктор
	for (std::tuple<float, int, std::string, float> rs : parser) {
		std::cout << rs << std::endl;
	}

	return 0;
}