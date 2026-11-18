SELECT
    ri.recipe_id,
    i.id,
    i.name,
    i.measurement_unit,
    ri.amount
FROM foodgram.recipe_ingredients AS ri
JOIN foodgram.ingredients AS i ON i.id = ri.ingredient_id
WHERE ri.recipe_id = ANY($1::bigint[])
ORDER BY ri.recipe_id DESC, i.id;
