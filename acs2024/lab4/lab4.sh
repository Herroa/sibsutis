Поднимаем докеры с образами nginx
docker run -d --name backend1 -p 8081:80 nginx
docker run -d --name backend2 -p 8082:80 nginx

docker rm $(docker ps -q)

настраваем конфигурацию
sudo nano /etc/nginx/nginx.conf
http {
    upstream backend_servers {
        server 127.0.0.1:8081; 
        server 127.0.0.1:8082;  
    }

    server {
        listen 80; 

        location / {
            proxy_pass http://backend_servers;  
            proxy_set_header Host $host;
            proxy_set_header X-Real-IP $remote_addr;
            proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        }
    }
}

sudo nginx -t
sudo systemctl enable nginx
sudo systemctl start nginx
sudo systemctl restart nginx


cd /usr/share/nginx/html/
echo "Server 8082" > index.html

curl http://localhost
curl http://localhost
curl http://localhost




sudo nano /etc/haproxy/haproxy.cfg
frontend http_front
    bind *:80
    default_backend backend_servers

backend backend_servers
    balance roundrobin
    server backend1 localhost:8081 check
    server backend2 localhost:8082 check


systemctl stop nginx
sudo systemctl restart haproxy

docker run -d --name traefik -p 80:80 -p 8080:8080 -v /var/run/docker.sock:/var/run/docker.sock traefik --api.insecure=true --providers.docker

docker run -d \
  --name backend1 \
  -p 8081:80 \
  --label 'traefik.http.routers.backend1.rule=Host("example.com")' \
  --label 'traefik.http.services.backend1.loadbalancer.server.port=80' \
  nginx

docker run -d \
  --name backend2 \
  -p 8082:80 \
  --label 'traefik.http.routers.backend2.rule=Host("example.com")' \
  --label 'traefik.http.services.backend2.loadbalancer.server.port=80' \
  nginx
http://localhost:8080/dashboard/
curl http://localhost

docker-compose down
docker-compose up -d


sudo ipvsadm -A -t 192.168.1.1:80 -s rr
sudo ipvsadm -a -t 192.168.1.1:80 -r 192.168.1.2:80 -m
sudo ipvsadm -a -t 192.168.1.1:80 -r 192.168.1.3:80 -m
sudo ipvsadm -L -n


sudo systemctl stop ipvsadm