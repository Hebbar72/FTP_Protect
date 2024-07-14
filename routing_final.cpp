#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>
#include <thread>

#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include <linux/netfilter_ipv4.h>

#define MAX_EVENTS 10


using namespace std;

unordered_map<int, bool> socket_active;
    unordered_map<string, pair<int, int>> conn_map;
    unordered_map<int, string> response_map; 
   unordered_map<int, string> helper_map;
struct connection
{
    string src_ip;
    string dst_ip;
    int src_port;
    int dst_port;
    int src_socket;
    int dst_socket;
    int trigger;
};

void send_file(string file_name, int dest, int src, int dest_socket, int src_socket, int dir)
{
	cout << file_name << " thread\n";
	for(auto i : response_map)
		cout << i.first << " " << i.second << "\n";
	
string response_msg = response_map[src];
cout << response_msg << "yello\n";

string cmd = "sudo clamdscan --fdpass " + file_name;
        int virus_detected;
 	do {
        virus_detected = system(cmd.c_str());
    } while (virus_detected == -1 && errno == EINTR);
    FILE* file = fopen(file_name.c_str(), "rb");
    try
    {
	if(virus_detected)
		throw runtime_error("426 Virus Detected");
	
		cout << "FILE is VIRUS free\n";
        char buffer[1024] = "";
        int x = fread(buffer, 1, 1024, file);
        while(x)
        {
            printf("sending from thread 1 %s", buffer);
            if(send(dest_socket, buffer, x, 0) < 0)
		perror("Error thread send");
            x = fread(buffer, 1, 1024, file);
        }  
        cout << "hulu" + response_map[src] << "\n";

while(dir && response_map[src].find("150") == 0);
    // cout << "looping\n";
    cout << response_map[src] << "ayyo\n";
        cout << "sending from thread" + response_map[src] << "\n";
        close(dest_socket);
	close(src_socket);
    if(dir)
        send(src, response_map[src].c_str(), response_map[src].length(), 0);

        fclose(file);
	
        remove(file_name.c_str());
	socket_active[src] = true;
        socket_active[dest] = true;
	cout << "list\n";
	for(auto i : socket_active)
		cout << i.first << " " << i.second << "\n";
    }

    catch(const std::exception& e)
    {
	printf("EXCEPTION RAISED IN THREAD: %s\n", e.what());

cout << "|" << response_msg << " | " << response_map[src] << " | finale\n"; 
        //sleep(5);
	cout << "mulu" + response_map[src] << "\n";
	while(dir && response_map[src].find("150") == 0);
		// cout << "looping\n";
	int t = send(src, "1451 VIRUS DETECTED.\n\0", strlen("1451 VIRUS DETECTED.\n\0"), 0);
    // if(!dir)
    // {
    //     fseek(file, 0, SEEK_END);
    //     int sz = ftell(file);
    //     char t_buffer[1024] = "";
    //     memset(t_buffer, '.', 1024);
    //     strcpy(t_buffer, "VIRUS DETECTED!!!");
    //     int sent = 0;
    //     while(sent < sz)
    //     {
    //         sent += send(dest_socket, t_buffer, min(1024, sz - sent), 0);
    //     }
    // }

    if(dir)
    {
        socket_active[src] = true;
        cout << t << "t\n";

	socket_active[dest] = true;
    }
	cout << t << "t\n";
    	close(dest_socket);
cout << t << "t\n";

        close(src_socket);
cout << t << "t\n";
    while(!dir && response_map[src].find("150") == 0);
		//perror("this is the last stretch");
    if(!dir){

    
	socket_active[src] = true;
        cout << t << "t\n";

	socket_active[dest] = true;
	cout << t << "t\n";
    }
        //fclose(file);
cout << t << "t\n";



        remove(file_name.c_str());

	cout << "done terminating\n";
    }
}

int set_socket_non_blocking(int socket_fd)
 {
    int flags = fcntl(socket_fd, F_GETFL, 0);
    if (flags == -1) 
    {
        perror("fcntl F_GETFL");
        return -1;
    }

    flags |= O_NONBLOCK;
    if (fcntl(socket_fd, F_SETFL, flags) == -1) 
    {
        perror("fcntl F_SETFL O_NONBLOCK");
        return -1;
    }

    return 0;
}

