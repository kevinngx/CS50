-- In 2.sql, write a SQL query to list the names of all songs in increasing order of tempo.
-- Your query should output a table with a single column for the name of each song.
select name from songs order by tempo asc;