-- In 10.sql, write a SQL query to list the names of all people who have directed a movie that received a rating of at least 9.0.
-- Your query should output a table with a single column for the name of each person.
-- If a person directed more than one movie that received a rating of at least 9.0, they should only appear in your results once.

select p.name
from movies m
inner join directors d on m.id = d.movie_id
inner join people p on d.person_id = p.id
inner join ratings r on r.movie_id = m.id
where r.rating >= 9;