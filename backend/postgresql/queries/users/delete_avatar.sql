UPDATE foodgram.users
SET avatar = NULL
WHERE id = $1
RETURNING avatar;
