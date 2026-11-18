SELECT
    COUNT(*)
FROM foodgram.recipes AS r
WHERE
    ($2::bigint IS NULL OR r.author_id = $2)
    AND (
        $3::boolean IS NULL
        OR EXISTS (
            SELECT 1
            FROM foodgram.favorite_recipes AS f
            WHERE f.recipe_id = r.id AND f.user_id = $1
        ) = $3
    )
    AND (
        $4::boolean IS NULL
        OR EXISTS (
            SELECT 1
            FROM foodgram.purchases AS p
            WHERE p.recipe_id = r.id AND p.user_id = $1
        ) = $4
    );
