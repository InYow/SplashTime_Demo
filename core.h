#pragma once
#include "raylib.h"
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <functional>
class Animator;
class GameObject;
//��������void, �����б�Ϊ��
using Event = void(*)();
class Events;
using namespace std;

namespace mathf
{
	int Carry(int digit, int value)
	{
		int x = value + 1;
		return x % digit;
	}
}

constexpr auto LAYER_SUM = 11;
constexpr auto LAYER_UI = 9;
constexpr auto LAYER_PLHOU = 2;
constexpr auto GAME_FPS = 60;
float PLAYER_MOVE_SPEED = 4.8f;
constexpr float SCREENWIDTH = 2160.f;
constexpr float SCREENHEIGHT = 1440.f;
bool Intoing = false;
double testtime = 0.l;

//����-----------------------------------------------------------------------
//(int sence, float position_x, float position_y, string path, int layer, int sum_frame, int speed_frame)
class Object
{
public:
	// ��������
	string name;
	Vector2 position;//���������λ��
	Vector2 pivot;//ê�㡣�������������ԭ���ƫ����
	int layer;//����������ͼ��
	int InWhichScene;//���ڳ���
	//int IndexInScene;//�ڳ����еĵ�ַ
	Color color;
	// ͼƬ
	string PNG_path;//PNG��ַ�ؼ���
	Texture Object_texture;
	Texture* texture;//���������ַ
	Texture*& textureTodraw = texture;
private:
	bool active_Object;
public:
	void Draw(const Vector2* camera_position);
	Object(int sence, string name, float position_x, float position_y, float pivot_x, float pivot_y, string path, int layer);
	Object(string name, float position_x, float position_y, float pivot_x, float pivot_y, string path, int layer);
	void ChangeSence(int TargetScene, int targetLayer);
	void SetActive_Object(bool B);
	bool GetActive_Object();
	~Object();
};

bool CompareObjectByy(const Object* a, const Object* b)
{
	return (a->position.y + a->pivot.y) < (b->position.y + b->pivot.y);
}
class  DefaultCamera
{
public:
	Vector2 position;
	Vector2 view;
	Vector2 rangeLeftUp;
	Vector2 rangeRightDown;
public:
	bool IfInView(const Vector2* Object_position);
	void SetPosition(Vector2 V);
};
Vector2 GetPositionOnScreen(const Vector2 World_position, const Vector2* camera_position);
DefaultCamera camera{};

class Scene
{
public:
	string name;
	int index;
	list<Object*> ObjectList[LAYER_SUM];//������ͼ����б�.
public:
	Scene(string name, int indexInList);
};
Scene* SceneList[3] = {
new Scene{"StartSence",0},
new Scene{"ExploreScene",1},
new Scene{"FightScene",2}
};
int SceneIndex;

class Component
{
public:
	string name;
	bool active;
	GameObject& gameObject;
	int index_gameObject;
public:
	Component(string name, bool active, GameObject& gameObject);
	//����
	virtual void Active();
	//��ӵ�������
	virtual void AddToList(GameObject& gameObject);
	//��������ɾ�����ͷ�����
	virtual void DeleteFromList();
	~Component();
};
class GameObject :public Object
{
public:
	vector<Component*> ComponentList;
	Animator* animator = nullptr;
public:
	GameObject(int sence, string name, float position_x, float position_y, float pivot_x, float pivot_y, string path, int layer);
	//�Դ���̬����ײ�����Ϸ����
	GameObject(int sence, string name, float posi_x, float posi_y, float pivot_x, float pivot_y, float C_width, float C_height, float C_x, float C_y, string path, int layer);
	//��ײ���Ե��ײ������غϵ���Ϸ����
	GameObject(string name, float posi_x, float posi_y, float pivot_y, string path, int layer);
	//�Դ������������Ϸ����
	GameObject(int sence, string name, float position_x, float position_y, float pivot_x, float pivot_y, string path, int layer, int sum_frame, int speed_frame);
	~GameObject();
};

class AnimationClip
{
public:
	string name;
	int index;//��Animator�еı��
	int sum;//����֡
	int speed;//֡��
	float time;//����ʱ��
	int indicator;//��ǰ֡
	int countor;//֡������
	int digit;//֡λ��
	string PNG_path;//PNG��ַ�ؼ���
	Image* image;//image����
	Texture* texture;//texture����
	Events* Events;//�����¼�
public:
	AnimationClip(string name, Animator& animator, int sum_frame, int speed_frame, string path);
	void UpdateCountor();
	void AddToList(Animator& animator);
	void ToDefault();
	~AnimationClip();
};
class Animator : public Component
{
public:
	vector<AnimationClip*> AnimationClipList;
	int currentClip;
	int nextClip;
public:
	Animator(string name, bool active, GameObject& gameObject);
	void TransintoUnderGivenClip(string name, AnimationClip* Clip);
	void TransintoClip(AnimationClip* Clip);
	void TransintoClip(int ClipIndex);
	void SetClip(int ClipIndex);
	void SetNextClip(AnimationClip* Clip);
	void SetNextClip(int index);
	void AddToAnimatorList(int WhichSence);
	void DeleteFromAnimatorList();
	void UpdateCurrentClip();
	~Animator();
};
void DefaultEvent()
{
};
class Events
{
public:
	AnimationClip* Clip;
	Event* EventList;//�¼�����
public:
	Events(AnimationClip* animationClip);
	//�Ǽ��¼�����Ӧλ��֡
	void AddEvent(Event event, int frame);
	//ɾ����Ӧλ��֡���¼�
	void DeleteEvent(int frame);
};
typedef enum MotionType
{
	Type_Dynamic = 0,
	Type_Static,
	Type_Trigger
};
class Collision : public Component
{
public:
	Rectangle Rec_Last;
	Rectangle Rec_Current;
	MotionType Type;
	Vector2 Vector_Offset;
	int Index_CollisionList;
	/*float ElasticityCoefficient;*/
public:
	Collision(string name, bool active, GameObject& gameObject, Vector2 Size, Vector2 Offset, MotionType Type /*, float ElasticityCoefficient*/);
	void DeleteFromList() override;
	void TransintoType(MotionType type);

	void TransintoDynamic();
	void TransintoStatic();
	void TransintoTrigger();

	void SyncPosition();
	bool CheckCollidor(Rectangle& OtherRec);//����Ƿ���ײ
	bool CheckCollidor(Collision& OtherCollision);//����Ƿ���ײ
	void BackToLastPosition();//������һ֡���ڵ�λ��

