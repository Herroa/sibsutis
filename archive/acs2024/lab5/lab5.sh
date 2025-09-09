качаем minikube kubectl
minikube start


применяем манифесты
kubectl apply -f nginx-deployment.yaml
kubectl describe service nginx-service

minikube ip


kubectl get pods
kubectl get service nginx-service
kubectl describe deployment nginx-deployment
kubectl describe replicaset -l app=nginx
