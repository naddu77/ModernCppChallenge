// 079_Finding_files_in_a_ZIP_archive.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - 저자 코드는 소문자 jpg에서만 동작해서 예제 수행을 위해 JPG로 수정
// - ZipLib Library 연동이 꽤 어려움 (방법을 몰라서 종속되는 프로젝트 다 포함시킴-_-)

#include "pch.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <regex>
#include <ZipFile.h>

namespace fs = std::filesystem;

std::vector<std::string> find_in_archive(
	fs::path const & archivepath,
	std::string_view pattern)
{
	std::vector<std::string> results;

	if (fs::exists(archivepath))
	{
		try
		{
			auto archive = ZipFile::Open(archivepath.string());

			for (size_t i = 0; i < archive->GetEntriesCount(); ++i)
			{
				auto entry = archive->GetEntry(i);
				if (entry)
				{
					if (!entry->IsDirectory())
					{
						auto name = entry->GetName();
						if (std::regex_match(name,
							std::regex{ pattern.data() }))
						{
							results.push_back(entry->GetFullName());
						}
					}
				}
			}
		}
		catch (std::exception const & ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}

	return results;
}

int main()
{
	auto results = find_in_archive("sample79.zip", R"(^.*\.JPG$)");

	for (auto const & name : results)
	{
		std::cout << name << std::endl;
	}

	return 0;
}