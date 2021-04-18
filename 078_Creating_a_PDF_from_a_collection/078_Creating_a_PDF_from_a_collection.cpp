// 078_Creating_a_PDF_from_a_collection.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - 저자 코드는 대문자 JPG 확장자는 처리 안함
// - 저자 코드는 jpg 파일을 하나도 못 찾았을 경우 pdf 파일에 오류있다고 나옴 (Range-v3로 해결)

#include "pch.h"
#include <iostream>
#include <filesystem>
#include <cctype>
#include <PDFWriter.h>
#include <PDFPage.h>
#include <PageContentContext.h>
#include <range/v3/all.hpp>

namespace fs = std::filesystem;

std::vector<std::string> get_images(fs::path const & dirpath)
{
	std::vector<std::string> paths;

	for (auto const & p : fs::directory_iterator(dirpath))
	{
		std::string extension = p.path().extension().string();

		std::string lowercase = extension
			| ranges::view::transform(std::tolower)
			;

		if (lowercase == ".jpg")
			paths.push_back(p.path().string());
	}

	return paths;
}

void print_pdf(fs::path const & pdfpath,
	fs::path const & dirpath)
{
	const int height = 842;
	const int width = 595;
	const int margin = 20;

	auto image_paths = get_images(dirpath);

	PDFWriter pdf;
	pdf.StartPDF(pdfpath.string(), ePDFVersion13);

	PDFPage* page = nullptr;
	PageContentContext* context = nullptr;

	auto top = height - margin;
	for (size_t i = 0; i < image_paths.size(); ++i)
	{
		auto dims = pdf.GetImageDimensions(image_paths[i]);

		if (i == 0 || top - dims.second < margin)
		{
			if (page != nullptr)
			{
				pdf.EndPageContentContext(context);
				pdf.WritePageAndRelease(page);
			}

			page = new PDFPage();
			page->SetMediaBox(PDFRectangle(0, 0, width, height));
			context = pdf.StartPageContentContext(page);

			top = height - margin;
		}

		context->DrawImage(margin, top - dims.second, image_paths[i]);

		top -= static_cast<int>(dims.second) + margin;
	}

	if (page != nullptr)
	{
		pdf.EndPageContentContext(context);
		pdf.WritePageAndRelease(page);
	}

	pdf.EndPDF();
}

int main()
{
	print_pdf("sample.pdf", "res");

	return 0;
}