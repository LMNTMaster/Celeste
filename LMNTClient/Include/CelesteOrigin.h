//using system;
//using system.collections;
//using system.collections.generic;
//using monocle;
//using microsoft.xna.framework;
//using microsoft.xna.framework.input;

#include <string>

namespace celeste
{
	class actor
	{

	};
	class particletype
	{

	};
}

namespace celeste
{
	class player : actor
	{
#pragma region constants

	public:
		static particletype p_dasha;
		static particletype p_dashb;
		static particletype p_cassettefly;
		static particletype p_split;
		static particletype p_summitlanda;
		static particletype p_summitlandb;
		static particletype p_summitlandc;

	public:
		const float maxfall = 160.0f;

	private:
		const float gravity = 900.0f;
		const float halfgravthreshold = 40.0f;
		const float fastmaxfall = 240.0f;
		const float fastmaxaccel = 300.0f;
	public:
		const float maxrun = 90.0f;
		const float runaccel = 1000.0f;

	private:
		const float runreduce = 400.0f;
		const float airmult = .65f;
		const float holdingmaxrun = 70.0f;
		const float holdmintime = .35f;
		const float bounceautojumptime = .1f;
		const float duckfriction = 500.0f;
		const int duckcorrectcheck = 4;
		const float duckcorrectslide = 50.0f;
		const float dodgeslidespeedmult = 1.2f;
		const float ducksuperjumpxmult = 1.25f;
		const float ducksuperjumpymult = .5f;

		const float jumpgracetime = 0.1f;
		const float jumpspeed = -105.0f;
		const float jumphboost = 40.0f;
		const float varjumptime = .2f;
		const float ceilingvarjumpgrace = .05f;
		const int upwardcornercorrection = 4;
		const float wallspeedretentiontime = .06f;
		const int walljumpcheckdist = 3;
		const float walljumpforcetime = .16f;
		const float walljumphspeed = maxrun + jumphboost;
	public:
		const float wallslidestartmax = 20.0f;
		
	private:
		const float wallslidetime = 1.2f;
		const float bouncevarjumptime = .2f;
		const float bouncespeed = -140.0f;
		const float superbouncevarjumptime = .2f;
		const float superbouncespeed = -185.0f;
		const float superjumpspeed = jumpspeed;
		const float superjumph = 260.0f;
		const float superwalljumpspeed = -160.0f;
		const float superwalljumpvartime = .25f;
		const float superwalljumpforcetime = .2f;
		const float superwalljumph = maxrun + jumphboost * 2;
		const float dashspeed = 240.0f;
		const float enddashspeed = 160.0f;
		const float enddashupmult = .75f;
		const float dashtime = .15f;
		const float dashcooldown = .2f;
		const float dashrefillcooldown = .1f;
		const int dashhjumpthrunudge = 6;
		const int dashcornercorrection = 4;
		const int dashvfloorsnapdist = 3;
		const float dashattacktime = .3f;
		const float boostmovespeed = 80.0f;
	public:
		const float boosttime = .25f;	
	private:
		const float duckwindmult = 0.0f;
		const int windwalldistance = 3;
		const float reboundspeedx = 120.0f;
		const float reboundspeedy = -120.0f;
		const float reboundvarjumptime = .15f;
		const float reflectboundspeed = 220.0f;
		const float dreamdashspeed = dashspeed;
		const int dreamdashendwiggle = 5;
		const float dreamdashmintime = .1f;
	public:
		const float climbmaxstamina = 110;

	private:
		const float climbupcost = 100 / 2.2f;
		const float climbstillcost = 100 / 10.0f;
		const float climbjumpcost = 110 / 4.0f;
		const int climbcheckdist = 2;
		const int climbupcheckdist = 2;
		const float climbnomovetime = .1f;
	public:
		const float climbtiredthreshold = 20.0f;

	private:
		const float climbupspeed = -45.0f;
		const float climbdownspeed = 80.0f;
		const float climbslipspeed = 30.0f;
		const float climbaccel = 900.0f;
		const float climbgrabymult = .2f;
		const float climbhopy = -120.0f;
		const float climbhopx = 100.0f;
		const float climbhopforcetime = .2f;
		const float climbjumpboosttime = .2f;
		const float climbhopnowindtime = .3f;
		const float launchspeed = 280.0f;
		const float launchcancelthreshold = 220.0f;
		const float liftycap = -130.0f;
		const float liftxcap = 250.0f;
		const float jumpthruassistspeed = -40.0f;
		const float infinitedashestime = 2.0f;
		const float infinitedashesfirsttime = .5f;
		const float flypowerflashtime = .5f;
		const float throwrecoil = 80.0f;
		static Vector2 carryoffsettarget = new Vector2(0, -12);
		const float chaserstatemaxtime = 4.0f;
		
	public:

		const float walkspeed = 64.0f;
		const int stnormal = 0;
		const int stclimb = 1;
		const int stdash = 2;
		const int stswim = 3;
		const int stboost = 4;
		const int streddash = 5;
		const int sthitsquash = 6;
		const int stlaunch = 7;
		const int stpickup = 8;
		const int stdreamdash = 9;
		const int stsummitlaunch = 10;
		const int stdummy = 11;
		const int stintrowalk = 12;
		const int stintrojump = 13;
		const int stintrorespawn = 14;
		const int stintrowakeup = 15;
		const int stbirddashtutorial = 16;
		const int stfrozen = 17;
		const int streflectionfall = 18;
		const int ststarfly = 19;
		const int sttemplefall = 20;
		const int stcassettefly = 21;
		const int stattract = 22;
		const string talksfx = "player_talk";
		
#pragma endregion
		
	public:
		vector2 speed;
		facings facing;
		playersprite sprite;
		playerhair hair;
		statemachine statemachine;
		vector2 cameraanchor;
		bool cameraanchorignorex;
		bool cameraanchorignorey;
		vector2 cameraanchorlerp;
		bool forcecameraupdate;
		leader leader;
		vertexlight light;
		int dashes;
		float stamina = climbmaxstamina;
		bool strawberriesblocked;
		vector2 previousposition;
		bool dummyautoanimate = true;
		vector2 forcestrongwindhair;
		vector2 ? overridedashdirection;
		bool flipinreflection = false;
		bool justrespawned;  // true if the player hasn't moved since respawning
		bool dead{ get; private set; };

	private:
		level level;
		collision oncollideh;
		collision oncollidev;
		bool onground;
		bool wasonground;
		int movex;
		bool flash;
		bool wasducking;
		float idletimer;
		static chooser<string> idlecoldoptions = new chooser<string>().add("idlea", 5f).add("idleb", 3f).add("idlec", 1f);
		static chooser<string> idlenobackpackoptions = new chooser<string>().add("idlea", 1f).add("idleb", 3f).add("idlec", 3f);
		static chooser<string> idlewarmoptions = new chooser<string>().add("idlea", 5f).add("idleb", 3f);
		
		public:
		public int strawberrycollectindex = 0;
		public float strawberrycollectresettimer = 0.0f;

		private hitbox hurtbox;
		private float jumpgracetimer;

		public bool autojump;
		public float autojumptimer;
		private float varjumpspeed;
		private float varjumptimer;
		private int forcemovex;
		private float forcemovextimer;
		private int hopwaitx;   // if you climb hop onto a moving solid, snap to beside it until you get above it
		private float hopwaitxspeed;
		private vector2 lastaim;
		private float dashcooldowntimer;
		private float dashrefillcooldowntimer;

		public vector2 dashdir;
		private float wallslidetimer = wallslidetime;
		private int wallslidedir;
		private float climbnomovetimer;
		private vector2 carryoffset;
		private vector2 deadoffset;
		private float introease;
		private float wallspeedretentiontimer; // if you hit a wall, start this timer. if coast is clear within this timer, retain h-speed
		private float wallspeedretained;
		private int wallboostdir;
		private float wallboosttimer;   // if you climb jump and then do a sideways input within this timer, switch to wall jump
		private float maxfall;
		private float dashattacktimer;
		private list<chaserstate> chaserstates;
		private bool wastired;
		private hashset<trigger> triggersinside;
		private float highestairy;
		private bool dashstartedonground;
		private bool fastjump;
		private int lastclimbmove;
		private float nowindtimer;
		private float dreamdashcanendtimer;
		private solid climbhopsolid;
		private vector2 climbhopsolidposition;
		private soundsource wallslidesfx;
		private soundsource swimsurfaceloopsfx;
		private float playfootsteponland;
		private float minholdtimer;
		public booster currentbooster;
		private booster lastbooster;
		private bool calleddashevents;
		private int lastdashes;
		private sprite sweatsprite;
		private int starthaircount;
		private bool launched;
		private float launchedtimer;
		private float dashtrailtimer;
		private list<chaserstatesound> activesounds = new list<chaserstatesound>();
		private fmod.studio.eventinstance idlesfx;

		private readonly hitbox normalhitbox = new hitbox(8, 11, -4, -11);
		private readonly hitbox duckhitbox = new hitbox(8, 6, -4, -6);
		private readonly hitbox normalhurtbox = new hitbox(8, 9, -4, -11);
		private readonly hitbox duckhurtbox = new hitbox(8, 4, -4, -6);
		private readonly hitbox starflyhitbox = new hitbox(8, 8, -4, -10);
		private readonly hitbox starflyhurtbox = new hitbox(6, 6, -3, -9);

		private vector2 normallightoffset = new vector2(0, -8);
		private vector2 duckinglightoffset = new vector2(0, -3);

		private list<entity> temp = new list<entity>();
		// hair
		public static readonly color normalhaircolor = calc.hextocolor("ac3232");
		public static readonly color flypowerhaircolor = calc.hextocolor("f2eb6d");
		public static readonly color usedhaircolor = calc.hextocolor("44b7ff");
		public static readonly color flashhaircolor = color.white;
		public static readonly color twodasheshaircolor = calc.hextocolor("ff6def");
		private float hairflashtimer;
		public color ? overridehaircolor;

		private vector2 winddirection;
		private float windtimeout;
		private float windhairtimer;

		// level-start intro

		public enum introtypes { transition, respawn, walkinright, walkinleft, jump, wakeup, fall, templemirrorvoid, none }
		public introtypes introtype;

		private mirrorreflection reflection;



#pragma region Vars




			#region constructor / added / removed



			public player(vector2 position, playerspritemode spritemode)

			: base(new vector2((int)position.x, (int)position.y))

		{

			depth = depths.player;

			tag = tags.persistent;



			// sprite

			sprite = new playersprite(spritemode);

			add(hair = new playerhair(sprite));

			add(sprite);

			hair.color = normalhaircolor;

			starthaircount = sprite.haircount;



			// sweat sprite

			sweatsprite = gfx.spritebank.create("player_sweat");

			add(sweatsprite);



			// physics

			collider = normalhitbox;

			hurtbox = normalhurtbox;

			oncollideh = oncollideh;

			oncollidev = oncollidev;



			// states

			statemachine = new statemachine(23);

			statemachine.setcallbacks(stnormal, normalupdate, null, normalbegin, normalend);

			statemachine.setcallbacks(stclimb, climbupdate, null, climbbegin, climbend);

			statemachine.setcallbacks(stdash, dashupdate, dashcoroutine, dashbegin, dashend);

			statemachine.setcallbacks(stswim, swimupdate, null, swimbegin, null);

			statemachine.setcallbacks(stboost, boostupdate, boostcoroutine, boostbegin, boostend);

			statemachine.setcallbacks(streddash, reddashupdate, reddashcoroutine, reddashbegin, reddashend);

			statemachine.setcallbacks(sthitsquash, hitsquashupdate, null, hitsquashbegin, null);

			statemachine.setcallbacks(stlaunch, launchupdate, null, launchbegin, null);

			statemachine.setcallbacks(stpickup, null, pickupcoroutine, null, null);

			statemachine.setcallbacks(stdreamdash, dreamdashupdate, null, dreamdashbegin, dreamdashend);

			statemachine.setcallbacks(stsummitlaunch, summitlaunchupdate, null, summitlaunchbegin, null);

			statemachine.setcallbacks(stdummy, dummyupdate, null, dummybegin, null);

			statemachine.setcallbacks(stintrowalk, null, introwalkcoroutine, null, null);

			statemachine.setcallbacks(stintrojump, null, introjumpcoroutine, null, null);

			statemachine.setcallbacks(stintrorespawn, null, null, introrespawnbegin, introrespawnend);

			statemachine.setcallbacks(stintrowakeup, null, introwakeupcoroutine, null, null);

			statemachine.setcallbacks(sttemplefall, templefallupdate, templefallcoroutine);

			statemachine.setcallbacks(streflectionfall, reflectionfallupdate, reflectionfallcoroutine, reflectionfallbegin, reflectionfallend);

			statemachine.setcallbacks(stbirddashtutorial, birddashtutorialupdate, birddashtutorialcoroutine, birddashtutorialbegin, null);

			statemachine.setcallbacks(stfrozen, frozenupdate, null, null, null);

			statemachine.setcallbacks(ststarfly, starflyupdate, starflycoroutine, starflybegin, starflyend);

			statemachine.setcallbacks(stcassettefly, cassetteflyupdate, cassetteflycoroutine, cassetteflybegin, cassetteflyend);

			statemachine.setcallbacks(stattract, attractupdate, null, attractbegin, attractend);

			add(statemachine);



			// other stuff

			add(leader = new leader(new vector2(0, -8)));

			lastaim = vector2.unitx;

			facing = facings.right;

			chaserstates = new list<chaserstate>();

			triggersinside = new hashset<trigger>();

			add(light = new vertexlight(normallightoffset, color.white, 1f, 32, 64));

			add(new waterinteraction(() = > { return statemachine.state == stdash || statemachine.state == streflectionfall; }));



			//wind

			add(new windmover(windmove));



			add(wallslidesfx = new soundsource());

			add(swimsurfaceloopsfx = new soundsource());



			sprite.onframechange = (anim) = >

			{

				if (scene != null && !dead)

				{

					// footsteps

					var frame = sprite.currentanimationframe;

					if ((anim.equals(playersprite.runcarry) && (frame == 0 || frame == 6)) ||

						(anim.equals(playersprite.runfast) && (frame == 0 || frame == 6)) ||

						(anim.equals(playersprite.runslow) && (frame == 0 || frame == 6)) ||

						(anim.equals(playersprite.walk) && (frame == 0 || frame == 6)) ||

						(anim.equals(playersprite.runstumble) && frame == 6) ||

						(anim.equals(playersprite.flip) && frame == 4) ||

						(anim.equals(playersprite.runwind) && (frame == 0 || frame == 6)) ||

						(anim.equals("idlec") && sprite.mode == playerspritemode.madelinenobackpack && (frame == 3 || frame == 6 || frame == 8 || frame == 11)) ||

						(anim.equals("carrytheowalk") && (frame == 0 || frame == 6)))

					{

						var landed = surfaceindex.getplatformbypriority(collideall<platform>(position + vector2.unity, temp));

						if (landed != null)

							play(sfxs.char_mad_footstep, surfaceindex.param, landed.getstepsoundindex(this));

					}

					// climbing (holds)

					else if ((anim.equals(playersprite.climbup) && (frame == 5)) ||

						(anim.equals(playersprite.climbdown) && (frame == 5)))

					{

						var holding = surfaceindex.getplatformbypriority(collideall<solid>(center + vector2.unitx * (int)facing, temp));

						if (holding != null)

							play(sfxs.char_mad_handhold, surfaceindex.param, holding.getwallsoundindex(this, (int)facing));

					}

					else if (anim.equals("wakeup") && frame == 19)

						play(sfxs.char_mad_campfire_stand);

					else if (anim.equals("sitdown") && frame == 12)

						play(sfxs.char_mad_summit_sit);

					else if (anim.equals("push") && (frame == 8 || frame == 15))

						dust.burstfg(position + new vector2(-(int)facing * 5, -1), new vector2(-(int)facing, -0.5f).angle(), 1, 0);

				}

			};



			sprite.onlastframe = (anim) = >

			{

				if (scene != null && !dead && sprite.currentanimationid == "idle" && !level.incutscene && idletimer > 3f)

				{

					if (calc.random.chance(0.2f))

					{

						var next = "";

						if (sprite.mode == playerspritemode.madeline)

							next = (level.coremode == session.coremodes.hot ? idlewarmoptions : idlecoldoptions).choose();

						else

							next = idlenobackpackoptions.choose();



						if (!string.isnullorempty(next))

						{

							sprite.play(next);



							if (sprite.mode == playerspritemode.madeline)

							{

								if (next == "idleb")

									idlesfx = play(sfxs.char_mad_idle_scratch);

								else if (next == "idlec")

									idlesfx = play(sfxs.char_mad_idle_sneeze);

							}

							else if (next == "idlea")

								idlesfx = play(sfxs.char_mad_idle_crackknuckles);

						}

					}

				}

			};



			// cancel special idle sounds if the anim changed

			sprite.onchange = (last, next) = >

			{

				if ((last == "idleb" || last == "idlec") && next != null && !next.startswith("idle") && idlesfx != null)

					audio.stop(idlesfx);

			};



			add(reflection = new mirrorreflection());

		}



		public override void added(scene scene)

