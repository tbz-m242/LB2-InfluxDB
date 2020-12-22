/** LB02 - MultiTemp
 */
#include "mbed.h"
#include "HTS221Sensor.h"
#include "http_request.h"
#include "network-helper.h"
#include "OLEDDisplay.h"
#include "DHT.h"
#include "https_request.h"

DHT sensor( A5, DHT11 );

// UI
OLEDDisplay oled( MBED_CONF_IOTKIT_OLED_RST, MBED_CONF_IOTKIT_OLED_SDA, MBED_CONF_IOTKIT_OLED_SCL );

static DevI2C devI2c( MBED_CONF_IOTKIT_I2C_SDA, MBED_CONF_IOTKIT_I2C_SCL );
static HTS221Sensor hum_temp(&devI2c);

/** InfluxDB Settings  */
char host[] = "https://192.168.1.166:8086/write";
char key[] = "dGMvpXnAU822Oxhh7awuRbhjiqNf0n";
char db[] = "m242";
char user[] = "iotkit";
char tl[] = "lb02";
char hostname[] = "iot-37";
char location[] = "Genf";

// I/O Buffer
char message[1024];
char body[1024];

DigitalOut myled(D10);

const char SSL_CA_PEM[] =  "-----BEGIN CERTIFICATE-----\n"
"MIIDlzCCAn+gAwIBAgIUbRObQFKTAAZM1OuHUCPgHlaqgKUwDQYJKoZIhvcNAQEL\n"
"BQAwWzELMAkGA1UEBhMCQ0gxEDAOBgNVBAgMB3ByaXZhdGUxEDAOBgNVBAcMB3By\n"
"aXZhdGUxEDAOBgNVBAoMB1p1ZXJpY2gxFjAUBgNVBAMMDTE5Mi4xNjguMS4xNjYw\n"
"HhcNMjAwMzE5MTM1MzIwWhcNMzAwMzE3MTM1MzIwWjBbMQswCQYDVQQGEwJDSDEQ\n"
"MA4GA1UECAwHcHJpdmF0ZTEQMA4GA1UEBwwHcHJpdmF0ZTEQMA4GA1UECgwHWnVl\n"
"cmljaDEWMBQGA1UEAwwNMTkyLjE2OC4xLjE2NjCCASIwDQYJKoZIhvcNAQEBBQAD\n"
"ggEPADCCAQoCggEBANVZ050P4qGKURiX6e5PjRnO0etvEiF4MxZa7m2ga9NgJCr2\n"
"qQvYPr0Hiq+ILTkXFV1sPtDWPpLM3jGl3PiB2dqlOKPan0fNOX+ulkRkCBGmfDvI\n"
"Wp/04eYR/1ZYYsLYrxUyN7E7v2tYbr5NAlGHLbQlluOq+ZkDszFIRRUUDyUz5S1H\n"
"KnDUp7MlyM/uhP44nMCAuqySjoVl9QrRLXWqsLb/GFfRuKFxwCg37zciap7GAmYs\n"
"HqoCTfTZpcBG/AX9EFBdnx/pAF9ypC6B+qJ9VKuSENEe3xnCR8JXc1lEB9iX/HBn\n"
"S97ewAZev+6iMV7JK7/nOuQmFscZSC8yNSqkbQ0CAwEAAaNTMFEwHQYDVR0OBBYE\n"
"FEMQXah028Lyr8avQe9NS/ynjpjhMB8GA1UdIwQYMBaAFEMQXah028Lyr8avQe9N\n"
"S/ynjpjhMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBAF57LIDz\n"
"WFlx/0g1nhu5gBmuS24fRZMZRHbypvGhSU2lUmJemjsRYZCJHop2aIoU+GmXduyr\n"
"Rn8hPEaqqb3SXDD53D6tkmhq4W9ABaoPdnq4uUFBBo/u5F6J9PALc4T7NDxOxO0O\n"
"TmeK5nTS3pU3wmEfxBOabgHTcQreakDKyg+vpz0oWlmi3bZMa6o5cM0jhCNwgjSt\n"
"iKqsDQdloAtwqudmGnO7DebYxTkAFCPBxiiJrmvTbwokpW3rMsbZqtqs0xKqJBxe\n"
"xlS6XTcvo7rosWMQ++YH00o1oMwSrOXEvErGc4Niw3CUqtaX05g70ftOOtIza6zK\n"
"TxglwaehergCPm4=\n"
"-----END CERTIFICATE-----";

int main()
{
    uint8_t id;
    float value1, value2, h = 0.0f, c = 0.0f;
    int rc = 0;
    
    printf("\tLB02RedunantTemp\n");

    /* Init all sensors with default params */
    hum_temp.init(NULL);
    hum_temp.enable();
    
    hum_temp.read_id(&id);
    printf("HTS221  humidity & temperature    = 0x%X\r\n", id);    
    
    // Connect to the network with the default networking interface
    // if you use WiFi: see mbed_app.json for the credentials
    NetworkInterface* network = connect_to_default_network_interface();
    
    if (!network) {
        printf("Cannot connect to the network, see serial output\n");
        return 1;
    }

    while( 1 ) 
    {
        hum_temp.get_temperature(&value1);
        hum_temp.get_humidity(&value2);
        
        rc = sensor.readData();
        if ( rc == 0 )
        {
            c   = sensor.ReadTemperature(CELCIUS);
            h   = sensor.ReadHumidity();
        }
        else
            return  ( -1 );         // Fehler - Programm beenden
    
        
        sprintf( message, "%s?db=%s&u=%s&p=%s", host, db, user, key);
        printf( "%s\n", message );
        oled.cursor( 1, 0 );
        oled.printf( "temp: %3.2f   %4.2f\nhum : %3.2f   %4.2f", value1, c, value2, h );
        
        myled = 1;
        HttpsRequest* post_req = new HttpsRequest(network, SSL_CA_PEM, HTTP_POST, message);
        post_req->set_header("Content-Type", "application/json");
        
        sprintf( body, "%s,host=%s,location=%s in_temperature=%f,in_humidity=%f,ex_temperature=%f,ex_humidity=%f", tl, hostname, location, value1, value2, c, h );


        HttpResponse* post_res = post_req->send(body, strlen(body));
        if (!post_res) 
        {
            printf("HttpsRequest failed (error code %d)\n", post_req->get_error());
            return 1;
        }
        delete post_req;     
        myled = 0;   
        wait(10);
    }
}
