#include <mictcp.h>
#include <api/mictcp_core.h>



mic_tcp_sock socket_m ;
/*Test de pull dans terminal*/
/*Test pull 22222*/
/*
 * Permet de créer un socket entre l’application et MIC-TCP
 * Retourne le descripteur du socket ou bien -1 en cas d'erreur
 */
int mic_tcp_socket(start_mode sm)
{
    int result = -1;

    printf("[MIC-TCP] Appel de la fonction: ");  printf(__FUNCTION__); printf("\n");
    if (initialize_components(sm)==-1 ) return result; /* Appel obligatoire */
    set_loss_rate(0);

    mic_tcp_sock sock;
    sock.fd = 0;
    //sock.state = IDLE;

    socket_m = sock;
    result = sock.fd;

    return result;
}

/*
 * Permet d’attribuer une adresse à un socket.
 * Retourne 0 si succès, et -1 en cas d’échec
 */
int mic_tcp_bind(int socket, mic_tcp_sock_addr addr)
{
   printf("[MIC-TCP] Appel de la fonction: ");  printf(__FUNCTION__); printf("\n");
   /*
   //retrouver socket puis :
   socket_m.fd = socket;
   socket_m.state = IDLE;
   socket_m.addr = addr;*/
   return 0;
   
   
}
   

/*
 * Met le socket en état d'acceptation de connexions
 * Retourne 0 si succès, -1 si erreur
 */
int mic_tcp_accept(int socket, mic_tcp_sock_addr* addr)
{
    printf("[MIC-TCP] Appel de la fonction: ");  printf(__FUNCTION__); printf("\n");
    return 0;
}

/*
 * Permet de réclamer l’établissement d’une connexion
 * Retourne 0 si la connexion est établie, et -1 en cas d’échec
 */
int mic_tcp_connect(int socket, mic_tcp_sock_addr addr)
{
    printf("[MIC-TCP] Appel de la fonction: ");  printf(__FUNCTION__); printf("\n");
    return 0;
}

/*
 * Permet de réclamer l’envoi d’une donnée applicative
 * Retourne la taille des données envoyées, et -1 en cas d'erreur
 */
int mic_tcp_send (int mic_sock, char* mesg, int mesg_size)
{
    printf("[MIC-TCP] Appel de la fonction: "); printf(__FUNCTION__); printf("\n");
    int result = -1;
    mic_tcp_pdu pdu;
    //retrouver socket puis : 
    mic_tcp_sock_addr addr_dest = socket_m.addr; //a remplir avec addr sans socket

    //pdu.header.source_port = htons(API_CS_Port);
    pdu.header.dest_port = socket_m.addr.port;
    
    pdu.payload.data=mesg;
    pdu.payload.size=mesg_size;
    
    result = IP_send(pdu,addr_dest);

    return result;
}

/*
 * Permet à l’application réceptrice de réclamer la récupération d’une donnée
 * stockée dans les buffers de réception du socket
 * Retourne le nombre d’octets lu ou bien -1 en cas d’erreur
 * NB : cette fonction fait appel à la fonction app_buffer_get()
 */
int mic_tcp_recv (int socket, char* mesg, int max_mesg_size)
{
    printf("[MIC-TCP] Appel de la fonction: "); printf(__FUNCTION__); printf("\n");
    int result = -1 ;
    //retrouver socket ici socket_m
    mic_tcp_payload data_buf ;
    data_buf.data = mesg;
    data_buf.size = max_mesg_size;
    /*
    mic_tcp_sock_addr addr_rec = socket_m.addr;
    mic_tcp_pdu pdu;
    IP_recv(&pdu,&addr_rec,0.01);//Voir quoi mettre pour timeout
    */
    result = app_buffer_get(data_buf);

    //*mesg = (data_buf).data;

    
    return result;
}

/*
 * Permet de réclamer la destruction d’un socket.
 * Engendre la fermeture de la connexion suivant le modèle de TCP.
 * Retourne 0 si tout se passe bien et -1 en cas d'erreur
 */
int mic_tcp_close (int socket)
{
    printf("[MIC-TCP] Appel de la fonction :  "); printf(__FUNCTION__); printf("\n");
    //retrouer socket puis close dans state
    //socket_m.state = CLOSED;
    return -1;
}

/*
 * Traitement d’un PDU MIC-TCP reçu (mise à jour des numéros de séquence
 * et d'acquittement, etc.) puis insère les données utiles du PDU dans
 * le buffer de réception du socket. Cette fonction utilise la fonction
 * app_buffer_put().
 */
void process_received_PDU(mic_tcp_pdu pdu, mic_tcp_sock_addr addr)
{
    printf("[MIC-TCP] Appel de la fonction: "); printf(__FUNCTION__); printf("\n");

    //mic_tcp_header header =pdu.header; 
    mic_tcp_payload payload = pdu.payload;

    app_buffer_put(payload);
    


}