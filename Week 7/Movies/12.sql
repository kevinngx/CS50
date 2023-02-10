-- In 12.sql, write a SQL query to list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred.
-- Your query should output a table with a single column for the title of each movie.
-- You may assume that there is only one person in the database with the name Johnny Depp.
-- You may assume that there is only one person in the database with the name Helena Bonham Carter.

select title from
(
select m.title, count(*) as count
from movies m
inner join stars s on m.id = s.movie_id
inner join people p on s.person_id = p.id
where p.name in ('Johnny Depp', 'Helena Bonham Carter')
group by m.title
)
where count > 1;