	void virtual OnTrigger(GameObject& gameObject);
	void virtual NotTrigger(GameObject& gameObject);
	void virtual Active()
	{}
	//ֻ������ѭ����ʹ�õĺ���
	void CheckCollidorInUpdate(Rectangle& OtherRec);
private:
	void DeleteFromCollisionList();
	void AddToCollisionList();
};
vector<Collision*> CollisionList_Dynamic;
vector<Collision*> CollisionList_Static;
vector<Collision*> CollisionList_Trigger;
#define LEFT CLITERAL(Vector2){-1.f,0.f}
#define DOWN CLITERAL(Vector2){0.f,1.f}
#define RIGHT CLITERAL(Vector2){1.f,0.f}
#define UP CLITERAL(Vector2){0.f,-1.f}
#define ZERO_VECTOR CLITERAL(Vector2){0.f,0.f}
#define FONT_COLOR CLITERAL(Color) {36, 35, 53, 255}
void DrawRecOnScreen(Rectangle Rec, DefaultCamera& Camera, Color Colo)
{
	Vector2 position = GetPositionOnScreen({ Rec.x, Rec.y }, &Camera.position);
	DrawRectangleLines(position.x, position.y, Rec.width, Rec.height, Colo);
}
class Text
{
public:
	int size;
	bool active_Text;
	string path;
	char* data;
	char* data_ToPrint;
	Vector2 position;
	int Index;
public:
	Text(bool active, string path, Vector2 position, int size);
	Text(bool active, string path, Vector2 position);
	void SetActive(bool active);
	void virtual PrintOnScreen(DefaultCamera camera);
private:
	void AddToList();
	void DeleteFromList();
};
Font font;//���ƺ���ʹ�õ�����
char* ALLCHARACTER;//��ʼ������������ַ�
char DELIMITER;//����ķָ����
constexpr auto FONTSIZE = 32;
constexpr auto FONTSPACE = 4;
class Text_Component :public Component, public Text
{
public:
	Text_Component(string name, bool active, GameObject& gameObject, string path, Vector2 position, int size);
	Text_Component(string name, bool active, GameObject& gameObject, string path, Vector2 position);
public:
	void PrintOnScreen(DefaultCamera camera)override;
};
vector<Text*> TextList;
unsigned char* fontFileData; int fileSize;
class Dialog_Box
{
private:
	static Text* textptr;
	static Rectangle Rec;
	static const  char** textList;
	static int List_Count;
	static int List_Index;
	static bool active;
	static Color Color_Under;
	static Color Color_Line;
public:
	static void PushText(Text* T);
	static void SetText(Text* T);
	static void PopText();
	static void SetActive(bool B);
	static void ProcessText(const char* text);
	static void Next();
	static void Print();
	static bool GetActive();
};
Text DefaultDialog = { false,"default_dialog",{0.f,0.f} };//�Ի����Ĭ������
Text* Dialog_Box::textptr = &DefaultDialog;
Rectangle Dialog_Box::Rec = { SCREENWIDTH / 2 - 600,SCREENHEIGHT - 100 - 300,1200,300 };
const char** Dialog_Box::textList = TextSplit(DefaultDialog.data, DELIMITER, &List_Count);
int Dialog_Box::List_Count = 0;
int Dialog_Box::List_Index = 0;
bool Dialog_Box::active = true;
Color Dialog_Box::Color_Under = { 226, 227, 234, 200 };
Color Dialog_Box::Color_Line = { 163, 157, 163, 255 };
//�ڴ˴���дTrigger���͵�Collision
//����һ��Ϊģ��
class TamplateTrigger :public Collision
{
public:
	//�Զ����Ա�ͷ���
private:
	//�Զ����Ա�ͷ���

public:
	virtual void OnTrigger(GameObject& gameObject) override
	{//��������д������
	}
	//Ĭ�Ϲ��캯��
	TamplateTrigger(string name, bool active, GameObject& gameObject, Vector2 Size, Vector2 Offset) :Collision(name, active, gameObject, Size, Offset, Type_Trigger)
	{
	}
};
////////////////////////////���±�д�Զ������///////////////////////////////
//class ShiQuTrigger :public Collision
//{
//public:
//	Color color;
//	GameObject* Skill;
//	//�Զ����Ա�ͷ���
//private:
//	//�Զ����Ա�ͷ���
//
//public:
//	void OnTrigger(GameObject &gameObject) override
//	{//��������д������
//		this->gameObject.color = color;
//	}
//	//Ĭ�Ϲ��캯��
//	ShiQuTrigger(string name, bool active, GameObject& gameObject, Vector2 Size, Vector2 Offset,Color color) :Collision(name, active, gameObject, Size, Offset, Type_Trigger)
//	{
//		this->color = color;
//	}
//};
Sound* hitSoundEffectInto;
enum Tag
{
	entity = 0,
	player,
	enemy
};
class Entity :public GameObject
{
public:
	int max_health;
	int health;
	int ATK;
	int speed;
	int slow_speed;
	Entity* Target;
	Tag tag;
public:
	Entity(int sence, string name, float position_x, float position_y, float pivot_x, float pivot_y, string path, int layer) :GameObject(sence, name, position_x, position_y, pivot_x, pivot_y, path, layer)
	{
		max_health = 5;
		health = max_health;
		ATK = 1;
		Target = nullptr;
		speed = 100;
		slow_speed = 150;
		tag = entity;
	}
	//������ֵ
	virtual void SetValue(int max_health, int atk)
	{
		max_health = max_health;
		ATK = atk;
	}
	//������ֵ
	void SyncHealthToMax()
	{
		health = max_health;
	}

