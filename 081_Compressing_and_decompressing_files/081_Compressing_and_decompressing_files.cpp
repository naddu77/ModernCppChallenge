// 081_Compressing_and_decompressing_files.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <filesystem>
#include <functional>
#include <algorithm>
#include <fstream>
#include <ZipFile.h>
#include <utils/stream_utils.h>

namespace fs = std::filesystem;

void compress(fs::path const & source,
	fs::path const & archive,
	std::string_view password,
	std::function<void(std::string_view)> reporter)
{
	if (fs::is_regular_file(source))
	{
		if (reporter) reporter("Compressing " + source.string());
		ZipFile::AddEncryptedFile(
			archive.string(),
			source.string(),
			source.filename().string(),
			password.data(),
			LzmaMethod::Create());
	}
	else
	{
		for (auto const & p : fs::recursive_directory_iterator(source))
		{
			if (reporter) reporter("Compressing " + p.path().string());

			if (fs::is_directory(p))
			{
				auto zipArchive = ZipFile::Open(archive.string());
				auto entry = zipArchive->CreateEntry(p.path().string());
				entry->SetAttributes(ZipArchiveEntry::Attributes::Directory);
				ZipFile::SaveAndClose(zipArchive, archive.string());
			}
			else if (fs::is_regular_file(p))
			{
				ZipFile::AddEncryptedFile(
					archive.string(),
					p.path().string(),
					p.path().filename().string(),
					password.data(),
					LzmaMethod::Create());
			}
		}
	}
}

void ensure_directory_exists(fs::path const & dir)
{
	if (!fs::exists(dir))
	{
		std::error_code err;
		fs::create_directories(dir, err);
	}
}

void decompress(fs::path const & destination,
	fs::path const & archive,
	std::string_view password,
	std::function<void(std::string_view)> reporter)
{
	ensure_directory_exists(destination);

	auto zipArchive = ZipFile::Open(archive.string());

	for (size_t i = 0; i < zipArchive->GetEntriesCount(); ++i)
	{
		auto entry = zipArchive->GetEntry(i);
		if (entry)
		{
			auto filepath = destination / fs::path{
			entry->GetFullName() }.relative_path();
			if (reporter) reporter("Creating " + filepath.string());

			if (entry->IsPasswordProtected())
				entry->SetPassword(password.data());

			if (entry->IsDirectory())
			{
				ensure_directory_exists(filepath);
			}
			else
			{
				ensure_directory_exists(filepath.parent_path());
				std::ofstream destFile;
				destFile.open(filepath.string().c_str(),
					std::ios::binary | std::ios::trunc);

				if (!destFile.is_open())
				{
					if (reporter)
						reporter("Cannot create destination file!");
				}

				auto dataStream = entry->GetDecompressionStream();
				if (dataStream)
				{
					utils::stream::copy(*dataStream, destFile);
				}
			}
		}
	}
}

int main()
{
	char option = 0;
	std::cout << "Select [c]ompress/[d]ecompress?";
	std::cin >> option;

	if (option == 'c')
	{
		std::string archivepath;
		std::string inputpath;
		std::string password;
		std::cout << "Enter file or dir to compress:";
		std::cin >> inputpath;
		std::cout << "Enter archive path:";
		std::cin >> archivepath;
		std::cout << "Enter password: ";
		std::cin >> password;

		compress(inputpath, archivepath, password,
			[](std::string_view message) {
			std::cout << message << std::endl; });
	}
	else if (option == 'd')
	{
		std::string archivepath;
		std::string outputpath;
		std::string password;
		std::cout << "Enter dir to decompress:";
		std::cin >> outputpath;
		std::cout << "Enter archive path:";
		std::cin >> archivepath;
		std::cout << "Enter password: ";
		std::cin >> password;

		decompress(outputpath, archivepath, password,
			[](std::string_view message) {
			std::cout << message << std::endl; });
	}
	else
	{
		std::cout << "invalid option" << std::endl;
	}

	return 0;
}