#include <stdio.h>

typedef struct entity_t {
    int hitpoints;
    int damage;
    int armor;
    int mana;
} entity_t;

typedef struct spell_t {
    int hitpoints;
    int dammage;
    int armor;
    int mana;
    int timer;
    int cost;
} spell_t;

static const spell_t spell_table[5] = {
    {0, 4, 0, 0, -1, 52},
    {2, 2, 0, 0, -1, 73},
    {0, 0, 7, 0, 6, 113},
    {0, 3, 0, 0, 6, 173},
    {0, 0, 0, 101, 5, 229}
};

static int simulate_battle(
        entity_t player, entity_t enemy, 
        const int shield, const int poison, const int recharge)
{
    static int min = 999999999;
    int s = shield, p = poison, r = recharge;

    entity_t P = player, E = enemy;
    /*printf("%d, %d, %d\nPlayer: %d, %d\nEnemy: %d %d\n",
        s, p, r, P.hitpoints, P.mana, E.hitpoints,
        E.damage
    );*/
    
    for (int i = 4; i >= 0; --i) {

        // Skip already ongoing spells
        if ((s && (i == 2)) || (p && (i == 3)) || (r && (i == 4))) {
            continue;
        }
        
        // Spells effects
        if (s) {
            P.armor = 7;
            --s;
        }

        if (p) {
            E.hitpoints -= 3;
            --p;
        }

        if (r) {
            P.mana += 101;
            --r;
        }

        // Player turn
        switch (i) {
            case 0: {
                P.mana -= 53;
                E.hitpoints -= 4;
                break;
            }
            case 1: {
                P.mana -= 73;
                E.hitpoints -= 2;
                P.hitpoints += 2;
                break;
            }
            case 2: {
                P.mana -= 113;
                s = 6;
                break;
            }
            case 3: {
                P.mana -= 173;
                p = 6;
                break; 
            }
            case 4: {
                P.mana -= 229;
                r = 5;
                break;
            }
        }

        // Check if no mana or dead
        if (P.mana < 0) {
            return 0;
        }

        else if (E.hitpoints <= 0) {
            const int score = 500 - P.mana;
            if (score > 0 && score < min) {
                min = score;
                printf("Score: %d\n", score);
            }
            return 500 - P.mana;
        }

        // Spells effects
        if (s) {
            P.armor = 7;
            --s;
        }

        if (p) {
            E.hitpoints -= 3;
            --p;
        }

        if (r) {
            P.mana += 101;
            --r;
        }

        //Enemy turn

        int dif = E.damage - P.armor;
        dif = dif > 0 ? dif : 1;
        P.hitpoints -= dif;

        //Check if dead
        if (P.hitpoints <= 0) {
            return 0;
        }

        int step = simulate_battle(P, E, s, p, r);
        if (step > 0 && step < min) {
            min = step;
        }
    }

    return simulate_battle(P, E, s, p, r);
}

int main(void)
{
    entity_t player = {50, 0, 0, 500};
    entity_t enemy = {51, 0, 0, 500};

    int result = simulate_battle(player, enemy, 0, 0, 0);
    printf("Puzzle 1: %d\n", result);

    return 0;
}
