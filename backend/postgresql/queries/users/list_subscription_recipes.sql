SELECT
    recent.id,
    recent.author_id,
    recent.name,
    recent.image,
    recent.cooking_time
FROM unnest($1::bigint[]) AS authors(author_id)
CROSS JOIN LATERAL (
    SELECT
        r.id,
        r.author_id,
        r.name,
        r.image,
        r.cooking_time
    FROM foodgram.recipes AS r
    WHERE r.author_id = authors.author_id
    ORDER BY r.id DESC
    LIMIT $2
) AS recent
ORDER BY recent.author_id, recent.id DESC;
