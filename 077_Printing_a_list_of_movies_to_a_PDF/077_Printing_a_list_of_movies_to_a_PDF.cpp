// 077_Printing_a_list_of_movies_to_a_PDF.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
// Note
// - PDFWriter Library 연동이 꽤 어렵다-_-a

#include "pch.h"
#include <iostream>
#include <vector>
#include <string_view>
#include <iomanip>
#include <PDFWriter/PDFWriter.h>
#include <PDFWriter/AbstractContentContext.h>
#include <PDFWriter/PageContentContext.h>
#include <PDFWriter/PDFPage.h>
#include <PDFWriter/PDFUsedFont.h>

#ifdef _WIN32
static const std::string fonts_dir = R"(c:\windows\fonts\)";
#elif defined (__APPLE__)
static const std::string fonts_dir = R"(/Library/Fonts/)";
#else
static const std::string fonts_dir = R"(/usr/share/fonts)";
#endif

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

void print_pdf(movie_list const & movies,
	std::string_view path)
{
	const int height = 842;
	const int width = 595;
	const int left = 60;
	const int top = 770;
	const int right = 535;
	const int bottom = 60;
	const int line_height = 28;

	PDFWriter pdf;
	pdf.StartPDF(path.data(), ePDFVersion13);
	auto font = pdf.GetFontForFile(fonts_dir + "arial.ttf");

	AbstractContentContext::GraphicOptions pathStrokeOptions(
		AbstractContentContext::eStroke,
		AbstractContentContext::eRGB,
		0xff000000,
		1);

	PDFPage* page = nullptr;
	PageContentContext* context = nullptr;
	int index = 0;
	for (size_t i = 0; i < movies.size(); ++i)
	{
		index = i % 25;
		if (index == 0)
		{
			if (page != nullptr)
			{
				DoubleAndDoublePairList pathPoints;
				pathPoints.push_back(DoubleAndDoublePair(left, bottom));
				pathPoints.push_back(DoubleAndDoublePair(right, bottom));
				context->DrawPath(pathPoints, pathStrokeOptions);

				pdf.EndPageContentContext(context);
				pdf.WritePageAndRelease(page);
			}

			page = new PDFPage();
			page->SetMediaBox(PDFRectangle(0, 0, width, height));
			context = pdf.StartPageContentContext(page);

			{
				DoubleAndDoublePairList pathPoints;
				pathPoints.push_back(DoubleAndDoublePair(left, top));
				pathPoints.push_back(DoubleAndDoublePair(right, top));
				context->DrawPath(pathPoints, pathStrokeOptions);
			}
		}

		if (i == 0)
		{
			AbstractContentContext::TextOptions const textOptions(
				font, 26, AbstractContentContext::eGray, 0);

			context->WriteText(left, top + 15,
				"List of movies", textOptions);
		}

		auto textw = 0;
		{
			AbstractContentContext::TextOptions const textOptions(
				font, 20, AbstractContentContext::eGray, 0);

			context->WriteText(left, top - 20 - line_height * index,
				movies[i].title, textOptions);
			auto textDimensions = font->CalculateTextDimensions(
				movies[i].title, 20);
			textw = textDimensions.width;
		}

		{
			AbstractContentContext::TextOptions const textOptions(
				font, 16, AbstractContentContext::eGray, 0);

			context->WriteText(left + textw + 5,
				top - 20 - line_height * index,
				" (" + std::to_string(movies[i].year) + ")",
				textOptions);

			std::stringstream s;
			s << movies[i].length / 60 << ':' << std::setw(2)
				<< std::setfill('0') << movies[i].length % 60;

			context->WriteText(right - 30, top - 20 - line_height * index,
				s.str(),
				textOptions);
		}
	}

	DoubleAndDoublePairList pathPoints;
	pathPoints.push_back(
		DoubleAndDoublePair(left, top - line_height * (index + 1)));
	pathPoints.push_back(
		DoubleAndDoublePair(right, top - line_height * (index + 1)));
	context->DrawPath(pathPoints, pathStrokeOptions);

	if (page != nullptr)
	{
		pdf.EndPageContentContext(context);
		pdf.WritePageAndRelease(page);
	}

	pdf.EndPDF();
}

int main()
{
	movie_list movies
	{
	   { 1, "The Matrix", 1999, 136},
	   { 2, "Forrest Gump", 1994, 142},
	   // .. other movies
	   { 28, "L.A. Confidential", 1997, 138},
	   { 29, "Shutter Island", 2010, 138},
	};

	print_pdf(movies, "movies.pdf");
}