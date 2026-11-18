SELECT
    id,
    name,
    measurement_unit
FROM foodgram.ingredients
WHERE $1::text IS NULL OR name ILIKE $1 || '%'
ORDER BY name;
