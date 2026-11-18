SELECT
    id,
    email,
    username,
    first_name,
    last_name,
    avatar,
    password
FROM foodgram.users
WHERE email = $1;