		{

			base.added(scene);

			level = sceneas<level>();



			lastdashes = dashes = maxdashes;



			if (x > level.bounds.center.x && introtype != introtypes.none)

				facing = facings.left;



			switch (introtype)

			{

			case introtypes.respawn:

				statemachine.state = stintrorespawn;

				justrespawned = true;

				break;



			case introtypes.walkinright:

				introwalkdirection = facings.right;

				statemachine.state = stintrowalk;

				break;



			case introtypes.walkinleft:

				introwalkdirection = facings.left;

				statemachine.state = stintrowalk;

				break;



			case introtypes.jump:

				statemachine.state = stintrojump;

				break;



			case introtypes.wakeup:

				sprite.play("asleep");

				facing = facings.right;

				statemachine.state = stintrowakeup;

				break;



			case introtypes.none:

				statemachine.state = stnormal;

				break;



			case introtypes.fall:

				statemachine.state = streflectionfall;

				break;



			case introtypes.templemirrorvoid:

				starttemplemirrorvoidsleep();

				break;

			}

			introtype = introtypes.transition;



			starthair();

			previousposition = position;

		}



		public void starttemplemirrorvoidsleep()

		{

			sprite.play("asleep");

			facing = facings.right;

			statemachine.state = stdummy;

			statemachine.locked = true;

			dummyautoanimate = false;

			dummygravity = false;

		}



		public override void removed(scene scene)

		{

			base.removed(scene);

			level = null;

			audio.stop(conveyorloopsfx);



			foreach(var trigger in triggersinside)

			{

				trigger.triggered = false;

				trigger.onleave(this);

			}

		}



		public override void sceneend(scene scene)

		{

			base.sceneend(scene);

			audio.stop(conveyorloopsfx);

		}



		#endregion



			#region rendering



			public override void render()

		{

			var was = sprite.renderposition;

			sprite.renderposition = sprite.renderposition.floor();



			if (statemachine.state == stintrorespawn)

			{

				deatheffect.draw(center + deadoffset, hair.color, introease);

			}

			else

			{

				if (statemachine.state != ststarfly)

				{

					if (istired && flash)

						sprite.color = color.red;

					else

						sprite.color = color.white;

				}



				// set up scale

				if (reflection.isrendering && flipinreflection)

				{

					facing = (facings)(-(int)facing);

					hair.facing = facing;

				}

				sprite.scale.x *= (int)facing;



				// sweat scale

				if (sweatsprite.lastanimationid == "idle")

					sweatsprite.scale = sprite.scale;

				else

				{

					sweatsprite.scale.y = sprite.scale.y;

					sweatsprite.scale.x = math.abs(sprite.scale.x) * math.sign(sweatsprite.scale.x);

				}



				// draw

				base.render();



				// star fly transform

				if (sprite.currentanimationid == playersprite.startstarfly)

				{

					var p = (sprite.currentanimationframe / (float)sprite.currentanimationtotalframes);

					var white = gfx.game.getatlassubtexturesat("characters/player/startstarflywhite", sprite.currentanimationframe);

					white.draw(sprite.renderposition, sprite.origin, starflycolor * p, sprite.scale, sprite.rotation, 0);

				}



				// revert scale

				sprite.scale.x *= (int)facing;

				if (reflection.isrendering && flipinreflection)

				{

					facing = (facings)(-(int)facing);

					hair.facing = facing;

				}

			}



			sprite.renderposition = was;

		}



		public override void debugrender(camera camera)

		{

			base.debugrender(camera);



			{

				collider was = collider;

				collider = hurtbox;

				draw.hollowrect(collider, color.lime);

				collider = was;

			}

		}



		#endregion



			#region updating



			public override void update()

		{

			//infinite stamina variant

			if (savedata.instance.assistmode && savedata.instance.assists.infinitestamina)

				stamina = climbmaxstamina;



			previousposition = position;



			//vars       

			{

				// strawb reset timer

				strawberrycollectresettimer -= engine.deltatime;

				if (strawberrycollectresettimer <= 0)

					strawberrycollectindex = 0;



				// idle timer

				idletimer += engine.deltatime;

				if (level != null && level.incutscene)

					idletimer = -5;

				else if (speed.x != 0 || speed.y != 0)

					idletimer = 0;



				//underwater music

				if (!dead)

					audio.musicunderwater = underwatermusiccheck();



				//just respawned

				if (justrespawned && speed != vector2.zero)

					justrespawned = false;



				//get ground

				if (statemachine.state == stdreamdash)

					onground = onsafeground = false;

				else if (speed.y >= 0)

				{

					platform first = collidefirst<solid>(position + vector2.unity);

					if (first == null)

						first = collidefirstoutside<jumpthru>(position + vector2.unity);



					if (first != null)

					{

						onground = true;

						onsafeground = first.safe;

					}

					else

						onground = onsafeground = false;

				}

				else

					onground = onsafeground = false;



				if (statemachine.state == stswim)

					onsafeground = true;



				//safe ground blocked?

				if (onsafeground)

				{

					foreach(safegroundblocker blocker in scene.tracker.getcomponents<safegroundblocker>())

					{

						if (blocker.check(this))

						{

							onsafeground = false;

							break;

						}

					}

				}



				playfootsteponland -= engine.deltatime;



				//highest air y

				if (onground)

					highestairy = y;

				else

					highestairy = math.min(y, highestairy);



				//flashing

				if (scene.oninterval(.05f))

					flash = !flash;



				//wall slide

				if (wallslidedir != 0)

				{

					wallslidetimer = math.max(wallslidetimer - engine.deltatime, 0);

					wallslidedir = 0;

				}



				//wall boost

				if (wallboosttimer > 0)

				{

					wallboosttimer -= engine.deltatime;

					if (movex == wallboostdir)

					{

						speed.x = walljumphspeed * movex;

						stamina += climbjumpcost;

						wallboosttimer = 0;

						sweatsprite.play("idle");

					}

				}



				//after dash

				if (onground && statemachine.state != stclimb)

				{

					autojump = false;

					stamina = climbmaxstamina;

					wallslidetimer = wallslidetime;

				}



				//dash attack

				if (dashattacktimer > 0)

					dashattacktimer -= engine.deltatime;



				//jump grace

				if (onground)

				{

					dreamjump = false;

					jumpgracetimer = jumpgracetime;

				}

				else if (jumpgracetimer > 0)

					jumpgracetimer -= engine.deltatime;



				//dashes

				{

					if (dashcooldowntimer > 0)

						dashcooldowntimer -= engine.deltatime;



					if (dashrefillcooldowntimer > 0)

						dashrefillcooldowntimer -= engine.deltatime;

					else if (savedata.instance.assistmode && savedata.instance.assists.dashmode == assists.dashmodes.infinite && !level.incutscene)

						refilldash();

					else if (!inventory.norefills)

					{

						if (statemachine.state == stswim)

							refilldash();

						else if (onground)

							if (collidecheck<solid, negablock>(position + vector2.unity) || collidecheckoutside<jumpthru>(position + vector2.unity))

								if (!collidecheck<spikes>(position) || (savedata.instance.assistmode && savedata.instance.assists.invincible))

									refilldash();

					}

				}



				//var jump

				if (varjumptimer > 0)

					varjumptimer -= engine.deltatime;



				//auto jump

				if (autojumptimer > 0)

				{

					if (autojump)

					{

						autojumptimer -= engine.deltatime;

						if (autojumptimer <= 0)

							autojump = false;

					}

					else

						autojumptimer = 0;

				}



				//force move x

				if (forcemovextimer > 0)

				{

					forcemovextimer -= engine.deltatime;

					movex = forcemovex;

				}

				else

				{

					movex = input.movex.value;

					climbhopsolid = null;

				}



				//climb hop solid movement

				if (climbhopsolid != null && !climbhopsolid.collidable)

					climbhopsolid = null;

				else if (climbhopsolid != null && climbhopsolid.position != climbhopsolidposition)

				{

					var move = climbhopsolid.position - climbhopsolidposition;

					climbhopsolidposition = climbhopsolid.position;

					movehexact((int)move.x);

					movevexact((int)move.y);

				}



				//wind

				if (nowindtimer > 0)

					nowindtimer -= engine.deltatime;



				//facing

				if (movex != 0 && incontrol

					&& statemachine.state != stclimb && statemachine.state != stpickup && statemachine.state != streddash && statemachine.state != sthitsquash)

				{

					var to = (facings)movex;

					if (to != facing && ducking)

						sprite.scale = new vector2(0.8f, 1.2f);

					facing = to;

				}



				//aiming

				lastaim = input.getaimvector(facing);



				//wall speed retention

				if (wallspeedretentiontimer > 0)

				{

					if (math.sign(speed.x) == -math.sign(wallspeedretained))

						wallspeedretentiontimer = 0;

					else if (!collidecheck<solid>(position + vector2.unitx * math.sign(wallspeedretained)))

					{

						speed.x = wallspeedretained;

						wallspeedretentiontimer = 0;

					}

					else

						wallspeedretentiontimer -= engine.deltatime;

				}



				//hop wait x

				if (hopwaitx != 0)

				{

					if (math.sign(speed.x) == -hopwaitx || speed.y > 0)

						hopwaitx = 0;

					else if (!collidecheck<solid>(position + vector2.unitx * hopwaitx))

					{

						speed.x = hopwaitxspeed;

						hopwaitx = 0;

					}

				}



				// wind timeout

				if (windtimeout > 0)

					windtimeout -= engine.deltatime;



				// hair

				{

					var winddir = winddirection;

					if (forcestrongwindhair.length() > 0)

						winddir = forcestrongwindhair;



					if (windtimeout > 0 && winddir.x != 0)

					{

						windhairtimer += engine.deltatime * 8f;



						hair.steppersegment = new vector2(winddir.x * 5f, (float)math.sin(windhairtimer));

						hair.stepinfacingpersegment = 0.0f;

						hair.stepapproach = 128f;

						hair.stepysinepersegment = 0;

					}

					else if (dashes > 1)

					{

						hair.steppersegment = new vector2((float)math.sin(scene.timeactive * 2) * 0.7f - (int)facing * 3, (float)math.sin(scene.timeactive * 1f));

						hair.stepinfacingpersegment = 0.0f;

						hair.stepapproach = 90.0f;

						hair.stepysinepersegment = 1f;



						hair.steppersegment.y += winddir.y * 2f;

					}

					else

					{

						hair.steppersegment = new vector2(0, 2f);

						hair.stepinfacingpersegment = 0.5f;

						hair.stepapproach = 64f;

						hair.stepysinepersegment = 0;



						hair.steppersegment.y += winddir.y * 0.5f;

					}

				}



				if (statemachine.state == streddash)

					sprite.haircount = 1;

				else if (statemachine.state != ststarfly)

					sprite.haircount = (dashes > 1 ? 5 : starthaircount);



				//min hold time

				if (minholdtimer > 0)

					minholdtimer -= engine.deltatime;



				//launch particles

				if (launched)

				{

					var sq = speed.lengthsquared();

					if (sq < launchedminspeedsq)

						launched = false;

					else

					{

						var was = launchedtimer;

						launchedtimer += engine.deltatime;



						if (launchedtimer >= .5f)

						{

							launched = false;

							launchedtimer = 0;

						}

						else if (calc.oninterval(launchedtimer, was, .15f))

							level.add(engine.pooler.create<speedring>().init(center, speed.angle(), color.white));

					}

				}

				else

					launchedtimer = 0;

			}



			if (istired)

			{

				input.rumble(rumblestrength.light, rumblelength.short);

				if (!wastired)

				{

					wastired = true;

				}

			}

			else

				wastired = false;



			base.update();



			//light offset

			if (ducking)

				light.position = duckinglightoffset;

			else

				light.position = normallightoffset;



			//jump thru assist

			if (!onground && speed.y <= 0 && (statemachine.state != stclimb || lastclimbmove == -1) && collidecheck<jumpthru>() && !jumpthruboostblockedcheck())

				movev(jumpthruassistspeed * engine.deltatime);



			//dash floor snapping

			if (!onground && dashattacking && dashdir.y == 0)

			{

				if (collidecheck<solid>(position + vector2.unity * dashvfloorsnapdist) || collidecheckoutside<jumpthru>(position + vector2.unity * dashvfloorsnapdist))

					movevexact(dashvfloorsnapdist);

			}



			//falling unducking

			if (speed.y > 0 && canunduck && collider != starflyhitbox && !onground)

				ducking = false;



			//physics

			if (statemachine.state != stdreamdash && statemachine.state != stattract)

				moveh(speed.x * engine.deltatime, oncollideh);

			if (statemachine.state != stdreamdash && statemachine.state != stattract)

				movev(speed.y * engine.deltatime, oncollidev);



			//swimming

			if (statemachine.state == stswim)

			{

				//stay at water surface

				if (speed.y < 0 && speed.y >= swimmaxrise)

				{

					while (!swimcheck())

					{

						speed.y = 0;

						if (movevexact(1))

							break;

					}

				}

			}

			else if (statemachine.state == stnormal && swimcheck())

				statemachine.state = stswim;

			else if (statemachine.state == stclimb && swimcheck())

			{

				var water = collidefirst<water>(position);

				if (water != null && center.y < water.center.y)

				{

					while (swimcheck())

						if (movevexact(-1))

							break;

					if (swimcheck())

						statemachine.state = stswim;

				}

				else

					statemachine.state = stswim;

			}



			// wall slide sfx

			{

				var issliding = sprite.currentanimationid != null && sprite.currentanimationid.equals(playersprite.wallslide) && speed.y > 0;

				if (issliding)

				{

					if (!wallslidesfx.playing)

						loop(wallslidesfx, sfxs.char_mad_wallslide);



					var platform = surfaceindex.getplatformbypriority(collideall<solid>(center + vector2.unitx * (int)facing, temp));

					if (platform != null)

						wallslidesfx.param(surfaceindex.param, platform.getwallsoundindex(this, (int)facing));

				}

				else

					stop(wallslidesfx);

			}



			// update sprite

			updatesprite();



			//carry held item

			updatecarry();



			//triggers

			if (statemachine.state != streflectionfall)

			{

				foreach(trigger trigger in scene.tracker.getentities<trigger>())

				{

					if (collidecheck(trigger))

					{

						if (!trigger.triggered)

						{

							trigger.triggered = true;

							triggersinside.add(trigger);

							trigger.onenter(this);

						}

						trigger.onstay(this);

					}

					else if (trigger.triggered)

					{

						triggersinside.remove(trigger);

						trigger.triggered = false;

						trigger.onleave(this);

					}

				}

			}



			//strawberry block

			strawberriesblocked = collidecheck<blockfield>();



			// camera (lerp by distance using delta-time)

			if (incontrol || forcecameraupdate)

			{

				if (statemachine.state == streflectionfall)

				{

					level.camera.position = cameratarget;

				}

				else

				{

					var from = level.camera.position;

					var target = cameratarget;

					var multiplier = statemachine.state == sttemplefall ? 8 : 1f;



					level.camera.position = from + (target - from) * (1f - (float)math.pow(0.01f / multiplier, engine.deltatime));

				}

			}



			//player colliders

			if (!dead && statemachine.state != stcassettefly)

			{

				collider was = collider;

				collider = hurtbox;



				foreach(playercollider pc in scene.tracker.getcomponents<playercollider>())

				{

					if (pc.check(this) && dead)

					{

						collider = was;

						return;

					}

				}



				// if the current collider is not the hurtbox we set it to, that means a collision callback changed it. keep the new one!

				bool keepnew = (collider != hurtbox);



				if (!keepnew)

					collider = was;

			}



			//bounds

			if (incontrol && !dead && statemachine.state != stdreamdash)

				level.enforcebounds(this);



			updatechaserstates();

			updatehair(true);



			//sounds on ducking state change

			if (wasducking != ducking)

			{

				wasducking = ducking;

				if (wasducking)

					play(sfxs.char_mad_duck);

				else if (onground)

					play(sfxs.char_mad_stand);

			}



			// shallow swim sfx

			if (speed.x != 0 && ((statemachine.state == stswim && !swimunderwatercheck()) || (statemachine.state == stnormal && collidecheck<water>(position))))

			{

				if (!swimsurfaceloopsfx.playing)

					swimsurfaceloopsfx.play(sfxs.char_mad_water_move_shallow);

			}

			else

				swimsurfaceloopsfx.stop();



			wasonground = onground;

		}



		private void createtrail()

		{

			trailmanager.add(this, wasdashb ? normalhaircolor : usedhaircolor);

		}



		public void cleanuptriggers()

		{

			if (triggersinside.count > 0)

			{

				foreach(var trigger in triggersinside)

				{

					trigger.onleave(this);

					trigger.triggered = false;

				}



				triggersinside.clear();

			}

		}



		private void updatechaserstates()

		{

			while (chaserstates.count > 0 && scene.timeactive - chaserstates[0].timestamp > chaserstatemaxtime)

				chaserstates.removeat(0);

			chaserstates.add(new chaserstate(this));

			activesounds.clear();

		}



		#endregion



			#region hair & sprite



			private void starthair()

		{

			hair.facing = facing;

			hair.start();

			updatehair(true);

		}



		public void updatehair(bool applygravity)

		{

			// color

			if (statemachine.state == ststarfly)

			{

				hair.color = sprite.color;

				applygravity = false;

			}

			else if (dashes == 0 && dashes < maxdashes)

				hair.color = color.lerp(hair.color, usedhaircolor, 6f * engine.deltatime);

			else

			{

				color color;

				if (lastdashes != dashes)

				{

					color = flashhaircolor;

					hairflashtimer = .12f;

				}

				else if (hairflashtimer > 0)

				{

					color = flashhaircolor;

					hairflashtimer -= engine.deltatime;

				}

				else if (dashes == 2)

					color = twodasheshaircolor;

				else

					color = normalhaircolor;



				hair.color = color;

			}



			if (overridehaircolor != null)

				hair.color = overridehaircolor.value;



			hair.facing = facing;

			hair.simulatemotion = applygravity;

			lastdashes = dashes;

		}



		private void updatesprite()

