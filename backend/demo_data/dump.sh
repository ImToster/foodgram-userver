#!/bin/sh
# Dump the running stack into backend/demo_data:
#   demo.sql  -- PostgreSQL data (users, recipes, subscriptions, ...)
#   media/    -- silo bucket contents (images)
#
# The stack must be up (docker compose up -d).
# Ingredients are not dumped: they live in backend/postgresql/initial_data.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
cd "$ROOT"

mkdir -p backend/demo_data/media

echo "Dumping PostgreSQL to backend/demo_data/demo.sql..."
docker compose exec -T db sh -c \
    'pg_dump -U "$POSTGRES_USER" -d "$POSTGRES_DB" --data-only --column-inserts --no-owner --no-privileges --exclude-table-data=foodgram.ingredients' \
    > backend/demo_data/demo.sql

echo "Mirroring silo bucket to backend/demo_data/media/..."
docker compose run --rm -T --no-deps --entrypoint sh createbuckets -c \
    'mc alias set local http://silo:9000 "$S3_ACCESS_KEY" "$S3_SECRET_KEY" >/dev/null && mc mirror --overwrite "local/$S3_BUCKET" /seed'

echo "Done:"
echo "  backend/demo_data/demo.sql"
echo "  backend/demo_data/media/"
