SELECT
    COUNT(*)
FROM foodgram.subscriptions
WHERE subscriber_id = $1;
