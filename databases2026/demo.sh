docker exec -it demo-postgres psql -U demo_user -d demo
docker-compose up -d

docker cp demo-big-20170815.sql demo-postgres:demo-big-20170815.sql
docker exec -it demo-postgres psql -U demo_user -d demo -f ./demo-big-20170815.sql 