		{

			//tween

			sprite.scale.x = calc.approach(sprite.scale.x, 1f, 1.75f * engine.deltatime);

			sprite.scale.y = calc.approach(sprite.scale.y, 1f, 1.75f * engine.deltatime);



			//animation

			if (incontrol && sprite.currentanimationid != playersprite.throw && statemachine.state != sttemplefall &&

				statemachine.state != streflectionfall && statemachine.state != ststarfly && statemachine.state != stcassettefly)

			{

				if (statemachine.state == stattract)

				{

					sprite.play(playersprite.fallfast);

				}

				else if (statemachine.state == stsummitlaunch)

				{

					sprite.play(playersprite.launch);

				}

				// picking up

				else if (statemachine.state == stpickup)

				{

					sprite.play(playersprite.pickup);

				}

				// swiming

				else if (statemachine.state == stswim)

				{

					if (input.movey.value > 0)

						sprite.play(playersprite.swimdown);

					else if (input.movey.value < 0)

						sprite.play(playersprite.swimup);

					else

						sprite.play(playersprite.swimidle);

				}

				// dream dashing

				else if (statemachine.state == stdreamdash)

				{

					if (sprite.currentanimationid != playersprite.dreamdashin && sprite.currentanimationid != playersprite.dreamdashloop)

						sprite.play(playersprite.dreamdashin);

				}

				else if (sprite.dreamdashing && sprite.lastanimationid != playersprite.dreamdashout)

				{

					sprite.play(playersprite.dreamdashout);

				}

				else if (sprite.currentanimationid != playersprite.dreamdashout)

				{

					// during dash

					if (dashattacking)

					{

						if (onground && dashdir.y == 0 && !ducking && speed.x != 0 && movex == -math.sign(speed.x))

						{

							if (scene.oninterval(.02f))

								dust.burst(position, calc.up, 1);

							sprite.play(playersprite.skid);

						}

						else

							sprite.play(playersprite.dash);

					}

					// climbing

					else if (statemachine.state == stclimb)

					{

						if (lastclimbmove < 0)

							sprite.play(playersprite.climbup);

						else if (lastclimbmove > 0)

							sprite.play(playersprite.wallslide);

						else if (!collidecheck<solid>(position + new vector2((int)facing, 6)))

							sprite.play(playersprite.dangling);

						else if (input.movex == -(int)facing)

						{

							if (sprite.currentanimationid != playersprite.climblookback)

								sprite.play(playersprite.climblookbackstart);

						}

						else

							sprite.play(playersprite.wallslide);

					}

					// ducking

					else if (ducking && statemachine.state == stnormal)

					{

						sprite.play(playersprite.duck);

					}

					else if (onground)

					{

						fastjump = false;

						if (holding == null && movex != 0 && collidecheck<solid>(position + vector2.unitx * movex))

						{

							sprite.play("push");

						}

						else if (math.abs(speed.x) <= runaccel / 40f && movex == 0)

						{

							if (holding != null)

							{

								sprite.play(playersprite.idlecarry);

							}

							else if (!scene.collidecheck<solid>(position + new vector2((int)facing * 1, 2)) && !scene.collidecheck<solid>(position + new vector2((int)facing * 4, 2)) && !collidecheck<jumpthru>(position + new vector2((int)facing * 4, 2)))

							{

								sprite.play(playersprite.frontedge);

							}

							else if (!scene.collidecheck<solid>(position + new vector2(-(int)facing * 1, 2)) && !scene.collidecheck<solid>(position + new vector2(-(int)facing * 4, 2)) && !collidecheck<jumpthru>(position + new vector2(-(int)facing * 4, 2)))

							{

								sprite.play("edgeback");

							}

							else if (input.movey.value == -1)

							{

								if (sprite.lastanimationid != playersprite.lookup)

									sprite.play(playersprite.lookup);

							}

							else

							{

								if (sprite.currentanimationid != null && !sprite.currentanimationid.contains("idle"))

									sprite.play(playersprite.idle);

							}

						}

						else if (holding != null)

						{

							sprite.play(playersprite.runcarry);

						}

						else if (math.sign(speed.x) == -movex && movex != 0)

						{

							if (math.abs(speed.x) > maxrun)

								sprite.play(playersprite.skid);

							else if (sprite.currentanimationid != playersprite.skid)

								sprite.play(playersprite.flip);

						}

						else if (winddirection.x != 0 && windtimeout > 0f && (int)facing == -math.sign(winddirection.x))

						{

							sprite.play(playersprite.runwind);

						}

						else if (!sprite.running)

						{

							if (math.abs(speed.x) < maxrun * .5f)

								sprite.play(playersprite.runslow);

							else

								sprite.play(playersprite.runfast);

						}

					}

					// wall sliding

					else if (wallslidedir != 0 && holding == null)

					{

						sprite.play(playersprite.wallslide);

					}

					// jumping up

					else if (speed.y < 0)

					{

						if (holding != null)

						{

							sprite.play(playersprite.jumpcarry);

						}

						else if (fastjump || math.abs(speed.x) > maxrun)

						{

							fastjump = true;

							sprite.play(playersprite.jumpfast);

						}

						else

							sprite.play(playersprite.jumpslow);

					}

					// falling down

					else

					{

						if (holding != null)

						{

							sprite.play(playersprite.fallcarry);

						}

						else if (fastjump || speed.y >= maxfall || level.inspace)

						{

							fastjump = true;

							if (sprite.lastanimationid != playersprite.fallfast)

								sprite.play(playersprite.fallfast);

						}

						else

							sprite.play(playersprite.fallslow);

					}

				}

			}



			if (statemachine.state != player.stdummy)

			{

				if (level.inspace)

					sprite.rate = .5f;

				else

					sprite.rate = 1f;

			}

		}



		public void createsplitparticles()

		{

			level.particles.emit(p_split, 16, center, vector2.one * 6);

		}



		#endregion



			#region getters



			public vector2 cameratarget

		{

			get

		{

			vector2 at = new vector2();

		vector2 target = new vector2(x - celeste.gamewidth / 2, y - celeste.gameheight / 2);

		if (statemachine.state != streflectionfall)

			target += new vector2(level.cameraoffset.x, level.cameraoffset.y);



		if (statemachine.state == ststarfly)

		{

			target.x += .2f * speed.x;

			target.y += .2f * speed.y;

		}

		else if (statemachine.state == streddash)

		{

			target.x += 48 * math.sign(speed.x);

			target.y += 48 * math.sign(speed.y);

		}

		else if (statemachine.state == stsummitlaunch)

		{

			target.y -= 64;

		}

		else if (statemachine.state == streflectionfall)

		{

			target.y += 32;

		}



		if (cameraanchorlerp.length() > 0)

		{

			if (cameraanchorignorex && !cameraanchorignorey)

				target.y = mathhelper.lerp(target.y, cameraanchor.y, cameraanchorlerp.y);

			else if (!cameraanchorignorex && cameraanchorignorey)

				target.x = mathhelper.lerp(target.x, cameraanchor.x, cameraanchorlerp.x);

			else if (cameraanchorlerp.x == cameraanchorlerp.y)

				target = vector2.lerp(target, cameraanchor, cameraanchorlerp.x);

			else

			{

				target.x = mathhelper.lerp(target.x, cameraanchor.x, cameraanchorlerp.x);

				target.y = mathhelper.lerp(target.y, cameraanchor.y, cameraanchorlerp.y);

			}

		}



		at.x = mathhelper.clamp(target.x, level.bounds.left, level.bounds.right - celeste.gamewidth);

		at.y = mathhelper.clamp(target.y, level.bounds.top, level.bounds.bottom - celeste.gameheight);



		if (level.cameralockmode != level.cameralockmodes.none)

		{

			var locker = scene.tracker.getcomponent<cameralocker>();



			//x snapping

			if (level.cameralockmode != level.cameralockmodes.boostsequence)

			{

				at.x = math.max(at.x, level.camera.x);

				if (locker != null)

					at.x = math.min(at.x, math.max(level.bounds.left, locker.entity.x - locker.maxxoffset));

			}



			//y snapping

			if (level.cameralockmode == level.cameralockmodes.finalboss)

			{

				at.y = math.max(at.y, level.camera.y);

				if (locker != null)

					at.y = math.min(at.y, math.max(level.bounds.top, locker.entity.y - locker.maxyoffset));

			}

			else if (level.cameralockmode == level.cameralockmodes.boostsequence)

			{

				level.cameraupwardmaxy = math.min(level.camera.y + cameralocker.upwardmaxyoffset, level.cameraupwardmaxy);

				at.y = math.min(at.y, level.cameraupwardmaxy);

				if (locker != null)

					at.y = math.max(at.y, math.min(level.bounds.bottom - 180, locker.entity.y - locker.maxyoffset));

			}

		}



		// snap above killboxes

		var killboxes = scene.tracker.getentities<killbox>();

		foreach(var box in killboxes)

		{

			if (!box.collidable)

				continue;



			if (top < box.bottom && right > box.left && left < box.right)

				at.y = math.min(at.y, box.top - 180);

		}



		return at;

		}

		}



			public bool getchaseposition(float scenetime, float timeago, out chaserstate chasestate)

		{

			if (!dead)

			{

				bool toolongagofound = false;

				foreach(var state in chaserstates)

				{

					float time = scenetime - state.timestamp;

					if (time <= timeago)

					{

						if (toolongagofound || timeago - time < .02f)

						{

							chasestate = state;

							return true;

						}

						else

						{

							chasestate = new chaserstate();

							return false;

						}

					}

					else

						toolongagofound = true;

				}

			}



			chasestate = new chaserstate();

			return false;

		}



		public bool canretry

		{

			get

		{

			switch (statemachine.state)

			{

			default:

				return true;



			case stintrojump:

			case stintrowalk:

			case stintrowakeup:

			case stintrorespawn:

			case streflectionfall:

				return false;

			}

		}

		}



			public bool timepaused

		{

			get

		{

			if (dead)

			return true;



		switch (statemachine.state)

		{

		default:

			return false;



		case stintrojump:

		case stintrowalk:

		case stintrowakeup:

		case stintrorespawn:

		case stsummitlaunch:

			return true;

		}

		}

		}



			public bool incontrol

		{

			get

		{

			switch (statemachine.state)

			{

			default:

				return true;



			case stintrojump:

			case stintrowalk:

			case stintrowakeup:

			case stintrorespawn:

			case stdummy:

			case stfrozen:

			case stbirddashtutorial:

				return false;

			}

		}

		}



			public playerinventory inventory

		{

			get

		{

			if (level != null && level.session != null)

			return level.session.inventory;

		return playerinventory.default;

		}

		}



			#endregion



			#region transitions



			public void ontransition()

		{

			wallslidetimer = wallslidetime;

			jumpgracetimer = 0;

			forcemovextimer = 0;

			chaserstates.clear();

			refilldash();

			refillstamina();



			leader.transferfollowers();

		}



		public bool transitionto(vector2 target, vector2 direction)

		{

			movetowardsx(target.x, 60f * engine.deltatime);

			movetowardsy(target.y, 60f * engine.deltatime);



			//update hair because the normal update loop is not firing right now!

			updatehair(false);

			updatecarry();



			//returns true when transition is complete

			if (position == target)

			{

				zeroremainderx();

				zeroremaindery();

				speed.x = (int)math.round(speed.x);

				speed.y = (int)math.round(speed.y);

				return true;

			}

			else

				return false;

		}



		public void beforesidetransition()

		{



		}



		public void beforedowntransition()

		{

			if (statemachine.state != streddash && statemachine.state != streflectionfall && statemachine.state != ststarfly)

			{

				statemachine.state = stnormal;



				speed.y = math.max(0, speed.y);

				autojump = false;

				varjumptimer = 0;

			}



			foreach(var platform in scene.tracker.getentities<platform>())

				if (!(platform is solidtiles) && collidecheckoutside(platform, position + vector2.unity * height))

					platform.collidable = false;

		}



		public void beforeuptransition()

		{

			speed.x = 0;



			if (statemachine.state != streddash && statemachine.state != streflectionfall && statemachine.state != ststarfly)

			{

				varjumpspeed = speed.y = jumpspeed;



				if (statemachine.state == stsummitlaunch)

					statemachine.state = stintrojump;

				else

					statemachine.state = stnormal;



				autojump = true;

				autojumptimer = 0;

				varjumptimer = varjumptime;

			}



			dashcooldowntimer = 0.2f;

		}



		#endregion



			#region jumps 'n' stuff



			public bool onsafeground

		{

			get; private set;

		}



			public bool loseshards

		{

			get

		{

			return onground;

		}

		}



		private const float launchedboostcheckspeedsq = 100 * 100;

		private const float launchedjumpcheckspeedsq = 220 * 220;

		private const float launchedminspeedsq = 140 * 140;

		private const float launcheddoublespeedsq = 150 * 150;



		private bool launchedboostcheck()

		{

			if (liftboost.lengthsquared() >= launchedboostcheckspeedsq && speed.lengthsquared() >= launchedjumpcheckspeedsq)

			{

				launched = true;

				return true;

			}

			else

			{

				launched = false;

				return false;

			}

		}



		public void jump(bool particles = true, bool playsfx = true)

		{

			input.jump.consumebuffer();

			jumpgracetimer = 0;

			varjumptimer = varjumptime;

			autojump = false;

			dashattacktimer = 0;

			wallslidetimer = wallslidetime;

			wallboosttimer = 0;



			speed.x += jumphboost * movex;

			speed.y = jumpspeed;

			speed += liftboost;

			varjumpspeed = speed.y;



			launchedboostcheck();



			if (playsfx)

			{

				if (launched)

					play(sfxs.char_mad_jump_assisted);



				if (dreamjump)

					play(sfxs.char_mad_jump_dreamblock);

				else

					play(sfxs.char_mad_jump);

			}



			sprite.scale = new vector2(.6f, 1.4f);

			if (particles)

				dust.burst(bottomcenter, calc.up, 4);



			savedata.instance.totaljumps++;

		}



		private void superjump()

		{

			input.jump.consumebuffer();

			jumpgracetimer = 0;

			varjumptimer = varjumptime;

			autojump = false;

			dashattacktimer = 0;

			wallslidetimer = wallslidetime;

			wallboosttimer = 0;



			speed.x = superjumph * (int)facing;

			speed.y = jumpspeed;

			speed += liftboost;



			play(sfxs.char_mad_jump);



			if (ducking)

			{

				ducking = false;

				speed.x *= ducksuperjumpxmult;

				speed.y *= ducksuperjumpymult;

				play(sfxs.char_mad_jump_superslide);

			}

			else

				play(sfxs.char_mad_jump_super);



			varjumpspeed = speed.y;

			launched = true;



			sprite.scale = new vector2(.6f, 1.4f);



			dust.burst(bottomcenter, calc.up, 4);



			savedata.instance.totaljumps++;

		}



		private bool walljumpcheck(int dir)

		{

			return climbboundscheck(dir) && collidecheck<solid>(position + vector2.unitx * dir * walljumpcheckdist);

		}



		private void walljump(int dir)

		{

			ducking = false;

			input.jump.consumebuffer();

			jumpgracetimer = 0;

			varjumptimer = varjumptime;

			autojump = false;

			dashattacktimer = 0;

			wallslidetimer = wallslidetime;

			wallboosttimer = 0;

			if (movex != 0)

			{

				forcemovex = dir;

				forcemovextimer = walljumpforcetime;

			}



			//get lift of wall jumped off of

			if (liftspeed == vector2.zero)

			{

				solid wall = collidefirst<solid>(position + vector2.unitx * walljumpcheckdist);

				if (wall != null)

					liftspeed = wall.liftspeed;

			}



			speed.x = walljumphspeed * dir;

			speed.y = jumpspeed;

			speed += liftboost;

			varjumpspeed = speed.y;



			launchedboostcheck();



			// wall-sound?

			var pushoff = surfaceindex.getplatformbypriority(collideall<platform>(position - vector2.unitx * dir * 4, temp));

			if (pushoff != null)

				play(sfxs.char_mad_land, surfaceindex.param, pushoff.getwallsoundindex(this, -dir));



			// jump sfx

			play(dir < 0 ? sfxs.char_mad_jump_wall_right : sfxs.char_mad_jump_wall_left);

			sprite.scale = new vector2(.6f, 1.4f);



			if (dir == -1)

				dust.burst(center + vector2.unitx * 2, calc.upleft, 4);

			else

				dust.burst(center + vector2.unitx * -2, calc.upright, 4);



			savedata.instance.totalwalljumps++;

		}



		private void superwalljump(int dir)

		{

			ducking = false;

			input.jump.consumebuffer();

			jumpgracetimer = 0;

			varjumptimer = superwalljumpvartime;

			autojump = false;

			dashattacktimer = 0;

			wallslidetimer = wallslidetime;

			wallboosttimer = 0;



			speed.x = superwalljumph * dir;

			speed.y = superwalljumpspeed;

			speed += liftboost;

			varjumpspeed = speed.y;

			launched = true;



			play(dir < 0 ? sfxs.char_mad_jump_wall_right : sfxs.char_mad_jump_wall_left);

			play(sfxs.char_mad_jump_superwall);

			sprite.scale = new vector2(.6f, 1.4f);



			if (dir == -1)

				dust.burst(center + vector2.unitx * 2, calc.upleft, 4);

			else

				dust.burst(center + vector2.unitx * -2, calc.upright, 4);



			savedata.instance.totalwalljumps++;

		}



		private void climbjump()

		{

			if (!onground)

			{

				stamina -= climbjumpcost;



				sweatsprite.play("jump", true);

				input.rumble(rumblestrength.light, rumblelength.medium);

			}



			dreamjump = false;

			jump(false, false);



			if (movex == 0)

			{

				wallboostdir = -(int)facing;

				wallboosttimer = climbjumpboosttime;

			}



			if (facing == facings.right)

			{

				play(sfxs.char_mad_jump_climb_right);

				dust.burst(center + vector2.unitx * 2, calc.upleft, 4);

			}

			else

			{

				play(sfxs.char_mad_jump_climb_left);

				dust.burst(center + vector2.unitx * -2, calc.upright, 4);

			}

		}



