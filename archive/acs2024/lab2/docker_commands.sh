# Команды для работы с образами (images)

docker images                            # Список образов
docker build -t <имя_образа>:<тег> <путь_к_Dockerfile>  # Создание образа из Dockerfile
docker pull <имя_образа>:<тег>          # Скачивание образа из Docker Hub
docker rmi <имя_образа>:<тег>           # Удаление образа
docker pull <имя_образа>                 # Обновление образа

# Команды для работы с контейнерами (containers)

docker run [OPTIONS] <имя_образа>       # Запуск контейнера
docker ps                                # Список запущенных контейнеров
docker ps -a                             # Список всех контейнеров (включая остановленные)
docker stop <имя_или_ID_контейнера>     # Остановка контейнера
docker rm <имя_или_ID_контейнера>       # Удаление контейнера
docker restart <имя_или_ID_контейнера>  # Перезапуск контейнера
docker inspect <имя_или_ID_контейнера>  # Получение информации о контейнере
docker exec -it <имя_или_ID_контейнера> <команда>  # Запуск команды в работающем контейнере

# Команды для работы с сетями (networks)

docker network ls                        # Список сетей
docker network create <имя_сети>        # Создание сети
docker network rm <имя_сети>            # Удаление сети

# Команды для работы с томами (volumes)

docker volume ls                         # Список томов
docker volume create <имя_тома>         # Создание тома
docker volume rm <имя_тома>             # Удаление тома

# Управление Docker

sudo systemctl start docker             # Запуск службы Docker
sudo systemctl stop docker              # Остановка службы Docker
sudo systemctl status docker            # Проверка статуса службы Docker

# Полезные команды

docker version                           # Получение информации о версии Docker
docker info                              # Получение информации о системе Docker
docker search <ключевое_слово>          # Поиск образа в Docker Hub
