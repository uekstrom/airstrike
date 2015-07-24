#include <math.h>
#include "mech.h"
#include "level.h"

vector_t mech_global_pos(mech_sprite_t *s, vector_t local)
{
  return vadd(s->sprite.pos,vrot(local,(int)s->angle));
}

void mech_defaults(mech_sprite_t *s)
{
  s->sprite.flags |= SPRITE_MECH;

  s->rmass = 1; 
  if (s->sprite.flags & SPRITE_CANROTATE)
    s->rinmom = 2.0;
  else
    s->rinmom = 0;
  s->gravity = 1;
  s->ang_vel = 0;
  s->ang_impulse = 0;

  s->air_resistance = 0.00001;
  s->air_resistance_ang = 0.0001;

  s->bounce = 0.5;
}

static void mech_air(mech_sprite_t *s)
{
  /* Isometric air resistance */
  float iso;
  iso = -vdot(s->sprite.vel,s->sprite.vel)*(s->air_resistance);
  s->lin_impulse = vmadd(s->lin_impulse,s->sprite.vel,iso);

  s->ang_impulse -= sgn(s->ang_vel)*s->ang_vel*s->ang_vel*s->air_resistance_ang;
}


void mech_refresh_image(mech_sprite_t *s)
{
  if (s->sprite.flags & SPRITE_CANROTATE)
    s->sprite.anim_p = (((int)(s->angle + 0.5)) & 255) / MECH_ANG_DIV;
  sprite_update_image((sprite_t *)s);
}

void mech_update(mech_sprite_t *s)
{
  if (!(s->sprite.flags & SPRITE_IMMOBILE))
    {
      mech_boundary(s);
      mech_air(s);
      ((sprite_t *)s)->vel = vmadd(((sprite_t *)s)->vel,s->lin_impulse,s->rmass);
      s->lin_impulse.x = 0;
      s->lin_impulse.y = 0;
      s->sprite.vel.y += 
	current_level->gravity*s->gravity;
    }
  if (s->sprite.flags & SPRITE_CANROTATE)
    {
      s->ang_vel += s->ang_impulse*s->rinmom;
      s->ang_vel *= 0.95;
      s->angle += s->ang_vel;
      s->sprite.anim_p = (((int)(s->angle + 0.5)) & 255) / MECH_ANG_DIV;
      s->ang_impulse = 0;
    }
}

#if 0
/* TODO: this one is obsolete */
void mech_turn_to(mech_sprite_t *s, const float *n, float strength)
{
  return;
  if (s->sprite.flags & SPRITE_CANROTATE)
    {
      /* replace by actual angular impulse instead */
      if (vdot(n,mech_heading(s)) > 0)
	s->angle -= strength*vcross(n,mech_heading(s));
      else
	s->angle += strength*vcross(n,mech_heading(s));
    }
}
#endif

vector_t mech_point_vel(mech_sprite_t *s, const float *p)
{
  return ((sprite_t *)s)->vel; /* nonrotating bodies only for now */
}

void mech_apply_impulse(mech_sprite_t *s, const float *p, vector_t i)
{
  s->lin_impulse = vadd(s->lin_impulse,i);
  return;
}

/*
  Sprite velocity can change only in the direction of the normal (n).
  momentum conservation gives
  dp1 = -dp2 <=> dv1*m1 = -dv2*m2 

  dv1 = J*n, J scalar to be determined
  dv2 = -J*n*m1/m2

  Kinetic energy after impact is:
  Ek' = (v1 + dv1)^2*m1/2 + (v2 + dv2)^2*m2/2
  before impact is was
  Ek  = v1^2*m1/2 + v2^2*m2/2
  so 
  dEk = Ek' - Ek = (2*v1.dv1 + dv1^2)*m1/2 + (2*v2.dv2 + dv2^2)*m2/2 < 0
  
  dEk = (2*J*v1.n + J^2*n^2)*m1/2 + (-2*J*m1/m2*v2.n + J^2*n^2*m1^2/m2^2)*m2/2

  maximum elasticity: dEk = 0
  (2*J*v1.n + J^2*n^2)*m1/2 + (-2*J*m1/m2*v2.n + J^2*n^2*m1^2/m2^2)*m2/2 = 0
  => 2*J*v1.n + J^2*n^2 -2*J*v2.n + J^2*n^2*m1/m2 = 0
  => J(2*v1.n -2*v2.n) + J^2(n^2 + n^2*m1/m2) = 0 => { J != 0 }
  J = 2*(v2.n - v1.n)/(n^2(1 + m1/m2))

  
  maximum energy loss:
  ddEk/dJ = (2*v1.n + 2*J*n^2)*m1/2 + (-2*m1*v2.n + 2*J*n^2*m1^2/m2)/2 = 0
  => v1.n + J*n^2 + -v2.n + J*n^2*m1/m2 = 0 =>

                   J = (v2.n - v1.n)/(n^2*(1 + m1/m2))

  so, J_max = 2*J_min !
*/