		public void bounce(float fromy)

		{

			if (statemachine.state == stboost && currentbooster != null)

			{

				currentbooster.playerreleased();

				currentbooster = null;

			}



			var was = collider;

			collider = normalhitbox;

			{

				movevexact((int)(fromy - bottom));

				if (!inventory.norefills)

					refilldash();

				refillstamina();

				statemachine.state = stnormal;



				jumpgracetimer = 0;

				varjumptimer = bouncevarjumptime;

				autojump = true;

				autojumptimer = bounceautojumptime;

				dashattacktimer = 0;

				wallslidetimer = wallslidetime;

				wallboosttimer = 0;



				varjumpspeed = speed.y = bouncespeed;

				launched = false;



				input.rumble(rumblestrength.light, rumblelength.medium);

				sprite.scale = new vector2(.6f, 1.4f);

			}

			collider = was;

		}



		public void superbounce(float fromy)

		{

			if (statemachine.state == stboost && currentbooster != null)

			{

				currentbooster.playerreleased();

				currentbooster = null;

			}



			var was = collider;

			collider = normalhitbox;



			{

				movev(fromy - bottom);

				if (!inventory.norefills)

					refilldash();

				refillstamina();

				statemachine.state = stnormal;



				jumpgracetimer = 0;

				varjumptimer = superbouncevarjumptime;

				autojump = true;

				autojumptimer = 0;

				dashattacktimer = 0;

				wallslidetimer = wallslidetime;

				wallboosttimer = 0;



				speed.x = 0;

				varjumpspeed = speed.y = superbouncespeed;

				launched = false;



				level.directionalshake(-vector2.unity, .1f);

				input.rumble(rumblestrength.medium, rumblelength.medium);

				sprite.scale = new vector2(.5f, 1.5f);

			}



			collider = was;

		}



		private const float sidebouncespeed = 240;

		private const float sidebounceforcemovextime = .3f;



		public void sidebounce(int dir, float fromx, float fromy)

		{

			var was = collider;

			collider = normalhitbox;

			{

				movev(calc.clamp(fromy - bottom, -4, 4));

				if (dir > 0)

					moveh(fromx - left);

				else if (dir < 0)

					moveh(fromx - right);

				if (!inventory.norefills)

					refilldash();

				refillstamina();

				statemachine.state = stnormal;



				jumpgracetimer = 0;

				varjumptimer = bouncevarjumptime;

				autojump = true;

				autojumptimer = 0;

				dashattacktimer = 0;

				wallslidetimer = wallslidetime;

				forcemovex = dir;

				forcemovextimer = sidebounceforcemovextime;

				wallboosttimer = 0;

				launched = false;



				speed.x = sidebouncespeed * dir;

				varjumpspeed = speed.y = bouncespeed;



				level.directionalshake(vector2.unitx * dir, .1f);

				input.rumble(rumblestrength.medium, rumblelength.medium);

				sprite.scale = new vector2(1.5f, .5f);

			}

			collider = was;

		}



		public void rebound(int direction = 0)

		{

			speed.x = direction * reboundspeedx;

			speed.y = reboundspeedy;

			varjumpspeed = speed.y;

			varjumptimer = reboundvarjumptime;

			autojump = true;

			autojumptimer = 0;

			dashattacktimer = 0;

			wallslidetimer = wallslidetime;

			wallboosttimer = 0;

			launched = false;



			dashattacktimer = 0;

			forcemovextimer = 0;

			statemachine.state = stnormal;

		}



		public void reflectbounce(vector2 direction)

		{

			if (direction.x != 0)

				speed.x = direction.x * reflectboundspeed;

			if (direction.y != 0)

				speed.y = direction.y * reflectboundspeed;



			autojumptimer = 0;

			dashattacktimer = 0;

			wallslidetimer = wallslidetime;

			wallboosttimer = 0;

			launched = false;



			dashattacktimer = 0;

			forcemovextimer = 0;

			statemachine.state = stnormal;

		}



		public int maxdashes

		{

			get

		{

			if (savedata.instance.assistmode && savedata.instance.assists.dashmode != assists.dashmodes.normal && !level.incutscene)

			return 2;

			else

				return inventory.dashes;

		}

		}



			public bool refilldash()

		{

			if (dashes < maxdashes)

			{

				dashes = maxdashes;

				return true;

			}

			else

				return false;

		}



		public bool userefill()

		{

			if (dashes < maxdashes || stamina < climbtiredthreshold)

			{

				dashes = maxdashes;

				refillstamina();

				return true;

			}

			else

				return false;

		}



		public void refillstamina()

		{

			stamina = climbmaxstamina;

		}



		public playerdeadbody die(vector2 direction, bool evenifinvincible = false, bool registerdeathinstats = true)

		{

			var session = level.session;

			bool invincible = (!evenifinvincible && savedata.instance.assistmode && savedata.instance.assists.invincible);



			if (!dead && !invincible && statemachine.state != streflectionfall)

			{

				stop(wallslidesfx);



				if (registerdeathinstats)

				{

					session.deaths++;

					session.deathsincurrentlevel++;

					savedata.instance.adddeath(session.area);

				}



				// has gold strawb?

				strawberry goldenstrawb = null;

				foreach(var strawb in leader.followers)

					if (strawb.entity is strawberry && (strawb.entity as strawberry).golden && !(strawb.entity as strawberry).winged)

						goldenstrawb = (strawb.entity as strawberry);



				dead = true;

				leader.losefollowers();

				depth = depths.top;

				speed = vector2.zero;

				statemachine.locked = true;

				collidable = false;

				drop();



				if (lastbooster != null)

					lastbooster.playerdied();



				level.incutscene = false;

				level.shake();

				input.rumble(rumblestrength.light, rumblelength.medium);



				var body = new playerdeadbody(this, direction);

				if (goldenstrawb != null)

				{

					body.deathaction = () = >

					{

						var exit = new levelexit(levelexit.mode.goldenberryrestart, session);

						exit.goldenstrawberryentrylevel = goldenstrawb.id.level;

						engine.scene = exit;

					};

				}



				scene.add(body);

				scene.remove(this);



				var lookout = scene.tracker.getentity<lookout>();

				if (lookout != null)

					lookout.stopinteracting();



				return body;

			}



			return null;

		}



		private vector2 liftboost

		{

			get

		{

			vector2 val = liftspeed;



		if (math.abs(val.x) > liftxcap)

			val.x = liftxcap * math.sign(val.x);



		if (val.y > 0)

			val.y = 0;

		else if (val.y < liftycap)

			val.y = liftycap;



		return val;

		}

		}



			#endregion



			#region ducking



			public bool ducking

		{

			get

		{

			return collider == duckhitbox || collider == duckhurtbox;

		}



			set

		{

			if (value)

			{

				collider = duckhitbox;

				hurtbox = duckhurtbox;



			}

			else

			{

				collider = normalhitbox;

				hurtbox = normalhurtbox;

			}

		}

		}



			public bool canunduck

		{

			get

		{

			if (!ducking)

			return true;



		collider was = collider;

		collider = normalhitbox;

		bool ret = !collidecheck<solid>();

		collider = was;

		return ret;

		}

		}



			public bool canunduckat(vector2 at)

		{

			vector2 was = position;

			position = at;

			bool ret = canunduck;

			position = was;

			return ret;

		}



		public bool duckfreeat(vector2 at)

		{

			vector2 oldp = position;

			collider oldc = collider;

			position = at;

			collider = duckhitbox;



			bool ret = !collidecheck<solid>();



			position = oldp;

			collider = oldc;



			return ret;

		}



		private void duck()

		{

			collider = duckhitbox;

		}



		private void unduck()

		{

			collider = normalhitbox;

		}



		#endregion



			#region holding



			public holdable holding

		{

			get; set;

		}



			public void updatecarry()

		{

			if (holding != null)

			{

				// don't hold stuff that doesn't exist anymore

				if (holding.scene == null)

					holding = null;

				else

					holding.carry(position + carryoffset + vector2.unity * sprite.carryyoffset);

			}

		}



		public void swat(int dir)

		{

			if (holding != null)

			{

				holding.release(new vector2(.8f * dir, -.25f));

				holding = null;

			}

		}



		private bool pickup(holdable pickup)

		{

			if (pickup.pickup(this))

			{

				ducking = false;

				holding = pickup;

				minholdtimer = holdmintime;

				return true;

			}

			else

				return false;

		}



		private void throw()

		{

			if (holding != null)

			{

				if (input.movey.value == 1)

					drop();

				else

				{

					input.rumble(rumblestrength.strong, rumblelength.short);

					holding.release(vector2.unitx * (int)facing);

					speed.x += throwrecoil * -(int)facing;

				}



				play(sfxs.char_mad_crystaltheo_throw);

				sprite.play("throw");

				holding = null;

			}

		}



		private void drop()

		{

			if (holding != null)

			{

				input.rumble(rumblestrength.light, rumblelength.short);

				holding.release(vector2.zero);

				holding = null;

			}

		}



		#endregion



			#region physics



			public void startjumpgracetime()

		{

			jumpgracetimer = jumpgracetime;

		}



		public override bool isriding(solid solid)

		{

			if (statemachine.state == stdreamdash)

				return collidecheck(solid);



			if (statemachine.state == stclimb || statemachine.state == sthitsquash)

				return collidecheck(solid, position + vector2.unitx * (int)facing);



			return base.isriding(solid);

		}



		public override bool isriding(jumpthru jumpthru)

		{

			if (statemachine.state == stdreamdash)

				return false;



			return statemachine.state != stclimb && speed.y >= 0 && base.isriding(jumpthru);

		}



		public bool bouncecheck(float y)

		{

			return bottom <= y + 3;

		}



		public void pointbounce(vector2 from)

		{

			const float bouncespeed = 200.0f;

			const float minx = 120.0f;



			if (statemachine.state == stboost && currentbooster != null)

				currentbooster.playerreleased();



			refilldash();

			refillstamina();

			speed = (center - from).safenormalize(bouncespeed);

			speed.x *= 1.2f;



			if (math.abs(speed.x) < minx)

			{

				if (speed.x == 0)

					speed.x = -(int)facing * minx;

				else

					speed.x = math.sign(speed.x) * minx;

			}

		}



		private void windmove(vector2 move)

		{

			if (!justrespawned && nowindtimer <= 0 && incontrol && statemachine.state != stboost && statemachine.state != stdash && statemachine.state != stsummitlaunch)

			{

				//horizontal

				if (move.x != 0 && statemachine.state != stclimb)

				{

					windtimeout = 0.2f;

					winddirection.x = math.sign(move.x);



					if (!collidecheck<solid>(position + vector2.unitx * -math.sign(move.x) * windwalldistance))

					{

						if (ducking && onground)

							move.x *= duckwindmult;



						if (move.x < 0)

							move.x = math.max(move.x, level.bounds.left - (exactposition.x + collider.left));

						else

							move.x = math.min(move.x, level.bounds.right - (exactposition.x + collider.right));



						moveh(move.x);

					}

				}



				//vertical

				if (move.y != 0)

				{

					windtimeout = 0.2f;

					winddirection.y = math.sign(move.y);



					if (speed.y < 0 || !onground())

					{



						if (statemachine.state == stclimb)

						{

							if (move.y > 0 && climbnomovetimer <= 0)

								move.y *= .4f;

							else

								return;

						}



						movev(move.y);

					}

				}

			}

		}



		private void oncollideh(collisiondata data)

		{

			if (statemachine.state == ststarfly)

			{

				if (starflytimer < starflyendnobouncetime)

					speed.x = 0;

				else

				{

					play(sfxs.game_06_feather_state_bump);

					input.rumble(rumblestrength.light, rumblelength.medium);

					speed.x *= starflywallbounce;

				}

				return;

			}



			if (statemachine.state == stdreamdash)

				return;



			//dash blocks

			if (dashattacking && data.hit != null && data.hit.ondashcollide != null && data.direction.x == math.sign(dashdir.x))

			{

				var result = data.hit.ondashcollide(this, data.direction);

				if (statemachine.state == streddash)

					result = dashcollisionresults.ignore;



				if (result == dashcollisionresults.rebound)

				{

					rebound(-math.sign(speed.x));

					return;

				}

				else if (result == dashcollisionresults.bounce)

				{

					reflectbounce(new vector2(-math.sign(speed.x), 0));

					return;

				}

				else if (result == dashcollisionresults.ignore)

					return;

			}



			//dash corner correction

			if (statemachine.state == stdash || statemachine.state == streddash)

			{

				if (onground && duckfreeat(position + vector2.unitx * math.sign(speed.x)))

				{

					ducking = true;

					return;

				}

				else if (speed.y == 0 && speed.x != 0)

				{

					for (int i = 1; i <= dashcornercorrection; i++)

					{

						for (int j = 1; j >= -1; j -= 2)

						{

							if (!collidecheck<solid>(position + new vector2(math.sign(speed.x), i * j)))

							{

								movevexact(i * j);

								movehexact(math.sign(speed.x));

								return;

							}

						}

					}

				}

			}



			//dream dash

			if (dreamdashcheck(vector2.unitx * math.sign(speed.x)))

			{

				statemachine.state = stdreamdash;

				dashattacktimer = 0;

				return;

			}



			//speed retention

			if (wallspeedretentiontimer <= 0)

			{

				wallspeedretained = speed.x;

				wallspeedretentiontimer = wallspeedretentiontime;

			}



			//collide event

			if (data.hit != null && data.hit.oncollide != null)

				data.hit.oncollide(data.direction);



			speed.x = 0;

			dashattacktimer = 0;



			if (statemachine.state == streddash)

			{

				input.rumble(rumblestrength.medium, rumblelength.short);

				level.displacement.addburst(center, .5f, 8, 48, .4f, ease.quadout, ease.quadout);

				statemachine.state = sthitsquash;

			}

		}



		private void oncollidev(collisiondata data)

		{

			if (statemachine.state == ststarfly)

			{

				if (starflytimer < starflyendnobouncetime)

					speed.y = 0;

				else

				{

					play(sfxs.game_06_feather_state_bump);

					input.rumble(rumblestrength.light, rumblelength.medium);

					speed.y *= starflywallbounce;

				}

				return;

			}

			else if (statemachine.state == stswim)

			{

				speed.y = 0;

				return;

			}

			else if (statemachine.state == stdreamdash)

				return;



			//dash blocks

			if (data.hit != null && data.hit.ondashcollide != null)

			{

				if (dashattacking && data.direction.y == math.sign(dashdir.y))

				{

					var result = data.hit.ondashcollide(this, data.direction);

					if (statemachine.state == streddash)

						result = dashcollisionresults.ignore;



					if (result == dashcollisionresults.rebound)

					{

						rebound(0);

						return;

					}

					else if (result == dashcollisionresults.bounce)

					{

						reflectbounce(new vector2(0, -math.sign(speed.y)));

						return;

					}

					else if (result == dashcollisionresults.ignore)

						return;

				}

				else if (statemachine.state == stsummitlaunch)

				{

					data.hit.ondashcollide(this, data.direction);

					return;

				}

			}



			if (speed.y > 0)

			{

				//dash corner correction

				if ((statemachine.state == stdash || statemachine.state == streddash) && !dashstartedonground)

				{

					if (speed.x <= 0)

					{

						for (int i = -1; i >= -dashcornercorrection; i--)

						{

							if (!onground(position + new vector2(i, 0)))

							{

								movehexact(i);

								movevexact(1);

								return;

							}

						}

					}



					if (speed.x >= 0)

					{

						for (int i = 1; i <= dashcornercorrection; i++)

						{

							if (!onground(position + new vector2(i, 0)))

							{

								movehexact(i);

								movevexact(1);

								return;

							}

						}

					}

				}



				//dream dash

				if (dreamdashcheck(vector2.unity * math.sign(speed.y)))

				{

					statemachine.state = stdreamdash;

					dashattacktimer = 0;

					return;

				}



				//dash slide

				if (dashdir.x != 0 && dashdir.y > 0 && speed.y > 0)

				{

					dashdir.x = math.sign(dashdir.x);

					dashdir.y = 0;

					speed.y = 0;

					speed.x *= dodgeslidespeedmult;

					ducking = true;

				}



				//landed on ground effects

				if (statemachine.state != stclimb)

				{

					float squish = math.min(speed.y / fastmaxfall, 1);

					sprite.scale.x = mathhelper.lerp(1, 1.6f, squish);

					sprite.scale.y = mathhelper.lerp(1, .4f, squish);



					if (speed.y >= maxfall / 2)

						dust.burst(position, calc.angle(new vector2(0, -1)), 8);



					if (highestairy < y - 50 && speed.y >= maxfall && math.abs(speed.x) >= maxrun)

						sprite.play(playersprite.runstumble);



					input.rumble(rumblestrength.light, rumblelength.short);



					// landed sfx

					var platform = surfaceindex.getplatformbypriority(collideall<platform>(position + new vector2(0, 1), temp));

					if (platform != null)

					{

						var surface = platform.getlandsoundindex(this);

						if (surface >= 0)

							play(playfootsteponland > 0f ? sfxs.char_mad_footstep : sfxs.char_mad_land, surfaceindex.param, surface);

						if (platform is dreamblock)

							(platform as dreamblock).footstepripple(position);

					}



					playfootsteponland = 0.0f;

				}

			}

			else

			{

				if (speed.y < 0)

				{

					//corner correction

					{

						if (speed.x <= 0)

						{

							for (int i = 1; i <= upwardcornercorrection; i++)

							{

								if (!collidecheck<solid>(position + new vector2(-i, -1)))

								{

									position += new vector2(-i, -1);

									return;

								}

							}

						}



						if (speed.x >= 0)

						{

							for (int i = 1; i <= upwardcornercorrection; i++)

							{

								if (!collidecheck<solid>(position + new vector2(i, -1)))

								{

									position += new vector2(i, -1);

									return;

								}

							}

						}

					}



					if (varjumptimer < varjumptime - ceilingvarjumpgrace)

						varjumptimer = 0;

				}



				//dream dash

				if (dreamdashcheck(vector2.unity * math.sign(speed.y)))

				{

					statemachine.state = stdreamdash;

					dashattacktimer = 0;

					return;

				}

			}



			//collide event

			if (data.hit != null && data.hit.oncollide != null)

				data.hit.oncollide(data.direction);



			dashattacktimer = 0;

			speed.y = 0;



			if (statemachine.state == streddash)

			{

				input.rumble(rumblestrength.medium, rumblelength.short);

				level.displacement.addburst(center, .5f, 8, 48, .4f, ease.quadout, ease.quadout);

				statemachine.state = sthitsquash;

			}

		}



