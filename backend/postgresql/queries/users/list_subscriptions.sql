SELECT
    u.id,
    u.email,
    u.username,
    u.first_name,
    u.last_name,
    u.avatar,
    TRUE AS is_subscribed,
    (
        SELECT COUNT(*)
        FROM foodgram.recipes AS r
        WHERE r.author_id = u.id
    ) AS recipes_count
FROM foodgram.subscriptions AS s
JOIN foodgram.users AS u ON u.id = s.author_id
WHERE s.subscriber_id = $1
ORDER BY u.id
LIMIT $2
OFFSET $3;
