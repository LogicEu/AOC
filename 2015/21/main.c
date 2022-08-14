#include <stdio.h>
#include <string.h>

typedef struct entity_t {
    int hitpoints;
    int damage;
    int armor;
} entity_t;

typedef struct item_t {
    int cost;
    int damage;
    int armor;
} item_t;

static const item_t weapons[5] = {
    {8, 4, 0},
    {10, 5, 0},
    {25, 6, 0},
    {40, 7, 0},
    {74, 8, 0}
};

static const item_t armors[6] = {
    {0, 0, 0},
    {13, 0, 1},
    {31, 0, 2},
    {53, 0, 3},
    {75, 0, 4},
    {102, 0, 5}
};

static const item_t rings[7] = {
    {0, 0, 0},
    {25, 1, 0},
    {50, 2, 0},
    {100, 3, 0},
    {20, 0, 1},
    {40, 0, 2},
    {80, 0, 3}
};

static inline void move_battle(entity_t* active, entity_t* defense)
{
    int score = active->damage - defense->armor;
    defense->hitpoints -= score + !score;
}

static int simulate_battle(entity_t player, entity_t enemy)
{
    entity_t* active = &player;
    entity_t* defense = &enemy;

    while (player.hitpoints >= 0 && enemy.hitpoints >= 0) {
        move_battle(active, defense);
        entity_t* swp = active;
        active = defense;
        defense = swp;        
    }

    return player.hitpoints > 0;
}

static void try_battle(const entity_t enemy)
{
    static int min = 99999999;
    static int max = 0;

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 6; ++j) {
            for (int m = 0; m < 7; ++m) {
                for (int n = 0; n < 7; ++n) {
                    
                    if (m == n && m) {
                        continue;
                    }
                    
                    entity_t player = {
                        100, 
                        weapons[i].damage + rings[m].damage + rings[n].damage,
                        armors[j].armor + rings[m].armor + rings[n].armor
                    };

                    int cost = weapons[i].cost + 
                        armors[j].cost + 
                        rings[m].cost + 
                        rings[n].cost;

                    int result = simulate_battle(player, enemy);

                    if (result && cost < min) {
                        min = cost;
                    }

                    if (!result && cost > max) {
                        max = cost;
                    }
                }
            }
        }
    }

    printf("Puzzle 1: %d\n", min);
    printf("Puzzle 2: %d\n", max);
}

int main(void)
{
    const entity_t enemy = {109, 8, 2};    
    try_battle(enemy);
    return 0;
}
