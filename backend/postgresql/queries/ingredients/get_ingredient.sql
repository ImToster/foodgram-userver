SELECT
    id,
    name,
    measurement_unit
FROM foodgram.ingredients
WHERE id = $1;
