#include "wifi.h"
#include "event.h"
#include <CoreWLAN/CoreWLAN.h>
#include <SystemConfiguration/SystemConfiguration.h>

char *g_current_ssid = NULL;
void update_ssid(SCDynamicStoreRef store, CFArrayRef keys, void *info) {
    @autoreleasepool {
        NSData *data = [[[CWWiFiClient sharedWiFiClient] interface] ssidData];
        char ssid[[data length] + 1];
        memcpy(ssid, [data bytes], [data length]);
        ssid[[data length]] = '\0';

        if (!g_current_ssid || strcmp(g_current_ssid, ssid) != 0) {
            if (g_current_ssid)
                free(g_current_ssid);
            g_current_ssid = string_copy(ssid);

            struct event event = {(void *)ssid, WIFI_CHANGED};
            event_post(&event);
        }
    }
}

void forced_network_event() {
    if (g_current_ssid)
        free(g_current_ssid);
    g_current_ssid = NULL;
    update_ssid(NULL, NULL, NULL);
}

void begin_receiving_network_events() {
    SCDynamicStoreContext context = {0, NULL, NULL, NULL, NULL};
    SCDynamicStoreRef store =
        SCDynamicStoreCreate(NULL, CFSTR("network"), update_ssid, &context);

    const void *values[] = {CFSTR(".*/Network/Global/IPv4")};
    CFArrayRef keys = CFArrayCreate(NULL, values, 1, &kCFTypeArrayCallBacks);
    SCDynamicStoreSetNotificationKeys(store, NULL, keys);
    CFRunLoopSourceRef loop_source =
        SCDynamicStoreCreateRunLoopSource(NULL, store, 0);

    CFRunLoopAddSource(CFRunLoopGetCurrent(), loop_source,
                       kCFRunLoopDefaultMode);
    CFRelease(keys);
}
