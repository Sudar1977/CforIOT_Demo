#include "stdio.h"
#include "stdint.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"

#include "MQTTClient.h"

//~ #define ADDRESS     "tcp://localhost:1883"
//~ #define ADDRESS     "mqtt.eclipseprojects.io:1883"
#define ADDRESS     "192.168.1.30:1883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "/node-red/temp"
#define PAYLOAD     "32"
//~ #define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L
double DELAY = 5;


struct sensor {
    uint16_t year;
    uint8_t month;
    uint16_t day;
    uint8_t hour;
    uint8_t minute;
    int8_t t;
};

void AddRecord (struct sensor* info, int number, 
uint16_t year,uint8_t month,uint16_t day,uint8_t hour,uint8_t minute,int8_t t)
{
    info[number].year = year;
    info[number].month = month;
    info[number].day = day;
    info[number].hour = hour;
    info[number].minute = minute;
    info[number].t = t;
}


int main(int argc, char* argv[])
{
    FILE *file;
	file = fopen("temperature_small1.csv","r");
    
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = "IoT";
    conn_opts.password = "student1";
    

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    
    struct sensor*info = malloc (365*24*60*sizeof(struct sensor));
    

        
        int Y,M,D,H,Min,T;
        int r;
        int count=0;
        
        for (;(r=fscanf(file,"%d;%d;%d;%d;%d;%d",&Y,&M,&D,&H,&Min,&T))>0;count++)
        {
            if (r<6)
            {
                char s[20], c;
                r = fscanf (file, "%[^\n]%c",s,&c);
                printf("Wrong format in line %s\n",s);
            }
            else
            {
                printf("%d %d %d %d %d %d\n",Y,M,D,H,Min,T);
                AddRecord(info,count,Y,M,D,H,Min,T);
            }
		}
        fclose(file);
        
        int i = 0;

    while(1)
    {

        
        clock_t begin = clock();       

        char str[255];
        sprintf(str,"%d",info[i++].t);
        printf("%s,%d\n",str,i);
        
        if(i>=count)
            i=0;
        
        
        pubmsg.payload = str;
        pubmsg.payloadlen = strlen(str);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;
        MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        //printf("Waiting for up to %d seconds for publication of %s\n"
        //        "on topic %s for client with ClientID: %s\n",
        //        (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
        rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        //printf("Message with delivery token %d delivered\n", token);
        
        while ((double)(clock() - begin)/CLOCKS_PER_SEC<DELAY)
        {}
    }
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
