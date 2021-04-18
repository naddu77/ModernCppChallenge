// 075_Serializing_data_to_JSON.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <string_view>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>

struct casting_role
{
	std::string actor;
	std::string role;
};

struct movie
{
	unsigned int              id;
	std::string               title;
	unsigned int              year;
	unsigned int              length;
	std::vector<casting_role> cast;
	std::vector<std::string>  directors;
	std::vector<std::string>  writers;
};

using movie_list = std::vector<movie>;

using json = nlohmann::json;

void to_json(json& j, casting_role const & c)
{
	j = json{ {"star", c.actor}, {"name", c.role} };
}

void to_json(json& j, movie const & m)
{
	j = json::object({
	   {"id", m.id},
	   {"title", m.title},
	   {"year", m.year},
	   {"length", m.length},
	   {"cast", m.cast },
	   {"directors", m.directors},
	   {"writers", m.writers}
		});
}

void serialize(movie_list const & movies, std::string_view filepath)
{
	json jdata{ { "movies", movies } };

	std::ofstream ofile(filepath.data());
	if (ofile.is_open())
	{
		ofile << std::setw(2) << jdata << std::endl;
	}
}

int main()
{
	movie_list movies
	{
	   {
		  11001, "The Matrix", 1999, 196,
		  { {"Keanu Reeves", "Neo"},
			{"Laurence Fishburne", "Morpheus"},
			{"Carrie-Anne Moss", "Trinity"},
			{"Hugo Weaving", "Agent Smith"} },
		  {"Lana Wachowski", "Lilly Wachowski"},
		  {"Lana Wachowski", "Lilly Wachowski"},
	   },
	   {
		  9871, "Forrest Gump", 1994, 202,
		  { {"Tom Hanks", "Forrest Gump"},
			{"Sally Field", "Mrs. Gump"},
			{"Robin Wright","Jenny Curran"},
			{"Mykelti Williamson","Bubba Blue"} },
		  {"Robert Zemeckis"},
		  {"Winston Groom", "Eric Roth"},
	   }
	};

	serialize(movies, "movies.json");

	return 0;
}