		private bool dreamdashcheck(vector2 dir)

		{

			if (inventory.dreamdash && dashattacking && (dir.x == math.sign(dashdir.x) || dir.y == math.sign(dashdir.y)))

			{

				var block = collidefirst<dreamblock>(position + dir);



				if (block != null)

				{

					if (collidecheck<solid, dreamblock>(position + dir))

					{

						vector2 side = new vector2(math.abs(dir.y), math.abs(dir.x));



						bool checknegative, checkpositive;

						if (dir.x != 0)

						{

							checknegative = speed.y <= 0;

							checkpositive = speed.y >= 0;

						}

						else

						{

							checknegative = speed.x <= 0;

							checkpositive = speed.x >= 0;

						}



						if (checknegative)

						{

							for (int i = -1; i >= -dashcornercorrection; i--)

							{

								var at = position + dir + side * i;

								if (!collidecheck<solid, dreamblock>(at))

								{

									position += side * i;

									dreamblock = block;

									return true;

								}

							}

						}



						if (checkpositive)

						{

							for (int i = 1; i <= dashcornercorrection; i++)

							{

								var at = position + dir + side * i;

								if (!collidecheck<solid, dreamblock>(at))

								{

									position += side * i;

									dreamblock = block;

									return true;

								}

							}

						}



						return false;

					}

					else

					{

						dreamblock = block;

						return true;

					}

				}

			}



			return false;

		}



		public void onboundsh()

		{

			speed.x = 0;



			if (statemachine.state == streddash)

				statemachine.state = stnormal;

		}



		public void onboundsv()

		{

			speed.y = 0;



			if (statemachine.state == streddash)

				statemachine.state = stnormal;

		}



		override protected void onsquish(collisiondata data)

		{

			bool ducked = false;

			if (!ducking)

			{

				ducked = true;

				ducking = true;

				data.pusher.collidable = true;



				if (!collidecheck<solid>())

				{

					data.pusher.collidable = false;

					return;

				}



				var was = position;

				position = data.targetposition;

				if (!collidecheck<solid>())

				{

					data.pusher.collidable = false;

					return;

				}



				position = was;

				data.pusher.collidable = false;

			}



			if (!trysquishwiggle(data))

				die(vector2.zero);

			else if (ducked && canunduck)

				ducking = false;

		}



		#endregion



			#region normal state



			private void normalbegin()

		{

			maxfall = maxfall;

		}



		private void normalend()

		{

			wallboosttimer = 0;

			wallspeedretentiontimer = 0;

			hopwaitx = 0;

		}



		public bool climbboundscheck(int dir)

		{

			return left + dir * climbcheckdist >= level.bounds.left && right + dir * climbcheckdist < level.bounds.right;

		}



		public bool climbcheck(int dir, int yadd = 0)

		{

			return climbboundscheck(dir) && !climbblocker.check(scene, this, position + vector2.unity * yadd + vector2.unitx * climbcheckdist * (int)facing) && collidecheck<solid>(position + new vector2(dir * climbcheckdist, yadd));

		}



		private const float spacephysicsmult = .6f;



		private int normalupdate()

		{

			//use lift boost if walked off platform

			if (liftboost.y < 0 && wasonground && !onground && speed.y >= 0)

				speed.y = liftboost.y;



			if (holding == null)

			{

				if (input.grab.check && !istired && !ducking)

				{

					//grabbing holdables

					foreach(holdable hold in scene.tracker.getcomponents<holdable>())

						if (hold.check(this) && pickup(hold))

							return stpickup;



					//climbing

					if (speed.y >= 0 && math.sign(speed.x) != -(int)facing)

					{

						if (climbcheck((int)facing))

						{

							ducking = false;

							return stclimb;

						}



						if (input.movey < 1 && level.wind.y <= 0)

						{

							for (int i = 1; i <= climbupcheckdist; i++)

							{

								if (!collidecheck<solid>(position + vector2.unity * -i) && climbcheck((int)facing, -i))

								{

									movevexact(-i);

									ducking = false;

									return stclimb;

								}

							}

						}

					}

				}



				//dashing

				if (candash)

				{

					speed += liftboost;

					return startdash();

				}



				//ducking

				if (ducking)

				{

					if (onground && input.movey != 1)

					{

						if (canunduck)

						{

							ducking = false;

							sprite.scale = new vector2(.8f, 1.2f);

						}

						else if (speed.x == 0)

						{

							for (int i = duckcorrectcheck; i > 0; i--)

							{

								if (canunduckat(position + vector2.unitx * i))

								{

									moveh(duckcorrectslide * engine.deltatime);

									break;

								}

								else if (canunduckat(position - vector2.unitx * i))

								{

									moveh(-duckcorrectslide * engine.deltatime);

									break;

								}

							}

						}

					}

				}

				else if (onground && input.movey == 1 && speed.y >= 0)

				{

					ducking = true;

					sprite.scale = new vector2(1.4f, .6f);

				}

			}

			else

			{

				//throw

				if (!input.grab.check && minholdtimer <= 0)

					throw();



				//ducking

				if (!ducking && onground && input.movey == 1 && speed.y >= 0)

				{

					drop();

					ducking = true;

					sprite.scale = new vector2(1.4f, .6f);

				}

			}



			//running and friction

			if (ducking && onground)

				speed.x = calc.approach(speed.x, 0, duckfriction * engine.deltatime);

			else

			{

				float mult = onground ? 1 : airmult;

				if (onground && level.coremode == session.coremodes.cold)

					mult *= .3f;



				float max = holding == null ? maxrun : holdingmaxrun;

				if (level.inspace)

					max *= spacephysicsmult;

				if (math.abs(speed.x) > max && math.sign(speed.x) == movex)

					speed.x = calc.approach(speed.x, max * movex, runreduce * mult * engine.deltatime);  //reduce back from beyond the max speed

				else

					speed.x = calc.approach(speed.x, max * movex, runaccel * mult * engine.deltatime);   //approach the max speed

			}



			//vertical

			{

				//calculate current max fall speed

				{

					float mf = maxfall;

					float fmf = fastmaxfall;



					if (level.inspace)

					{

						mf *= spacephysicsmult;

						fmf *= spacephysicsmult;

					}



					//fast fall

					if (input.movey == 1 && speed.y >= mf)

					{

						maxfall = calc.approach(maxfall, fmf, fastmaxaccel * engine.deltatime);



						float half = mf + (fmf - mf) * .5f;

						if (speed.y >= half)

						{

							float spritelerp = math.min(1f, (speed.y - half) / (fmf - half));

							sprite.scale.x = mathhelper.lerp(1f, .5f, spritelerp);

							sprite.scale.y = mathhelper.lerp(1f, 1.5f, spritelerp);

						}

					}

					else

						maxfall = calc.approach(maxfall, mf, fastmaxaccel * engine.deltatime);

				}



				//gravity

				if (!onground)

				{

					float max = maxfall;



					//wall slide

					if ((movex == (int)facing || (movex == 0 && input.grab.check)) && input.movey.value != 1)

					{

						if (speed.y >= 0 && wallslidetimer > 0 && holding == null && climbboundscheck((int)facing) && collidecheck<solid>(position + vector2.unitx * (int)facing) && canunduck)

						{

							ducking = false;

							wallslidedir = (int)facing;

						}



						if (wallslidedir != 0)

						{

							if (wallslidetimer > wallslidetime * .5f && climbblocker.check(level, this, position + vector2.unitx * wallslidedir))

								wallslidetimer = wallslidetime * .5f;



							max = mathhelper.lerp(maxfall, wallslidestartmax, wallslidetimer / wallslidetime);

							if (wallslidetimer / wallslidetime > .65f)

								createwallslideparticles(wallslidedir);

						}

					}



					float mult = (math.abs(speed.y) < halfgravthreshold && (input.jump.check || autojump)) ? .5f : 1f;



					if (level.inspace)

						mult *= spacephysicsmult;



					speed.y = calc.approach(speed.y, max, gravity * mult * engine.deltatime);

				}



				//variable jumping

				if (varjumptimer > 0)

				{

					if (autojump || input.jump.check)

						speed.y = math.min(speed.y, varjumpspeed);

					else

						varjumptimer = 0;

				}



				//jumping

				if (input.jump.pressed)

				{

					water water = null;

					if (jumpgracetimer > 0)

					{

						jump();

					}

					else if (canunduck)

					{

						bool canunduck = canunduck;

						if (canunduck && walljumpcheck(1))

						{

							if (facing == facings.right && input.grab.check && stamina > 0 && holding == null && !climbblocker.check(scene, this, position + vector2.unitx * walljumpcheckdist))

								climbjump();

							else if (dashattacking && dashdir.x == 0 && dashdir.y == -1)

								superwalljump(-1);

							else

								walljump(-1);

						}

						else if (canunduck && walljumpcheck(-1))

						{

							if (facing == facings.left && input.grab.check && stamina > 0 && holding == null && !climbblocker.check(scene, this, position + vector2.unitx * -walljumpcheckdist))

								climbjump();

							else if (dashattacking && dashdir.x == 0 && dashdir.y == -1)

								superwalljump(1);

							else

								walljump(1);

						}

						else if ((water = collidefirst<water>(position + vector2.unity * 2)) != null)

						{

							jump();

							water.topsurface.doripple(position, 1);

						}

					}

				}

			}



			return stnormal;

		}



		public void createwallslideparticles(int dir)

		{

			if (scene.oninterval(.01f))

			{

				vector2 at = center;

				if (dir == 1)

					at += new vector2(5, 4);

				else

					at += new vector2(-5, 4);

				dust.burst(at, calc.up, 1);

			}

		}



		#endregion



			#region climb state



			private bool istired

		{

			get

		{

			return checkstamina < climbtiredthreshold;

		}

		}



			private float checkstamina

		{

			get

		{

			if (wallboosttimer > 0)

			return stamina + climbjumpcost;

			else

				return stamina;

		}

		}



			private void playsweateffectdangeroverride(string state)

		{

			if (stamina <= climbtiredthreshold)

				sweatsprite.play("danger");

			else

				sweatsprite.play(state);

		}



		private fmod.studio.eventinstance conveyorloopsfx;



		private void climbbegin()

		{

			autojump = false;

			speed.x = 0;

			speed.y *= climbgrabymult;

			wallslidetimer = wallslidetime;

			climbnomovetimer = climbnomovetime;

			wallboosttimer = 0;

			lastclimbmove = 0;



			input.rumble(rumblestrength.medium, rumblelength.short);



			for (int i = 0; i < climbcheckdist; i++)

				if (!collidecheck<solid>(position + vector2.unitx * (int)facing))

					position += vector2.unitx * (int)facing;

				else

					break;



			// tell the thing we grabbed it

			var platform = surfaceindex.getplatformbypriority(collideall<solid>(position + vector2.unitx * (int)facing, temp));

			if (platform != null)

				play(sfxs.char_mad_grab, surfaceindex.param, platform.getwallsoundindex(this, (int)facing));

		}



		private void climbend()

		{

			if (conveyorloopsfx != null)

			{

				conveyorloopsfx.setparametervalue("end", 1);

				conveyorloopsfx.release();

				conveyorloopsfx = null;

			}

			wallspeedretentiontimer = 0;

			if (sweatsprite != null && sweatsprite.currentanimationid != "jump")

				sweatsprite.play("idle");

		}



		private const float wallboosterspeed = -160.0f;

		private const float wallboosterliftspeed = -80;

		private const float wallboosteraccel = 600.0f;

		private const float wallboostinghophspeed = 100.0f;

		private const float wallboosterovertopspeed = -180.0f;

		private const float iceboosterspeed = 40;

		private const float iceboosteraccel = 300.0f;

		private bool wallboosting;



		private int climbupdate()

		{

			climbnomovetimer -= engine.deltatime;



			//refill stamina on ground

			if (onground)

				stamina = climbmaxstamina;



			//wall jump

			if (input.jump.pressed && (!ducking || canunduck))

			{

				if (movex == -(int)facing)

					walljump(-(int)facing);

				else

					climbjump();



				return stnormal;

			}



			//dashing

			if (candash)

			{

				speed += liftboost;

				return startdash();

			}



			//let go

			if (!input.grab.check)

			{

				speed += liftboost;

				play(sfxs.char_mad_grab_letgo);

				return stnormal;

			}



			//no wall to hold

			if (!collidecheck<solid>(position + vector2.unitx * (int)facing))

			{

				//climbed over ledge?

				if (speed.y < 0)

				{

					if (wallboosting)

					{

						speed += liftboost;

						play(sfxs.char_mad_grab_letgo);

					}

					else

						climbhop();

				}



				return stnormal;

			}



			var booster = wallboostercheck();

			if (climbnomovetimer <= 0 && booster != null)

			{

				//wallbooster

				wallboosting = true;



				if (conveyorloopsfx == null)

					conveyorloopsfx = audio.play(sfxs.game_09_conveyor_activate, position, "end", 0);

				audio.position(conveyorloopsfx, position);



				speed.y = calc.approach(speed.y, wallboosterspeed, wallboosteraccel * engine.deltatime);

				liftspeed = vector2.unity * math.max(speed.y, wallboosterliftspeed);

				input.rumble(rumblestrength.light, rumblelength.short);

			}

			else

			{

				wallboosting = false;



				if (conveyorloopsfx != null)

				{

					conveyorloopsfx.setparametervalue("end", 1);

					conveyorloopsfx.release();

					conveyorloopsfx = null;

				}



				//climbing

				float target = 0;

				bool tryslip = false;

				if (climbnomovetimer <= 0)

				{

					if (climbblocker.check(scene, this, position + vector2.unitx * (int)facing))

						tryslip = true;

					else if (input.movey.value == -1)

					{

						target = climbupspeed;



						//up limit

						if (collidecheck<solid>(position - vector2.unity) || (climbhopblockedcheck() && slipcheck(-1)))

						{

							if (speed.y < 0)

								speed.y = 0;

							target = 0;

							tryslip = true;

						}

						else if (slipcheck())

						{

							//hopping

							climbhop();

							return stnormal;

						}

					}

					else if (input.movey.value == 1)

					{

						target = climbdownspeed;



						if (onground)

						{

							if (speed.y > 0)

								speed.y = 0;

							target = 0;

						}

						else

							createwallslideparticles((int)facing);

					}

					else

						tryslip = true;

				}

				else

					tryslip = true;

				lastclimbmove = math.sign(target);



				//slip down if hands above the ledge and no vertical input

				if (tryslip && slipcheck())

					target = climbslipspeed;



				//set speed

				speed.y = calc.approach(speed.y, target, climbaccel * engine.deltatime);

			}



			//down limit

			if (input.movey.value != 1 && speed.y > 0 && !collidecheck<solid>(position + new vector2((int)facing, 1)))

				speed.y = 0;



			//stamina

			if (climbnomovetimer <= 0)

			{

				if (lastclimbmove == -1)

				{

					stamina -= climbupcost * engine.deltatime;



					if (stamina <= climbtiredthreshold)

						sweatsprite.play("danger");

					else if (sweatsprite.currentanimationid != "climbloop")

						sweatsprite.play("climb");

					if (scene.oninterval(.2f))

						input.rumble(rumblestrength.climb, rumblelength.short);

				}

				else

				{

					if (lastclimbmove == 0)

						stamina -= climbstillcost * engine.deltatime;



					if (!onground)

					{

						playsweateffectdangeroverride("still");

						if (scene.oninterval(.8f))

							input.rumble(rumblestrength.climb, rumblelength.short);

					}

					else

						playsweateffectdangeroverride("idle");

				}

			}

			else

				playsweateffectdangeroverride("idle");



			//too tired           

			if (stamina <= 0)

			{

				speed += liftboost;

				return stnormal;

			}



			return stclimb;

		}



		private wallbooster wallboostercheck()

		{

			if (climbblocker.check(scene, this, position + vector2.unitx * (int)facing))

				return null;



			foreach(wallbooster booster in scene.tracker.getentities<wallbooster>())

				if (booster.facing == facing && collidecheck(booster))

					return booster;



			return null;

		}



		private void climbhop()

		{

			climbhopsolid = collidefirst<solid>(position + vector2.unitx * (int)facing);

			playfootsteponland = 0.5f;



			if (climbhopsolid != null)

			{

				climbhopsolidposition = climbhopsolid.position;

				hopwaitx = (int)facing;

				hopwaitxspeed = (int)facing * climbhopx;

			}

			else

			{

				hopwaitx = 0;

				speed.x = (int)facing * climbhopx;

			}



			speed.y = math.min(speed.y, climbhopy);

			forcemovex = 0;

			forcemovextimer = climbhopforcetime;

			fastjump = false;

			nowindtimer = climbhopnowindtime;

			play(sfxs.char_mad_climb_ledge);

		}



