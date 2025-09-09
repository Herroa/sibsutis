mkdir -p ~/ansible_project/{inventory,playbooks,roles}

ansible.cfg
[defaults]
inventory = inventory
roles_path = roles

inventory/hosts.ini
[my_hosts]
host1 ansible_host=192.168.1.2 ansible_user=your_user ansible_ssh_private_key_file=/path/to/private/key
host2 ansible_host=192.168.1.3 ansible_user=your_user ansible_ssh_private_key_file=/path/to/private/key

docker exec -it node1 bash


ansible -m ping all

ansible -m setup host1




Для выполнения ad-hoc команд используйте:
ansible -m ping host1,host2
ansible -m shell -a 'hostname' host1

playbooks/info_playbook.yml
- name: Gather system info
  hosts: my_hosts
  tasks:
    - name: Gather facts
      setup:

    - name: Create structured info file
      template:
        src: templates/info.j2
        dest: /tmp/system_info.yaml


playbooks/templates/info.j2:
host: {{ ansible_hostname }}
os: {{ ansible_os_family }}
version: {{ ansible_distribution_version }}

ansible-playbook playbooks/info_playbook.yml



ansible-galaxy role init my_docker_role

roles/my_docker_role/tasks/main.yml
- name: Manage Docker containers
  hosts: my_hosts
  roles:
    - my_docker_role


