// 074_Selecting_data_from_XML_using_XPath.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <pugixml.hpp>

pugi::xml_document doc;

int main()
{
	std::string text = R"(
		<?xml version="1.0"?>
		<movies>
		  <movie id="11001" title="The Matrix" year="1999" length="196">
			<cast>
			  <role star="Keanu Reeves" name="Neo" />
			  <role star="Laurence Fishburne" name="Morpheus" />
			  <role star="Carrie-Anne Moss" name="Trinity" />
			  <role star="Hugo Weaving" name=" Agent Smith" />
			</cast>
			<directors>
			  <director name="Lana Wachowski" />
			  <director name="Lilly Wachowski" />
			</directors>
			<writers>
			  <writer name="Lana Wachowski" />
			  <writer name="Lilly Wachowski" />
			</writers>
		  </movie>
		  <movie id="9871" title="Forrest Gump" year="1994" length="202">
			<cast>
			  <role star="Tom Hanks" name="Forrest Gump" />
			  <role star="Sally Field" name="Mrs. Gump" />
			  <role star="Robin Wright" name="Jenny Curran" />
			  <role star="Mykelti Williamson" name="Bubba Blue" />
			</cast>
			<directors>
			  <director name="Robert Zemeckis" />
			</directors>
			<writers>
			  <writer name="Winston Groom" />
			  <writer name="Eric Roth" />
			</writers>
		  </movie>
		</movies>
	)";

	if (doc.load_string(text.c_str()))
	{
		try
		{
			auto titles = doc.select_nodes("/movies/movie[@year>1995]");

			for (auto it : titles)
			{
				std::cout << it.node().attribute("title").as_string()
					<< std::endl;
			}
		}
		catch (pugi::xpath_exception const & e)
		{
			std::cout << e.result().description() << std::endl;
		}

		try
		{
			auto titles = doc.select_nodes("/movies/movie/cast/role[last()]");

			for (auto it : titles)
			{
				std::cout << it.node().attribute("star").as_string()
					<< std::endl;
			}
		}
		catch (pugi::xpath_exception const & e)
		{
			std::cout << e.result().description() << std::endl;
		}
	}

	return 0;
}