		private bool slipcheck(float addy = 0)

		{

			vector2 at;

			if (facing == facings.right)

				at = topright + vector2.unity * (4 + addy);

			else

				at = topleft - vector2.unitx + vector2.unity * (4 + addy);



			return !scene.collidecheck<solid>(at) && !scene.collidecheck<solid>(at + vector2.unity * (-4 + addy));

		}



		private bool climbhopblockedcheck()

		{

			//if you have a cassette note, you're blocked

			foreach(follower follower in leader.followers)

				if (follower.entity is strawberryseed)

					return true;



			//if you hit a ledge blocker, you're blocked

			foreach(ledgeblocker blocker in scene.tracker.getcomponents<ledgeblocker>())

				if (blocker.hopblockcheck(this))

					return true;



			//if there's a solid in the way, you're blocked

			if (collidecheck<solid>(position - vector2.unity * 6))

				return true;



			return false;

		}



		private bool jumpthruboostblockedcheck()

		{

			foreach(ledgeblocker blocker in scene.tracker.getcomponents<ledgeblocker>())

				if (blocker.jumpthruboostcheck(this))

					return true;

			return false;

		}



		#endregion



			#region dash state



			public int startdash()

		{

			wasdashb = dashes == 2;

			dashes = math.max(0, dashes - 1);

			input.dash.consumebuffer();

			return stdash;

		}



		public bool dashattacking

		{

			get

		{

			return dashattacktimer > 0 || statemachine.state == streddash;

		}

		}



		private vector2 beforedashspeed;

		private bool wasdashb;



		public bool candash

		{

			get

		{

			return input.dash.pressed && dashcooldowntimer <= 0 && dashes > 0 &&

			(talkcomponent.playerover == null || !input.talk.pressed);

		}

		}



			public bool starteddashing

		{

			get; private set;

		}



			private void calldashevents()

		{

			if (!calleddashevents)

			{

				calleddashevents = true;

				if (currentbooster == null)

				{

					// increment counter

					savedata.instance.totaldashes++;

					level.session.dashes++;

					stats.increment(stat.dashes);



					// sfxs

					{

						var rightdashsound = dashdir.y < 0 || (dashdir.y == 0 && dashdir.x > 0);

						if (dashdir == vector2.zero)

							rightdashsound = facing == facings.right;



						if (rightdashsound)

						{

							if (wasdashb)

								play(sfxs.char_mad_dash_pink_right);

							else

								play(sfxs.char_mad_dash_red_right);

						}

						else

						{

							if (wasdashb)

								play(sfxs.char_mad_dash_pink_left);

							else

								play(sfxs.char_mad_dash_red_left);

						}



						if (swimcheck())

							play(sfxs.char_mad_water_dash_gen);

					}



					//dash listeners

					foreach(dashlistener dl in scene.tracker.getcomponents<dashlistener>())

						if (dl.ondash != null)

							dl.ondash(dashdir);

				}

				else

				{

					//booster

					currentbooster.playerboosted(this, dashdir);

					currentbooster = null;

				}

			}

		}



		private void dashbegin()

		{

			calleddashevents = false;

			dashstartedonground = onground;

			launched = false;



			if (engine.timerate > 0.25f)

				celeste.freeze(.05f);

			dashcooldowntimer = dashcooldown;

			dashrefillcooldowntimer = dashrefillcooldown;

			starteddashing = true;

			wallslidetimer = wallslidetime;

			dashtrailtimer = 0;



			level.displacement.addburst(center, .4f, 8, 64, .5f, ease.quadout, ease.quadout);



			input.rumble(rumblestrength.strong, rumblelength.medium);



			dashattacktimer = dashattacktime;

			beforedashspeed = speed;

			speed = vector2.zero;

			dashdir = vector2.zero;



			if (!onground && ducking && canunduck)

				ducking = false;

		}



		private void dashend()

		{

			calldashevents();

		}



		private int dashupdate()

		{

			starteddashing = false;



			//trail

			if (dashtrailtimer > 0)

			{

				dashtrailtimer -= engine.deltatime;

				if (dashtrailtimer <= 0)

					createtrail();

			}



			//grab holdables

			if (holding == null && input.grab.check && !istired && canunduck)

			{

				//grabbing holdables

				foreach(holdable hold in scene.tracker.getcomponents<holdable>())

					if (hold.check(this) && pickup(hold))

						return stpickup;

			}



			if (dashdir.y == 0)

			{

				//jumpthru correction

				foreach(jumpthru jt in scene.tracker.getentities<jumpthru>())

					if (collidecheck(jt) && bottom - jt.top <= dashhjumpthrunudge)

						movevexact((int)(jt.top - bottom));



				//super jump

				if (canunduck && input.jump.pressed && jumpgracetimer > 0)

				{

					superjump();

					return stnormal;

				}

			}



			if (dashdir.x == 0 && dashdir.y == -1)

			{

				if (input.jump.pressed && canunduck)

				{

					if (walljumpcheck(1))

					{

						superwalljump(-1);

						return stnormal;

					}

					else if (walljumpcheck(-1))

					{

						superwalljump(1);

						return stnormal;

					}

				}

			}

			else

			{

				if (input.jump.pressed && canunduck)

				{

					if (walljumpcheck(1))

					{

						walljump(-1);

						return stnormal;

					}

					else if (walljumpcheck(-1))

					{

						walljump(1);

						return stnormal;

					}

				}

			}



			if (speed != vector2.zero && level.oninterval(0.02f))

				level.particlesfg.emit(wasdashb ? p_dashb : p_dasha, center + calc.random.range(vector2.one *-2, vector2.one * 2), dashdir.angle());

			return stdash;

		}



		private ienumerator dashcoroutine()

		{

			yield return null;



			var dir = lastaim;

			if (overridedashdirection.hasvalue)

				dir = overridedashdirection.value;



			var newspeed = dir * dashspeed;

			if (math.sign(beforedashspeed.x) == math.sign(newspeed.x) && math.abs(beforedashspeed.x) > math.abs(newspeed.x))

				newspeed.x = beforedashspeed.x;

			speed = newspeed;



			if (collidecheck<water>())

				speed *= swimdashspeedmult;



			dashdir = dir;

			sceneas<level>().directionalshake(dashdir, .2f);



			if (dashdir.x != 0)

				facing = (facings)math.sign(dashdir.x);



			calldashevents();



			//feather particles

			if (statemachine.previousstate == ststarfly)

				level.particles.emit(flyfeather.p_boost, 12, center, vector2.one * 4, (-dir).angle());



			//dash slide

			if (onground && dashdir.x != 0 && dashdir.y > 0 && speed.y > 0

				&& (!inventory.dreamdash || !collidecheck<dreamblock>(position + vector2.unity)))

			{

				dashdir.x = math.sign(dashdir.x);

				dashdir.y = 0;

				speed.y = 0;

				speed.x *= dodgeslidespeedmult;

				ducking = true;

			}



			slashfx.burst(center, dashdir.angle());

			createtrail();

			dashtrailtimer = .08f;



			//swap block check

			if (dashdir.x != 0 && input.grab.check)

			{

				var swapblock = collidefirst<swapblock>(position + vector2.unitx * math.sign(dashdir.x));

				if (swapblock != null && swapblock.direction.x == math.sign(dashdir.x))

				{

					statemachine.state = stclimb;

					speed = vector2.zero;

					yield break;

				}

			}



			//stick to swap blocks

			vector2 swapcancel = vector2.one;

			foreach(swapblock swapblock in scene.tracker.getentities<swapblock>())

			{

				if (collidecheck(swapblock, position + vector2.unity))

				{

					if (swapblock != null && swapblock.swapping)

					{

						if (dashdir.x != 0 && swapblock.direction.x == math.sign(dashdir.x))

							speed.x = swapcancel.x = 0;

						if (dashdir.y != 0 && swapblock.direction.y == math.sign(dashdir.y))

							speed.y = swapcancel.y = 0;

					}

				}

			}



			yield return dashtime;



			createtrail();



			autojump = true;

			autojumptimer = 0;

			if (dashdir.y <= 0)

			{

				speed = dashdir * enddashspeed;

				speed.x *= swapcancel.x;

				speed.y *= swapcancel.y;

			}

			if (speed.y < 0)

				speed.y *= enddashupmult;

			statemachine.state = stnormal;

		}



		#endregion



			#region swim state



			private const float swimyspeedmult = .5f;

		private const float swimmaxrise = -60;

		private const float swimvdeccel = 600.0f;

		private const float swimmax = 80.0f;

		private const float swimunderwatermax = 60.0f;

		private const float swimaccel = 600.0f;

		private const float swimreduce = 400.0f;

		private const float swimdashspeedmult = .75f;



		private bool swimcheck()

		{

			return collidecheck<water>(position + vector2.unity * -8) && collidecheck<water>(position);

		}



		private bool swimunderwatercheck()

		{

			return collidecheck<water>(position + vector2.unity * -9);

		}



		private bool swimjumpcheck()

		{

			return !collidecheck<water>(position + vector2.unity * -14);

		}



		private bool swimrisecheck()

		{

			return !collidecheck<water>(position + vector2.unity * -18);

		}



		private bool underwatermusiccheck()

		{

			return collidecheck<water>(position) && collidecheck<water>(position + vector2.unity * -12);

		}



		private void swimbegin()

		{

			if (speed.y > 0)

				speed.y *= swimyspeedmult;



			stamina = climbmaxstamina;





		}



		private int swimupdate()

		{

			if (!swimcheck())

				return stnormal;



			//never duck if possible

			if (canunduck)

				ducking = false;



			//dashing

			if (candash)

			{

				//dashes = math.max(0, dashes - 1);

				input.dash.consumebuffer();

				return stdash;

			}



			bool underwater = swimunderwatercheck();



			//climbing

			if (!underwater && speed.y >= 0 && input.grab.check && !istired && canunduck)

			{

				//climbing

				if (math.sign(speed.x) != -(int)facing && climbcheck((int)facing))

				{

					if (!movevexact(-1))

					{

						ducking = false;

						return stclimb;

					}

				}

			}



			//movement

			{

				vector2 move = input.aim.value;

				move = move.safenormalize();



				float maxx = underwater ? swimunderwatermax : swimmax;

				float maxy = swimmax;



				if (math.abs(speed.x) > swimmax && math.sign(speed.x) == math.sign(move.x))

					speed.x = calc.approach(speed.x, maxx * move.x, swimreduce * engine.deltatime);  //reduce back from beyond the max speed

				else

					speed.x = calc.approach(speed.x, maxx * move.x, swimaccel * engine.deltatime);   //approach the max speed



				if (move.y == 0 && swimrisecheck())

				{

					//rise if close to the surface and not pressing a direction

					speed.y = calc.approach(speed.y, swimmaxrise, swimaccel * engine.deltatime);

				}

				else if (move.y >= 0 || swimunderwatercheck())

				{

					if (math.abs(speed.y) > swimmax && math.sign(speed.y) == math.sign(move.y))

						speed.y = calc.approach(speed.y, maxy * move.y, swimreduce * engine.deltatime);  //reduce back from beyond the max speed

					else

						speed.y = calc.approach(speed.y, maxy * move.y, swimaccel * engine.deltatime);   //approach the max speed

				}

			}



			//popping up onto ledge

			const int ledgepopdist = -3;

			if (!underwater && movex != 0

				&& collidecheck<solid>(position + vector2.unitx * movex)

				&& !collidecheck<solid>(position + new vector2(movex, ledgepopdist)))

			{

				climbhop();

			}



			//jumping

			if (input.jump.pressed && swimjumpcheck())

			{

				jump();

				return stnormal;

			}



			return stswim;

		}



		#endregion



			#region boost state



			private vector2 boosttarget;

		private bool boostred;



		public void boost(booster booster)

		{

			statemachine.state = stboost;

			speed = vector2.zero;

			boosttarget = booster.center;

			boostred = false;

			lastbooster = currentbooster = booster;

		}



		public void redboost(booster booster)

		{

			statemachine.state = stboost;

			speed = vector2.zero;

			boosttarget = booster.center;

			boostred = true;

			lastbooster = currentbooster = booster;

		}



		private void boostbegin()

		{

			refilldash();

			refillstamina();

		}



		private void boostend()

		{

			var to = (boosttarget - collider.center).floor();

			movetox(to.x);

			movetoy(to.y);

		}



		private int boostupdate()

		{

			vector2 targetadd = input.aim.value * 3;

			var to = calc.approach(exactposition, boosttarget - collider.center + targetadd, boostmovespeed * engine.deltatime);

			movetox(to.x);

			movetoy(to.y);



			if (input.dash.pressed)

			{

				input.dash.consumepress();

				if (boostred)

					return streddash;

				else

					return stdash;

			}



			return stboost;

		}



		private ienumerator boostcoroutine()

		{

			yield return boosttime;



			if (boostred)

				statemachine.state = streddash;

			else

				statemachine.state = stdash;

		}



		#endregion



			#region red dash state



			private void reddashbegin()

		{

			calleddashevents = false;

			dashstartedonground = false;



			celeste.freeze(.05f);

			dust.burst(position, calc.angle(-dashdir), 8);

			dashcooldowntimer = dashcooldown;

			dashrefillcooldowntimer = dashrefillcooldown;

			starteddashing = true;



			level.displacement.addburst(center, .5f, 0, 80, .666f, ease.quadout, ease.quadout);



			input.rumble(rumblestrength.strong, rumblelength.medium);



			dashattacktimer = dashattacktime;

			speed = vector2.zero;



			if (!onground && ducking && canunduck)

				ducking = false;

		}



		private void reddashend()

		{

			calldashevents();

		}



		private int reddashupdate()

		{

			starteddashing = false;



			if (candash)

				return startdash();



			if (dashdir.y == 0)

			{

				//jumpthru correction

				foreach(jumpthru jt in scene.tracker.getentities<jumpthru>())

					if (collidecheck(jt) && bottom - jt.top <= dashhjumpthrunudge)

						movevexact((int)(jt.top - bottom));



				//super jump

				if (canunduck && input.jump.pressed && jumpgracetimer > 0)

				{

					superjump();

					return stnormal;

				}

			}

			else if (dashdir.x == 0 && dashdir.y == -1)

			{

				if (input.jump.pressed && canunduck)

				{

					if (walljumpcheck(1))

					{

						superwalljump(-1);

						return stnormal;

					}

					else if (walljumpcheck(-1))

					{

						superwalljump(1);

						return stnormal;

					}

				}

			}

			else

			{

				if (input.jump.pressed && canunduck)

				{

					if (walljumpcheck(1))

					{

						walljump(-1);

						return stnormal;

					}

					else if (walljumpcheck(-1))

					{

						walljump(1);

						return stnormal;

					}

				}

			}



			return streddash;

		}



		private ienumerator reddashcoroutine()

		{

			yield return null;



			speed = lastaim * dashspeed;

			dashdir = lastaim;

			sceneas<level>().directionalshake(dashdir, .2f);



			calldashevents();

		}



		#endregion



			#region hit squash state



			private const float hitsquashnomovetime = .1f;

		private const float hitsquashfriction = 800;



		private float hitsquashnomovetimer;



		private void hitsquashbegin()

		{

			hitsquashnomovetimer = hitsquashnomovetime;

		}



		private int hitsquashupdate()

		{

			speed.x = calc.approach(speed.x, 0, hitsquashfriction * engine.deltatime);

			speed.y = calc.approach(speed.y, 0, hitsquashfriction * engine.deltatime);



			if (input.jump.pressed)

			{

				if (onground)

					jump();

				else if (walljumpcheck(1))

					walljump(-1);

				else if (walljumpcheck(-1))

					walljump(1);

				else

					input.jump.consumebuffer();



				return stnormal;

			}



			if (candash)

				return startdash();



			if (input.grab.check && climbcheck((int)facing))

				return stclimb;



			if (hitsquashnomovetimer > 0)

				hitsquashnomovetimer -= engine.deltatime;

			else

				return stnormal;



			return sthitsquash;

		}



		#endregion



			#region launch state



			private float ? launchapproachx;



		public vector2 explodelaunch(vector2 from, bool snapup = true)

		{

			input.rumble(rumblestrength.strong, rumblelength.medium);

			celeste.freeze(.1f);

			launchapproachx = null;



			vector2 normal = (center - from).safenormalize(-vector2.unity);

			var dot = vector2.dot(normal, vector2.unity);



			if (snapup && dot <= -.7f)

			{

				//if close to up, make it up

				normal.x = 0;

				normal.y = -1;

			}

			else if (dot <= .55f && dot >= -.55f)

			{

				//if partially down, make it sideways

				normal.y = 0;

				normal.x = math.sign(normal.x);

			}



			speed = launchspeed * normal;

			if (speed.y <= 50f)

			{

				speed.y = math.min(-150f, speed.y);

				autojump = true;

			}



			if (input.movex.value == math.sign(speed.x))

				speed.x *= 1.2f;



			slashfx.burst(center, speed.angle());

			if (!inventory.norefills)

				refilldash();

			refillstamina();

			dashcooldowntimer = dashcooldown;

			statemachine.state = stlaunch;



			return normal;

		}



		public void finalbosspushlaunch(int dir)

		{

			launchapproachx = null;

			speed.x = .9f * dir * launchspeed;

			speed.y = -150;

			autojump = true;



			input.rumble(rumblestrength.strong, rumblelength.medium);

			slashfx.burst(center, speed.angle());

			refilldash();

			refillstamina();

			dashcooldowntimer = .28f;

			statemachine.state = stlaunch;

		}



		public void badelineboostlaunch(float atx)

