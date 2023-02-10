-- In 6.sql, write a SQL query to determine the average rating of all movies released in 2012.
-- Your query should output a table with a single column and a single row (not including the header) containing the average rating.

select avg(r.rating)
from movies m
inner join ratings r
on m.id = r.movie_id
where m.year = 2012;