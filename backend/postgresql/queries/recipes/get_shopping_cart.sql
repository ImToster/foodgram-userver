SELECT
    i.name,
    i.measurement_unit,
    SUM(ri.amount)::integer AS amount
FROM foodgram.purchases AS p
JOIN foodgram.recipe_ingredients AS ri ON ri.recipe_id = p.recipe_id
JOIN foodgram.ingredients AS i ON i.id = ri.ingredient_id
WHERE p.user_id = $1
GROUP BY i.id, i.name, i.measurement_unit
ORDER BY i.name;
