#cmd_ping
This is a small program to revise network program in using C. <br/>
In this project, I implement the cmd **ping** in linux.<br/>
Step:<br/>

    - Check IP or Domain name, and analyse the IP of Domain name by function gethostbyname(). 
    - Create a new socket file descriptor. 
    - Package the info in a icmp struct. 
    - Send Package to destination in a loop.
    - Receive the package return in the loop.
    - Unpackage the package return in the loop.
    - Calculte the time and show info return.
