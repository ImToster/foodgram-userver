SELECT
    u.id,
    u.email,
    u.username,
    u.first_name,
    u.last_name,
    u.avatar,
    EXISTS (
        SELECT 1
        FROM foodgram.subscriptions AS s
        WHERE s.author_id = u.id AND s.subscriber_id = $1
    ) AS is_subscribed
FROM foodgram.users AS u
ORDER BY u.id
LIMIT $2
OFFSET $3;