	//�����Ǹ��ַ���
	virtual void SetTarget(Entity* const T)
	{
		Target = T;
	}
	virtual void GetHurt(int damage)
	{
		health -= damage;
		if (health <= 0)
		{
			health = 0;
			Die();
		}
	}
	virtual void Die()
	{
		//����д����
		//��������״̬�����ٸ�����ȵȡ�������
	}
};
class Player;
class Enemy;
class Skill :public GameObject
{
public:
	bool finished;
	double inittime;
	AnimationClip* Master_Clip;
public:
	Skill(string name, string path, int sum_frame, int speed_frame, AnimationClip* Clip) :GameObject(SceneIndex, name, 0.f, 0.f, 0.f, 0.f, path, LAYER_PLHOU, sum_frame, speed_frame)
	{
		Master_Clip = Clip;
		inittime = GetTime();
		finished = false;
	}
	virtual void SetPosition(Vector2 V)
	{
		position = V;
	}
	virtual void Update()
	{
	}
};
class Skill_Enemy_Attack :public Skill
{
public:
	Vector2 moveForward;
	Skill_Enemy_Attack(AnimationClip* Clip, Vector2 moveforward) :Skill("Skill_Enemy_Attack", "enemy_attack", 4, 15, Clip)
	{
		moveForward = moveforward;
	}
	void Update() override
	{
		if (GetTime() - inittime > 1.21)
		{
			finished = true;
			cout << "Skill_Enemy_Attack ������" << endl;
		}
		else
		{
			cout << "Skill_Enmey_Attack ������" << endl;
			position += moveForward * 15.f;
		}
	}
};
class IntoBattle;
void Enemy_Attack();
void LoadSence(int sence);
//����ս��ʱ��roundManager��ʼ��. 
//�������������Ǽ������Ǽ�
class RoundManager
{
public:
	Text* win_text;
	list<Entity*> entityList;
	list<Entity*> roundList;
	Skill* skill;
	GameObject* Locker;
public:
	void Init()
	{
		skill = nullptr;
		entityList.clear();
		roundList.clear();
	}
	void AddEntity(Entity* E)
	{
		entityList.push_back(E);
	}
	void DeleteEntity(Entity* E)
	{
		entityList.remove(E);
	}
	bool BattleFinish()
	{
		if (entityList.empty())
		{
			return true;
		}
		else
		{
			Tag t = entityList.front()->tag;
			for (auto const& enti : entityList)
			{
				if (t != enti->tag)
				{
					return false;
				}
			}
			return true;
		}
	}
	bool IfCanAct(Entity* entity)
	{
		return roundList.front() == entity && skill == nullptr;
	}
	//���߼��Ŀ�ͷִ��
	void Update()
	{
		if (SceneIndex == 2)
		{
			if (!BattleFinish())
			{
				Locker->SetActive_Object(true);
				BuildRound();
				Locker->position = roundList.front()->position;
				Locker->position += {-96.f, -128.f};
				if (skill != nullptr)
				{
					skill->Update();
					if (skill->finished)
					{
						skill->SetActive_Object(false);
						skill = nullptr;
						roundList.pop_front();
					}
				}
				//����ǵ�����������˽���
				if (roundList.front()->tag == enemy && skill == nullptr)
				{
					Skill_Enemy_Attack* skillptr = new Skill_Enemy_Attack{ roundList.front()->animator->AnimationClipList[2],LEFT };
					Vector2 v = roundList.front()->position;
					v += {-16.f, -128.f};
					skillptr->SetPosition(v);
					roundList.front()->animator->TransintoClip(2);
					roundList.front()->animator->SetNextClip(0);
					this->skill = skillptr;
				}
			}
			else
			{
				skill->SetActive_Object(false);
				Locker->SetActive_Object(false);
				if (!Dialog_Box::GetActive())
				{
					Dialog_Box::SetText(win_text);
					Dialog_Box::SetActive(true);
				}
			}
		}
	};
	void BuildRound()
	{
		if (!BattleFinish())
			if (roundList.size() < 2)
			{
				Entity* fastest = entityList.front();
				for (auto& enti : entityList)
				{
					if (enti->speed < fastest->speed)
					{
						fastest = enti;
					}
				}
				roundList.push_back(fastest);
				fastest->speed += fastest->slow_speed;
			}
		if (!roundList.empty())
		{
			for (auto const& roun : roundList)
			{
				cout << "�еĻغ�Ϊ" << roun->name << ",  ";
			}
			cout << endl;
		}
	}
};
RoundManager roundManager;
class Skill_Heart_Attack :public Skill
{
public:
	Vector2 moveForward;
	Skill_Heart_Attack(AnimationClip* Clip, Vector2 moveforward) :Skill("Skill_Heart_Attack", "heart", 4, 12, Clip)
	{
		moveForward = moveforward;
	}
	void Update()override
	{
		if (GetTime() - inittime > 1.2l)
		{
			finished = true;
			cout << "Skill_Heart_Attack ������" << endl;
		}
		else
		{
			cout << "Skill_Heart_Attack ������" << endl;
			position += {15.f, 0};
		}
	}
};
//������ģ��
class Skill_Attack :public Skill
{
public:
	Vector2 moveForward;
	Skill_Attack(AnimationClip* Clip, Vector2 moveforward) :Skill("Skill_Attack", "hair", 12, 30, Clip)
	{
		moveForward = moveforward;
	}
	virtual void Update()
	{
		if (GetTime() - inittime > 1.21)
		{
			finished = true;
			cout << "Skill_Attack ������" << endl;
		}
		else
		{
			cout << "Skill_Attack ������" << endl;
			position += {15.f, 0};
		}
	}
};
class Enemy :public Entity
{
public:
	Vector2 PosiIn2;
public:
	Enemy(int sence, string name, float position_x, float position_y, float pivot_x, float pivot_y, string path, int layer) :Entity(sence, name, position_x, position_y, pivot_x, pivot_y, path, layer)
	{
		tag = enemy;
		PosiIn2 = { 1400.f - 2160.f,700.f };
		//�������ʼ��
	}
	void Die()
	{
		roundManager.DeleteEntity(this);
		SetActive_Object(false);
	}
};
class Player :public Entity
{
public:
	vector<Collision*> TriggerList;
	Vector2 PosiIn1;
	Vector2 PosiIn2;
public:
	Player(int sence, string name, float position_x, float position_y, float pivot_x, float pivot_y, string path, int layer) :Entity(sence, name, position_x, position_y, pivot_x, pivot_y, path, layer)
	{
		max_health = 19;
		health = 19;
		speed = 80;
		tag = player;
		PosiIn1 = { 0,0 };
		PosiIn2 = { 400.f - 2160.f,700.f };
		//�˴���ʼ��
	}
};
Enemy* enemy1Into;
Enemy* enemy2Into;
Enemy* enemy3Into;
Player* playerInto;

void Player_Attack_Heart()
{
	for (auto& entity : roundManager.entityList)
	{
		if (entity->tag == enemy)
		{
			entity->GetHurt(2);
			PlaySound(*hitSoundEffectInto);
			return;
		}
	}
}
void Enemy_Attack()
{
	for (auto& entity : roundManager.entityList)
	{
		if (entity->tag == player)
		{
			entity->GetHurt(1);
			PlaySound(*hitSoundEffectInto);
			return;
		}
	}
}
void Player_Attack()
{
	for (auto& entity : roundManager.entityList)
	{
		if (entity->tag == enemy)
		{
			entity->GetHurt(1);
			PlaySound(*hitSoundEffectInto);
			return;
		}
	}
}

