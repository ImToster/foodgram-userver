#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component_list.hpp>
#include <userver/components/component.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/congestion_control/component.hpp>
#include <userver/server/handlers/implicit_options.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/server/middlewares/cors.hpp>
#include <userver/testsuite/testsuite_support.hpp>

#include <userver/storages/postgres/component.hpp>

#include <userver/utils/daemon_run.hpp>

#include <auth/auth_bearer.hpp>
#include <handlers/auth/login.hpp>
#include <handlers/auth/logout.hpp>
#include <handlers/ingredients/get_ingredient.hpp>
#include <handlers/ingredients/list_ingredients.hpp>
#include <handlers/recipes/add_favorite.hpp>
#include <handlers/recipes/add_to_shopping_cart.hpp>
#include <handlers/recipes/create_recipe.hpp>
#include <handlers/recipes/delete_recipe.hpp>
#include <handlers/recipes/download_shopping_cart.hpp>
#include <handlers/recipes/get_recipe.hpp>
#include <handlers/recipes/list_recipes.hpp>
#include <handlers/recipes/remove_favorite.hpp>
#include <handlers/recipes/remove_from_shopping_cart.hpp>
#include <handlers/recipes/update_recipe.hpp>
#include <handlers/users/delete_avatar.hpp>
#include <handlers/users/get_current_user.hpp>
#include <handlers/users/get_user.hpp>
#include <handlers/users/list_subscriptions.hpp>
#include <handlers/users/list_users.hpp>
#include <handlers/users/register_user.hpp>
#include <handlers/users/set_avatar.hpp>
#include <handlers/users/set_password.hpp>
#include <handlers/users/subscribe.hpp>
#include <handlers/users/unsubscribe.hpp>
#include <storage/media.hpp>

int main(int argc, char* argv[]) {
    userver::server::handlers::auth::RegisterAuthCheckerFactory<foodgram::auth::CheckerFactory>();

    auto component_list =
        userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::server::handlers::ImplicitOptions>()
            .Append<userver::server::middlewares::CorsFactory>()
            .Append<userver::components::TestsuiteSupport>()
            .AppendComponentList(userver::clients::http::ComponentList())
            .Append<userver::clients::dns::Component>()
            .Append<userver::server::handlers::TestsControl>()
            .Append<userver::congestion_control::Component>()
            .Append<userver::components::Postgres>("postgres-db-1")
            .Append<foodgram::storage::Media>()
            .Append<foodgram::handlers::users::ListUsers>()
            .Append<foodgram::handlers::users::RegisterUser>()
            .Append<foodgram::handlers::users::GetUser>()
            .Append<foodgram::handlers::users::GetCurrentUser>()
            .Append<foodgram::handlers::users::SetAvatar>()
            .Append<foodgram::handlers::users::DeleteAvatar>()
            .Append<foodgram::handlers::users::ListSubscriptions>()
            .Append<foodgram::handlers::users::Subscribe>()
            .Append<foodgram::handlers::users::Unsubscribe>()
            .Append<foodgram::handlers::users::SetPassword>()
            .Append<foodgram::handlers::auth::Login>()
            .Append<foodgram::handlers::auth::Logout>()
            .Append<foodgram::handlers::recipes::ListRecipes>()
            .Append<foodgram::handlers::recipes::CreateRecipe>()
            .Append<foodgram::handlers::recipes::GetRecipe>()
            .Append<foodgram::handlers::recipes::UpdateRecipe>()
            .Append<foodgram::handlers::recipes::DeleteRecipe>()
            .Append<foodgram::handlers::recipes::DownloadShoppingCart>()
            .Append<foodgram::handlers::recipes::AddFavorite>()
            .Append<foodgram::handlers::recipes::RemoveFavorite>()
            .Append<foodgram::handlers::recipes::AddToShoppingCart>()
            .Append<foodgram::handlers::recipes::RemoveFromShoppingCart>()
            .Append<foodgram::handlers::ingredients::ListIngredients>()
            .Append<foodgram::handlers::ingredients::GetIngredient>();

    return userver::utils::DaemonMain(argc, argv, component_list);
}
