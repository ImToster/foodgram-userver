DELETE FROM foodgram.subscriptions
WHERE subscriber_id = $1 AND author_id = $2;
