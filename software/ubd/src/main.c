#include <config.h>
#include <stdio.h>
#include <glib.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>
#include <string.h>
#include <gio/gio.h>

#include "ubpacket.h"
#include "serial.h"
#include "debug.h"
#include "net6.h"
#include "packet.h"
#include "ubpacket.h"
#include "busmgt.h"
#include "address6.h"
#include "mgt.h"
#include "cmdparser.h"
#include "xmlconfig.h"
#include "groups.h"
#include "config.h"

int main (void)
{
    if (!g_thread_supported ()) g_thread_init (NULL);
    g_type_init();
    

    nodes_init();
    groups_init();
    xml_init("ubdconfig.xml");
    if( config.interface == NULL ){
        fprintf(stderr, "Please specify an interface to use.\n");
        return -1;
    }

    if( config.base == NULL ){
        fprintf(stderr, "Please specify a base address to use.\n");
        return -1;
    }

    if( net_init(config.interface, 
                        config.base,
                        config.prefix) ){
        fprintf(stderr, "Failed to set up network.\n"
                "Interface=%s\nBaseaddress=%s\n"
                "Prefix=%d\nAborting.\n",
                config.interface, config.base, config.prefix);
        return -1;
    }

    mgt_init();

    if( serial_open(config.device) ){
        printf( "Failed to open serial device %s\n"
                "Aborting.\n", config.device);
        return -1;
    }

    //activate bridge
    serial_switch();
    packet_init();     
    busmgt_init();
    cmdparser_init();

    GMainLoop * mainloop = g_main_loop_new(NULL,TRUE);
    g_main_loop_run(mainloop);
    return 0;
}

