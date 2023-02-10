-- In 8.sql, write a SQL query to list the names of all people who starred in Toy Story.
-- Your query should output a table with a single column for the name of each person.
-- You may assume that there is only one movie in the database with the title Toy Story.

select p.name
from movies m
inner join stars s on m.id = s.movie_id
inner join people p on s.person_id = p.id
where m.title = 'Toy Story';