static void mech_sprite_rot_collide(sprite_t *s1_sp, sprite_t *s2_sp, int x, int y);

void mech_sprite_collide(sprite_t *s1_sp, sprite_t *s2_sp, int x, int y)
{
  float J,l1,mratio;
  vector_t dv,n;
  /* note: for convenience's sake damage is here sometimes negative */
  int dx,dy,overlap,damage;
  mech_sprite_t *s1,*s2;  
  s1 = (mech_sprite_t *)s1_sp;
  s2 = (mech_sprite_t *)s2_sp;
  /* Handle immovable objects */
  if (s1->rmass == 0)
    {
      if (s2->rmass == 0)
	goto react;
      else
	return mech_sprite_collide(s2_sp,s1_sp,x,y);
    }  
  if (s1->rinmom > 0 && s1->rmass > 0)
    return mech_sprite_rot_collide(s1_sp,s2_sp,x,y);
  else if (s2->rinmom > 0 && s2->rmass > 0)
    return mech_sprite_rot_collide(s2_sp,s1_sp,x,y);
  /* Get normal and overlapping area */
  overlap = coresprite_overlap_normal(&s1_sp->base,&s2_sp->base,&dx,&dy);
  if (dx == 0 && dy == 0)
    goto react; /* Not so much to do here; one sprite sits inside another. */
  n.x = dx; 
  n.y = dy;   
  dv = vsub(s2->sprite.vel,s1->sprite.vel);
  mratio = s2->rmass/s1->rmass;
  J = vdot(dv,n)/(vdot(n,n)*(1 + mratio));
  if (J > 0) /* No attractive forces allowed here */
    {       
      /* Now that J is chosen for maximum energy loss, we can scale it for 
	 bouncy collisions, up to a maximum of 2*J. */
      J *= 1 + 0.5*(s1->bounce+s2->bounce); /* Realistic? */
      s1->sprite.vel = vmadd(s1->sprite.vel,n,J);
      s2->sprite.vel = vmadd(s2->sprite.vel,n,-J*mratio);
      damage = abs(J*sqrt((1 + mratio)*vdot(n,n))) - 2;
      if (damage > 0)
	{
	  sprite_msg(s1_sp,msg_set_damage(damage));
	  sprite_msg(s2_sp,msg_set_damage(damage));
	}
    }
  /* Separate the sprites to get overlap ~ 0*/
  J = -overlap/vdot(n,n);
  l1 = -J/(mratio + 1);
  sprite_set_pos(&s1->sprite, 
		 s1->sprite.pos.x + l1*n.x,
		 s1->sprite.pos.y + l1*n.y);
  sprite_set_pos(&s2->sprite, 
		 s2->sprite.pos.x + (J + l1)*n.x,
		 s2->sprite.pos.y + (J + l1)*n.y);
  /* Let the sprites react to the collision */
 react:
  {
    sprite_type_t *s1type = (sprite_type_t *)obj_type(s1_sp);
    sprite_type_t *s2type = (sprite_type_t *)obj_type(s2_sp);
    if (s1type->collide)
      s1type->collide(s1_sp,s2_sp,x,y);
    if (s2type->collide)
      s2type->collide(s2_sp,s1_sp,x,y);
  }
}

