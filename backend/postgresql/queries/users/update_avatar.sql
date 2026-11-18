UPDATE foodgram.users
SET avatar = $2
WHERE id = $1
RETURNING avatar;