Music* battleMusicInto;
Music* StrollMusicInto;
void LoadSence(int sence)
{
	switch (sence)
	{
	case 1:
	{
		if (Dialog_Box::GetActive())
		{
			Dialog_Box::SetActive(false);
		}
		SceneIndex = sence;
		camera.rangeLeftUp = { 0.f,0.f };
		camera.rangeRightDown = { 19072.f, 6144.f };
		if (playerInto != nullptr)
		{
			playerInto->position = playerInto->PosiIn1;
			playerInto->ChangeSence(1, LAYER_PLHOU);
		}
		if (IsMusicStreamPlaying(*battleMusicInto))
		{
			StopMusicStream(*battleMusicInto);
		}
		ResumeMusicStream(*StrollMusicInto);
		break;
	}
	case 2:
	{
		if (Dialog_Box::GetActive())
		{
			Dialog_Box::SetActive(false);
		}
		SceneIndex = sence;
		Vector2 LeftUp, RightDown;
		Object* O = SceneList[SceneIndex]->ObjectList->front();
		LeftUp = O->position;
		RightDown = { O->position.x + O->Object_texture.width, O->position.y + O->Object_texture.height };
		camera.rangeLeftUp = LeftUp;
		camera.rangeRightDown = RightDown;
		break;
	}
	default:
		SceneIndex = sence;
		Vector2 LeftUp, RightDown;
		Object* O = SceneList[SceneIndex]->ObjectList->front();
		LeftUp = O->position;
		RightDown = { O->position.x + O->Object_texture.width, O->position.y + O->Object_texture.height };
		camera.rangeLeftUp = LeftUp;
		camera.rangeRightDown = RightDown;
		break;
	}
}

class BattleObject : public GameObject
{
public:
	Enemy* startEntity;
public:
	using GameObject::GameObject;
};
BattleObject* battleObjectInto;

void battleObjectLoadSence()
{
	battleObjectInto->startEntity->speed = 100;
	playerInto->speed = 110;
	roundManager.AddEntity(battleObjectInto->startEntity);
	roundManager.AddEntity(playerInto);
	battleObjectInto->startEntity->ChangeSence(2, LAYER_PLHOU);
	playerInto->ChangeSence(2, LAYER_PLHOU);
	playerInto->PosiIn1 = playerInto->position;
	playerInto->position = playerInto->PosiIn2;
	battleObjectInto->startEntity->position = battleObjectInto->startEntity->PosiIn2;
	LoadSence(2);
	battleObjectInto->SetActive_Object(false);
	Intoing = false;
}

class EnemyTrigger :public Collision
{
public:
	//�Զ����Ա�ͷ���
	Animator* animator;
	AnimationClip* clip;
	bool Intoing = false;
private:
	//�Զ����Ա�ͷ���

public:
	void OnTrigger(GameObject& gameObject) override
	{//��������д������
		if (!Intoing)
		{
			animator->TransintoClip(clip);
			animator->SetNextClip(0);
			Intoing = true;
		}
	}
	void NotTrigger(GameObject& gameObject) override
	{
		if (animator->currentClip == 1)
		{
			animator->TransintoClip(0);
		}
		if (animator->currentClip == 0)
		{
			Intoing = false;
		}
	}
	//Ĭ�Ϲ��캯��
	EnemyTrigger(GameObject& gameObject, Animator* anima, AnimationClip* cl) :Collision("enemyTrigger", true, gameObject, { 192,192 }, { -64,-64 }, Type_Trigger)
	{
		animator = anima;
		clip = cl;
	}
};

EnemyTrigger* enemy1TriggerInto;
EnemyTrigger* enemy2TriggerInto;
EnemyTrigger* enemy3TriggerInto;