static void mech_sprite_rot_collide(sprite_t *s1_sp, sprite_t *s2_sp, int x, int y)
{
  float na1,na2,p,q,s;
  float J,l1,mratio;
  int dAdx1,dAdy1,dAda1,dAda2,overlap;
  mech_sprite_t *s1,*s2;  
  vector_t n;

  s1 = (mech_sprite_t *)s1_sp;
  s2 = (mech_sprite_t *)s2_sp;
  assert(s1->rmass > 0 && s1->rinmom > 0);

  /* Get normal and overlapping area */
  overlap = coresprite_overlap_normal(&s1_sp->base,&s2_sp->base,&dAdx1,&dAdy1);
  dAda1 = -sprite_pgrad(s1_sp,&s2_sp->base);

  if (s2->rinmom > 0)
    dAda2 = -sprite_pgrad(s2_sp,&s1_sp->base);
  else
    dAda2 = 0;

  if (dAdx1 == 0 && dAdy1 == 0)
    goto react; /* Not so much to do here; one sprite sits inside another. */

  n.x = dAdx1;
  n.y = dAdy1;
  na1 = dAda1;
  na2 = dAda2;

  p = 2*(vdot(n,s1->sprite.vel) - vdot(n,s2->sprite.vel) +
	 na1*s1->ang_vel + na2*s2->ang_vel);
  
  q = vabs2(n)*(s1->rmass + s2->rmass) + 
    sq(na1)*s1->rinmom + sq(na2)*s2->rinmom;

  if (q < 1e-6)
    s = 0;
  else
    s = -p/q*0.8;

  if (s > 0) /* No attractive forces allowed here */
    {       
      s1->sprite.vel = vmadd(s1->sprite.vel,n,s*s1->rmass);
      s2->sprite.vel = vmadd(s2->sprite.vel,n,-s*s2->rmass);
      s1->ang_vel += s*na1*s1->rinmom;
      s2->ang_vel += s*na2*s2->rinmom;

	/*
      damage = abs(J*sqrt((1 + mratio)*vdot(n,n))) - 2;
      if (damage > 0)
	{
	  sprite_msg(s1_sp,msg_set_damage(damage));
	  sprite_msg(s2_sp,msg_set_damage(damage));
	}
	*/
    }
  /* Separate the sprites to get overlap ~ 0*/
  mratio = s2->rmass/s1->rmass;
  J = -overlap/vdot(n,n);
  l1 = -J/(mratio + 1);
  sprite_set_pos(&s1->sprite, 
		 s1->sprite.pos.x + l1*n.x,
		 s1->sprite.pos.y + l1*n.y);
  sprite_set_pos(&s2->sprite, 
		 s2->sprite.pos.x + (J + l1)*n.x,
		 s2->sprite.pos.y + (J + l1)*n.y);

  /* Let the sprites react to the collision */
 react:
  {
    sprite_type_t *s1type = (sprite_type_t *)obj_type(s1_sp);
    sprite_type_t *s2type = (sprite_type_t *)obj_type(s2_sp);
    if (s1type->collide)
      s1type->collide(s1_sp,s2_sp,x,y);
    if (s2type->collide)
      s2type->collide(s2_sp,s1_sp,x,y);
  }
}

static void mech_sprite_bg_collide_rot(sprite_t *s1_sp, int x, int y)
{
  float J,na,p,q;
  vector_t n;
  int dx,dy,overlap;
  mech_sprite_t *s1 = (mech_sprite_t *)s1_sp;
  /* Handle immovable objects */
  if (s1->rmass == 0)
    goto react;

  /* Get normal and overlapping area */
  overlap = coresprite_overlap_normal(&s1_sp->base,&current_level->bg_sprite,&dx,&dy);
  
  if (dx == 0 && dy == 0)
    {
      /* The sprite sits inside the background. */
      sprite_msg(s1_sp,msg_short(MSG_BGSTUCK));
      goto react; 
    }
  n.x = dx;
  n.y = dy;
  na = -sprite_pgrad(s1_sp,&current_level->bg_sprite);

  p = 2*(vdot(n,s1->sprite.vel) +
	 na*s1->ang_vel);
  q = vabs2(n)*s1->rmass + sq(na)*s1->rinmom;

  J = -p/q*0.7;
  if (J > 0) /* No attractive forces allowed here */
    { 
      s1->sprite.vel.x += n.x*J*s1->rmass;
      s1->sprite.vel.y += n.y*J*s1->rmass;
      s1->ang_vel += na*J*s1->rinmom;
      /*
      damage = abs(J*sqrt(vdot(n,n))) - 2;
      if (damage > 0)
	sprite_msg(s1_sp,msg_set_damage(damage));
      */
    }
  
  /* Separate the sprites to get overlap ~ 0
     Fine tune this..  
  */
  
  if (overlap)
    {
      J = overlap/(vdot(n,n) + na*na);
      s1->sprite.pos = vmadd(s1->sprite.pos,n,J*3);
      s1->angle += J*3*na;
      s1->sprite.vel = vmul(s1->sprite.vel,
			    1.0f/(1 + current_level->ground_friction*overlap));
    }

  /* Let the sprites react to the collision */
 react:
  {
    sprite_type_t *s1type = (sprite_type_t *)obj_type(s1_sp);
    if (s1type->bg_collide)
      s1type->bg_collide(s1_sp,x,y);
  }
}


