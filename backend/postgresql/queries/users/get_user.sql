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
        WHERE s.author_id = u.id AND s.subscriber_id = $2
    ) AS is_subscribed
FROM foodgram.users AS u
WHERE u.id = $1;
