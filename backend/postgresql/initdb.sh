#!/bin/sh
set -e
for sql_file in /migrations/*.sql /initial_data/*.sql /demo_data/*.sql; do
    [ -e "$sql_file" ] || continue
    psql -v ON_ERROR_STOP=1 -U "$POSTGRES_USER" -d "$POSTGRES_DB" -f "$sql_file"
done
