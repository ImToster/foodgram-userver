cmake_minimum_required(VERSION 3.14...3.31)
project(foodgram CXX C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(DownloadUserver)

find_package(
    userver
    COMPONENTS core postgresql chaotic s3api
    QUIET
)
if(NOT userver_FOUND)
    # Tries TRY_DIR first, falls back to downloading userver from GitHub using CPM.
    download_userver(TRY_DIR third_party/userver)
endif()

userver_setup_environment()

include(FetchContent)

FetchContent_Declare(
    cpp-jwt
    GIT_REPOSITORY https://github.com/arun11299/cpp-jwt.git
    GIT_TAG v1.5.1
    GIT_SHALLOW TRUE
)
set(CPP_JWT_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CPP_JWT_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CPP_JWT_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(cpp-jwt)

FetchContent_Declare(
    pdfgen
    GIT_REPOSITORY https://github.com/AndreRenaud/PDFGen.git
    GIT_TAG 17e72faaa69814ed824359908339c8fd7c852e00
)
FetchContent_MakeAvailable(pdfgen)

add_library(foodgram_pdfgen STATIC "${pdfgen_SOURCE_DIR}/pdfgen.c")
target_include_directories(foodgram_pdfgen SYSTEM PUBLIC "${pdfgen_SOURCE_DIR}")
if(UNIX)
    target_link_libraries(foodgram_pdfgen PUBLIC m)
endif()

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/fonts/LiberationSans-Regular.ttf" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/fonts")

# Common sources
include_directories(src)

add_library(
    ${PROJECT_NAME}_objs OBJECT
    src/auth/auth_bearer.cpp
    src/handlers/auth/login.cpp
    src/handlers/auth/logout.cpp
    src/handlers/common.cpp
    src/handlers/ingredients/get_ingredient.cpp
    src/handlers/ingredients/list_ingredients.cpp
    src/handlers/recipes/add_favorite.cpp
    src/handlers/recipes/add_to_shopping_cart.cpp
    src/handlers/recipes/create_recipe.cpp
    src/handlers/recipes/delete_recipe.cpp
    src/handlers/recipes/download_shopping_cart.cpp
    src/handlers/recipes/get_recipe.cpp
    src/handlers/recipes/ingredient_errors.cpp
    src/handlers/recipes/list_recipes.cpp
    src/handlers/recipes/recipe_data.cpp
    src/handlers/recipes/remove_favorite.cpp
    src/handlers/recipes/remove_from_shopping_cart.cpp
    src/handlers/recipes/update_recipe.cpp
    src/handlers/users/delete_avatar.cpp
    src/handlers/users/get_current_user.cpp
    src/handlers/users/get_user.cpp
    src/handlers/users/list_subscriptions.cpp
    src/handlers/users/list_users.cpp
    src/handlers/users/register_user.cpp
    src/handlers/users/set_avatar.cpp
    src/handlers/users/set_password.cpp
    src/handlers/users/subscribe.cpp
    src/handlers/users/unsubscribe.cpp
    src/storage/media.cpp
    src/utils/image.cpp
    src/utils/jwt.cpp
    src/utils/password.cpp
    src/utils/pdf.cpp
)
target_link_libraries(
    ${PROJECT_NAME}_objs
    PUBLIC userver::core
           userver::postgresql
           userver::s3api
           cpp-jwt
           foodgram_pdfgen
)


# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

userver_target_generate_chaotic(
    ${PROJECT_NAME}-chgen
    LAYOUT "/components/schemas/([^/]*)/=foodgram::schemas::{0}"
    GENERATE_SERIALIZERS
    OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}"
    SCHEMAS "${CMAKE_CURRENT_SOURCE_DIR}/docs/api/openapi-schema.yml"
    RELATIVE_TO "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC ${PROJECT_NAME}-chgen)

userver_add_sql_library(
    ${PROJECT_NAME}_sql
    NAMESPACE foodgram
    OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}"
    DTO_DIALECT postgresql
    MIGRATIONS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/postgresql/schemas/foodgram/migrations"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/postgresql/queries"
    SQL_FILES "*.sql"
    DUMP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/postgresql"
)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC ${PROJECT_NAME}_sql)

# Functional testing
userver_testsuite_add_simple()

# Install
include(GNUInstallDirs)

if(DEFINED ENV{PREFIX})
    message(STATUS "Set install prefix: $ENV{PREFIX}")
    file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
    set(CMAKE_INSTALL_PREFIX "${PREFIX_PATH}")
endif()

file(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT ${PROJECT_NAME}
)
install(
    FILES ${CONFIGS_FILES}
    DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}
    COMPONENT ${PROJECT_NAME}
)
