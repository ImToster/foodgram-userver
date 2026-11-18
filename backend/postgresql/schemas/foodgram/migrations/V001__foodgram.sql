CREATE SCHEMA IF NOT EXISTS foodgram;

CREATE TABLE IF NOT EXISTS foodgram.users (
    id BIGSERIAL PRIMARY KEY,
    password VARCHAR(128) NOT NULL,
    last_login TIMESTAMPTZ,
    username VARCHAR(150) NOT NULL,
    first_name VARCHAR(150) NOT NULL,
    last_name VARCHAR(150) NOT NULL,
    email VARCHAR(254) NOT NULL,
    date_joined TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    avatar TEXT,
    CONSTRAINT uniq_users_username UNIQUE (username),
    CONSTRAINT uniq_users_email UNIQUE (email)
);

CREATE TABLE IF NOT EXISTS foodgram.subscriptions (
    id BIGSERIAL PRIMARY KEY,
    subscriber_id BIGINT NOT NULL,
    author_id BIGINT NOT NULL,
    CONSTRAINT fk_subscriptions_subscriber FOREIGN KEY (subscriber_id)
        REFERENCES foodgram.users (id) ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    CONSTRAINT fk_subscriptions_author FOREIGN KEY (author_id)
        REFERENCES foodgram.users (id) ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    CONSTRAINT uniq_subscriptions_author_subscriber UNIQUE (author_id, subscriber_id)
);

CREATE INDEX IF NOT EXISTS idx_subscriptions_subscriber
    ON foodgram.subscriptions (subscriber_id);

CREATE TABLE IF NOT EXISTS foodgram.ingredients (
    id BIGSERIAL PRIMARY KEY,
    name VARCHAR(128) NOT NULL,
    measurement_unit VARCHAR(64) NOT NULL,
    CONSTRAINT uniq_ingredients_name UNIQUE (name)
);

CREATE TABLE IF NOT EXISTS foodgram.recipes (
    id BIGSERIAL PRIMARY KEY,
    author_id BIGINT NOT NULL,
    name VARCHAR(256) NOT NULL,
    image VARCHAR(255) NOT NULL,
    text TEXT NOT NULL,
    cooking_time INTEGER NOT NULL,
    CONSTRAINT fk_recipes_author FOREIGN KEY (author_id)
        REFERENCES foodgram.users (id) ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED
);

CREATE INDEX IF NOT EXISTS idx_recipes_author ON foodgram.recipes (author_id);

CREATE TABLE IF NOT EXISTS foodgram.recipe_ingredients (
    id BIGSERIAL PRIMARY KEY,
    recipe_id BIGINT NOT NULL,
    ingredient_id BIGINT NOT NULL,
    amount INTEGER NOT NULL,
    CONSTRAINT fk_recipe_ingredients_recipe FOREIGN KEY (recipe_id)
        REFERENCES foodgram.recipes (id) ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    CONSTRAINT fk_recipe_ingredients_ingredient FOREIGN KEY (ingredient_id)
        REFERENCES foodgram.ingredients (id) ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    CONSTRAINT uniq_recipe_ingredients_recipe_ingredient UNIQUE (recipe_id, ingredient_id)
);

CREATE INDEX IF NOT EXISTS idx_recipe_ingredients_ingredient
    ON foodgram.recipe_ingredients (ingredient_id);

CREATE TABLE IF NOT EXISTS foodgram.favorite_recipes (
    id BIGSERIAL PRIMARY KEY,
    user_id BIGINT NOT NULL,
    recipe_id BIGINT NOT NULL,
    CONSTRAINT fk_favorite_recipes_user FOREIGN KEY (user_id)
        REFERENCES foodgram.users (id) ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    CONSTRAINT fk_favorite_recipes_recipe FOREIGN KEY (recipe_id)
        REFERENCES foodgram.recipes (id) ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    CONSTRAINT uniq_favorite_recipes_user_recipe UNIQUE (user_id, recipe_id)
);

CREATE INDEX IF NOT EXISTS idx_favorite_recipes_recipe
    ON foodgram.favorite_recipes (recipe_id);

CREATE TABLE IF NOT EXISTS foodgram.purchases (
    id BIGSERIAL PRIMARY KEY,
    user_id BIGINT NOT NULL,
    recipe_id BIGINT NOT NULL,
    CONSTRAINT fk_purchases_user FOREIGN KEY (user_id)
        REFERENCES foodgram.users (id) ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    CONSTRAINT fk_purchases_recipe FOREIGN KEY (recipe_id)
        REFERENCES foodgram.recipes (id) ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED,
    CONSTRAINT uniq_purchases_user_recipe UNIQUE (user_id, recipe_id)
);

CREATE INDEX IF NOT EXISTS idx_purchases_recipe ON foodgram.purchases (recipe_id);
