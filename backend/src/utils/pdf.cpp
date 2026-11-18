#include "utils/pdf.hpp"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>

#include <fmt/format.h>
#include <pdfgen.h>

namespace foodgram::utils {
namespace {

constexpr float kFontSize = 14.0f;
constexpr float kLineHeight = 20.0f;
constexpr float kTitleX = 50.0f;
constexpr float kItemX = 70.0f;
constexpr float kTopMargin = 50.0f;
constexpr float kBottomMargin = 50.0f;

class PdfDocument final {
public:
    PdfDocument() {
        pdf_info info{};
        std::snprintf(info.creator, sizeof(info.creator), "Foodgram");
        std::snprintf(info.producer, sizeof(info.producer), "Foodgram");
        std::snprintf(info.title, sizeof(info.title), "Shopping list");

        doc_ = pdf_create(PDF_A4_WIDTH, PDF_A4_HEIGHT, &info);
        if (!doc_) {
            throw std::runtime_error("Failed to create PDF document");
        }
    }

    ~PdfDocument() { pdf_destroy(doc_); }

    PdfDocument(const PdfDocument&) = delete;
    PdfDocument& operator=(const PdfDocument&) = delete;

    pdf_doc* Get() const { return doc_; }

private:
    pdf_doc* doc_{};
};

pdf_object* AppendPage(pdf_doc* doc) {
    auto* page = pdf_append_page(doc);
    if (!page) {
        throw std::runtime_error("Failed to add a page to PDF document");
    }
    return page;
}

void AddText(pdf_doc* doc, pdf_object* page, const std::string& text, float x, float y) {
    if (pdf_add_text(doc, page, text.c_str(), kFontSize, x, y, PDF_BLACK) < 0) {
        throw std::runtime_error("Failed to add text to PDF document");
    }
}

void SetFont(pdf_doc* doc, const PdfFont& font) {
    // fmemopen does not modify the buffer in read mode, so sharing the cached
    // font bytes between requests is safe.
    std::FILE* stream = fmemopen(const_cast<char*>(font.data.data()), font.data.size(), "rb");
    if (!stream) {
        throw std::runtime_error("Failed to open PDF font stream: " + font.path);
    }

    const char* result = pdf_set_font_ttf_file(doc, stream, font.path.c_str());
    std::fclose(stream);

    if (!result) {
        throw std::runtime_error("Failed to load PDF font: " + font.path);
    }
}

}  // namespace

PdfFont LoadPdfFont(std::string path) {
    std::ifstream file{path, std::ios::binary};
    if (!file) {
        throw std::runtime_error("Failed to open PDF font: " + path);
    }

    std::string data{
        std::istreambuf_iterator<char>{file},
        std::istreambuf_iterator<char>{}
    };
    if (data.empty()) {
        throw std::runtime_error("PDF font is empty: " + path);
    }

    return PdfFont{std::move(path), std::move(data)};
}

std::string RenderShoppingListPdf(
    const std::vector<ShoppingListIngredient>& ingredients,
    const PdfFont& font
) {
    PdfDocument pdf;
    SetFont(pdf.Get(), font);

    auto* page = AppendPage(pdf.Get());
    float y = pdf_page_height(page) - kTopMargin;

    AddText(pdf.Get(), page, "Список ингредиентов:", kTitleX, y);
    y -= kLineHeight;

    std::size_t index = 1;
    for (const auto& ingredient : ingredients) {
        if (y < kBottomMargin) {
            page = AppendPage(pdf.Get());
            y = pdf_page_height(page) - kTopMargin;
        }

        const auto line = fmt::format(
            "{}. {} ({}) - {}",
            index++,
            ingredient.name,
            ingredient.measurement_unit,
            ingredient.amount
        );
        AddText(pdf.Get(), page, line, kItemX, y);
        y -= kLineHeight;
    }

    char* buffer = nullptr;
    std::size_t size = 0;
    std::FILE* stream = open_memstream(&buffer, &size);
    if (!stream) {
        throw std::runtime_error("Failed to allocate PDF buffer");
    }

    const int save_result = pdf_save_file(pdf.Get(), stream);
    std::fclose(stream);

    std::string data;
    if (buffer) {
        data.assign(buffer, size);
        std::free(buffer);
    }

    if (save_result < 0) {
        throw std::runtime_error("Failed to save PDF document");
    }

    return data;
}

}  // namespace foodgram::utils
