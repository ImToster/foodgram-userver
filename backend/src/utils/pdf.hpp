#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace foodgram::utils {

struct ShoppingListIngredient {
    std::string name;
    std::string measurement_unit;
    std::int32_t amount{};
};

struct PdfFont {
    std::string path;
    std::string data;
};

// Reads the font file once so that rendering does not touch the filesystem on
// every request.
PdfFont LoadPdfFont(std::string path);

std::string RenderShoppingListPdf(
    const std::vector<ShoppingListIngredient>& ingredients,
    const PdfFont& font
);

}  // namespace foodgram::utils
