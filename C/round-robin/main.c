
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N_INGRESS_PORTS 4

typedef unsigned char logic;
typedef unsigned char logic_arr3;

int next_grant;

void reset() {
    next_grant = 0;
}

logic_arr3 get_grant(logic tvalid[N_INGRESS_PORTS], logic_arr3 tid[N_INGRESS_PORTS]) {
    // variables
    int i;
    logic_arr3 max_tid = 0;
    int max_tids[N_INGRESS_PORTS] = {0};
    int n_max_tids = 0;

    printf("[");
    for (i = 0; i < N_INGRESS_PORTS; ++i) {
        if (tvalid[i]) {
            printf("In %d req id %1x;", i, tid[i]);
        }
    }
    printf("]");

    // find highest tid
    for (i = 0; i < N_INGRESS_PORTS; ++i) {
        if (tvalid[i]) {
            if (tid[i] > max_tid) {
                n_max_tids = 0;
                max_tid = tid[i];
                memset(max_tids, 0, sizeof(max_tids));
            }
            if (tid[i] == max_tid) {
                max_tids[i] = 1;
                n_max_tids++;
            }

        }
    }

    // round robin arbitration
    if (n_max_tids) {
        for (i = next_grant; i < next_grant + N_INGRESS_PORTS; ++i) {
            if (max_tids[i % N_INGRESS_PORTS]) {
                break;
            }
        }
        printf("Grant to %d", i);

        // update round robin
        next_grant = (i + 1) % N_INGRESS_PORTS;
        
        // return activation string
        i = 1 << i;
    }
    else {
        i = 0;
        printf("No grant");
    }

    // update
    printf("\n");
    return i;
}

void test_grant(logic tvalid[N_INGRESS_PORTS], logic_arr3 tid[N_INGRESS_PORTS], int expected_i) {
    logic_arr3 g = get_grant(tvalid, tid);
    expected_i = (1 << expected_i) & 0xf;
    if (g != expected_i) {
        printf("ERROR: expected %x got %x\n", expected_i, g);
    }
}

int main() {
    logic tvalid[N_INGRESS_PORTS] = {0};
    int local_mem_pad;
    logic_arr3 tid[N_INGRESS_PORTS] = {0};

    int *tvalid_int = (int*)tvalid;
    int *tid_int = (int*)tid;

    // reset FSM
    reset();

    // no active requests
    *tvalid_int = 0x00000000;
    *tid_int    = 0x05040302;
    test_grant(tvalid, tid, -1);

    // distinct tids
    *tvalid_int = 0x01010101;
    *tid_int    = 0x05040302;
    test_grant(tvalid, tid, 3);

    // same tids
    *tvalid_int = 0x01010101;
    *tid_int    = 0x00000000;
    test_grant(tvalid, tid, 0);

    // same tids
    *tvalid_int = 0x01010101;
    *tid_int    = 0x00000000;
    test_grant(tvalid, tid, 1);

    // same tids
    *tvalid_int = 0x01010101;
    *tid_int    = 0x00000000;
    test_grant(tvalid, tid, 2);

    // same tids
    *tvalid_int = 0x01010101;
    *tid_int    = 0x00000000;
    test_grant(tvalid, tid, 3);

    // same tids
    *tvalid_int = 0x01010101;
    *tid_int    = 0x00000000;
    test_grant(tvalid, tid, 0);

    // same tids
    *tvalid_int = 0x01010001;
    *tid_int    = 0x00000000;
    test_grant(tvalid, tid, 2);
}