		{

			launchapproachx = atx;

			speed.x = 0;

			speed.y = -330.0f;

			autojump = true;



			slashfx.burst(center, speed.angle());

			refilldash();

			refillstamina();

			dashcooldowntimer = dashcooldown;

			statemachine.state = stlaunch;

		}



		private void launchbegin()

		{

			launched = true;

		}



		private int launchupdate()

		{

			//approach x

			if (launchapproachx.hasvalue)

				movetowardsx(launchapproachx.value, 60 * engine.deltatime);



			//dashing

			if (candash)

				return startdash();



			//decceleration

			if (speed.y < 0)

				speed.y = calc.approach(speed.y, maxfall, gravity * .5f * engine.deltatime);

			else

				speed.y = calc.approach(speed.y, maxfall, gravity * .25f * engine.deltatime);

			speed.x = calc.approach(speed.x, 0, runaccel * .2f * engine.deltatime);



			if (speed.length() < launchcancelthreshold)

				return stnormal;

			else

				return stlaunch;

		}



		#endregion



			#region summit launch state



			private float summitlaunchtargetx;

		private float summitlaunchparticletimer;



		public void summitlaunch(float targetx)

		{

			summitlaunchtargetx = targetx;



			statemachine.state = stsummitlaunch;

		}



		private void summitlaunchbegin()

		{

			wallboosttimer = 0;

			sprite.play("launch");

			speed = -vector2.unity * dashspeed;

			summitlaunchparticletimer = .4f;

		}



		private int summitlaunchupdate()

		{

			summitlaunchparticletimer -= engine.deltatime;

			if (summitlaunchparticletimer > 0 && scene.oninterval(.03f))

				level.particlesfg.emit(badelineboost.p_move, 1, center, vector2.one * 4);



			facing = facings.right;

			movetowardsx(summitlaunchtargetx, 20 * engine.deltatime);

			speed = -vector2.unity * dashspeed;

			if (level.oninterval(0.2f))

				level.add(engine.pooler.create<speedring>().init(center, calc.down, color.white));

			return stsummitlaunch;

		}



		public void stopsummitlaunch()

		{

			statemachine.state = stnormal;

			speed.y = bouncespeed;

			autojump = true;

			varjumpspeed = speed.y;

		}



		#endregion



			#region pickup state



			private ienumerator pickupcoroutine()

		{

			play(sfxs.char_mad_crystaltheo_lift);

			input.rumble(rumblestrength.medium, rumblelength.short);



			vector2 oldspeed = speed;

			float varjump = varjumptimer;

			speed = vector2.zero;



			vector2 begin = holding.entity.position - position;

			vector2 end = carryoffsettarget;

			vector2 control = new vector2(begin.x + math.sign(begin.x) * 2, carryoffsettarget.y - 2);

			simplecurve curve = new simplecurve(begin, end, control);



			carryoffset = begin;

			var tween = tween.create(tween.tweenmode.oneshot, ease.cubeinout, .16f, true);

			tween.onupdate = (t) = >

			{

				carryoffset = curve.getpoint(t.eased);

			};

			add(tween);

			yield return tween.wait();



			speed = oldspeed;

			speed.y = math.min(speed.y, 0);

			varjumptimer = varjump;

			statemachine.state = stnormal;

		}



		#endregion



			#region dream dash state



			private dreamblock dreamblock;

		private soundsource dreamsfxloop;

		private bool dreamjump;



		private void dreamdashbegin()

		{

			if (dreamsfxloop == null)

				add(dreamsfxloop = new soundsource());



			speed = dashdir * dreamdashspeed;

			treatnaive = true;

			depth = depths.playerdreamdashing;

			dreamdashcanendtimer = dreamdashmintime;

			stamina = climbmaxstamina;

			dreamjump = false;



			play(sfxs.char_mad_dreamblock_enter);

			loop(dreamsfxloop, sfxs.char_mad_dreamblock_travel);

		}



		private void dreamdashend()

		{

			depth = depths.player;

			if (!dreamjump)

			{

				autojump = true;

				autojumptimer = 0;

			}

			if (!inventory.norefills)

				refilldash();

			refillstamina();

			treatnaive = false;



			if (dreamblock != null)

			{

				if (dashdir.x != 0)

				{

					jumpgracetimer = jumpgracetime;

					dreamjump = true;

				}

				else

					jumpgracetimer = 0;



				dreamblock.onplayerexit(this);

				dreamblock = null;

			}



			stop(dreamsfxloop);

			play(sfxs.char_mad_dreamblock_exit);

			input.rumble(rumblestrength.medium, rumblelength.short);

		}



		private int dreamdashupdate()

		{

			input.rumble(rumblestrength.light, rumblelength.medium);



			var oldpos = position;

			naivemove(speed * engine.deltatime);

			if (dreamdashcanendtimer > 0)

				dreamdashcanendtimer -= engine.deltatime;



			var block = collidefirst<dreamblock>();

			if (block == null)

			{

				if (dreamdashedintosolid())

				{

					if (savedata.instance.assistmode && savedata.instance.assists.invincible)

					{

						position = oldpos;

						speed *= -1;

						play(sfxs.game_assist_dreamblockbounce);

					}

					else

						die(vector2.zero);

				}

				else if (dreamdashcanendtimer <= 0)

				{

					celeste.freeze(.05f);



					if (input.jump.pressed && dashdir.x != 0)

					{

						dreamjump = true;

						jump();

					}

					else

					{

						bool left = climbcheck(-1);

						bool right = climbcheck(1);



						if (input.grab.check && (dashdir.y >= 0 || dashdir.x != 0) && ((movex == 1 && right) || (movex == -1 && left)))

						{

							facing = (facings)movex;

							return stclimb;

						}

					}



					return stnormal;

				}

			}

			else

			{

				dreamblock = block;



				if (scene.oninterval(0.1f))

					createtrail();



				//displacement effect

				if (level.oninterval(0.04f))

				{

					var displacement = level.displacement.addburst(center, .3f, 0f, 40f);

					displacement.worldclipcollider = dreamblock.collider;

					displacement.worldclippadding = 2;

				}

			}



			return stdreamdash;

		}



		private bool dreamdashedintosolid()

		{

			if (collidecheck<solid>())

			{

				for (int x = 1; x <= dreamdashendwiggle; x++)

				{

					for (int xm = -1; xm <= 1; xm += 2)

					{

						for (int y = 1; y <= dreamdashendwiggle; y++)

						{

							for (int ym = -1; ym <= 1; ym += 2)

							{

								vector2 add = new vector2(x * xm, y * ym);

								if (!collidecheck<solid>(position + add))

								{

									position += add;

									return false;

								}

							}

						}

					}

				}



				return true;

			}

			else

				return false;

		}



		#endregion



			#region star fly state



			private const float starflytransformdeccel = 1000.0f;

		private const float starflytime = 2f;

		private const float starflystartspeed = 250.0f;

		private const float starflytargetspeed = 140.0f;

		private const float starflymaxspeed = 190.0f;

		private const float starflymaxlerptime = 1f;

		private const float starflyslowspeed = starflytargetspeed * .65f;

		private const float starflyaccel = 1000.0f;

		private const float starflyrotatespeed = 320 * calc.dtr;

		private const float starflyendx = 160.0f;

		private const float starflyendxvarjumptime = .1f;

		private const float starflyendflashduration = 0.5f;

		private const float starflyendnobouncetime = .2f;

		private const float starflywallbounce = -.5f;

		private const float starflymaxexity = 0.0f;

		private const float starflymaxexitx = 140;

		private const float starflyexitup = -100;



		private color starflycolor = calc.hextocolor("ffd65c");

		private bloompoint starflybloom;



		private float starflytimer;

		private bool starflytransforming;

		private float starflyspeedlerp;

		private vector2 starflylastdir;



		private soundsource starflyloopsfx;

		private soundsource starflywarningsfx;



		public bool startstarfly()

		{

			refillstamina();



			if (statemachine.state == streflectionfall)

				return false;



			if (statemachine.state == ststarfly)

			{

				starflytimer = starflytime;

				sprite.color = starflycolor;

				input.rumble(rumblestrength.medium, rumblelength.medium);

			}

			else

				statemachine.state = ststarfly;



			return true;

		}



		private void starflybegin()

		{

			sprite.play(playersprite.startstarfly);



			starflytransforming = true;

			starflytimer = starflytime;

			starflyspeedlerp = 0;

			jumpgracetimer = 0;



			if (starflybloom == null)

				add(starflybloom = new bloompoint(new vector2(0, -6), 0f, 16));

			starflybloom.visible = true;

			starflybloom.alpha = 0.0f;



			collider = starflyhitbox;

			hurtbox = starflyhurtbox;



			if (starflyloopsfx == null)

			{

				add(starflyloopsfx = new soundsource());

				starflyloopsfx.disposeontransition = false;

				add(starflywarningsfx = new soundsource());

				starflywarningsfx.disposeontransition = false;

			}

			starflyloopsfx.play(sfxs.game_06_feather_state_loop, "feather_speed", 1);

			starflywarningsfx.stop();

		}



		private void starflyend()

		{

			play(sfxs.game_06_feather_state_end);



			starflywarningsfx.stop();

			starflyloopsfx.stop();

			hair.drawplayerspriteoutline = false;

			sprite.color = color.white;

			level.displacement.addburst(center, 0.25f, 8, 32);

			starflybloom.visible = false;

			sprite.haircount = starthaircount;



			starflyreturntonormalhitbox();



			if (statemachine.state != stdash)

				level.particles.emit(flyfeather.p_boost, 12, center, vector2.one * 4, (-speed).angle());

		}



		private void starflyreturntonormalhitbox()

		{

			collider = normalhitbox;

			hurtbox = normalhurtbox;



			if (collidecheck<solid>())

			{

				vector2 start = position;



				//try moving up

				y -= (normalhitbox.bottom - starflyhitbox.bottom);

				if (collidecheck<solid>())

					position = start;

				else

					return;



				//try ducking and moving up

				ducking = true;

				y -= (duckhitbox.bottom - starflyhitbox.bottom);

				if (collidecheck<solid>())

					position = start;

				else

					return;



				throw new exception("could not get out of solids when exiting star fly state!");

			}

		}



		private ienumerator starflycoroutine()

		{

			while (sprite.currentanimationid == playersprite.startstarfly)

				yield return null;



			while (speed != vector2.zero)

				yield return null;



			yield return .1f;



			sprite.color = starflycolor;

			sprite.haircount = 7;

			hair.drawplayerspriteoutline = true;



			level.displacement.addburst(center, 0.25f, 8, 32);

			starflytransforming = false;

			starflytimer = starflytime;



			refilldash();

			refillstamina();



			//speed boost

			{

				var dir = input.aim.value;

				if (dir == vector2.zero)

					dir = vector2.unitx * (int)facing;

				speed = dir * starflystartspeed;

				starflylastdir = dir;



				level.particles.emit(flyfeather.p_boost, 12, center, vector2.one * 4, (-dir).angle());

			}



			input.rumble(rumblestrength.strong, rumblelength.medium);

			level.directionalshake(starflylastdir);



			while (starflytimer > starflyendflashduration)

				yield return null;



			starflywarningsfx.play(sfxs.game_06_feather_state_warning);

		}



		private int starflyupdate()

		{

			starflybloom.alpha = calc.approach(starflybloom.alpha, 0.7f, engine.deltatime * 2f);



			if (starflytransforming)

			{

				speed = calc.approach(speed, vector2.zero, starflytransformdeccel * engine.deltatime);

			}

			else

			{

				//movement

				{

					vector2 aim = input.aim.value;

					bool slow = false;

					if (aim == vector2.zero)

					{

						slow = true;

						aim = starflylastdir;

					}



					//figure out direction

					vector2 currentdir = speed.safenormalize(vector2.zero);

					if (currentdir == vector2.zero)

						currentdir = aim;

					else

						currentdir = currentdir.rotatetowards(aim.angle(), starflyrotatespeed * engine.deltatime);

					starflylastdir = currentdir;



					//figure out max speed

					float maxspeed;

					if (slow)

					{

						starflyspeedlerp = 0;

						maxspeed = starflyslowspeed;

					}

					else if (currentdir != vector2.zero && vector2.dot(currentdir, aim) >= .45f)

					{

						starflyspeedlerp = calc.approach(starflyspeedlerp, 1, engine.deltatime / starflymaxlerptime);

						maxspeed = mathhelper.lerp(starflytargetspeed, starflymaxspeed, starflyspeedlerp);

					}

					else

					{

						starflyspeedlerp = 0;

						maxspeed = starflytargetspeed;

					}



					starflyloopsfx.param("feather_speed", slow ? 0 : 1);



					//approach max speed

					float currentspeed = speed.length();

					currentspeed = calc.approach(currentspeed, maxspeed, starflyaccel * engine.deltatime);



					//set speed

					speed = currentdir * currentspeed;



					//particles

					if (level.oninterval(.02f))

						level.particles.emit(flyfeather.p_flying, 1, center, vector2.one * 2, (-speed).angle());

				}



				//jump cancelling

				if (input.jump.pressed)

				{

					if (onground(3))

					{

						jump();

						return stnormal;

					}

					else if (walljumpcheck(-1))

					{

						walljump(1);

						return stnormal;

					}

					else if (walljumpcheck(1))

					{

						walljump(-1);

						return stnormal;

					}

				}



				//grab cancelling

				if (input.grab.check)

				{

					if (input.movex.value != -1 && climbcheck(1))

					{

						facing = facings.right;

						return stclimb;

					}

					else if (input.movex.value != 1 && climbcheck(-1))

					{

						facing = facings.left;

						return stclimb;

					}

				}



				//dash cancelling

				if (candash)

					return startdash();



				//timer

				starflytimer -= engine.deltatime;

				if (starflytimer <= 0)

				{

					if (input.movey.value == -1)

						speed.y = starflyexitup;



					if (input.movey.value < 1)

					{

						varjumpspeed = speed.y;

						autojump = true;

						autojumptimer = 0;

						varjumptimer = varjumptime;

					}



					if (speed.y > starflymaxexity)

						speed.y = starflymaxexity;



					if (math.abs(speed.x) > starflymaxexitx)

						speed.x = starflymaxexitx * math.sign(speed.x);



					input.rumble(rumblestrength.medium, rumblelength.medium);



					return stnormal;

				}



				// flicker at end

				if (starflytimer < starflyendflashduration && scene.oninterval(0.05f))

				{

					if (sprite.color == starflycolor)

						sprite.color = normalhaircolor;

					else

						sprite.color = starflycolor;

				}

			}



			return ststarfly;

		}



		#endregion



			#region cassette fly state



			private simplecurve cassetteflycurve;

		private float cassetteflylerp;



		public void startcassettefly(vector2 targetposition, vector2 control)

		{

			statemachine.state = stcassettefly;

			cassetteflycurve = new simplecurve(position, targetposition, control);

			cassetteflylerp = 0;

			speed = vector2.zero;

		}



		private void cassetteflybegin()

		{

			sprite.play("bubble");

			sprite.y += 5;

		}



		private void cassetteflyend()

		{



		}



		private int cassetteflyupdate()

		{

			return stcassettefly;

		}



		private ienumerator cassetteflycoroutine()

		{

			level.canretry = false;

			level.formationbackdrop.display = true;

			level.formationbackdrop.alpha = 0.5f;

			sprite.scale = vector2.one * 1.25f;

			depth = depths.formationsequences;

			yield return 0.4f;



			while (cassetteflylerp < 1f)

			{

				if (level.oninterval(.03f))

					level.particles.emit(p_cassettefly, 2, center, vector2.one * 4);



				cassetteflylerp = calc.approach(cassetteflylerp, 1f, 1.6f * engine.deltatime);

				position = cassetteflycurve.getpoint(ease.sineinout(cassetteflylerp));

				level.camera.position = cameratarget;

				yield return null;

			}



			position = cassetteflycurve.end;

			sprite.scale = vector2.one * 1.25f;

			sprite.y -= 5;

			sprite.play(playersprite.fallfast);

			yield return 0.2f;



			level.canretry = true;

			level.formationbackdrop.display = false;

			level.formationbackdrop.alpha = 0.5f;

			statemachine.state = stnormal;

			depth = depths.player;

			yield break;

		}



		#endregion



			#region attract state



			private vector2 attractto;



		public void startattract(vector2 attractto)

		{

			this.attractto = calc.round(attractto);

			statemachine.state = stattract;

		}



		private void attractbegin()

		{

			speed = vector2.zero;

		}



		private void attractend()

		{



		}



		private int attractupdate()

		{

			if (vector2.distance(attractto, exactposition) <= 1.5f)

			{

				position = attractto;

				zeroremainderx();

				zeroremaindery();

			}

			else

			{

				vector2 at = calc.approach(exactposition, attractto, 200 * engine.deltatime);

				movetox(at.x);

				movetoy(at.y);

			}



			return stattract;

		}



		public bool atattracttarget

		{

			get

		{

			return statemachine.state == stattract && exactposition == attractto;

		}

		}



			#endregion



			#region dummy state



			public bool dummymoving = false;

		public bool dummygravity = true;

		public bool dummyfriction = true;

		public bool dummymaxspeed = true;



		private void dummybegin()

		{

			dummymoving = false;

			dummygravity = true;

			dummyautoanimate = true;

		}



		private int dummyupdate()

