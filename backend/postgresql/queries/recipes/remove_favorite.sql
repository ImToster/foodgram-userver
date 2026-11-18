DELETE FROM foodgram.favorite_recipes
WHERE user_id = $1 AND recipe_id = $2;
