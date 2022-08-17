#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPELLCOUNT 5
#define ENEMYDAMAGE 9

typedef struct stats_t {
    int player;
    int enemy;
    int mana;
    int shield;
    int poison;
    int recharge;
    int spent;
} stats_t;

static int min;

static int battle(const stats_t stats, const int poison)
{
    for (int i = 0; i < SPELLCOUNT; ++i) {

        int cost;
        stats_t s = stats;

        // TURN PLAYER
        
        // Puzzle 2 poison
        s.player -= poison;
        if (s.player <= 0 || s.mana < 53) {
            continue;
        }
        
        // Spell effects apply
        if (s.poison) {
            s.enemy -= 3;
            --s.poison;
        }

        if (s.recharge) {
            s.mana += 101;
            --s.recharge;
        }

        if (s.shield) {
            --s.shield;
        }

        if (s.enemy <= 0) {
            if (s.spent < min) {
                min = s.spent;
            }
            continue;
        }

        // Player casts new spell
        switch (i) {
            case 0: {
                cost = 53;
                s.enemy -= 4;
                break;
            }
            case 1: {
                cost = 73;
                s.enemy -= 2;
                s.player += 2;
                break;
            }
            case 2: {
                if (i == 2 && !s.shield) {
                    cost = 113;
                    s.shield = 6;
                    break;
                } 
            }
            case 3: {
                if (i == 3 && !s.poison) {
                    cost = 173;
                    s.poison = 6;
                    break;
                }
            }
            case 4: {
                if (i == 4 && !s.recharge) {
                    cost = 229;
                    s.recharge = 5;
                    break;
                }
            }
            default: {
                continue;
            }
        }

        // Check if no mana or already spent more than minimum
        s.spent += cost;
        s.mana -= cost;
        if (s.mana < 0 || s.spent >= min) {
            continue;
        }

        // Check if enemy is dead
        if (s.enemy <= 0) {
            if (s.spent < min) {
                min = s.spent;
            }
            continue;
        }
        
        //TURN ENEMY

        // Spell effects apply
        if (s.poison) {
            s.enemy -= 3;
            --s.poison;
        }

        if (s.recharge) {
            s.mana += 101;
            --s.recharge;
        }

        if (s.shield) {
            --s.shield;
        }

        // Check if enemy is dead
        if (s.enemy <= 0) {
            if (s.spent < min) {
                min = s.spent;
            }
            continue;
        }
        
        // Enemy attacks
        int damage = ENEMYDAMAGE - !!s.shield * 7;
        s.player -= damage + !damage;
        if (s.player <= 0) {
            continue;
        }

        battle(s, poison);
    }
    
    return min;
}

int main(void)
{
    const stats_t stats = {50, 51, 500, 0, 0, 0, 0};
    
    min = 9999999;
    battle(stats, 0);
    printf("Puzzle 1: %d\n", min);

    min = 9999999;
    battle(stats, 1);
    printf("Puzzle 2: %d\n", min);
    return EXIT_SUCCESS;
}
