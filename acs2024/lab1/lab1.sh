echo Version:$(lsb_release -d | cut -d: -f2) 
echo Architecture Core: $(uname -m) 
echo Count Core: $(lscpu | grep "CPU(s):" | cut -d: -f2) 
echo $(lscpu | grep "Model:") 
echo Frequency: $(cat "/proc/cpuinfo" | grep "MHz") 
echo $(cat "/proc/cpuinfo" | grep "cache size") 
echo $(cat /proc/meminfo | grep "MemTotal") 
echo $(cat /proc/meminfo | grep "MemFree") 
echo $(cat /proc/meminfo | grep "MemAvailable") 
echo iface: $(ip -o link show | awk '{print $2,$9}') 
echo ip address: $(ip addr show| grep "inet\b" | awk '{print $2}') 
echo MAC address: $(ip link show | grep "brd\b" | awk '{print $2}') 
# echo Speed Network: $(ifconfig | grep "RX packets" | awk '{print $3$4}') 
# echo "enp1s0/speed: " $(cat /sys/class/net/enp1s0/speed)
# for iface in $(ls /sys/class/net/); do
#   echo "$iface:"
#   sudo ethtool $iface | grep "Speed"
# done
iwconfig


 
df -h | while prob=' ' read -r _ size used available _ point; do 
 
  echo "Filesystem: $point" 
  echo "1K-blocks: $size" 
  echo "Used: $used" 
  echo "Available: $available" 
 
done 



echo "Количество процессоров (логические ядра):"
nproc

echo "Интерфейсы и их IP-адреса:"

for iface in $(ls /sys/class/net/); do
    ipv4=$(ip -4 addr show $iface | grep -oP '(?<=inet\s)\d+(\.\d+){3}')
    ipv6=$(ip -6 addr show $iface | grep -oP '(?<=inet6\s)[\da-f:]*')

    if [[ -n "$ipv4" || -n "$ipv6" ]]; then
        echo "Интерфейс: $iface"
        
        if [[ -n "$ipv4" ]]; then
            echo "  IPv4 адреса:"
            echo "$ipv4"
        else
            echo "  IPv4: Нет IP"
        fi

        if [[ -n "$ipv6" ]]; then
            echo "  IPv6 адреса:"
            echo "$ipv6"
        else
            echo "  IPv6: Нет IP"
        fi
    fi
done


