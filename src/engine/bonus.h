/* 
 * Bonus handling (w) 2003-2004 by Eero Tamminen
 * This code is released under the GPL
 */
#ifndef BONUS_H
#define BONUS_H

/* these need to be in same order as images are in bonus.png */
enum {
	BONUS_AIRBALLOON,	/* bonusmachine makes airballoon */
	BONUS_BIRD,		/* birds move toward plane */
	BONUS_BOMB,		/* plane gets more bombs */
	BONUS_CLONE,		/* the sprite is cloned */
	BONUS_CLOUD,		/* a cloud covers the sprite */
	BONUS_ENERGY,		/* plane gets all damage repaired */
	BONUS_FUEL,		/* plane fuel is restored to max. */
	BONUS_HIPPO,		/* bonusmachine makes hippo */
	BONUS_LOVE,		/* AI plane doesn't shoot at player */
	BONUS_MISSILE,		/* bonusmachine creates seeker missile */
	BONUS_PARACHUTE,	/* plane pilot jumps with parachute */
	BONUS_UFO,		/* ufo appears & fires missiles at plane */
	BONUS_ZEPPELIN,		/* zeppelin moves toward plane */
	ALL_BONUSES		/* number of bonuses */
};

/* 
 * Bonus setup for a level
 */
void bonuses_setup(void);

/*
 * Clear bonuses for the level
 */
void bonuses_clear(void);

/*
 * Return type of next bonus sprite (if such bonus has been catched)
 * or NULL.  After the call, the sprite type is set to NULL and changed
 * again if some game object catches a new sprite bonus.
 * 
 * This is used only by bonus creation machine to check whether it should
 * create a bonus or some other ("bonus") sprite
 */
sprite_type_t *get_bonus_sprite_type(void);

/*
 * Return random bonus ID (int: 0...ALL_BONUSES) from the set of
 * bonuses that are currently active (set when level is loaded).
 */
unsigned int get_random_bonus(void);

/*
 * Return non-zero if bonus with given ID was acknowledged / activated
 * for the given sprite.  ID is the same as returned from get_random_bonus().
 */
int bonus_activate(unsigned int bonustype, struct sprite *it);

/* If structure member is set, the corresponding game objects
 * aim to given sprite, either by moving or shooting towards it.
 * 
 * This is suitable only for object of which there are only one
 * instance on the screen at the same time or which move at different
 * speeds (e.g. airballoon & zeppelin) or avoid each other (birds)
 * or get destroyed (ufo missiles).
 * 
 * Structure members are set by bonuses when player catches them
 * and reset when the pointed player sprite dies (they have to
 * call bonus_clear_aim() when being killed).
 */
struct global_aim_t {
	sprite_t *bird;
	sprite_t *missile;
	sprite_t *zeppelin;
};

extern struct global_aim_t global_aim;

/* clears aims for the given sprite */
void global_aim_clear(sprite_t *s);

#endif /* BONUS_H */