		{

			if (canunduck)

				ducking = false;



			// gravity

			if (!onground && dummygravity)

			{

				float mult = (math.abs(speed.y) < halfgravthreshold && (input.jump.check || autojump)) ? .5f : 1f;



				if (level.inspace)

					mult *= spacephysicsmult;



				speed.y = calc.approach(speed.y, maxfall, gravity * mult * engine.deltatime);

			}



			// variable jumping

			if (varjumptimer > 0)

			{

				if (autojump || input.jump.check)

					speed.y = math.min(speed.y, varjumpspeed);

				else

					varjumptimer = 0;

			}



			if (!dummymoving)

			{

				if (math.abs(speed.x) > maxrun && dummymaxspeed)

					speed.x = calc.approach(speed.x, maxrun * math.sign(speed.x), runaccel * 2.5f * engine.deltatime);

				if (dummyfriction)

					speed.x = calc.approach(speed.x, 0, runaccel * engine.deltatime);

			}



			//sprite

			if (dummyautoanimate)

			{

				if (onground)

				{

					if (speed.x == 0)

						sprite.play("idle");

					else

						sprite.play(playersprite.walk);

				}

				else

				{

					if (speed.y < 0)

						sprite.play(playersprite.jumpslow);

					else

						sprite.play(playersprite.fallslow);

				}



			}



			return stdummy;

		}



		public ienumerator dummywalkto(float x, bool walkbackwards = false, float speedmultiplier = 1f, bool keepwalkingintowalls = false)

		{

			statemachine.state = stdummy;



			if (math.abs(x - x) > 4 && !dead)

			{

				dummymoving = true;



				if (walkbackwards)

				{

					sprite.rate = -1;

					facing = (facings)math.sign(x - x);

				}

				else

				{

					facing = (facings)math.sign(x - x);

				}



				while (math.abs(x - x) > 4 && scene != null && (keepwalkingintowalls || !collidecheck<solid>(position + vector2.unitx * math.sign(x - x))))

				{

					speed.x = calc.approach(speed.x, math.sign(x - x) * walkspeed * speedmultiplier, runaccel * engine.deltatime);

					yield return null;

				}



				sprite.rate = 1;

				sprite.play(playersprite.idle);

				dummymoving = false;

			}

		}



		public ienumerator dummywalktoexact(int x, bool walkbackwards = false, float speedmultiplier = 1f)

		{

			statemachine.state = stdummy;



			if (x != x)

			{

				dummymoving = true;



				if (walkbackwards)

				{

					sprite.rate = -1;

					facing = (facings)math.sign(x - x);

				}

				else

				{

					facing = (facings)math.sign(x - x);

				}



				var last = math.sign(x - x);

				while (x != x && !collidecheck<solid>(position + new vector2((int)facing, 0)))

				{

					speed.x = calc.approach(speed.x, math.sign(x - x) * walkspeed * speedmultiplier, runaccel * engine.deltatime);



					// handle case where we overstep

					var next = math.sign(x - x);

					if (next != last)

					{

						x = x;

						break;

					}

					last = next;



					yield return null;

				}



				speed.x = 0;

				sprite.rate = 1;

				sprite.play(playersprite.idle);

				dummymoving = false;

			}

		}



		public ienumerator dummyrunto(float x, bool fastanim = false)

		{

			statemachine.state = stdummy;



			if (math.abs(x - x) > 4)

			{

				dummymoving = true;

				if (fastanim)

					sprite.play(playersprite.runfast);

				else if (!sprite.lastanimationid.startswith("run"))

					sprite.play(playersprite.runslow);

				facing = (facings)math.sign(x - x);



				while (math.abs(x - x) > 4)

				{

					speed.x = calc.approach(speed.x, math.sign(x - x) * maxrun, runaccel * engine.deltatime);

					yield return null;

				}



				sprite.play(playersprite.idle);

				dummymoving = false;

			}

		}



		#endregion



			#region frozen state



			private int frozenupdate()

		{

			return stfrozen;

		}



		#endregion



			#region temple fall state



			private int templefallupdate()

		{

			facing = facings.right;



			if (!onground)

			{

				var center = level.bounds.left + 160;

				int mx;

				if (math.abs(center - x) > 4)

					mx = math.sign(center - x);

				else

					mx = 0;



				speed.x = calc.approach(speed.x, maxrun * .6f * mx, runaccel * .5f * airmult * engine.deltatime);

			}

			if (!onground && dummygravity)

				speed.y = calc.approach(speed.y, maxfall * 2, gravity * 0.25f * engine.deltatime);



			return sttemplefall;

		}



		private ienumerator templefallcoroutine()

		{

			sprite.play(playersprite.fallfast);



			while (!onground)

				yield return null;



			play(sfxs.char_mad_mirrortemple_landing);

			if (dashes <= 1)

				sprite.play(playersprite.landinpose);

			else

				sprite.play(playersprite.idle);

			sprite.scale.y = 0.7f;



			input.rumble(rumblestrength.strong, rumblelength.medium);

			level.directionalshake(new vector2(0, 1f), 0.5f);

			speed.x = 0;



			level.particles.emit(p_summitlanda, 12, bottomcenter, vector2.unitx * 3, calc.up);

			level.particles.emit(p_summitlandb, 8, bottomcenter - vector2.unitx * 2, vector2.unitx * 2, calc.left + 15 * calc.dtr);

			level.particles.emit(p_summitlandb, 8, bottomcenter + vector2.unitx * 2, vector2.unitx * 2, calc.right - 15 * calc.dtr);



			for (var p = 0.0f; p < 1; p += engine.deltatime)

				yield return null;



			statemachine.state = stnormal;

		}



		#endregion



			#region reflection fall state



			private void reflectionfallbegin()

		{

			ignorejumpthrus = true;

		}



		private void reflectionfallend()

		{

			falleffects.show(false);

			ignorejumpthrus = false;

		}



		private int reflectionfallupdate()

		{

			facing = facings.right;



			if (scene.oninterval(.05f))

			{

				wasdashb = true;

				createtrail();

			}



			// fall

			if (collidecheck<water>())

				speed.y = calc.approach(speed.y, -20f, 400f * engine.deltatime);

			else

				speed.y = calc.approach(speed.y, maxfall * 2, gravity * 0.25f * engine.deltatime);



			// remove feathers

			var feathers = scene.tracker.getentities<flyfeather>();

			foreach(var feather in feathers)

				feather.removeself();



			// smash crystal spinners

			var hit = scene.collidefirst<crystalstaticspinner>(new rectangle((int)(x - 6), (int)(y - 6), 12, 12));

			if (hit != null)

			{

				hit.destroy();

				level.shake();

				input.rumble(rumblestrength.medium, rumblelength.medium);

				celeste.freeze(0.01f);

			}



			return streflectionfall;

		}



		private ienumerator reflectionfallcoroutine()

		{

			sprite.play(playersprite.fallbig);

			level.startcutscene(onreflectionfallskip);



			// wait a bit before entering

			for (float t = 0.0f; t < 2f; t += engine.deltatime)

			{

				speed.y = 0.0f;

				yield return null;

			}



			// start falling at max speed

			falleffects.show(true);

			speed.y = maxfall * 2f;



			// wait for waiter

			while (!collidecheck<water>())

				yield return null;



			input.rumble(rumblestrength.strong, rumblelength.medium);



			falleffects.show(false);

			sprite.play("bigfallrecover");

			level.session.audio.music.event = sfxs.music_reflection_main;

			level.session.audio.apply();

			level.endcutscene();



			yield return 1.2f;



			statemachine.state = stnormal;

		}



		private void onreflectionfallskip(level level)

		{

			level.onendofframe += () = >

			{

				level.remove(this);

				level.unloadlevel();

				level.session.level = "00";

				level.session.respawnpoint = level.getspawnpoint(new vector2(level.bounds.left, level.bounds.bottom));

				level.loadlevel(introtypes.none);



				falleffects.show(false);



				level.session.audio.music.event = sfxs.music_reflection_main;

				level.session.audio.apply();

			};

		}



		#endregion



			#region intro walk state



			private facings introwalkdirection;



		public ienumerator introwalkcoroutine()

		{

			var start = position;

			if (introwalkdirection == facings.right)

			{

				x = level.bounds.left - 16;

				facing = facings.right;

			}

			else

			{

				x = level.bounds.right + 16;

				facing = facings.left;

			}



			yield return .3f;



			sprite.play(playersprite.runslow);

			while (math.abs(x - start.x) > 2 && !collidecheck<solid>(position + new vector2((int)facing, 0)))

			{

				movetowardsx(start.x, 64 * engine.deltatime);

				yield return null;

			}



			position = start;

			sprite.play(playersprite.idle);

			yield return .2f;



			statemachine.state = stnormal;

		}



		#endregion



			#region intro jump state



			private ienumerator introjumpcoroutine()

		{

			var start = position;

			var wassummitjump = statemachine.previousstate == stsummitlaunch;



			depth = depths.top;

			facing = facings.right;



			if (!wassummitjump)

			{

				y = level.bounds.bottom + 16;

				yield return .5f;

			}

			else

			{

				start.y = level.bounds.bottom - 24;

				movetox((int)math.round(x / 8) * 8);

			}



			// move up

			{

				if (!wassummitjump)

					sprite.play(playersprite.jumpslow);

				while (y > start.y - 8)

				{

					y += -120 * engine.deltatime;

					yield return null;

				}

				speed.y = -100;

			}



			// slow down

			{

				while (speed.y < 0)

				{

					speed.y += engine.deltatime * 800.0f;

					yield return null;

				}



				speed.y = 0;

				if (wassummitjump)

				{

					yield return 0.2f;

					play(sfxs.char_mad_summit_areastart);

					sprite.play("launchrecover");

					yield return 0.1f;

				}

				else

					yield return 0.1f;

			}



			// fall down

			{

				if (!wassummitjump)

					sprite.play(playersprite.fallslow);



				while (!onground)

				{

					speed.y += engine.deltatime * 800.0f;

					yield return null;

				}

			}



			// land

			{

				if (statemachine.previousstate != stsummitlaunch)

					position = start;



				depth = depths.player;

				level.directionalshake(vector2.unity);

				input.rumble(rumblestrength.strong, rumblelength.medium);



			}



			if (wassummitjump)

			{

				level.particles.emit(p_summitlanda, 12, bottomcenter, vector2.unitx * 3, calc.up);

				level.particles.emit(p_summitlandb, 8, bottomcenter - vector2.unitx * 2, vector2.unitx * 2, calc.left + 15 * calc.dtr);

				level.particles.emit(p_summitlandb, 8, bottomcenter + vector2.unitx * 2, vector2.unitx * 2, calc.right - 15 * calc.dtr);

				level.particlesbg.emit(p_summitlandc, 30, bottomcenter, vector2.unitx * 5);



				yield return 0.35f;

				for (int i = 0; i < hair.nodes.count; i++)

					hair.nodes[i] = new vector2(0, 2 + i);

			}



			statemachine.state = stnormal;

		}



		#endregion



			#region intro wake up state



			private ienumerator introwakeupcoroutine()

		{

			sprite.play("asleep");



			yield return .5f;

			yield return sprite.playroutine("wakeup");

			yield return .2f;



			statemachine.state = stnormal;

		}



		#endregion



			#region intro respawn state



			private tween respawntween;



		private void introrespawnbegin()

		{

			play(sfxs.char_mad_revive);



			depth = depths.top;

			introease = 1f;



			vector2 from = position;

			const float pad = 40;

			from.x = mathhelper.clamp(from.x, level.bounds.left + pad, level.bounds.right - pad);

			from.y = mathhelper.clamp(from.y, level.bounds.top + pad, level.bounds.bottom - pad);

			deadoffset = from;

			from = from - position;



			respawntween = tween.create(tween.tweenmode.oneshot, null, .6f, true);

			respawntween.onupdate = (t) = >

			{

				deadoffset = vector2.lerp(from, vector2.zero, t.eased);

				introease = 1 - t.eased;

			};

			respawntween.oncomplete = (t) = >

			{

				if (statemachine.state == stintrorespawn)

				{

					statemachine.state = stnormal;

					sprite.scale = new vector2(1.5f, .5f);

				}

			};

			add(respawntween);

		}



		private void introrespawnend()

		{

			depth = depths.player;

			deadoffset = vector2.zero;

			remove(respawntween);

			respawntween = null;

		}



		#endregion



			#region bird dash tutorial



			private void birddashtutorialbegin()

		{

			dashbegin();

			play(sfxs.char_mad_dash_red_right);

			sprite.play(playersprite.dash);

		}



		private int birddashtutorialupdate()

		{

			return stbirddashtutorial;

		}



		private ienumerator birddashtutorialcoroutine()

		{

			yield return null;



			createtrail();

			add(alarm.create(alarm.alarmmode.oneshot, createtrail, 0.08f, true));

			add(alarm.create(alarm.alarmmode.oneshot, createtrail, dashtime, true));



			var aim = new vector2(1, -1).safenormalize();

			facing = facings.right;



			speed = aim * dashspeed;

			dashdir = aim;

			sceneas<level>().directionalshake(dashdir, .2f);

			slashfx.burst(center, dashdir.angle());



			for (float time = 0; time < dashtime; time += engine.deltatime)

			{

				if (speed != vector2.zero && level.oninterval(0.02f))

					level.particlesfg.emit(p_dasha, center + calc.random.range(vector2.one * -2, vector2.one * 2), dashdir.angle());

				yield return null;

			}



			autojump = true;

			autojumptimer = 0;

			if (dashdir.y <= 0)

				speed = dashdir * enddashspeed;

			if (speed.y < 0)

				speed.y *= enddashupmult;



			sprite.play(playersprite.fallfast);



			var climbing = false;

			while (!onground() && !climbing)

			{

				speed.y = calc.approach(speed.y, maxfall, gravity * engine.deltatime);

				if (collidecheck<solid>(position + new vector2(1, 0)))

					climbing = true;

				if (top > level.bounds.bottom)

				{

					level.cancelcutscene();

					die(vector2.zero);

				}

				yield return null;

			}



			if (climbing)

			{

				sprite.play(playersprite.wallslide);

				dust.burst(position + new vector2(4, -6), calc.angle(new vector2(-4, 0)), 1);

				speed.y = 0;

				yield return 0.2f;



				sprite.play(playersprite.climbup);

				while (collidecheck<solid>(position + new vector2(1, 0)))

				{

					y += climbupspeed * engine.deltatime;

					yield return null;

				}



				play(sfxs.char_mad_climb_ledge);

				sprite.play(playersprite.jumpfast);

				speed.y = jumpspeed;



				while (!onground())

				{

					speed.y = calc.approach(speed.y, maxfall, gravity * engine.deltatime);

					speed.x = 20.0f;

					yield return null;

				}



				speed.x = 0;

				speed.y = 0;



				sprite.play(playersprite.walk);

				for (float time = 0.0f; time < 0.5f; time += engine.deltatime)

				{

					x += 32 * engine.deltatime;

					yield return null;

				}



				sprite.play(playersprite.tired);

			}

			else

			{

				sprite.play(playersprite.tired);

				speed.y = 0;



				while (speed.x != 0)

				{

					speed.x = calc.approach(speed.x, 0, 240 * engine.deltatime);

					if (scene.oninterval(0.04f))

						dust.burst(bottomcenter + new vector2(0, -2), calc.upleft);

					yield return null;

				}

			}

		}



		#endregion



			#region chaser state tracking



			public fmod.studio.eventinstance play(string sound, string param = null, float value = 0)

		{

			addchaserstatesound(sound, param, value);

			return audio.play(sound, center, param, value);

		}



		public void loop(soundsource sfx, string sound)

		{

			addchaserstatesound(sound, null, 0, chaserstatesound.actions.loop);

			sfx.play(sound);

		}



		public void stop(soundsource sfx)

		{

			if (sfx.playing)

			{

				addchaserstatesound(sfx.eventname, null, 0, chaserstatesound.actions.stop);

				sfx.stop();

			}

		}



		private void addchaserstatesound(string sound, chaserstatesound.actions action)

		{

			addchaserstatesound(sound, null, 0, action);

		}



		private void addchaserstatesound(string sound, string param = null, float value = 0, chaserstatesound.actions action = chaserstatesound.actions.oneshot)

		{

			string eventname = null;

			sfxs.madelinetobadelinesound.trygetvalue(sound, out eventname);



			if (eventname != null)

				activesounds.add(new chaserstatesound(){ event = eventname, parameter = param, parametervalue = value, action = action });

		}



		public struct chaserstatesound

		{

			public enum actions

			{

				oneshot,

				loop,

				stop

			}



			public string event;

			public string parameter;

			public float parametervalue;

			public actions action;

		}



		public struct chaserstate

		{

			public vector2 position;

			public float timestamp;

			public string animation;

			public facings facing;

			public bool onground;

			public color haircolor;

			public int depth;



			private chaserstatesound sound0;

			private chaserstatesound sound1;

			private chaserstatesound sound2;

			private chaserstatesound sound3;

			private chaserstatesound sound4;

			public int sounds;



			public chaserstate(player player)

			{

				position = player.position;

				timestamp = player.scene.timeactive;

				animation = player.sprite.currentanimationid;

				facing = player.facing;

				onground = player.onground;

				haircolor = player.hair.color;

				depth = player.depth;



				var sounds = player.activesounds;

				sounds = math.min(5, sounds.count);



				sound0 = sounds > 0 ? sounds[0] : default(chaserstatesound);

				sound1 = sounds > 1 ? sounds[1] : default(chaserstatesound);

				sound2 = sounds > 2 ? sounds[2] : default(chaserstatesound);

				sound3 = sounds > 3 ? sounds[3] : default(chaserstatesound);

				sound4 = sounds > 4 ? sounds[4] : default(chaserstatesound);

			}



			public chaserstatesound this[int index]

			{

				get

			{

				switch (index)

				{

				case 0: return sound0;

				case 1: return sound1;

				case 2: return sound2;

				case 3: return sound3;

				case 4: return sound4;

				}



			return new chaserstatesound();

			}

			}

		}



		#endregion



	}

}