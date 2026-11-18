INSERT INTO foodgram.users (email, username, first_name, last_name, password)
VALUES ($1, $2, $3, $4, $5)
RETURNING
    id,
    email,
    username,
    first_name,
    last_name;
