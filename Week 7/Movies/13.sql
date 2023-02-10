-- In 13.sql, write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.
-- Your query should output a table with a single column for the name of each person.
-- There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
-- Kevin Bacon himself should not be included in the resulting list.


select p.name
    from movies m
    inner join stars s on m.id = s.movie_id
    inner join people p on s.person_id = p.id
where m.id in
(
    select m.id
    from movies m
    inner join stars s on m.id = s.movie_id
    inner join people p on s.person_id = p.id
    where p.name = 'Kevin Bacon'
)
and p.name != 'Kevin Bacon';