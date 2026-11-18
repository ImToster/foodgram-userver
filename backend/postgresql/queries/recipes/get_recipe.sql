SELECT
    r.id,
    r.name,
    r.image,
    r.text,
    r.cooking_time,
    u.id AS author_id,
    u.email AS author_email,
    u.username AS author_username,
    u.first_name AS author_first_name,
    u.last_name AS author_last_name,
    u.avatar AS author_avatar,
    EXISTS (
        SELECT 1
        FROM foodgram.subscriptions AS s
        WHERE s.author_id = u.id AND s.subscriber_id = $2
    ) AS author_is_subscribed,
    EXISTS (
        SELECT 1
        FROM foodgram.favorite_recipes AS f
        WHERE f.recipe_id = r.id AND f.user_id = $2
    ) AS is_favorited,
    EXISTS (
        SELECT 1
        FROM foodgram.purchases AS p
        WHERE p.recipe_id = r.id AND p.user_id = $2
    ) AS is_in_shopping_cart
FROM foodgram.recipes AS r
JOIN foodgram.users AS u ON u.id = r.author_id
WHERE r.id = $1;
