sudo lsof -i :80

Prometheus: https://localhost:9090
Grafana: https://localhost:3000 (логин/пароль: admin/admin)
Alertmanager: https://localhost:9093
Prometheus:
Перейди в Targets (http://localhost:9090/targets) и проверь, чтобы все экспортеры были в статусе UP.


container_memory_usage_bytes{container_name!=""}

rate(node_cpu_seconds_total{mode="idle"}[5m])

curl -X POST --data-binary @test_metric.yml http://localhost:9091/metrics/job/test_job
