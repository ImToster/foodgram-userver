WITH inserted AS (
    INSERT INTO foodgram.recipe_ingredients (recipe_id, ingredient_id, amount)
    SELECT
        $1,
        t.ingredient_id,
        t.amount
    FROM unnest($2::bigint[], $3::integer[]) AS t(ingredient_id, amount)
    RETURNING recipe_id, ingredient_id, amount
)
SELECT
    i.recipe_id,
    ing.id,
    ing.name,
    ing.measurement_unit,
    i.amount
FROM inserted AS i
JOIN foodgram.ingredients AS ing ON ing.id = i.ingredient_id
ORDER BY ing.id;
