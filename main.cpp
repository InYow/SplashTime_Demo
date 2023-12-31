#include "core.h"

//TODO:
// 1. 为物体Object切换场景
// 2. 设置按钮，与释放技能建立联系
// 3. 设置三种技能。一种无限制，一种有限制伤害，一种回血.
// 4. 回血技能可以在场景外使用(roundManager在场景一中不Update，skillManager在场景一中一直是没有finished)
// 5. roundManager和skillManager和初始化(切换场景时用)
// 6. 选择攻击对象(继承一个带动画的GameObject,引用roundManager和skillManager. 在玩家回合时(roundManager)显示自身. 默认伤害技能默认对面，治愈技能默认自己)
// 7. roundManager中，敌人死亡则战斗结束，返回场景一。

//------------------------------------------------------------------------------------
// 入口
//------------------------------------------------------------------------------------
int main(void)
{
	// 初始化 //////////////////////////////////////////////////////////////////////////////////////////////////////
	// 初始化音乐设备(必需)---------------------------------------------------------------------------------------
	InitAudioDevice();
	// 初始化窗口（必需）-----------------------------------------------  SCREENWIDTH  SCREENHEIGHT  200.f  ----------
	float screenWidth = SCREENWIDTH;
	float screenHeight = SCREENHEIGHT;
	InitWindow(screenWidth, screenHeight, "Splash Time");
	SetTraceLogLevel(LOG_WARNING); //在控制台窗口中只显示警告和错误信息
	SetTargetFPS(GAME_FPS);               // Set our game to run at 60 frames-per-second
	screenWidth = GetScreenWidth();
	screenHeight = GetScreenHeight();
	// 初始化字体（必需）--------------------------------------------------------  初始化的结果是font  ---------------
	fontFileData = LoadFileData("c:\\windows\\fonts\\SIMYOU.TTF", &fileSize);
	DELIMITER = LoadFileText("Text/split_character.txt")[0];
	ALLCHARACTER = LoadFileText("Text/ALLCHARACTER.txt");
	int codepointsCount;
	int* codepoints = LoadCodepoints(ALLCHARACTER, &codepointsCount);
	font = LoadFontFromMemory(".ttf", fontFileData, fileSize, FONTSIZE, codepoints, codepointsCount);
	UnloadFileText(ALLCHARACTER);
	UnloadCodepoints(codepoints);
	// 相机(必需)  ---------------------------------------------------------------------------------------------------------
	camera.position = { 0,0 }; camera.view = { screenWidth ,screenHeight };
	// 地图(必需)  -----------------------------------------------------------------------------------------
	Object Map_1 = { 1,"Map_1",0,0,0,0,"地图_1",0};
	Object Map_2 = { 1,"Map_2",9344,0,0,0,"地图_2",0 };

	Object BattleBack = { 2,"BattleBack",-2160,0,0,0,"战斗背景",0 };
	// 音乐和音效
	Music strollMusic = LoadMusicStream("Audio//StrollMusic.mp3");
	StrollMusicInto = &strollMusic;
	strollMusic.looping = true;

	Music battleMusic = LoadMusicStream("Audio//BattleMusic.mp3");
	battleMusicInto = &battleMusic;
	battleMusic.looping = true;

	Sound hitSoundEffect = LoadSound("Audio//HitSoundEffect.mp3");
	hitSoundEffectInto = &hitSoundEffect;

	PlayMusicStream(strollMusic);
	// 场景(必需） ---------------------------------------------------------------------------------------------------
	SceneIndex = 0;//程序开始场景
	LoadSence(1);

	BattleObject battleObject = { 1,"dangerbattle_object",0,0,0,0,"danger_battle1",LAYER_SUM-1 };
	battleObject.SetActive_Object(false);
	battleObjectInto = &battleObject;
	Animator dangerbattle_object_Animator = { "dangerbattle_object_Animator",true,battleObject };
	battleObject.animator = &dangerbattle_object_Animator;
	AnimationClip dangerbattle_object_AnimationClip = { "dangerbattle_object_AnimationClip",dangerbattle_object_Animator ,5,3,"danger_battle" };
	Events dangerbattle_object_AnimationClip_Events = { &dangerbattle_object_AnimationClip };
	dangerbattle_object_AnimationClip_Events.AddEvent(battleObjectLoadSence, 4);
	//回合制战斗(必需)
	roundManager = {}; 
	// 资源加载 /////////////////
	// 物体Object-----------------------------------------------------------------------
	GameObject thelocker = { 2,"locker",-100,0,0,0,"locker",LAYER_UI - 1,6,12 };
	roundManager.Locker = &thelocker;
#pragma region 地图数据
	GameObject Edge = { "Edge",0,0,0,"null",0 };
	Collision E1 = { "E1",true,Edge,{6400.f, 178.f},{0.f, 590.f},Type_Static };
	Collision E2 = { "E2",true,Edge,{6400.f, 128.f},{0.f, 896.f},Type_Static };
	Collision E3 = { "E3",true,Edge,{128.f, 896.f},{6144.f, 896.f},Type_Static };
	Collision E4 = { "E4",true,Edge,{6400.f, 128.f},{0.f, 1664.f},Type_Static };
	Collision E5 = { "E5",true,Edge,{},{},Type_Static };
	Collision E6 = { "E6",true,Edge,{},{},Type_Static };
	Collision E7 = { "E7",true,Edge,{},{},Type_Static };
	Collision E8 = { "E8",true,Edge,{},{},Type_Static };
	Collision E9 = { "E9",true,Edge,{},{},Type_Static };
	Collision E10 = { "E10",true,Edge,{},{},Type_Static };
	Collision E11 = { "E11",true,Edge,{},{},Type_Static };
	Collision E12 = { "E12",true,Edge,{},{},Type_Static };
	Collision E13 = { "E13",true,Edge,{},{},Type_Static };
	//使用三边重合的静态碰撞物体
	GameObject house1= { "house1", 6529 ,128  , 256 , "房子6", LAYER_PLHOU };
	GameObject house2= { "house2",7553  ,384  , 340 , "房子5", LAYER_PLHOU };
	GameObject house3= { "house3",8705  ,640  ,  256 , "房子2", LAYER_PLHOU };
	GameObject house4= { "house4", 9218 ,771  , 256 , "房子6", LAYER_PLHOU };
	GameObject house5= { 1,"house5", 9346 ,256  ,512,320,276,64,128,320,"棚子1", LAYER_PLHOU };
	GameObject house6= { "house6",9475 ,512  , 256 , "棚子2", LAYER_PLHOU };
	GameObject house7= { "house7",10240  ,641  ,384  , "台子1", LAYER_PLHOU };
	GameObject house8= { "house8",11136  , 385 , 512 , "房子1", LAYER_PLHOU };
	GameObject house9= { "house9", 5890 , 642 , 512 , "房子1", LAYER_PLHOU };
	GameObject house10= { "house10", 7426, 1664 , 256 , "棚子2", LAYER_PLHOU };
	GameObject house11= {1,"house11", 7935 , 1536 ,512,320,276,64,128,320,"棚子1", LAYER_PLHOU };
	GameObject house12= { "house12", 8576 ,  1664,   256 , "房子2", LAYER_PLHOU };
	GameObject house13= { "house13", 9344 ,1408  , 384 , "台子1", LAYER_PLHOU };
	GameObject house14= { "house14", 9599 , 1663 , 256 , "台子2", LAYER_PLHOU };
	GameObject house15= { "house15", 9985 , 1407 , 256 , "房子2", LAYER_PLHOU };
	GameObject item1 = { 1,"item1", 8063,511 ,128,408,224,80,20,408 ,"车1", LAYER_PLHOU };
	GameObject item2 = { 1,"item2",8192,769,8,88,120,40,8,88, "架子2", LAYER_PLHOU };
	GameObject item3 = { 1,"item3",8448, 639,8,98,120,146,8,98,"车2",LAYER_PLHOU };
	GameObject item4 = { "item4",8192,1152, 108,"墙1", LAYER_PLHOU };
	GameObject item5 = { "item5",10751,1024,108 ,"墙1",LAYER_PLHOU };
	GameObject item6 = { "item6",11520,1024,108, "墙1", LAYER_PLHOU };
	GameObject item7 = { 1,"item7",11775,128,0,324,149,60,0,324, "棚子3",LAYER_PLHOU };
	GameObject item8 = { 1,"item8",11776,1280,12,142,224,86,12,142, "车3",LAYER_PLHOU };
	GameObject item9 = { 1,"item9",11520,1664,8,98,120,146,8,98,"车2",LAYER_PLHOU };
	GameObject item10 = { 1,"item10",11392,1920,128,408,224,80,20,408 ,"车1",LAYER_PLHOU };
	GameObject item11 = { "item11",9472,1919,108,"墙4",LAYER_PLHOU };
	GameObject item12 = { 1,"item12",8704,2175,8,98,120,146,8,98,"车2",LAYER_PLHOU };

	GameObject flower1 = { 1,"flower",768,896,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower2 = { 1,"flower",896,1152,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower4 = { 1,"flower",1664,896,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower5 = { 1,"flower",4224,511,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower6 = { 1,"flower",4736,1152,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower7 = { 1,"flower",6400,128,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower8 = { 1,"flower",6272,1154,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower9 = { 1,"flower",7936,1024,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower10 = { 1,"flower",9984,384,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower11 = { 1,"flower",9856,1536,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower12 = { 1,"flower",12288,512,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower13 = { 1,"flower",12288,1408,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower14 = { 1,"flower",9728,3456,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower15 = { 1,"flower",8960,3968,48,51,"灌木1",LAYER_PLHOU,7,4 };
	GameObject flower16 = { 1,"flower",10496,4480,48,51,"灌木1",LAYER_PLHOU,7,4 };

	GameObject wall_final = { "the_final_wall",896 * 4,192 * 4,256,"墙5",LAYER_PLHOU };
	/*
	GameObject house1 = {"house1",400,400,512,"房子1",2 };
	GameObject wall1 = {"wall1",800,1300,108,"墙1",2 };
	GameObject wall2 = {"wall2",1200,1300,108,"墙1",2 };
	GameObject house3 = {"house3",0,0,384,"房子3",2 };

	GameObject ikun = { 1,"ikun",600.f,600.f,0,55,"ikun",2,4,4 };
	GameObject tupen1 = { 1,"tupen1",600,0,128,47,192,115,34,71,"土喷泉",2 };
	ShiQuTrigger tupen1_Trigger = { "tupen1_Trigger",true,tupen1,{256,256},{0,0},GRAY };

	GameObject car_tree1 = { 1,"car_tree1",0,300,128,408,224,80,20,405,"车1",2 };
	*/
	Object Splash = { 1,"Splash",0,0,0,0,"splash",LAYER_SUM - 1 };
#pragma endregion
	// 玩家player------------------------------------------------------------------------------------------------------
	Player player = { 1,"player",160,776,0,62,"idle1",2 };	playerInto = &player;

	Animator playerAnimator = { "playerAnimator", true, player };
	playerAnimatorInto = &playerAnimator;

	AnimationClip playerAnimationClip_Idle = { "idle",playerAnimator,12,12,"idle" };
	Events playerAnimationClip_Idle_Events = { &playerAnimationClip_Idle };
	playerAnimationClip_Idle_Events.AddEvent(testfuntion,11);

	AnimationClip playerAnimationClip_Walk = { "walk",playerAnimator,12,20,"walk" };

	AnimationClip playerAnimationClip_Attack_heart = { "attack_heart",playerAnimator,12,12,"attack_heart" };
	playerAnimationClip_Attack_heartInto = &playerAnimationClip_Attack_heart;

	AnimationClip playerAnimationClip_Attack = { "attack_heart",playerAnimator,12,12,"attack_heart" };
	playerAnimationClip_AttackInto = &playerAnimationClip_Attack;

	Rectangle playerCollision_Rectangle = { 0,0,46,20 };
	Collision playerCollision = { "playerCollision", true, player, {46,20},{8.f,44.f},Type_Dynamic };

	Rectangle playerTrigger_Rec = { 0,0,64,64 };
	Collision playerTrigger = { "playerTrigger",true, player,{64,64},{0,0},Type_Trigger };

	// 敌人enemy--------------------------------------------------------------------------------
	// 敌人1
	Enemy enemy1 = { 1,"enemy1",3000,776,0,61,"ikun1",2 };	enemy1Into = &enemy1;

	Animator enemy1Animator = { "enemy1Animator",true,enemy1 };
	enemy1.animator = &enemy1Animator;

	AnimationClip enemy1AnimationClip_idle = { "idle",enemy1Animator,4,6,"ikun" };

	AnimationClip enemy1AnimationClip_attack = { "attack",enemy1Animator,12,10,"enemy_body_attack" };
	Events enemy1AnimationClip_attack_Events= { &enemy1AnimationClip_attack };
	enemy1AnimationClip_attack_Events.AddEvent(enemy1LoadSence2, 10);
	
	AnimationClip enemy1AnimationClip_body_attack = { "body_attack", enemy1Animator,13,6,"ikun_attack" };
	Events enemy1AnimationClip_body_attack_Events = { &enemy1AnimationClip_body_attack };
	enemy1AnimationClip_body_attack_Events.AddEvent(Enemy_Attack, 6);

	EnemyTrigger enemy1Trigger = { enemy1,&enemy1Animator, &enemy1AnimationClip_attack };
	enemy1TriggerInto = &enemy1Trigger;

	Rectangle enemy1Collision_Rectangle = { 0,0,46,20 };
	Collision enemy1Collision = { "enemy1Collision",true,enemy1,{46,20},{8.f,44.f},Type_Dynamic };

	Bar enemy1HealthBar = {};
	enemy1HealthBar.SetValue(RED, BLACK);
	//敌人2
	Enemy enemy2 = { 1,"enemy2",3072*4,192*4,0,61,"ikun1",2 };	enemy2Into = &enemy2;

	Animator enemy2Animator = { "enemy2Animator",true,enemy2 };
	enemy2.animator = &enemy2Animator;

	AnimationClip enemy2AnimationClip_idle = { "idle",enemy2Animator,4,6,"ikun" };

	AnimationClip enemy2AnimationClip_attack = { "attack",enemy2Animator,12,10,"enemy_body_attack" };
	Events enemy2AnimationClip_attack_Events = { &enemy2AnimationClip_attack };
	enemy2AnimationClip_attack_Events.AddEvent(enemy2LoadSence2, 10);

	AnimationClip enemy2AnimationClip_body_attack = { "body_attack", enemy2Animator,13,6,"ikun_attack" };
	Events enemy2AnimationClip_body_attack_Events = { &enemy2AnimationClip_body_attack };
	enemy2AnimationClip_body_attack_Events.AddEvent(Enemy_Attack, 6);

	EnemyTrigger enemy2Trigger = { enemy2,&enemy2Animator, &enemy2AnimationClip_attack };
	enemy2TriggerInto = &enemy2Trigger;

	Rectangle enemy2Collision_Rectangle = { 0,0,46,20 };
	Collision enemy2Collision = { "enemy2Collision",true,enemy2,{46,20},{8.f,44.f},Type_Dynamic };

	Bar enemy2HealthBar = {};
	enemy2HealthBar.SetValue(RED, BLACK);
	// 小鸟（向导）---------------------------------------------------------------------------------------
	GameObject bird = { 1,"brid",800,640,0,0,"bird1",LAYER_PLHOU };
	Animator birdAnimation = {"birdAnimation",true,bird};
	bird.animator = &birdAnimation;
	AnimationClip birdAnimationClip = { "birdAnimationClip",birdAnimation,6,6,"bird" };

	BirdTell birdTell = { bird };
	//获得技能
	GameObject GetHeartAttackObject1 = { 1,"GetHeartAttackObject1",2944*4,96*4,4,-2,"箱子1",LAYER_PLHOU };
	GetSkill1_Trigger GetHeartAttackObject1_Trigger = { GetHeartAttackObject1 };

	GameObject GetAttackObject = { 1,"GetAttackObject",1920,876,28,88,"箱子2",LAYER_PLHOU };
	GetSkill2_Trigger GetAttackObject_Trigger = { GetAttackObject };
	//玩家相关的UI---------
	//血条
	Bar playerHealthBar = {};
	playerHealthBar.SetValue(GREEN, BLACK);
	//UI界面
	bag = {};

	Image_UI bagImage = {{1904,0},"背包背景" };
	bag.panel.Add(&bagImage);

	Image_UI bagImagetest = { {900,0},"背包背景test" };
	bag.panel.Add(&bagImagetest);

	Button_Bag button_bag = { {5,5},"角色" };
	button_bag.P = &bag.panel;

	////普通攻击按钮
	//Button_Attack button_Attack = { bag.GetPosition(),"hair12" };
	//bag.PushEmpty(&button_Attack);
	//button_Attack.player = &player;
	//button_Attack.playerAnimationClip_Attack = &playerAnimationClip_Attack;
	//button_Attack.playerAnimator = &playerAnimator;
	//button_Attack.SetActive(true);

	////爱心攻击按钮
	//Button_Attack_Heart button_Attack_Heart = {bag.GetPosition(),"heart3"};
	//bag.PushEmpty(&button_Attack_Heart);
	//button_Attack_Heart.player = &player;
	//button_Attack_Heart.playerAnimationClip_Attack_heart = &playerAnimationClip_Attack_heart;
	//button_Attack_Heart.playerAnimator = &playerAnimator;
	//button_Attack_Heart.SetActive(true);

	//Button_Attack_Heart button_Attack_Heart2 = { bag.GetPosition(),"heart3" };
	//bag.PushEmpty(&button_Attack_Heart2);
	//button_Attack_Heart2.player = &player;
	//button_Attack_Heart2.playerAnimationClip_Attack_heart = &playerAnimationClip_Attack_heart;
	//button_Attack_Heart2.playerAnimator = &playerAnimator;
	//button_Attack_Heart2.SetActive(true);

	// 文本Text--------------------------------------------------------------------------------------------------------
	Text text_get_skill = { false,"get_skill",{0.f,0.f} };
	text_get_skillInto = &text_get_skill;

	Text text_get_skillheart_first = { false, "get_skillheart_first",{0.f,0.f} };
	text_get_skillheart_firstInto = &text_get_skillheart_first;
	
	Text win_text = { false,"win_text",{0.f,0.f} };
	roundManager.win_text = &win_text;

	Text bird_tell_first = { false,"bird_tell_first",{0.f,0.f} };
	birdTell.bird_text_first = &bird_tell_first;

	Text bird_tell = {false,"bird_tell",{0.f,0.f} };
	birdTell.bird_text = &bird_tell;

	Text start_text = { false,"start_text",{0.f,0.f} };

	Dialog_Box::PushText(&start_text);
	Dialog_Box::SetActive(true);

	ToggleFullscreen();
	// 游戏主循环 ///////////////////////////////////////////////////////////////////////////////////////////////////
	while (!WindowShouldClose())	// Detect window close button or ESC key
	{
		UpdateMusicStream(strollMusic);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			if (Splash.GetActive_Object())
			{
				Splash.SetActive_Object(false);
			}
		}
		if (!Splash.GetActive_Object())
		{

			UpdateMusicStream(battleMusic);
			roundManager.Update();
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				if (SceneIndex == 2 && roundManager.BattleFinish())
				{
					LoadSence(1);
				}
			}
			if (IsKeyPressed(KEY_L))
			{
				if (IsMusicStreamPlaying(strollMusic))
				{
					PauseMusicStream(strollMusic);
				}
				else
				{
					ResumeMusicStream(strollMusic);
				}
			}
			if (IsKeyPressed(KEY_Q))
			{
				new Button_Attack_Heart{ bag.GetPosition(),"heart3" ,&playerAnimationClip_Attack_heart, &playerAnimator };
			}
			if (IsKeyPressed(KEY_F))
			{
				ToggleFullscreen();
			}
			if (IsKeyPressed(KEY_K))
			{
				enemy1.Die();
			}
			if (IsKeyPressed(KEY_J))
			{
				enemy1.GetHurt(1);
			}
			if (IsKeyPressed(KEY_B))
			{
				for (auto const& T : player.TriggerList)
				{
					T->Active();
				}
			}
			if (IsKeyPressed(KEY_R))
			{
				Skill_Heart_Attack* skillptr = new Skill_Heart_Attack{ &playerAnimationClip_Attack_heart,RIGHT };
				Events* events = new Events{ &playerAnimationClip_Attack_heart };
				events->AddEvent(Player_Attack_Heart, 11);
				skillptr->SetPosition(player.position);
				playerAnimator.TransintoClip(&playerAnimationClip_Attack_heart);
				playerAnimator.SetNextClip(&playerAnimationClip_Idle);
				roundManager.skill = skillptr;
			}
			if (IsKeyPressed(KEY_Z))
			{
				LoadSence(1);
			}
			if (IsKeyPressed(KEY_X))
			{
				LoadSence(2);
			}
			// 逻辑更新 ************************************************************************************************
			// 人物移动
			if (IsKeyDown(KEY_T))
			{
				bag.panel.SetActive(!bag.panel.GetActive());
			}
			Vector2 walk_InputForward = { 0.f,0.f };
			if (!Dialog_Box::GetActive())
			{
				if (IsKeyDown(KEY_A))
				{
					walk_InputForward += LEFT;
				}
				if (IsKeyDown(KEY_D))
				{
					walk_InputForward += RIGHT;
				}
				if (IsKeyDown(KEY_S))
				{
					walk_InputForward += DOWN;
				}
				if (IsKeyDown(KEY_W))
				{
					walk_InputForward += UP;
				}
				if (IsKeyPressed(KEY_E))
				{
					for (auto const& T : player.TriggerList)
					{//根据与playerTrigger相碰的Trigger进行处理
					}
				}
			}
			if (walk_InputForward != ZERO_VECTOR)
			{
				walk_InputForward = walk_InputForward.normalize();

				if (playerAnimator.currentClip == 0)
					playerAnimator.TransintoClip(&playerAnimationClip_Walk);
			}
			else
			{
				if (playerAnimator.currentClip == 1)
					playerAnimator.TransintoClip(&playerAnimationClip_Idle);
			}
			if (IsKeyDown(KEY_LEFT_SHIFT))
			{
				PLAYER_MOVE_SPEED = 10.8f;
			}
			else
			{
				PLAYER_MOVE_SPEED = 2.8f;
			}
			Vector2 walk_MoveForward = walk_InputForward * PLAYER_MOVE_SPEED;
			player.position += walk_MoveForward;

			//对话框
			if (Dialog_Box::GetActive())
			{
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					Dialog_Box::Next();
				}
			}
			if (IsKeyPressed(KEY_O))
			{
				Dialog_Box::SetActive(!Dialog_Box::GetActive());
			}
			if (IsKeyPressed(KEY_I))
			{
				Dialog_Box::PushText(&text_get_skillheart_first);
			}
			//UI点击检测
			for (auto const& U : UIList)
			{
				if (SceneIndex == 1 || (SceneIndex == 2 && roundManager.IfCanAct(&player)))
				{
					if (U->GetActive())
					{
						if (U->CheckClick(IsMouseButtonDown(MOUSE_BUTTON_LEFT), IsMouseButtonReleased(MOUSE_BUTTON_LEFT), GetMousePosition()))
						{
						}
					}
				}
			}

			//UI逻辑更新
			Vector2 playerHealthBar_Vector2 = player.position;
			playerHealthBar_Vector2 += {-32.f, -25.f};
			playerHealthBar.SetPosition(GetPositionOnScreen(playerHealthBar_Vector2, &camera.position));
			playerHealthBar.SetPercentage(player.health, player.max_health);
			playerHealthBar.SetActive(player.GetActive_Object());

			Vector2 enemy1HealthBar_Vector2 = enemy1.position;
			enemy1HealthBar_Vector2 += {-32.f, -25.f};
			enemy1HealthBar.SetPosition(GetPositionOnScreen(enemy1HealthBar_Vector2, &camera.position));
			enemy1HealthBar.SetPercentage(enemy1.health, enemy1.max_health);
			enemy1HealthBar.SetActive(enemy1.GetActive_Object());

			Vector2 enemy2HealthBar_Vector2 = enemy2.position;
			enemy2HealthBar_Vector2 += {-32.f, -25.f};
			enemy2HealthBar.SetPosition(GetPositionOnScreen(enemy2HealthBar_Vector2, &camera.position));
			enemy2HealthBar.SetPercentage(enemy2.health, enemy2.max_health);
			enemy2HealthBar.SetActive(enemy2.GetActive_Object());
			// 物理系统 *******************************************************************************************
			//触发类、动态类碰撞体要同步--------------------
			for (auto& C : CollisionList_Dynamic)
			{
				//数据同步
				if (C->gameObject.GetActive_Object())
					C->SyncPosition();
			}
			for (auto& C : CollisionList_Trigger)
			{
				//数据同步
				if (C->gameObject.GetActive_Object())
					C->SyncPosition();
			}
			//处理触发类碰撞体------------------------------
			player.TriggerList.clear();
			for (auto& T : CollisionList_Trigger)
			{
				if (T->gameObject.GetActive_Object())
				{
					if (T != &playerTrigger)
					{
						//判断是否与玩家碰撞，进行各种处理
						if (T->CheckCollidor(playerTrigger.Rec_Current))
						{
							player.TriggerList.push_back(T);
							T->OnTrigger(playerTrigger.gameObject);
						}
						else
						{
							T->NotTrigger(playerTrigger.gameObject);
						}
					}
				}
			}
			//处理动态类碰撞体------------------------------
			for (auto C : CollisionList_Dynamic)
			{
				if (C->gameObject.GetActive_Object())
				{
					//处理信息
					for (auto const Other : CollisionList_Static)
					{//静态Collision
						if (Other->gameObject.GetActive_Object())
							C->CheckCollidorInUpdate(Other->Rec_Current);
					}
					for (auto const Other : CollisionList_Dynamic)
					{//动态Collision
						if (Other->gameObject.GetActive_Object())
							if (C != Other)
							{
								C->CheckCollidorInUpdate(Other->Rec_Current);
							}
					}
				}
			}
			// 相机逻辑***************************************************************************************************
			Vector2 cameraPosition = player.position;
			cameraPosition -= camera.view * 0.5f;
			cameraPosition -= {32.f, 128.f};
			camera.SetPosition(cameraPosition);
			// 绘制****************************************************************************************************
			//TODO: 这里有一个整理，接收图层序号，然后按Y值大小排序，由小到大.
			BeginDrawing();
			ClearBackground(BLACK);
			for (auto& L : SceneList[SceneIndex]->ObjectList)
			{//图层
				L.sort(CompareObjectByy);
				for (auto& O : L)
				{//物体
					if (O->GetActive_Object())
					{
						if (O == &Map_1 || O == &Map_2)
						{
							O->Draw(&camera.position);
						}
						else if (O == &battleObject)
						{
						}
						else if (camera.IfInView(&O->position))
						{//要绘制的物体
							O->Draw(&camera.position);
						}
					}
				}
			}
			//绘制UI*********************************************************
			for (auto& U : UIList)
			{
				U->DrawNew();
			}
			Vector2 zeroV = { 0,0 };
			if (battleObject.GetActive_Object())
			{
				battleObject.Draw(&zeroV);
			}
			//这里更新场景AnimatorList的成员***************************************************************************

			for (auto& A : AnimatorList)
			{
				if (A->gameObject.GetActive_Object())
				{
					AnimationClip* AnimationC = A->AnimationClipList.at(A->currentClip);
					if (AnimationC->countor == 0 && AnimationC->Events != nullptr)
					{
						AnimationC->Events->EventList[AnimationC->indicator]();
					}
					A->UpdateCurrentClip();
				}
			}

			// 打印文本的地方******************************************************************************************
			for (auto& T : TextList)
			{
				T->PrintOnScreen(camera);
			}
			//打印对话框*************************************************************************************************
			Dialog_Box::Print();
			//绘制碰撞箱的地方******************************************************************************************
			/*for (const auto& C : CollisionList_Dynamic)
			{
				DrawRecOnScreen(C->Rec_Current, camera, GREEN);
			}
			for (const auto& C : CollisionList_Static)
			{
				DrawRecOnScreen(C->Rec_Current, camera, RED);
			}
			for (const auto& C : CollisionList_Trigger)
			{
				DrawRecOnScreen(C->Rec_Current, camera, YELLOW);
			}*/
		}
		else
		{
			BeginDrawing();
			ClearBackground(BLACK);
			
			Vector2 V = { 0.f,0.f };
			Splash.Draw(&V);
		}
		//结束这一帧的绘制 ///////////////////////////////////////////////////////////////////////////////////////////
		cout << "一帧" << endl;
		EndDrawing();

	}
	// 释放音乐
	UnloadMusicStream(strollMusic);
	// 释放音效
	UnloadSound(hitSoundEffect);

	// 去初始化
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	CloseAudioDevice();

	//--------------------------------------------------------------------------------------

	return 0;
	//定义--------------------------------------------------------------------------
}