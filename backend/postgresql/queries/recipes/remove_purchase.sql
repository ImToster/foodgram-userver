DELETE FROM foodgram.purchases
WHERE user_id = $1 AND recipe_id = $2;
