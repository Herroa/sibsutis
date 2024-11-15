качаем minikube kubectl

применяем манифесты

kubectl apply -f nginx-deployment.yaml

minikube ip
/etc/hosts

kubectl get pods -l app=nginx

kubectl describe service nginx-service

nginx.local