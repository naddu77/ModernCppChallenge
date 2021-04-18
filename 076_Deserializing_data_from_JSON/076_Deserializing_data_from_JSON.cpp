// 076_Deserializing_data_from_JSON.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <cassert>
#include <string_view>
#include <fstream>
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

movie_list deserialize(std::string_view filepath)
{
	movie_list movies;

	std::ifstream ifile(filepath.data());
	if (ifile.is_open())
	{
		json jdata;

		try
		{
			ifile >> jdata;

			if (jdata.is_object())
			{
				for (auto & element : jdata.at("movies"))
				{
					movie m;

					m.id = element.at("id").get<unsigned int>();
					m.title = element.at("title").get<std::string>();
					m.year = element.at("year").get<unsigned int>();
					m.length = element.at("length").get<unsigned int>();

					for (auto & role : element.at("cast"))
					{
						m.cast.push_back(casting_role{
						   role.at("star").get<std::string>(),
						   role.at("name").get<std::string>() });
					}

					for (auto & director : element.at("directors"))
					{
						m.directors.push_back(director);
					}

					for (auto & writer : element.at("writers"))
					{
						m.writers.push_back(writer);
					}

					movies.push_back(m);
				}
			}
		}
		catch (std::exception const & ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}

	return movies;
}

int main()
{
	auto movies = deserialize("movies.json");

	assert(movies.size() == 2);
	assert(movies[0].title == "The Matrix");
	assert(movies[1].title == "Forrest Gump");

	return 0;
}