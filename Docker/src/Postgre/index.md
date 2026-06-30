# PostgreSQL イメージ

```bash
# Docker
docker run --name my-pgsql -e POSTGRES_PASSWORD=password -e POSTGRES_USER=myuser -e POSTGRES_DB=mydb -p 5432:5432 -d postgres
```

```dockerfile
From postgres

ENV POSTGRES_USER=myuser
ENV POSTGRES_PASSWORD=password
ENV POSTGRES_DB=mydb

# Time ZoneAc
ENV TZ=Asia/Tokyo

# docker build -t dev/postgres -f Dockerfile.Postgre . 
# docker run -d -p 55432:5432 --name postgres-container dev/postgres
```