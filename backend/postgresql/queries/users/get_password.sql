SELECT
    password
FROM foodgram.users
WHERE id = $1;