int main(int argc, char* args[])
{
    char* ip = args[1];
    int port = htons(atoi(args[2]));

	 sockaddr_in* router_addr = new sockaddr_in();
    int addr_len = sizeof(*router_addr);
    int router_socket;
    router_addr->sin_family = AF_INET;
    router_addr->sin_port = port;
    inet_pton(AF_INET, ip, &router_addr->sin_addr);
    router_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(router_socket < 0)
    {
        perror("Error Creating Router Socket");
        exit(1);
    }

    if(set_socket_non_blocking(router_socket) < 0)
    {
        exit(1);
    }

    if(bind(router_socket, (sockaddr*)router_addr, sizeof(*router_addr)) < 0)
    {
        perror("Error Binding Router Socket");
        close(router_socket);
        exit(1);
    }
    
    if(listen(router_socket, 5) < 0)
    {
        perror("Error Listening Router Socket");
        close(router_socket);
        exit(1);
    }

    int epoll_fd;
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLRDHUP | EPOLLHUP;
    struct epoll_event events[MAX_EVENTS];
    epoll_fd = epoll_create1(0);
    if(epoll_fd < 0)
    {
        perror("Error Epoll_fd Creation");
        exit(1);
    }

    connection* router_conn = new connection();
    router_conn->trigger = router_socket;
    ev.data.ptr = router_conn;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, router_socket, &ev) < 0) {
        perror("epoll_ctl: server_fd");
        exit(1);
    }

    sockaddr_in client_addr;
    sockaddr_in server_addr;
    int client_socket;
    char addr_str[INET_ADDRSTRLEN];
    while(1)
    {
	    // printf("magane\n");
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        while (nfds == -1) 
        {
            nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
	}
	    printf("mahisha\n");

        for (int n = 0; n < nfds; ++n) 
        {
            if (((connection*)events[n].data.ptr)->trigger == router_socket) 
            {
                printf("router_socket\n");
                client_socket = accept(router_socket, (struct sockaddr *)&client_addr, (socklen_t*)&addr_len);
                if(client_socket < 0) 
                {
                    continue;
                }

                set_socket_non_blocking(client_socket);

                connection* conn[2] = {new connection(), new connection()};
                
                memset(addr_str, INET_ADDRSTRLEN, 0);
                inet_ntop(AF_INET, &client_addr.sin_addr, addr_str, sizeof(addr_str));
                conn[0]->src_ip = string(addr_str);
                conn[0]->src_port = ntohs(client_addr.sin_port);
                conn[0]->src_socket = client_socket;
                conn[0]->trigger = client_socket;
                getsockopt(conn[0]->src_socket, SOL_IP, SO_ORIGINAL_DST, &server_addr, (socklen_t *)&addr_len);

                memset(addr_str, INET_ADDRSTRLEN, 0);
                inet_ntop(AF_INET, &server_addr.sin_addr, addr_str, sizeof(addr_str));
                conn[0]->dst_ip = string(addr_str);
                conn[0]->dst_port = ntohs(server_addr.sin_port);
                conn[0]->dst_socket = socket(AF_INET, SOCK_STREAM, 0);

		        conn[1]->src_ip = conn[0]->src_ip;
                conn[1]->src_port = conn[0]->src_port;
		        conn[1]->src_socket = conn[0]->src_socket;
		        conn[1]->dst_ip = conn[0]->dst_ip;
                conn[1]->dst_port = conn[0]->dst_port;
		        conn[1]->dst_socket = conn[0]->dst_socket;
		        conn[1]->trigger = conn[1]->dst_socket;

		        connect(conn[0]->dst_socket, (sockaddr*)&server_addr, sizeof(server_addr));
                set_socket_non_blocking(conn[0]->dst_socket);
                ev.data.ptr = conn[0];

                if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn[0]->src_socket, &ev) < 0) 
                {
                    perror("epoll_ctl: new_socket");
                    exit(EXIT_FAILURE);
                }   

                ev.data.ptr = conn[1];
                if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn[0]->dst_socket, &ev) < 0) 
                {
                    perror("epoll_ctl: new_socket");
                    exit(EXIT_FAILURE);
                }


                if(ntohs(server_addr.sin_port) != 21)
                {
                    socket_active[conn[0]->src_socket] = false;
                    socket_active[conn[0]->dst_socket] = false;
                }
                else
                {
                    socket_active[conn[0]->src_socket] = true;
                    socket_active[conn[0]->dst_socket] = true;
                }
            }
            else 
            {
                char buffer[1024] = "";
                int s_socket, d_socket;
                if(((connection*)events[n].data.ptr)->trigger == ((connection*)events[n].data.ptr)->src_socket)
                {
	                printf("client_socket\n");
                    s_socket = ((connection*)events[n].data.ptr)->src_socket;
                    d_socket = ((connection*)events[n].data.ptr)->dst_socket;
                }
                else 
                {
		            printf("server_socket\n");
                    s_socket = ((connection*)events[n].data.ptr)->dst_socket;
                    d_socket = ((connection*)events[n].data.ptr)->src_socket;
                }
                
                memset(buffer, 0, 1024);
                int x = recv(s_socket, buffer, 1024, 0);
                if(x > 0)
                {
                    printf("buffer: %s\n", buffer);
			response_map[d_socket].assign(buffer, buffer + x);
                    if(socket_active[s_socket])
			{
                for(auto i : socket_active)
                    cout << i.first << " " << i.second << "\n";
                cout << "xxxyyyxxx\n";
                cout << s_socket << " " << d_socket << "\n";
                printf("sending from outside %s", buffer);
                        send(d_socket, buffer, x, 0);
if(((connection*)events[n].data.ptr)->dst_port == 21){
			//response_map[d_socket].assign(buffer, buffer + x);
                        cout << d_socket << response_map[d_socket] << socket_active[d_socket] << "yallah\n";}
			cout << "prathamanyasan\n";			
for(auto i : socket_active)
				cout << i.first << " " << i.second << "\n";
			}
                    else if(((connection*)events[n].data.ptr)->dst_port != 21)
                    {
                        string file_name = ((connection*)events[n].data.ptr)->dst_ip + ":" + to_string(((connection*)events[n].data.ptr)->dst_port) + ":" + ((connection*)events[n].data.ptr)->src_ip;
                        cout << "writing to file" << file_name << "\n";
			FILE* file = fopen(file_name.c_str(), "ab");
                        if(fwrite(buffer, 1, x, file) != x)
				perror("fwriet error");
                        fclose(file);
			
                    }
                }
                else if(x == 0)
                {
			        printf("terminating\n");
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, s_socket, NULL);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, d_socket, NULL);

                    if(((connection*)events[n].data.ptr)->dst_port != 21)
                    {
                        printf("data connection\n");
                        string file_name = ((connection*)events[n].data.ptr)->dst_ip + ":" + to_string(((connection*)events[n].data.ptr)->dst_port) + ":" + ((connection*)events[n].data.ptr)->src_ip;
                        auto conn_sockets = conn_map[file_name];
                        conn_map.erase(file_name);
                        socket_active.erase(s_socket);
                        socket_active.erase(d_socket);
			for(auto i : response_map)
			{
				cout << i.first << ":" << i.second;
			}
                        printf("%d %d\n", conn_sockets.first, conn_sockets.second);
			printf("%d\n", response_map[conn_sockets.second].find("226"));
			if(response_map[conn_sockets.second].find("150") == 0)
			{
				
				printf("take_all\n");
				for(auto i : response_map)
					cout << i.first << " " << i.second << "\n";
                cout << conn_sockets.second << " " << conn_sockets.first << "\n";
                if(response_map[conn_sockets.first].find("STOR") == 0)
                {
                    thread t1(send_file, file_name, conn_sockets.first, conn_sockets.second, d_socket, s_socket, 0);
			        t1.detach();
                    cout << "activate STORE\n";
                }
                else
                {
                    thread t1(send_file, file_name, conn_sockets.first, conn_sockets.second, d_socket, s_socket, 1);
			        t1.detach();
                    cout << "activate not STORE\n";
                }
                
}

                        else if(response_map[conn_sockets.second].find("226") == 0)
                        {
                            printf("take1\n");
                            // thread t1(send_file, file_name, conn_sockets, d_socket, response_map[s_socket]);
                        }
                        else
                        {
                            printf("take2\n");
                            strcpy(buffer, "426 Virus Detected");
                            printf("sending from outside %s", buffer);
                            send(conn_sockets.first, buffer, strlen(buffer), 0);
                        }
                        // response_map.erase(conn_sockets.second);
                    }
			else{
                    close(s_socket);
                    close(d_socket);}
                }
                else 
                {
                    perror("No Data Available");
                }

                if(((connection*)events[n].data.ptr)->dst_port == 21 && !strncmp(buffer, "229 Entering Extended Passive Mode", 34))
                {
                    char temp[1024] = "";
                    strcpy(temp, buffer);
                    strtok(temp, "|||");
                    char* temp_str = strtok(NULL, "|");
                    printf("%s\n", temp_str);
                    cout << "epsv\n";
                    for(auto i: conn_map)
                        cout << i.first << "\n";
                    cout << "epsv_end\n";
                    conn_map[((connection*)events[n].data.ptr)->dst_ip + ":" + string(temp_str) + ":" + ((connection*)events[n].data.ptr)->src_ip] = {s_socket, d_socket};
                    helper_map[((connection*)events[n].data.ptr)->dst_socket] = string(temp_str);
                    for(auto i: conn_map)
                        cout << i.first << "\n";
                    cout << "epsv_end_end\n";
		}
                else if(((connection*)events[n].data.ptr)->dst_port == 21 && (!strncmp(buffer, "150", 3) ))
                {
                    for(auto i : helper_map)
                        cout << "gante" << i.first << "\n";
                    cout << ((connection*)events[n].data.ptr)->dst_socket << "\n";
			cout << "yaasthee praja" << ((connection*)events[n].data.ptr)->dst_ip + ":" + helper_map[((connection*)events[n].data.ptr)->dst_socket]  + ":" + ((connection*)events[n].data.ptr)->src_ip << "\n";
			int a = conn_map[((connection*)events[n].data.ptr)->dst_ip + ":" + helper_map[((connection*)events[n].data.ptr)->dst_socket] + ":" + ((connection*)events[n].data.ptr)->src_ip].first;
			int b = conn_map[((connection*)events[n].data.ptr)->dst_ip + ":" + helper_map[((connection*)events[n].data.ptr)->dst_socket] + ":" + ((connection*)events[n].data.ptr)->src_ip].second;
                    socket_active[a] = false;
                    socket_active[b] = false;
                cout << "deva" << a << " " << b << " " << socket_active[a] << " " << socket_active[b] << "\n";
		}
            }
        }
    }
}
