const normalizeIngredients = (ingredients = []) =>
  ingredients.map(({ id, amount }) => ({
    id: Number(id),
    amount: Number(amount),
  }));

const ERROR_MESSAGES = {
  invalid_credentials: "Неверный email или пароль",
  email_taken: "Пользователь с таким email уже существует",
  username_taken: "Это имя пользователя уже занято",
  user_exists: "Пользователь уже существует",
  invalid_current_password: "Неверный текущий пароль",
  cannot_subscribe_to_self: "Нельзя подписаться на себя",
  subscription_exists: "Вы уже подписаны на этого пользователя",
  cannot_unsubscribe_from_self: "Нельзя отписаться от себя",
  subscription_not_exists: "Вы не подписаны на этого пользователя",
  recipe_already_favorited: "Рецепт уже в избранном",
  recipe_not_favorited: "Рецепт не в избранном",
  recipe_already_in_cart: "Рецепт уже в списке покупок",
  recipe_not_in_cart: "Рецепта нет в списке покупок",
  shopping_cart_empty: "Список покупок пуст",
  ingredient_not_found: "Ингредиент не найден",
  duplicate_ingredients: "Ингредиенты не должны повторяться",
  invalid_image: "Некорректное изображение",
  unsupported_image_format: "Неподдерживаемый формат изображения",
  unauthorized: "Требуется авторизация",
  forbidden: "Недостаточно прав",
  not_found: "Не найдено",
  bad_request: "Некорректный запрос",
  client_error: "Ошибка запроса",
  internal_server_error: "Внутренняя ошибка сервера",
};

class Api {
  constructor(url, headers) {
    this._url = url;
    this._headers = headers;
  }

  checkResponse(res) {
    if (res.status === 204) {
      return Promise.resolve(res);
    }
    return res
      .json()
      .catch(() => null)
      .then((data) => {
        if (res.status < 400) {
          return data;
        }
        if (res.status === 401 && data?.code === "unauthorized") {
          localStorage.removeItem("token");
        }
        if (data?.code === "validation_error" || (data?.code && !ERROR_MESSAGES[data.code])) {
          console.error("API error:", data);
        }
        throw data && typeof data === "object"
          ? data
          : { code: "unknown", message: res.statusText || `HTTP ${res.status}` };
      });
  }

  checkFileDownloadResponse(res) {
    if (res.status >= 400) {
      return res
        .json()
        .catch(() => null)
        .then((data) => {
          if (res.status === 401 && data?.code === "unauthorized") {
            localStorage.removeItem("token");
          }
          if (data?.code && !ERROR_MESSAGES[data.code]) {
            console.error("API error:", data);
          }
          throw data && typeof data === "object"
            ? data
            : { code: "unknown", message: res.statusText || `HTTP ${res.status}` };
        });
    }

    return res.blob().then((blob) => {
      const url = window.URL.createObjectURL(blob);
      const a = document.createElement("a");
      a.href = url;
      a.download = "shopping-list.pdf";
      document.body.appendChild(a); // we need to append the element to the dom -> otherwise it will not work in firefox
      a.click();
      a.remove(); //afterwards we remove the element again
      window.setTimeout(() => window.URL.revokeObjectURL(url), 0);
    });
  }

  signin({ email, password }) {
    return fetch("/api/auth/token/login/", {
      method: "POST",
      headers: this._headers,
      body: JSON.stringify({
        email,
        password,
      }),
    }).then(this.checkResponse);
  }

  signout() {
    const token = localStorage.getItem("token");
    return fetch("/api/auth/token/logout/", {
      method: "POST",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
    }).then(this.checkResponse);
  }

  signup({ email, password, username, first_name, last_name }) {
    return fetch(`/api/users/`, {
      method: "POST",
      headers: this._headers,
      body: JSON.stringify({
        email,
        password,
        username,
        first_name,
        last_name,
      }),
    }).then(this.checkResponse);
  }

  getUserData() {
    const token = localStorage.getItem("token");
    return fetch(`/api/users/me/`, {
      method: "GET",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
    }).then(this.checkResponse);
  }

  changePassword({ current_password, new_password }) {
    const token = localStorage.getItem("token");
    return fetch(`/api/users/set_password/`, {
      method: "POST",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
      body: JSON.stringify({ current_password, new_password }),
    }).then(this.checkResponse);
  }

  changeAvatar({ file }) {
    const token = localStorage.getItem("token");
    return fetch(`/api/users/me/avatar/`, {
      method: "PUT",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
      body: JSON.stringify({ avatar: file }),
    }).then(this.checkResponse);
  }

  deleteAvatar() {
    const token = localStorage.getItem("token");
    return fetch(`/api/users/me/avatar/`, {
      method: "DELETE",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
    }).then(this.checkResponse);
  }

  // recipes

  getRecipes({
    page = 1,
    limit = 6,
    is_favorited = 0,
    is_in_shopping_cart = 0,
    author
  } = {}) {
    const token = localStorage.getItem("token");
    const authorization = token ? { authorization: `Bearer ${token}` } : {};
    return fetch(
      `/api/recipes/?page=${page}&limit=${limit}${
        author ? `&author=${author}` : ""
      }${is_favorited ? `&is_favorited=${is_favorited}` : ""}${
        is_in_shopping_cart ? `&is_in_shopping_cart=${is_in_shopping_cart}` : ""
      }`,
      {
        method: "GET",
        headers: {
          ...this._headers,
          ...authorization,
        },
      }
    ).then(this.checkResponse);
  }

