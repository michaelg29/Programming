int mod(int n, int d);

typedef struct {
    char wrap;
    int x;
    int y;
    char *board;
} conway;

void conway_init(conway *c, char wrap, int x, int y);
void conway_seed(conway *c, char *seed, char empty);
void conway_seedTable(conway *c, char **seed, char empty);
void conway_simulate(conway *c);
void conway_simulateN(conway *c, int n);
void conway_destroy(conway *c);

int conway_cell(conway *c, int x, int y);
char **conway_print(conway *c);