void enemy1LoadSence2()
{
	roundManager.Init();

	battleObjectInto->startEntity = enemy1Into;
	battleObjectInto->SetActive_Object(true);
	battleObjectInto->animator->SetClip(0);
	PlaySound(*hitSoundEffectInto);
	enemy1TriggerInto->Intoing = true;
	PauseMusicStream(*StrollMusicInto);
	PlayMusicStream(*battleMusicInto);
}
void enemy2LoadSence2()
{
	roundManager.Init();

	battleObjectInto->startEntity = enemy2Into;
	battleObjectInto->SetActive_Object(true);
	battleObjectInto->animator->SetClip(0);
	PlaySound(*hitSoundEffectInto);
	enemy2TriggerInto->Intoing = true;
	PauseMusicStream(*StrollMusicInto);
	PlayMusicStream(*battleMusicInto);
}
void enemy3LoadSence2()
{
	roundManager.AddEntity(enemy3Into);
	roundManager.AddEntity(playerInto);
	LoadSence(2);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////       U   I        ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
class UI;
vector<UI*> UIList;
class UI
{
private:
	bool active;
public:
	int index;
public:
	UI()
	{
		index = UIList.size();
		UIList.push_back(this);
		active = true;
	}
	void DrawNew()
	{
		if (active)
		{
			Draw();
		}
	}
	virtual void Draw()
	{

	};
	virtual bool CheckClick(bool B, bool Bpressed, Vector2 point)
	{
		return false;
	}
	virtual void DeleteFromList()
	{
		UIList.erase(UIList.begin() + index);
	}
	virtual void SetActive(bool B)
	{
		if (active != B)
		{
			active = B;
		}
	}
	virtual bool GetActive()
	{
		return active;
	}
};

class Button :public UI
{
public:
	Rectangle Rec;
	Texture texture;
	Color color;
public:
	Button(Vector2 position, string path)
	{
		Rec.x = position.x;
		Rec.y = position.y;
		texture = LoadTexture(("Image/" + path + ".png").c_str());
		Rec.width = texture.width;
		Rec.height = texture.height;
		color = WHITE;
	}
	virtual bool CheckClick(bool B, bool Bpressed, Vector2 point) override
	{
		if (Bpressed)
		{
			if (CheckCollisionPointRec(point, Rec))
			{
				color = GRAY;
				ActivePressed();
				return true;
			}
			else
			{
				color = WHITE;
			}
		}
		else
		{
			color = WHITE;
		}
		if (B)
		{
			if (CheckCollisionPointRec(point, Rec))
			{
				color = GRAY;
				Active();
				return true;
			}
			else
			{
				color = WHITE;
				return false;
			}
		}
		else
		{
			color = WHITE;
			return false;
		}
	}
	virtual void Active()
	{
	}
	virtual void ActivePressed()
	{
	}
	void Draw()
	{
		DrawTexture(texture, Rec.x, Rec.y, color);
	}
};

class Panel :public UI
{
public:
	list<UI*> List;
public:
	void Add(UI* UI)
	{
		List.push_back(UI);
	}
	void Delete(UI* UI)
	{
		List.remove(UI);
	}
	void SetActive(bool B) override
	{
		for (auto& U : List)
		{
			U->SetActive(B);
		}
		UI::SetActive(B);
	}
};

class Image_UI :public UI
{
public:
	Vector2 position;
	Texture texture;
public:
	Image_UI(Vector2 position, string path)
	{
		this->position = position;
		texture = LoadTexture(("Image/" + path + ".png").c_str());
	}
	void Draw()
	{
		DrawTexture(texture, position.x, position.y, WHITE);
	}
};

class Bar : public UI
{
public:
	Rectangle Rec_Back;
	Rectangle Rec_Lines;
	Color BackColor;
	Color LineColor;
public:
	Bar()
	{
		Rectangle R = { 0,0,128,12 };
		Rec_Back = R;
		Rec_Lines = R;
		BackColor = GREEN;
		LineColor = BLACK;
	}
	void SetValue(Color B, Color L, Rectangle R)
	{
		SetValue(B, L);
		Rec_Back = R;
		Rec_Lines = R;
	}
	void SetValue(Color B, Color L)
	{
		BackColor = B;
		LineColor = L;
	}
	void SetPosition(Vector2 P)
	{
		Rec_Back.x = P.x;
		Rec_Back.y = P.y;

		Rec_Lines.x = P.x;
		Rec_Lines.y = P.y;
	}
	void SetPercentage(float value, float max)
	{
		float percentage = value / max;
		Rec_Back.width = Rec_Lines.width * percentage;
	}
	void Draw()
	{
		DrawRectangleRounded(Rec_Back, 30, 10, BackColor);
		DrawRectangleRoundedLines(Rec_Lines, 30, 10, 3, LineColor);
	}
};

class Bag
{
public:
	Panel panel = {};
	Button* buttonList[12];
	int buttonList_size;
public:
	Vector2 GetPosition()
	{
		int a = 0;
		for (int i = 0; i < 12; i++)
		{
			if (buttonList[i] == nullptr)
			{
				a = i;
				break;
			}
		}
		int i = a % 4;
		int j = a / 4;
		Vector2 v = { 1904,0 };
		v.x += i * 64;
		v.y += j * 64;
		return v;
	}
	void PushEmpty(Button* B)
	{
		if (buttonList_size == 12)
		{
			return;
		}
		if (B == nullptr)
		{
			return;
		}

		int a = 0;
		for (int i = 0; i < 12; i++)
		{
			if (buttonList[i] == nullptr)
			{
				a = i;
				break;
			}
		}
		buttonList[a] = B;
		buttonList_size++;
	}
	void Remove(Button* B)
	{
		if (buttonList_size == 0)
		{
			return;
		}
		if (B == nullptr)
		{
			return;
		}

		for (int i = 0; i < 12; i++)
		{
			if (buttonList[i] == B)
			{
				buttonList[i] = nullptr;
				buttonList_size--;
				return;
			}
		}
	}
};

class Button_Bag :public Button
{
public:
	Panel* P;
public:
	using Button::Button;
	void Active() override
	{
	}
	void ActivePressed()override
	{
		if (P->GetActive())
		{
			P->SetActive(false);
		}
		else
		{
			P->SetActive(true);
		}
	}
};

Bag bag;

class Button_Attack :public Button
{
public:
	Player* player;
	AnimationClip* playerAnimationClip_Attack;
	Animator* playerAnimator;
public:
	using Button::Button;
	Button_Attack(Vector2 posi, string path, AnimationClip* clip, Animator* animator) :Button(posi, path)
	{

		bag.PushEmpty(this);
		this->player = playerInto;
		this->playerAnimationClip_Attack = clip;
		this->playerAnimator = animator;
	}
	void Active() override
	{}
	void ActivePressed() override
	{
		Skill_Attack* skillptr = new Skill_Attack{ playerAnimationClip_Attack, RIGHT };
		Events* events = new Events{ playerAnimationClip_Attack };
		events->AddEvent(Player_Attack, 11);
		skillptr->SetPosition(player->position);
		playerAnimator->TransintoClip(playerAnimationClip_Attack);
		playerAnimator->SetNextClip(0);
		roundManager.skill = skillptr;
	}
};
class Button_Attack_Heart :public Button
{
public:
	Player* player;
	AnimationClip* playerAnimationClip_Attack_heart;
	Animator* playerAnimator;
public:
	using Button::Button;
	Button_Attack_Heart(Vector2 posi, string path, AnimationClip* clip, Animator* animator) :Button(posi, path)
	{
		bag.PushEmpty(this);
		this->player = playerInto;
		this->playerAnimationClip_Attack_heart = clip;
		this->playerAnimator = animator;
	}
	void Active() override
	{
	}
	void ActivePressed()override
	{
		Skill_Heart_Attack* skillptr = new Skill_Heart_Attack{ playerAnimationClip_Attack_heart,RIGHT };
		Events* events = new Events{ playerAnimationClip_Attack_heart };
		events->AddEvent(Player_Attack_Heart, 11);
		skillptr->SetPosition(player->position);
		playerAnimator->TransintoClip(playerAnimationClip_Attack_heart);
		playerAnimator->SetNextClip(0);
		roundManager.skill = skillptr;
		SetActive(false);
		bag.Remove(this);
	}
};


void testfuntion()
{
	cout << "�ҳ����ˣ�����" << endl;
}

vector<Animator*> AnimatorList;

Animator* playerAnimatorInto;
AnimationClip* playerAnimationClip_Attack_heartInto;
Text* text_get_skillheart_firstInto;
AnimationClip* playerAnimationClip_AttackInto;

class GetSkill1_Trigger :public Collision
{
public:
	bool actived;
	Object* B;
public:
	GetSkill1_Trigger(GameObject& gameObject) :Collision("enemyTrigger", true, gameObject, { 164,164 }, { -18,-18 }, Type_Trigger)
	{
		actived = false;
		Vector2 V = gameObject.position;
		V += {32, -64};
		B = new Object{ "B",V.x,V.y,0,0,"B",LAYER_PLHOU + 1 };

	}
	void OnTrigger(GameObject& gameObject) override
	{//��������д������
		if (!actived)
		{
			this->gameObject.color = GRAY;
			B->SetActive_Object(true);
		}
	}
	void NotTrigger(GameObject& gameObject) override
	{
		this->gameObject.color = WHITE;
		B->SetActive_Object(false);
	}
	void Active() override
	{
		if (!actived)
		{
			actived = true;
			new Button_Attack_Heart{ bag.GetPosition(),"heart3" ,playerAnimationClip_Attack_heartInto, playerAnimatorInto };
			this->gameObject.color = WHITE;
			B->SetActive_Object(false);
			Dialog_Box::PushText(text_get_skillheart_firstInto);
			Dialog_Box::SetActive(true);
		}
	}
};
Text* text_get_skillInto;
class GetSkill2_Trigger :public Collision
{
public:
	bool actived;
	Object* B;
public:
	GetSkill2_Trigger(GameObject& gameObject) :Collision("enemyTrigger", true, gameObject, { 164,164 }, { -18,-18 }, Type_Trigger)
	{
		actived = false;
		Vector2 V = gameObject.position;
		V += {32, -64};
		B = new Object{ "B",V.x,V.y,0,0,"B",LAYER_PLHOU + 1 };

	}
	void OnTrigger(GameObject& gameObject) override
	{//��������д������
		if (!actived)
		{
			this->gameObject.color = GRAY;
			B->SetActive_Object(true);
		}
	}
	void NotTrigger(GameObject& gameObject) override
	{
		this->gameObject.color = WHITE;
		B->SetActive_Object(false);
	}
	void Active() override
	{
		if (!actived)
		{
			actived = true;
			new Button_Attack{ bag.GetPosition(),"hair12" ,playerAnimationClip_AttackInto, playerAnimatorInto };
			this->gameObject.color = WHITE;
			B->SetActive_Object(false);
			Dialog_Box::PushText(text_get_skillInto);
			Dialog_Box::SetActive(true);
		}
	}
};

class BirdTell :public Collision
{
public:
	Object* B;
	Text* bird_text_first;
	Text* bird_text;
	bool first;
public:
	BirdTell(GameObject& gameObject) :Collision("birdTell", true, gameObject, { 128,256 }, { 0,0 }, Type_Trigger)
	{
		first = true;
		Vector2 V = gameObject.position;
		V += {32, -64};
		B = new Object{ "B",V.x,V.y,0,0,"B",LAYER_PLHOU + 1 };

	}
	void OnTrigger(GameObject& gameObject) override
	{
		if (!Dialog_Box::GetActive())
		{
			B->SetActive_Object(true);
		}
		else
		{
			B->SetActive_Object(false);
		}

	}
	void NotTrigger(GameObject& gameObject) override
	{
		B->SetActive_Object(false);
	}
	void Active() override
	{
		if (!Dialog_Box::GetActive())
		{
			if (first)
			{
				first = false;
				Dialog_Box::PushText(bird_text_first);
				Dialog_Box::SetActive(true);
			}
			else
			{

				Dialog_Box::PushText(bird_text);
				Dialog_Box::SetActive(true);
			}
		}
	}
};

GameObject* dangerbattle_Object;
GameObject* passionbattle_Object;
void DangerIntoBattle()
{
	LoadSence(2);
}
AnimationClip::AnimationClip(string name, Animator& animator, int sum_frame, int speed_frame, string path)
{
	this->name = name;
	this->PNG_path = "Image/" + path;
	this->sum = sum_frame;//֡��
	this->speed = speed_frame;//֡��
	this->time = static_cast<float>(sum_frame) / speed_frame;//����ʱ��
	this->indicator = 0;//ָʾ��
	this->digit = GAME_FPS / speed_frame;
	this->countor = 0;//������
	this->image = new Image[sum_frame];
	this->texture = new Texture[sum_frame];
	if (path != "null")
	{
		for (int i = 1; i <= sum_frame; i++)
		{
			if (i < 10)
			{
				this->image[i - 1] = LoadImage((PNG_path + "" + to_string(i) + ".png").c_str());
				this->texture[i - 1] = LoadTextureFromImage(image[i - 1]);
			}
			else
			{
				this->image[i - 1] = LoadImage((PNG_path + to_string(i) + ".png").c_str());
				this->texture[i - 1] = LoadTextureFromImage(image[i - 1]);
			}
		}
	}
	AddToList(animator);
}
void AnimationClip::UpdateCountor()
{
	if (sum > 1)//ֻ�ж�������Ҫ����
	{
		if (countor + 1 >= digit)
		{
			countor = 0;
			indicator = mathf::Carry(sum, indicator);
		}
		else
		{
			countor++;
		}
	}
}
void AnimationClip::AddToList(Animator& animator)
{
	index = animator.AnimationClipList.size();
	animator.AnimationClipList.push_back(this);
}
void AnimationClip::ToDefault()
{
	indicator = 0;
	countor = 0;
}
void Object::Draw(const Vector2* camera_position)
{
	DrawTexture(*this->texture, this->position.x - camera_position->x, this->position.y - camera_position->y, color);
	this->color = WHITE;
}
Vector2 GetPositionOnScreen(const Vector2 World_position, const Vector2* camera_position)
{
	return { World_position.x - camera_position->x, World_position.y - camera_position->y };
}
// ��������ͼƬ
Object::Object(int sence, string name, float position_x, float position_y, float pivot_x, float pivot_y, string path, int layer)
{
	this->name = name;
	this->position = { position_x,position_y };
	this->pivot = { pivot_x,pivot_y };
	this->layer = layer;
	this->InWhichScene = sence;
	//this->IndexInScene = SceneList[sence]->ObjectList[layer].size();
	SceneList[sence]->ObjectList[layer].push_back(this);//�Ǽ�����Ӧ�����Ķ�Ӧͼ��
	this->color = WHITE;
	this->active_Object = true;

	this->PNG_path = "Image/" + path;
	this->Object_texture = LoadTexture((PNG_path + ".png").c_str());
	this->texture = &this->Object_texture;
	SetTextureFilter(*this->texture, TEXTURE_FILTER_POINT);
	SetTextureWrap(*this->texture, TEXTURE_WRAP_CLAMP);
}

// ��ͼƬ
Object::Object(string name, float position_x, float position_y, float pivot_x, float pivot_y, string path, int layer) :Object(1, name, position_x, position_y, pivot_x, pivot_y, path, layer)
{

}
void Object::ChangeSence(int TargetScene, int targetLayer)
{
	SceneList[InWhichScene]->ObjectList[layer].remove(this);
	InWhichScene = TargetScene;
	SceneList[TargetScene]->ObjectList[targetLayer].push_back(this);
}
void Object::SetActive_Object(bool B)
{
	if (active_Object != B)
	{

	}
	active_Object = B;
}
bool Object::GetActive_Object()
{
	return active_Object;
}
Object::~Object()
{
	UnloadTexture(Object_texture);
}
Scene::Scene(string name, int indexInList)
{
	this->name = name;
	index = indexInList;
}

bool DefaultCamera::IfInView(const Vector2* GameObject_position)
{
	if ((GameObject_position->x >= position.x - view.x && GameObject_position->x <= position.x + view.x + 128.f)
		&& (GameObject_position->y >= position.y - view.y && GameObject_position->y <= position.y + view.y + 128.f))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void DefaultCamera::SetPosition(Vector2 V)
{
	if (V.x < rangeLeftUp.x)
	{
		position.x = rangeLeftUp.x;
	}
	else if (V.x > rangeRightDown.x - view.x)
	{
		position.x = rangeRightDown.x - view.x;
	}
	else
	{
		position.x = V.x;
	}

	if (V.y < rangeLeftUp.y)
	{
		position.y = rangeLeftUp.y;
	}
	else if (V.y > rangeRightDown.y - view.y)
	{
		position.y = rangeRightDown.y - view.y;
	}
	else
	{
		position.y = V.y;
	}
}




AnimationClip::~AnimationClip()
{
	for (int i = 1; i <= sum; i++)
	{
		UnloadImage(this->image[i - 1]);
		UnloadTexture(this->texture[i - 1]);
	}
}

Component::Component(string name, bool active, GameObject& gameObject)
	:name(name),
	active(active),
	gameObject(gameObject)
{
	AddToList(gameObject);
}

void Component::Active()
{

}

void Component::AddToList(GameObject& gameObject)
{
	index_gameObject = gameObject.ComponentList.size();
	gameObject.ComponentList.push_back(this);
}

void Component::DeleteFromList()
{
	gameObject.ComponentList.erase(gameObject.ComponentList.begin() + index_gameObject);
}

Component::~Component()
{
	DeleteFromList();
}

GameObject::GameObject(int sence, string name, float position_x, float position_y, float pivot_x, float pivot_y, string path, int layer) :Object(sence, name, position_x, position_y, pivot_x, pivot_y, path, layer)
{
}

GameObject::GameObject(int sence, string name, float posi_x, float posi_y, float pivot_x, float pivot_y, float C_width, float C_height, float C_x, float C_y, string path, int layer) :Object(sence, name, posi_x, posi_y, pivot_x, pivot_y, path, layer)
{
	string C_name = name + "_Collision";
	new Collision(C_name, true, *this, { C_width, C_height }, { C_x,C_y }, Type_Static);
}

GameObject::GameObject(string name, float posi_x, float posi_y, float pivot_y, string path, int layer) :Object(name, posi_x, posi_y, 0.f, pivot_y, path, layer)
{
	string C_name = name + "_Collision";
	float C_width, C_height, C_x, C_y;
	C_width = Object_texture.width;
	C_height = Object_texture.height - pivot_y;
	C_x = 0.f;
	C_y = pivot_y;
	new Collision(C_name, true, *this, { C_width, C_height }, { C_x,C_y }, Type_Static);
}


GameObject::GameObject(int sence, string name, float position_x, float position_y, float pivot_x, float pivot_y, string path, int layer, int sum_frame, int speed_frame) :Object(sence, name, position_x, position_y, pivot_x, pivot_y, path, layer)
{
	string AnimatorName = name + "Animator";
	animator = new Animator{ AnimatorName,true,*this };
	string ClipName = name + "AnimationClip";
	new AnimationClip{ ClipName,*animator ,sum_frame,speed_frame,path };
}

GameObject::~GameObject()
{
	for (auto C : ComponentList)
	{
		delete C;
	}
}

Animator::Animator(string name, bool active, GameObject& gameObject) :Component(name, active, gameObject)
{
	currentClip = 0;
	nextClip = -1;
	AddToAnimatorList(gameObject.InWhichScene);
}

void Animator::TransintoUnderGivenClip(string clip_name, AnimationClip* Clip)
{
	if (AnimationClipList.at(currentClip)->name == clip_name)
	{
		TransintoClip(Clip);
	}
}

void Animator::TransintoClip(AnimationClip* Clip)
{
	if (Clip->index != currentClip)
	{
		AnimationClipList.at(currentClip)->ToDefault();
		currentClip = Clip->index;
		AnimationClipList.at(currentClip)->ToDefault();
	}
}
void Animator::TransintoClip(int ClipIndex)
{
	if (ClipIndex != currentClip)
	{
		AnimationClipList.at(currentClip)->ToDefault();
		currentClip = ClipIndex;
		AnimationClipList.at(currentClip)->ToDefault();
	}
}
void Animator::SetClip(int ClipIndex)
{
	AnimationClipList.at(currentClip)->ToDefault();
	currentClip = ClipIndex;
	AnimationClipList.at(currentClip)->ToDefault();
}
void Animator::SetNextClip(AnimationClip* Clip)
{
	nextClip = Clip->index;
}

void Animator::SetNextClip(int index)
{
	nextClip = index;
}

void Animator::AddToAnimatorList(int WhichSence)
{
	AnimatorList.push_back(this);
}

//TODO:��Ҳû���զд������Ӧ���ò����������
void Animator::DeleteFromAnimatorList()
{
}

void Animator::UpdateCurrentClip()
{
	//��������˲�����Ĭ���л��Ķ���Clip

	if (nextClip != -1)
	{
		//����Ѿ������һ֡ 
		AnimationClip* currentClip_ptr = AnimationClipList.at(currentClip);
		if (currentClip_ptr->indicator == currentClip_ptr->sum - 1)
		{
			//�л�
			TransintoClip(AnimationClipList.at(nextClip));
			//����õ��ļ�¼��Ϊ��һ����׼��
			nextClip = -1;
		}
	}
	//���¶���֡��ʱ��
	AnimationClipList.at(currentClip)->UpdateCountor();
	gameObject.texture = &(AnimationClipList.at(currentClip)->texture[AnimationClipList.at(currentClip)->indicator]);
}
Animator::~Animator()
{
}
//TODO:�����������붯�����Ž���

Events::Events(AnimationClip* animationClip)
{
	this->Clip = animationClip;
	this->EventList = new Event[animationClip->sum];
	animationClip->Events = this;

	for (int i = 0; i < animationClip->sum; i++)
	{
		EventList[i] = DefaultEvent;
	}

}

void Events::AddEvent(Event event, int frame)
{
	EventList[frame] = event;
}

void Events::DeleteEvent(int frame)
{
	EventList[frame] = nullptr;
}

Collision::Collision(string name, bool active, GameObject& gameObject, Vector2 Size, Vector2 Offset, MotionType Type /*, float ElasticityCoefficient */) :Component(name, active, gameObject)
{
	this->Rec_Current.width = Size.x;
	this->Rec_Current.height = Size.y;
	this->Rec_Current.x = gameObject.position.x + Offset.x;
	this->Rec_Current.y = gameObject.position.y + Offset.y;
	this->Rec_Last = this->Rec_Current;
	this->Type = Type;
	this->Vector_Offset = Offset;
	/*this->ElasticityCoefficient = ElasticityCoefficient;*/

	//�������͵ǽ���Ӧ�б�
	if (Type == Type_Dynamic)
	{
		this->Index_CollisionList = CollisionList_Dynamic.size();
		CollisionList_Dynamic.push_back(this);
	}
	else if (Type == Type_Static)
	{
		this->Index_CollisionList = CollisionList_Static.size();
		CollisionList_Static.push_back(this);
	}
	else if (Type == Type_Trigger)
	{
		this->Index_CollisionList = CollisionList_Trigger.size();
		CollisionList_Trigger.push_back(this);
	}
	else
	{
		this->Index_CollisionList = CollisionList_Trigger.size();
		CollisionList_Trigger.push_back(this);
	}
}

void Collision::DeleteFromList()
{
	if (Type == Type_Dynamic)
	{
		CollisionList_Dynamic.erase(CollisionList_Dynamic.begin() + Index_CollisionList);
	}
	else if (Type == Type_Static)
	{
		CollisionList_Static.erase(CollisionList_Static.begin() + Index_CollisionList);
	}
	else if (Type == Type_Trigger)
	{
		CollisionList_Trigger.erase(CollisionList_Trigger.begin() + Index_CollisionList);
	}
	Component::DeleteFromList();
}

void Collision::TransintoType(MotionType type)
{
	if (Type != type)
	{
		DeleteFromCollisionList();
		Type = type;
		AddToCollisionList();
	}
}

void Collision::TransintoDynamic()
{
	TransintoType(Type_Dynamic);
}

void Collision::TransintoStatic()
{
	TransintoType(Type_Static);
}

void Collision::TransintoTrigger()
{
	TransintoType(Type_Trigger);
}

void Collision::SyncPosition()
{
	Rec_Last = Rec_Current;
	Rec_Current.x = gameObject.position.x + Vector_Offset.x;
	Rec_Current.y = gameObject.position.y + Vector_Offset.y;
}

//�ж��Ƿ�����
bool Collision::CheckCollidor(Rectangle& OtherRec)
{
	return CheckCollisionRecs(this->Rec_Current, OtherRec);
}

//�ж��Ƿ�����
bool Collision::CheckCollidor(Collision& OtherCollision)
{
	return CheckCollisionRecs(this->Rec_Current, OtherCollision.Rec_Current);
}

void Collision::BackToLastPosition()
{
	this->Rec_Current = this->Rec_Last;
	this->gameObject.position.x = this->Rec_Current.x - Vector_Offset.x;
	this->gameObject.position.y = this->Rec_Current.y - Vector_Offset.y;
}

void Collision::OnTrigger(GameObject& GameObject)
{
}
void Collision::NotTrigger(GameObject& gameObject)
{
}

void Collision::CheckCollidorInUpdate(Rectangle& OtherRec)
{
	if (CheckCollisionRecs(this->Rec_Current, OtherRec))
	{
		BackToLastPosition();
	}
}

void Collision::DeleteFromCollisionList()
{
	if (Type == Type_Static)
	{
		CollisionList_Static.erase(CollisionList_Static.begin() + Index_CollisionList);
	}
	else if (Type == Type_Dynamic)
	{
		CollisionList_Dynamic.erase(CollisionList_Dynamic.begin() + Index_CollisionList);
	}
	else if (Type == Type_Trigger)
	{
		CollisionList_Trigger.erase(CollisionList_Trigger.begin() + Index_CollisionList);
	}
}

void Collision::AddToCollisionList()
{
	if (Type == Type_Static)
	{
		Index_CollisionList = CollisionList_Static.size();
		CollisionList_Static.push_back(this);
	}
	else if (Type == Type_Dynamic)
	{
		Index_CollisionList = CollisionList_Dynamic.size();
		CollisionList_Dynamic.push_back(this);
	}
	else if (Type = Type_Trigger)
	{
		Index_CollisionList = CollisionList_Trigger.size();
		CollisionList_Trigger.push_back(this);
	}
}

Text::Text(bool active, string path, Vector2 position, int size)
{
	this->data = LoadFileText(("Text/" + path + ".txt").c_str());
	this->data_ToPrint = data;
	this->path = path;
	this->position = position;
	this->active_Text = active;
	this->size = size;
	if (active)
	{
		AddToList();
	}
}

Text::Text(bool active, string path, Vector2 position) :Text(active, path, position, FONTSIZE)
{
}

void Text::SetActive(bool active)
{
	if (this->active_Text != active)
	{
		this->active_Text = active;
		if (active)
		{
			AddToList();
		}
		else
		{
			DeleteFromList();
		}
	}
}

void Text::PrintOnScreen(DefaultCamera camera)
{
	DrawTextEx(font, data_ToPrint, position, size, FONTSPACE, FONT_COLOR);
}

void Text::AddToList()
{
	this->Index = TextList.size();
	TextList.push_back(this);
}

void Text::DeleteFromList()
{
	TextList.erase(TextList.begin() + Index);
	Index = -1;
}

Text_Component::Text_Component(string name, bool active, GameObject& gameObject, string path, Vector2 position, int size) :Component(name, active, gameObject), Text(active, path, position, size)
{

}

Text_Component::Text_Component(string name, bool active, GameObject& gameObject, string path, Vector2 position) :Component(name, active, gameObject), Text(active, path, position)
{
}

void Text_Component::PrintOnScreen(DefaultCamera camera)
{
	Vector2 V = GetPositionOnScreen(gameObject.position + position, &camera.position);
	DrawTextEx(font, data_ToPrint, V, size, FONTSPACE, FONT_COLOR);
}
void Dialog_Box::PushText(Text* T)
{
	textptr = T;
	ProcessText(T->data);
}
void Dialog_Box::SetText(Text* T)
{
	if (textptr != T)
	{
		PushText(T);
	}
}
void Dialog_Box::PopText()
{
	textptr = &DefaultDialog;
	ProcessText(DefaultDialog.data);
}
void Dialog_Box::SetActive(bool B)
{
	active = B;
}
void Dialog_Box::ProcessText(const char* text)
{
	textList = TextSplit(text, DELIMITER, &List_Count);
	List_Index = 0;
}
void Dialog_Box::Next()
{
	if (++List_Index == List_Count)
	{
		List_Index = 0;
		Dialog_Box::SetActive(false);
	}
}
void Dialog_Box::Print()
{
	if (active)
	{
		//�װ���ɫΪ{200, 220, 255, 200}�������ɫ����ѡ��Ϊ{150, 180, 230, 255}������Ϊ��ɫ
		DrawRectangleRounded(Rec, 30.f, 10, Color_Under);
		DrawRectangleRoundedLines(Rec, 30.f, 10, 5, Color_Line);
		DrawTextEx(font, textList[List_Index], { Rec.x + 60.f,Rec.y + 50.f }, FONTSIZE, FONTSPACE, FONT_COLOR);
	}
}

bool Dialog_Box::GetActive()
{
	return active;
}