/* This is a copy of mech_sprite_collide with s2->rmass = 0 */
void mech_sprite_bg_collide(sprite_t *s1_sp, int x, int y)
{
  float J,l1;
  vector_t dv,n;
  /* note: for convenience's sake damage is here sometimes negative */
  int dx,dy,overlap,damage;
  mech_sprite_t *s1 = (mech_sprite_t *)s1_sp;
  if (s1->sprite.flags & SPRITE_CANROTATE)
    return mech_sprite_bg_collide_rot(s1_sp,x,y);
  /* Handle immovable objects */
  if (s1->rmass == 0)
    goto react;
  /* Get normal and overlapping area */
  overlap = coresprite_overlap_normal(&s1_sp->base,&current_level->bg_sprite,&dx,&dy);
  if (dx == 0 && dy == 0)
    {
      /* The sprite sits inside the background. */
      sprite_msg(s1_sp,msg_short(MSG_BGSTUCK));
      goto react; 
    }
  n.x = dx; 
  n.y = dy;   
  dv = vmul(s1->sprite.vel,-1);
  J = vdot(dv,n)/vdot(n,n);
  if (J > 0) /* No attractive forces allowed here */
    {       
      /* Now that J is chosen for maximum energy loss, we can scale it for 
	 bouncy collisions, up to a maximum of 2*J */
      J *= 1 + MAX(s1->bounce,current_level->bounce); /* TODO: add parameter for bg bounce */
      s1->sprite.vel.x += n.x*J;
      s1->sprite.vel.y += n.y*J;
      damage = abs(J*sqrt(vdot(n,n))) - 2;
      if (damage > 0)
	sprite_msg(s1_sp,msg_set_damage(damage));
    }
  /* Separate the sprites to get overlap ~ 0*/
  J = -overlap/vdot(n,n);
  l1 = -J;
  sprite_set_pos(&s1->sprite, 
		 s1->sprite.pos.x + l1*n.x,
		 s1->sprite.pos.y + l1*n.y);
  /* Friction */
  s1->sprite.vel = vmul(s1->sprite.vel,1.0f/(1 + current_level->ground_friction*overlap));
  /* Let the sprites react to the collision */
 react:
  {
    sprite_type_t *s1type = (sprite_type_t *)obj_type(s1_sp);
    if (s1type->bg_collide)
      s1type->bg_collide(s1_sp,x,y);
  }
}


void mech_boundary(mech_sprite_t *ms)
{
  sprite_t *s = (sprite_t *)ms;

  if (s->base.x < 0) 
    {
      sprite_set_pos(s,s->base.x + current_level->width,s->base.y);
      sprite_msg(s,msg_short(MSG_WARP));
    }
  if (s->base.y < 0)
    {
      /*fprintf(stderr, "Sprite got ABOVE TOP\n");*/
      if (s->vel.y < 0)
	s->vel.y = 0.1;
      sprite_msg(s,msg_short(MSG_OUTOFBOUNDS));
    }
  if (s->base.x > current_level->width)
    {
      sprite_set_pos(s,s->base.x - current_level->width, s->base.y);
      sprite_msg(s,msg_short(MSG_WARP));
    }
  if (s->base.y > current_level->height)
    {
      /*fprintf(stderr, "Sprite got UNDER BOTTOM\n");*/
      if (s->vel.y > 0)
	s->vel.y = -0.1;
      sprite_msg(s,msg_short(MSG_OUTOFBOUNDS));
    }
}

/* Make s follow "it" at a distance dist, using a maximum force to accelerate
   TODO: Implement better than proportional control.
*/
void mech_follow(mech_sprite_t *s, sprite_t *it, float dist, float max_force)
{
  vector_t delta = vsub(it->pos,s->sprite.pos); 
  float force, d = vabs(delta); 
  force = d - dist;
  if (fabs(force) > max_force)
    force = sgn(force)*max_force;
  s->lin_impulse = vmadd(s->lin_impulse,delta,force/d);
}

object_type_t mech_sprite_type =
{
  .name = "mech_sprite",
  .base_type = &sprite_type,
};