  getRecipe({ recipe_id }) {
    const token = localStorage.getItem("token");
    const authorization = token ? { authorization: `Bearer ${token}` } : {};
    return fetch(`/api/recipes/${recipe_id}/`, {
      method: "GET",
      headers: {
        ...this._headers,
        ...authorization,
      },
    }).then(this.checkResponse);
  }

  createRecipe({
    name = "",
    image,
    cooking_time = 0,
    text = "",
    ingredients = [],
  }) {
    const token = localStorage.getItem("token");
    return fetch("/api/recipes/", {
      method: "POST",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
      body: JSON.stringify({
        name,
        image,
        cooking_time: Number(cooking_time),
        text,
        ingredients: normalizeIngredients(ingredients),
      }),
    }).then(this.checkResponse);
  }

  updateRecipe(
    { name, recipe_id, image, cooking_time, text, ingredients },
    wasImageUpdated
  ) {
    // image was changed
    const token = localStorage.getItem("token");
    return fetch(`/api/recipes/${recipe_id}/`, {
      method: "PATCH",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
      body: JSON.stringify({
        name,
        image: wasImageUpdated ? image : undefined,
        cooking_time: Number(cooking_time),
        text,
        ingredients: ingredients ? normalizeIngredients(ingredients) : undefined,
      }),
    }).then(this.checkResponse);
  }

  addToFavorites({ id }) {
    const token = localStorage.getItem("token");
    return fetch(`/api/recipes/${id}/favorite/`, {
      method: "POST",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
    }).then(this.checkResponse);
  }

  removeFromFavorites({ id }) {
    const token = localStorage.getItem("token");
    return fetch(`/api/recipes/${id}/favorite/`, {
      method: "DELETE",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
    }).then(this.checkResponse);
  }

  getUser({ id }) {
    const token = localStorage.getItem("token");
    const authorization = token ? { authorization: `Bearer ${token}` } : {};
    return fetch(`/api/users/${id}/`, {
      method: "GET",
      headers: {
        ...this._headers,
        ...authorization,
      },
    }).then(this.checkResponse);
  }

  getUsers({ page = 1, limit = 6 }) {
    const token = localStorage.getItem("token");
    return fetch(`/api/users/?page=${page}&limit=${limit}`, {
      method: "GET",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
    }).then(this.checkResponse);
  }

  // subscriptions

  getSubscriptions({ page, limit = 6, recipes_limit = 3 }) {
    const token = localStorage.getItem("token");
    return fetch(
      `/api/users/subscriptions/?page=${page}&limit=${limit}&recipes_limit=${recipes_limit}`,
      {
        method: "GET",
        headers: {
          ...this._headers,
          authorization: `Bearer ${token}`,
        },
      }
    ).then(this.checkResponse);
  }

  deleteSubscriptions({ author_id }) {
    const token = localStorage.getItem("token");
    return fetch(`/api/users/${author_id}/subscribe/`, {
      method: "DELETE",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
    }).then(this.checkResponse);
  }

  subscribe({ author_id }) {
    const token = localStorage.getItem("token");
    return fetch(`/api/users/${author_id}/subscribe/`, {
      method: "POST",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
    }).then(this.checkResponse);
  }

  // ingredients
  getIngredients({ name }) {
    const token = localStorage.getItem("token");
    return fetch(`/api/ingredients/?name=${encodeURIComponent(name)}`, {
      method: "GET",
      headers: {
        ...this._headers,
      },
    }).then(this.checkResponse);
  }


  addToOrders({ id }) {
    const token = localStorage.getItem("token");
    return fetch(`/api/recipes/${id}/shopping_cart/`, {
      method: "POST",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
    }).then(this.checkResponse);
  }

  removeFromOrders({ id }) {
    const token = localStorage.getItem("token");
    return fetch(`/api/recipes/${id}/shopping_cart/`, {
      method: "DELETE",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
    }).then(this.checkResponse);
  }

  deleteRecipe({ recipe_id }) {
    const token = localStorage.getItem("token");
    return fetch(`/api/recipes/${recipe_id}/`, {
      method: "DELETE",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
    }).then(this.checkResponse);
  }

  downloadFile() {
    const token = localStorage.getItem("token");
    return fetch(`/api/recipes/download_shopping_cart/`, {
      method: "GET",
      headers: {
        ...this._headers,
        authorization: `Bearer ${token}`,
      },
    }).then(this.checkFileDownloadResponse);
  }
}

export const getErrorMessage = (err) => {
  if (!err) {
    return "Что-то пошло не так";
  }
  if (typeof err === "string") {
    return err;
  }
  if (err.code === "validation_error") {
    return "Проверьте заполнение полей";
  }
  return ERROR_MESSAGES[err.code] || "Что-то пошло не так";
};

export const isUnauthorized = (err) => err?.code === "unauthorized";

export default new Api(process.env.API_URL || "http://localhost", {
  "content-type": "application/json",
});
